package main

import (
	"fmt"
	"os"
	"text/template"

	"gopkg.in/yaml.v3"
)

func main() {
	config := LoadConfig("vkxml.yml")
	for _, v := range config.ExportTranslator {
		switch v.Action {
		case "deprefix":
			TranslatorRules = append(TranslatorRules, exportTranslatorRuleDeprefix{v.Pattern})
		case "title":
			TranslatorRules = append(TranslatorRules, exportTranslatorRuleTitle{})
		}
	}

	enabledMap := make(map[string]bool, 0)
	for _, v := range config.Features {
		enabledMap[v] = true
	}
	for _, v := range config.Extensions {
		enabledMap[v] = true
	}

	registry := LoadRegistry(config.VkxmlPath)
	graph, constants := registry.Graph()

	enabled := make([]string, 0, len(config.Features)+len(config.Extensions))
	for _, v := range config.Features {
		n := fmt.Sprintf("%s::vulkan", v)
		graph.ApplyFeatureExtensions(n)
		enabled = append(enabled, n)
	}
	for _, v := range config.Extensions {
		graph.ApplyExtensionExtensions(v, enabledMap)
		enabled = append(enabled, v)
	}

	var data []interface{}
	data = append(data, ConstToData(constants))
	store := func(nodes []*RegistryNode) {
		node := nodes[len(nodes)-1]
		switch node.NodeType {
		case RegistryNodeType:
			if tiepuh := node.TypeElement(); tiepuh != nil {
				if b, d := TypeToData(node, *tiepuh); b {
					data = append(data, d)
				}
			}
		case RegistryNodeCommand:
			if command := node.CommandElement(); command != nil {
				if b, d := CommandToData(node, *command); b {
					data = append(data, d)
				}
			}
		}
	}
	graph.DepthFirstSearch(enabled, store)
	data = append([]interface{}{}, ConstToData(constants))
	graph.DepthFirstSearch(enabled, store)

	t := template.Must(template.New("primary").Parse(primaryTemplate))
	t = template.Must(t.Parse(constTemplate))
	t = template.Must(t.Parse(versionTemplate))
	t = template.Must(t.Parse(baseTemplate))
	t = template.Must(t.Parse(handleTemplate))
	t = template.Must(t.Parse(enumTemplate))
	t = template.Must(t.Parse(bitmaskTemplate))
	t = template.Must(t.Parse(funcTemplate))
	t = template.Must(t.Parse(structTemplate))
	t = template.Must(t.Parse(unionTemplate))
	t = template.Must(t.Parse(commandTemplate))
	err := t.Execute(os.Stdout, struct {
		PackageName string
		Data        []interface{}
	}{config.PackageName, data})

	if err != nil {
		panic(err)
	}
}

func LoadConfig(fn string) *Config {
	fh, err := os.OpenFile(fn, os.O_RDONLY, 0)
	if err != nil {
		panic(err)
	}
	defer fh.Close()

	var config Config
	decoder := yaml.NewDecoder(fh)
	if err := decoder.Decode(&config); err != nil {
		panic(err)
	}

	return &config
}

type Config struct {
	PackageName      string
	VkxmlPath        string
	Features         []string
	Extensions       []string
	ExportTranslator []TranslatorConfig
}

type TranslatorConfig struct {
	Action      string
	Pattern     string
	Replacement string
}

func LoadRegistry(fn string) *Registry {
	// Open the datasource.
	fh, err := os.OpenFile(fn, os.O_RDONLY, 0)
	if err != nil {
		panic(err)
	}
	defer fh.Close()

	if registry, err := DecodeRegistry(fh); err != nil {
		panic(err)
	} else {
		return registry
	}
}

