package main

import (
	"fmt"
	"log"
	"os"
	"regexp"
	"strconv"
	"strings"
	"text/template"

	strip "github.com/grokify/html-strip-tags-go"
)

const registryPath = "/usr/local/share/vulkan/registry/vk.xml"

var (
	enabledFeatures []string = []string{
		"VK_VERSION_1_0",
		"VK_VERSION_1_1",
		"VK_VERSION_1_2",
	}
	enabledExtensions []string = []string{
		"VK_KHR_surface",
		"VK_KHR_swapchain",
		"VK_KHR_display",
		"VK_KHR_display_swapchain",
	}
)

func main() {
	enabledMap := make(map[string]bool, 0)
	for _, v := range enabledFeatures {
		enabledMap[v] = true
	}
	for _, v := range enabledExtensions {
		enabledMap[v] = true
	}

	registry := LoadRegistry(registryPath)
	graph, constants := registry.Graph()

	enabled := make([]string, 0, len(enabledFeatures)+len(enabledExtensions))
	for _, v := range enabledFeatures {
		n := fmt.Sprintf("%s::vulkan", v)
		graph.ApplyFeatureExtensions(n)
		enabled = append(enabled, n)
	}
	for _, v := range enabledExtensions {
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
				if d := TypeToData(node, *tiepuh); d != nil {
					data = append(data, d)
				}
			}
		case RegistryNodeCommand:
			if command := node.CommandElement(); command != nil {
				if d := CommandToData(node, *command); d != nil {
					data = append(data, d)
				}
			}
		}
	}
	graph.DepthFirstSearch(enabled, store)

	t := template.Must(template.New("primary").Parse(primaryTemplate))
	t = template.Must(t.Parse(constTemplate))
	t = template.Must(t.Parse(baseTemplate))
	t = template.Must(t.Parse(handleTemplate))
	t = template.Must(t.Parse(enumTemplate))
	t = template.Must(t.Parse(bitmaskTemplate))
	t = template.Must(t.Parse(funcTemplate))
	t = template.Must(t.Parse(structTemplate))
	t = template.Must(t.Parse(unionTemplate))
	t = template.Must(t.Parse(commandTemplate))
	err := t.Execute(os.Stdout, data)
	log.Printf("Error %v", err)
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
const baseTemplate = `{{define "base"}}type {{.Name.Go}} {{.Type.Go}}
{{end}}`
const handleTemplate = `{{define "handle"}}// {{.Name.Go}} is a Handle to a vulkan resource.
type {{.Name.Go}} {{.Name.CGo}}
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
func (x *{{$struct.Name.Go}}) AsPtr() *{{$struct.Name.Go}} { return x }

// AsCPtr copies the object to C memory and returns the pointer.
func (x *{{$struct.Name.Go}}) AsCPtr() *{{$struct.Name.Go}} {
	clone := new{{$struct.Name.Go}}()
	*clone = *x
	return clone
}{{range .Members}}

// {{.Name.Go}} returns the value of {{.Name.C}} from {{$struct.Name.C}}
func (x {{$struct.Name.Go}}) {{.Name.Go}}() {{.Type.Go}} {
	ptr := {{.Type.CToGo}}(&x.{{.Name.CGo}}) 
	return *ptr
}{{if ne .Value nil}}

// WithDefault{{.Name.Go}} sets the value of {{.Name.Go}} to the value provided in the
// specification if there is only a single value in the specification.
func (x *{{$struct.Name.Go}}) WithDefault{{.Name.Go}}() *{{$struct.Name.Go}} {
	return x.With{{.Name.Go}}({{.Value.Go}})
}{{end}}{{if or (eq $struct.ReadOnly false) (or (eq .Name.Go "PNext") (eq .Name.Go "SType"))}}

// With{{.Name.Go}} copies the provided value into C space and stores it
// at {{.Name.C}} on {{$struct.Name.C}}
func (x *{{$struct.Name.Go}}) With{{.Name.Go}}(y {{.Type.Go}}) *{{$struct.Name.Go}} {
	ptr := {{.Type.GoToC}}(&y)
	{{if .Copy}}copy(x.{{.Name.CGo}}[:], unsafe.Slice(*ptr, len(y))){{else}}x.{{.Name.CGo}} = *ptr{{end}}
	return x
}{{end}}{{end}}
{{end}}{{end}}`
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
const primaryTemplate = `package vks

//#cgo LDFLAGS: -lvulkan
//#include <stdlib.h>
//#include <string.h>
//#include "vulkan/vulkan.h"
import "C"
import (
	"fmt"
	"unsafe"
)

{{range .}}{{if eq .Template "const"}}{{block "const" .Data}}{{.}}{{end}}
{{else if eq .Template "base"}}{{block "base" .Data}}{{.}}{{end}}
{{else if eq .Template "handle"}}{{block "handle" .Data}}{{.}}{{end}}
{{else if eq .Template "enum"}}{{block "enum" .Data}}{{.}}{{end}}
{{else if eq .Template "bitmask"}}{{block "bitmask" .Data}}{{.}}{{end}}
{{else if eq .Template "func"}}{{block "func" .Data}}{{.}}{{end}}
{{else if eq .Template "struct"}}{{block "struct" .Data}}{{.}}{{end}}
{{else if eq .Template "union"}}{{block "union" .Data}}{{.}}{{end}}
{{else if eq .Template "command"}}{{block "command" .Data}}{{.}}{{end}}
{{end}}{{end}}
`

