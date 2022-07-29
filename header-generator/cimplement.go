package main

import (
	"fmt"
	"os"
	"text/template"
)

func GenerateCImplementFile(config *Config, graph RegistryGraph) error {
	fn := fmt.Sprintf("%s.c", config.OutputName)
	header := fmt.Sprintf("%s.h", config.OutputName)

	var err error
	t := template.New(fn).Funcs(template.FuncMap{
		"cparam": handleCArraySyntax,
		"globalProcs": func() []string {
			keys := make([]string, 0, len(config.GlobalProcs))
			for _, v := range config.GlobalProcs {
				if v != "vkGetInstanceProcAddr" {
					keys = append(keys, v)
				}
			}
			return keys
		},
	})
	if t, err = t.Parse(cImplementPrimaryTemplate); err != nil {
		return err
	}

	data := []interface{}{}
	store := func(path []*RegistryNode) {
		node := path[len(path)-1]
		if command := node.CommandElement(); command != nil {
			if len(command.Alias()) == 0 {
				if ok, cmd := CommandToData(node, *command); ok {
					data = append(data, cmd)
				}
			}
		}
	}
	graph.DepthFirstSearch(config.Enabled(), store)

	var fh *os.File
	fh, err = os.OpenFile(fn, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, 0644)
	if err != nil {
		return err
	}
	defer fh.Close()

	err = t.Execute(fh, struct {
		Header string
		Data   []interface{}
	}{header, data})
	if err != nil {
		return err
	}

	return nil
}

const cImplementPrimaryTemplate = `#include <dlfcn.h>
#include "{{.Header}}"

// Global for proc addresses.
vksProcAddr vksProcAddresses;
void *vulkanHandle = NULL;

#if defined(_WIN64) || defined(_WIN32)
    #define LIB_NAME "vulkan.dll"
#elif defined(__linux__) || defined(__FreeBSD__) || defined (__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
    #define LIB_NAME "libvulkan.so"
#elif defined(__APPLE__) || defined(__MACH__)
    #define LIB_NAME "libvulkan.dylib"
#endif

// default dynamic loader?
VkResult vksDynamicLoad() {
	// TODO Start: Abstract this block to be specialized as needed.
	vulkanHandle = dlopen(LIB_NAME, RTLD_NOW | RTLD_LOCAL);
	if (vulkanHandle == NULL) {
		return VK_ERROR_UNKNOWN;
	}
	vksProcAddresses.pvkGetInstanceProcAddr = dlsym(vulkanHandle, "vkGetInstanceProcAddr");
	// TODO end
{{range globalProcs}}
	vksProcAddresses.p{{.}} = vkGetInstanceProcAddr(&vksProcAddresses, NULL, "{{.}}");{{end}}
	return VK_SUCCESS;
}

void vksDynamicUnload() {
	dlclose(vulkanHandle);
}

// call getProcAddress for all the commands.
void vksLoadInstanceProcAddrs(VkInstance hndl, vksProcAddr* addrs) { {{range .Data}}{{with .Data}}
	addrs->p{{.Name.C}} = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "{{.Name.C}}");{{end}}{{end}}
}

// call getProcAddress for all the commands.
void vksLoadDeviceProcAddrs(VkDevice hndl, vksProcAddr* addrs, vksProcAddr* parent) { {{range .Data}}{{with .Data}}
	addrs->p{{.Name.C}} = vkGetDeviceProcAddr(parent, hndl, "{{.Name.C}}");{{end}}{{end}}
}

{{range .Data}}{{with .Data}}{{.Return.C}} {{.Name.C}}(vksProcAddr* addrs{{range .Parameters}}, {{cparam .Type .Name}}{{end}}) {
	{{if ne .Return.C "void"}}{{.Return.C}} ret = {{end}}(((PFN_{{.Name.C}})addrs->p{{.Name.C}})({{range $idx, $param := .Parameters}}{{if ne $idx 0}}, {{end}}{{$param.Name.C}}{{end}}));{{if ne .Return.C "void"}}
	return ret;{{end}}
}
{{end}}{{end}}`