const constTemplate = `{{define "const"}}const ({{range .}}
	{{.Name.Go}} = {{.Value.Go}}{{end}}
)
{{end}}`
const versionTemplate = `{{define "version"}}// {{.Name.Go}} is an implementation of the Vulkan Make Api Version
// defines.
type {{.Name.Go}} uint32

// MakeApiVersion creates a Version based on the provided Variant, maJor, miNor,
// and Patch.
func Make{{.Name.Go}}(v, j, n, p int) {{.Name.Go}} {
	return {{.Name.Go}}((uint32(v) << 29) | (uint32(j) << 22) | (uint32(n) << 12) | (uint32(p)))
}

func (v {{.Name.Go}}) Major() uint32   { return uint32((v >> 22) & 0x7F) }
func (v {{.Name.Go}}) Minor() uint32   { return uint32((v >> 12) & 0x3FF) }
func (v {{.Name.Go}}) Patch() uint32   { return uint32(v & 0xFFF) }
func (v {{.Name.Go}}) Variant() uint32 { return uint32(v >> 29) }
func (v {{.Name.Go}}) String() string {
	return fmt.Sprintf("%d[v.%d.%d.%d.%d]", v, v.Variant(), v.Major(), v.Minor(), v.Patch())
}

var (
	VK_API_VERSION_1_0         {{.Name.Go}} = Make{{.Name.Go}}(0, 1, 0, 0)
	VK_API_VERSION_1_1         {{.Name.Go}} = Make{{.Name.Go}}(0, 1, 1, 0)
	VK_API_VERSION_1_2         {{.Name.Go}} = Make{{.Name.Go}}(0, 1, 2, 0)
	VK_HEADER_VERSION_COMPLETE {{.Name.Go}} = Make{{.Name.Go}}({{.Value.Go}}, {{.HeaderVersionName.Go}})
)
{{end}}{{define "headerversion"}}// Version of the vk specification used to generate this.
const {{.HeaderVersionName.Go}} = {{.Value.Go}}
{{end}}`
const baseTemplate = `{{define "base"}}type {{.Name.Go}} {{.Type.Go}}
{{end}}`
const handleTemplate = `{{define "handle"}}// {{.Name.Go}} is a Handle to a vulkan resource.
// See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/{{.Name.C}}.html
type {{.Name.Go}} {{.Name.CGo}}

var Null{{.Name.Go}} {{.Name.Go}}
{{end}}`
const enumTemplate = `{{define "enum"}}type {{.Name.Go}} {{.Type.Go}}{{if gt (len .Values) 0}}

const ({{range .Values}}
	{{.Name.Go}} {{.Type.Go}} = {{.Value.Go}}{{end}}
)
var (
	reverse{{.Name.Go}} map[{{.Name.Go}}]string = map[{{.Name.Go}}]string{ {{range .Values}}{{if eq .Alias false}}
		{{.Name.Go}}: "{{.Name.Go}}",{{end}}{{end}}
	}
)
func (x {{.Name.Go}}) String() string {
	if s, ok := reverse{{.Name.Go}}[x]; ok {
		return s
	}
	return fmt.Sprintf("{{.Name.Go}}=%d", x)
}{{end}}
{{end}}`
const bitmaskTemplate = `{{define "bitmask"}}type {{.Name.Go}} {{.Type.Go}}
{{end}}`
const funcTemplate = `{{define "func"}}type {{.Name.Go}} {{.Name.CGo}}
{{end}}`
const structTemplate = `{{define "struct"}}//{{.Name.Go}} provides a go interface for {{.Name.C}}.
// See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/{{.Name.C}}.html
type {{.Name.Go}} {{.Name.CGo}}

// New{{.Name.Go}} allocates an instance of this struct in the C memory instead
// of the Go memory.
func new{{.Name.Go}}() *{{.Name.Go}} {
	sz := unsafe.Sizeof({{.Name.Go}}{})
	ptr := C.malloc(C.ulong(sz))
	C.memset(ptr, 0, C.ulong(sz))
	return (*{{.Name.Go}})(ptr)
}{{with $struct := .}}

// Free releases the memory allocated by the New{{.Name.Go}} method.
// It does not free pointers stored in the structure. It should only
// be used on CPtr instances.
func (x *{{$struct.Name.Go}}) Free() {
	C.free(unsafe.Pointer(x))
}

// AsPtr returns the object as a pointer.
func (x {{$struct.Name.Go}}) AsPtr() *{{$struct.Name.Go}} { return &x }

// AsCPtr copies the object to C memory and returns the pointer.
func (x {{$struct.Name.Go}}) AsCPtr() *{{$struct.Name.Go}} {
	clone := new{{$struct.Name.Go}}()
	*clone = x
	return clone
}{{range .Members}}

// {{.Name.Go}} returns the value of {{.Name.C}} from {{$struct.Name.C}}
func (x {{$struct.Name.Go}}) {{.Name.Go}}() {{.Type.Go}} {
	ptr := {{.Type.CToGo}}(&x.{{.Name.CGo}}) 
	return *ptr
}{{if ne .Value nil}}

// WithDefault{{.Name.Go}} sets the value of {{.Name.Go}} to the value provided in the
// specification if there is only a single value in the specification.
func (x {{$struct.Name.Go}}) WithDefault{{.Name.Go}}() {{$struct.Name.Go}} {
	return x.With{{.Name.Go}}({{.Value.Go}})
}{{end}}{{if or (eq $struct.ReadOnly false) (or (eq .Name.Go "PNext") (eq .Name.Go "SType"))}}

// With{{.Name.Go}} copies the provided value into C space and stores it
// at {{.Name.C}} on {{$struct.Name.C}}
func (x {{$struct.Name.Go}}) With{{.Name.Go}}(y {{.Type.Go}}) {{$struct.Name.Go}} {
	ptr := {{.Type.GoToC}}(&y)
	{{if .Copy}}copy(x.{{.Name.CGo}}[:], unsafe.Slice(*ptr, len(y))){{else}}x.{{.Name.CGo}} = *ptr{{end}}
	return x
}{{end}}{{end}}
{{end}}{{end}}{{define "structalias"}}//{{.Name.Go}} is an alias to {{.Alias.Go}}.
// See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/{{.Name.C}}.html
//
// Deprecated: Most Aliases in the Vulkan spec are for compatibility purposes as extensions get
// promoted to features. If possible, update code to use the promoted name: {{.Alias.Go}}.
type {{.Name.Go}} = {{.Alias.Go}}{{end}}
`
const unionTemplate = `{{define "union"}}type {{.Name.Go}} {{.Name.CGo}}
{{with $root := .}}{{range .Members}}
func (x {{$root.Name.Go}}) {{.Name.Go}}() {{.Type.Go}} {
	return *(*{{.Type.Go}})(unsafe.Pointer(&x[0]))
}
func (x {{$root.Name.Go}}) Set{{.Name.Go}}(y {{.Type.Go}}) {
	*(*{{.Type.Go}})(unsafe.Pointer(&x[0])) = y
}
{{end}}{{end}}
{{end}}`
const commandTemplate = `{{define "command"}}func {{.Name.Go}}({{range .Parameters}}{{.Name.Go}} {{.Type.Go}}, {{end}}) {{if ne .Return.Go "void"}}{{.Return.Go}} {{end}}{ {{range $key, $val := .Parameters}}
	p{{$key}} := {{$val.Type.GoToC}}(&{{$val.Name.Go}}){{end}}
	{{if ne .Return.Go "void"}}ret := {{end}}{{.Name.CGo}}({{range $key, $val := .Parameters}}*p{{$key}}, {{end}})
	{{if ne .Return.Go "void"}}retPtr := {{.Return.CToGo}}(&ret)
	return *retPtr
{{end}}}
{{end}}`
const primaryTemplate = `package {{.PackageName}}

//#cgo LDFLAGS: -lvulkan
//#include <stdlib.h>
//#include <string.h>
//#include "vulkan/vulkan.h"
import "C"
import (
	"fmt"
	"unsafe"
)

{{range .Data}}{{if eq .Template "const"}}{{block "const" .Data}}{{.}}{{end}}
{{else if eq .Template "version"}}{{block "version" .Data}}{{.}}{{end}}
{{else if eq .Template "headerversion"}}{{block "headerversion" .Data}}{{.}}{{end}}
{{else if eq .Template "headerversioncomplete"}}{{block "headerversioncomplete" .Data}}{{.}}{{end}}
{{else if eq .Template "base"}}{{block "base" .Data}}{{.}}{{end}}
{{else if eq .Template "handle"}}{{block "handle" .Data}}{{.}}{{end}}
{{else if eq .Template "enum"}}{{block "enum" .Data}}{{.}}{{end}}
{{else if eq .Template "bitmask"}}{{block "bitmask" .Data}}{{.}}{{end}}
{{else if eq .Template "func"}}{{block "func" .Data}}{{.}}{{end}}
{{else if eq .Template "struct"}}{{block "struct" .Data}}{{.}}{{end}}
{{else if eq .Template "structalias"}}{{block "structalias" .Data}}{{.}}{{end}}
{{else if eq .Template "union"}}{{block "union" .Data}}{{.}}{{end}}
{{else if eq .Template "command"}}{{block "command" .Data}}{{.}}{{end}}
{{end}}{{end}}
`