func CommandToData(node *RegistryNode, command CommandElement) interface{} {
	if len(command.Alias()) > 0 {
		// TODO add support for Aliases later.
		return nil
	} else {
		var returnTranslator Translator
		if scalarXl8r := GetScalarTranslator(command.Proto.Type); scalarXl8r != nil {
			returnTranslator = scalarXl8r
		} else {
			returnTranslator = helperMemberTypeTranslator(command.Proto.Type, "", command.Proto.Raw, "")
		}
		data := CommandData{
			Name:   &ExportTranslator{&IdentifierTranslator{command.Name()}},
			Return: returnTranslator,
		}

		for _, v := range command.Params {
			data.Parameters = append(data.Parameters, CommandParamData{
				Name: &NameTranslator{&LiteralTranslator{v.Name}},
				Type: helperMemberTypeTranslator(v.Type, v.Length, v.Raw, ""),
			})
		}

		return &struct {
			Template string
			Data     CommandData
		}{"command", data}
	}
	return nil
}

func TypeToData(node *RegistryNode, tiepuh TypeElement) interface{} {
	switch tiepuh.Category {
	case TypeCategoryBasetype:
		return baseTypeToData(node, tiepuh)
	case TypeCategoryHandle:
		return handleTypeToData(node, tiepuh)
	case TypeCategoryEnum:
		return enumTypeToData(node, tiepuh)
	case TypeCategoryBitmask:
		return enumTypeToData(node, tiepuh)
	case TypeCategoryFuncpointer:
		return funcTypeToData(node, tiepuh)
	case TypeCategoryStruct:
		return structTypeToData(node, tiepuh)
	case TypeCategoryUnion:
		return unionTypeToData(node, tiepuh)
	}
	return nil
}

func baseTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     BaseData
} {
	if len(tiepuh.TypeTag) == 0 {
		return nil
	}
	data := BaseData{
		Type: GetScalarTranslator(tiepuh.TypeTag),
		Name: &IdentifierTranslator{tiepuh.Name()},
	}
	return &struct {
		Template string
		Data     BaseData
	}{"base", data}
}

func handleTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     HandleData
} {
	var data HandleData
	if t := GetHandleTranslator(tiepuh.Name()); t != nil {
		data.Name = t
	} else {
		t = &HandleTranslator{tiepuh.Name()}
		handleTranslatorList[tiepuh.Name()] = t
		data.Name = t
	}
	return &struct {
		Template string
		Data     HandleData
	}{"handle", data}
}

func enumTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     EnumData
} {
	ed := EnumData{
		Name: &IdentifierTranslator{tiepuh.Name()},
	}

	templateName := "enum"
	if tiepuh.Category == TypeCategoryBitmask {
		ed.Type = &IdentifierTranslator{tiepuh.TypeTag}
		templateName = "bitmask"
	}
	for _, v := range node.EnumsParents() {
		var bitmask bool
		if v.E.Type == EnumsElementBitmask {
			bitmask = true
		}
		var largestValue, smallestValue int64
		for _, v := range v.N.EnumParents() {
			evd := EnumValueData{
				Type: ed.Name,
				Name: &NameTranslator{&LiteralTranslator{v.E.Name}},
			}
			var valueTranslator Translator
			if bitmask {
				if len(v.E.Alias) > 0 {
					valueTranslator = &LiteralTranslator{v.E.Alias}
					evd.Alias = true
				} else if v.E.BitPos == 0 && len(v.E.Value) > 0 {
					valueTranslator = &ValueTranslator{v.E.Value}
				} else {
					valueTranslator = &BitValueTranslator{v.E.BitPos}
				}
			} else {
				if val, _ := strconv.ParseInt(v.E.Value, 10, 64); val > largestValue || val < smallestValue {
					if val > largestValue {
						largestValue = val
					} else if val < smallestValue {
						smallestValue = val
					}
				}
				if len(v.E.Alias) > 0 {
					valueTranslator = &LiteralTranslator{v.E.Alias}
					evd.Alias = true
				} else {
					valueTranslator = &ValueTranslator{v.E.Value}
				}
			}
			evd.Value = valueTranslator
			ed.Values = append(ed.Values, evd)
		}
		if smallestValue < 0 {
			ed.Type = Int32Translator
		}
	}
	if ed.Type == nil {
		ed.Type = Uint32Translator
	}

	return &struct {
		Template string
		Data     EnumData
	}{templateName, ed}
}

func funcTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     FuncData
} {
	data := FuncData{
		Type: GetScalarTranslator("void*"),
		Name: &IdentifierTranslator{tiepuh.Name()},
	}
	return &struct {
		Template string
		Data     FuncData
	}{"func", data}
}

func structTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     StructData
} {
	data := StructData{
		Name:     &StructTranslator{tiepuh.Name()},
		ReadOnly: tiepuh.ReturnedOnly,
	}
	for _, v := range tiepuh.StructMembers {
		subData := StructMemberData{
			Name: &NameTranslator{&ExportTranslator{&LiteralTranslator{v.Name}}},
			Type: helperMemberTypeTranslator(v.Type, v.Length, v.Raw, v.Comment),
		}
		if len(v.Values) > 0 && len(strings.Split(v.Values, ",")) == 1 && v.Name == "sType" {
			subData.Value = &LiteralTranslator{v.Values}
		} else if strings.ContainsAny(subData.Type.CGo(), "[]") {
			subData.Copy = true
		}
		data.Members = append(data.Members, subData)
	}
	return &struct {
		Template string
		Data     StructData
	}{"struct", data}
}

func unionTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     UnionData
} {
	data := UnionData{
		Name: &UnionTranslator{tiepuh.Name()},
	}
	for _, v := range tiepuh.StructMembers {
		data.Members = append(data.Members, UnionMemberData{
			Name: &NameTranslator{&ExportTranslator{&LiteralTranslator{v.Name}}},
			Type: helperMemberTypeTranslator(v.Type, v.Length, v.Raw, v.Comment),
		})
	}
	return &struct {
		Template string
		Data     UnionData
	}{"union", data}
}

var arrayNumericSizeRegexp *regexp.Regexp = regexp.MustCompile("\\[\\d+\\]")
var arrayConstSizeRegexp *regexp.Regexp = regexp.MustCompile("\\[[^\\]]+\\]")

