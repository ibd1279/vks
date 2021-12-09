package main

import (
	"fmt"
	"os"
	"text/template"
)

func GenerateCDefineFile(config *Config, graph RegistryGraph) error {
	fn := fmt.Sprintf("%s.h", config.OutputName)

	var err error
	t := template.New(fn).Funcs(template.FuncMap{
		"cparam":  handleCArraySyntax,
		"preproc": func() []string { return config.CDefinePreProc },
	})
	if t, err = t.Parse(cDefinePrimaryTemplate); err != nil {
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
		Data []interface{}
	}{data})
	if err != nil {
		return err
	}

	return nil
}

func handleCArraySyntax(a, b Translator) string {
	if t, ok := a.(*ArrayConverter); ok {
		return fmt.Sprintf("%s %s[%s]", t.orig.C(), b.C(), t.size.C())
	}
	return fmt.Sprintf("%s %s", a.C(), b.C())
}

const cDefinePrimaryTemplate = `#ifndef __VKS_H__
#define __VKS_H__{{range preproc}}
{{.}}{{end}}
#include <stdlib.h>
#include <string.h>
#include "vulkan/vulkan.h"

typedef struct vksProcAddr { {{range .Data}}{{with .Data}}
	void* p{{.Name.C}};{{end}}{{end}}
} vksProcAddr;

VkResult vksDynamicLoad();
void vksDynamicUnload();
{{range .Data}}{{with .Data}}{{.Return.C}} {{.Name.C}}({{range $idx, $param := .Parameters}}{{if ne $idx 0}}, {{end}}{{cparam $param.Type $param.Name}}{{end}});
{{end}}{{end}}
#endif`
