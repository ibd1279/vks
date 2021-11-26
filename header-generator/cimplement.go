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

// default dynamic loader?
VkResult vksDynamicLoad() {
	// TODO Start: Abstract this block to be specialized as needed.
	vulkanHandle = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
	if (vulkanHandle == NULL) {
		return VK_ERROR_UNKNOWN;
	}
	vksProcAddresses.pvkGetInstanceProcAddr = dlsym(vulkanHandle, "vkGetInstanceProcAddr");
	// TODO end

	vksProcAddresses.pvkEnumerateInstanceVersion = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceVersion");
	vksProcAddresses.pvkEnumerateInstanceExtensionProperties = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
	vksProcAddresses.pvkEnumerateInstanceLayerProperties = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
	vksProcAddresses.pvkCreateInstance = vkGetInstanceProcAddr(NULL, "vkCreateInstance");
	return VK_SUCCESS;
}

void vksDynamicUnload() {
	dlclose(vulkanHandle);
}

// call getProcAddress for all the commands.
void vksInitializeInstanceProcAddrs(VkInstance instance) {
	void* ptr = NULL;{{range .Data}}{{with .Data}}
	ptr = vkGetInstanceProcAddr(instance, "{{.Name.C}}");
	if (ptr) {
		vksProcAddresses.p{{.Name.C}} = ptr;
	}{{end}}{{end}}
}

{{range .Data}}{{with .Data}}{{.Return.C}} {{.Name.C}}({{range $idx, $param := .Parameters}}{{if ne $idx 0}}, {{end}}{{cparam $param.Type $param.Name}}{{end}}) {
	{{if ne .Return.C "void"}}{{.Return.C}} ret = {{end}}(((PFN_{{.Name.C}})vksProcAddresses.p{{.Name.C}})({{range $idx, $param := .Parameters}}{{if ne $idx 0}}, {{end}}{{$param.Name.C}}{{end}}));{{if eq .Name.C "vkCreateInstance"}}
	vksInitializeInstanceProcAddrs(*pInstance);{{end}}{{if ne .Return.C "void"}}
	return ret;{{end}}
}
{{end}}{{end}}`