func helperMemberTypeTranslator(specType, length, raw, comment string) Translator {
	if len(specType) == 0 {
		specType = "uint32_t"
	}
	if len(comment) > 0 {
		raw = strings.ReplaceAll(raw, fmt.Sprintf("<comment>%s</comment>", comment), "")
	}

	pointerCount := len(strings.Split(raw, "*")) - 1
	if pointerCount > 0 && specType == "void" {
		specType = "void*"
	}
	sliceCount := 0
	if len(length) > 0 {
		lengthValues := strings.Split(length, ",")
		sliceCount = len(lengthValues)
		for _, v := range lengthValues {
			v = strings.TrimSpace(v)
			if v == "null-terminated" {
				sliceCount = sliceCount - 1
			} else if v == "1" {
				sliceCount = sliceCount - 1
			}
		}
	}
	if len(strings.Split(raw, "[")) > 1 {
		sliceCount = sliceCount + 1
	}
	pointerCount = pointerCount - sliceCount
	if specType == "void*" {
		if pointerCount > 0 {
			pointerCount = pointerCount - 1
		} else {
			sliceCount = sliceCount - 1
		}
	}

	arraySize := arrayNumericSizeRegexp.FindString(raw)
	if sliceCount > 0 && len(arraySize) > 2 {
		sliceCount = sliceCount - 1
		arraySize = arraySize[1 : len(arraySize)-1]
	} else if arraySize = arrayConstSizeRegexp.FindString(raw); sliceCount > 0 && len(arraySize) > 2 {
		sliceCount = sliceCount - 1
		arraySize = arraySize[1 : len(arraySize)-1]
		arraySize = strip.StripTags(arraySize)
	} else {
		arraySize = ""
	}

	var typeTranslator Translator
	var typeConverter Converter
	if handleXl8r := GetHandleTranslator(specType); handleXl8r != nil {
		typeTranslator = handleXl8r
		typeConverter = handleXl8r
	} else if scalarXl8r := GetScalarTranslator(specType); scalarXl8r != nil {
		typeTranslator = scalarXl8r
		typeConverter = scalarXl8r
	} else {
		typeTranslator = &IdentifierTranslator{specType}
		typeConverter = &IdentifierTranslator{specType}
	}
	for h := 0; h < pointerCount; h++ {
		typeTranslator = &PointerTranslator{typeTranslator, typeConverter}
		typeConverter = &PointerTranslator{typeTranslator, typeConverter}
	}
	for h := 0; h < sliceCount; h++ {
		typeTranslator = &SliceTranslator{typeTranslator, typeConverter}
		typeConverter = &SliceTranslator{typeTranslator, typeConverter}
	}
	if len(arraySize) > 0 {
		typeTranslator = &ArrayTranslator{
			typeTranslator,
			&ValueTranslator{arraySize},
			typeConverter,
		}
	}

	return typeTranslator
}

func ConstToData(node *RegistryNode) *struct {
	Template string
	Data     []ConstData
} {
	list := make([]ConstData, 0, len(node.Parents))
	for _, v := range node.EnumsParents() {
		for _, v := range v.N.EnumParents() {
			var data *ConstData
			if len(v.E.Name) > 0 && len(v.E.Value) > 0 {
				var value = v.E.Value
				if (strings.HasSuffix(value, "F") || strings.HasSuffix(value, "f")) && !(strings.HasPrefix(value, "0x") || strings.HasPrefix(value, "0X")) {
					value = value[:len(value)-1]
				}
				if strings.HasPrefix(value, "(") && strings.HasSuffix(value, ")") {
					value = value[1 : len(value)-1]
				}
				var ulonglong bool
				if strings.HasSuffix(value, "ULL") || strings.HasSuffix(value, "ull") {
					ulonglong = true
					value = value[:len(value)-3]
				}
				var ulong bool
				if strings.HasSuffix(value, "U") || strings.HasSuffix(value, "u") {
					ulong = true
					value = value[:len(value)-1]
				}
				if strings.HasPrefix(value, "~") {
					if ulonglong {
						value = fmt.Sprintf("^uint64(%s)", value[1:])
					} else if ulong {
						value = fmt.Sprintf("^uint32(%s)", value[1:])
					} else {
						value = fmt.Sprintf("^%s", value[1:])
					}
				}
				data = &ConstData{
					Name:  &ExportTranslator{&IdentifierTranslator{v.E.Name}},
					Value: &ValueTranslator{value},
				}
			} else if len(v.E.Name) > 0 && len(v.E.Alias) > 0 {
				data = &ConstData{
					Name:  &ExportTranslator{&IdentifierTranslator{v.E.Name}},
					Value: &ExportTranslator{&IdentifierTranslator{v.E.Alias}},
				}
			}
			if data != nil {
				list = append(list, *data)
			}
		}
	}
	return &struct {
		Template string
		Data     []ConstData
	}{"const", list}
}
