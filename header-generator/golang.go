package main

import (
	"fmt"
	"os"
	"text/template"
)

func GenerateGoFile(config *Config, constants *RegistryNode, graph RegistryGraph) error {
	fn := fmt.Sprintf("%s.go", config.OutputName)
	header := fmt.Sprintf("%s.h", config.OutputName)
	needsFacade := determineFacades(config, graph)

	var err error
	t := template.New(fn).Funcs(template.FuncMap{
		"hasProcAddr":  func(name Translator) bool { _, ok := config.ProcLoaders[name.C()]; return ok },
		"procAddrFunc": func(name Translator) string { return config.ProcLoaders[name.C()] },
		"isGlobal": func(name Translator) bool {
			return config.IsGlobalProc(name.C())
		},
		"ooParams": func(name Translator, params []CommandParamData) []CommandParamData {
			if config.IsGlobalProc(name.C()) {
				return params
			}
			return params[1:]
		},
		"needsFacade": func(name Translator) bool {
			return needsFacade[name.C()]
		},
	})
	if t, err = t.Parse(goPrimaryTemplate); err != nil {
		return err
	}
	if t, err = t.Parse(goSubTemplates); err != nil {
		return err
	}
	if t, err = t.Parse(goCommandTemplate); err != nil {
		return err
	}
	if t, err = t.Parse(goStructTemplate); err != nil {
		return err
	}

	data := append([]interface{}{}, ConstToData(constants))
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
	graph.DepthFirstSearch(config.Enabled(), store)

	var fh *os.File
	fh, err = os.OpenFile(fn, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, 0644)
	if err != nil {
		return err
	}
	defer fh.Close()

	err = t.Execute(fh, struct {
		PackageName string
		Header      string
		Data        []interface{}
	}{config.PackageName, header, data})
	if err != nil {
		return err
	}

	return nil
}

func determineFacades(config *Config, graph RegistryGraph) map[string]bool {
	facades := make(map[string]bool, 0)
	dependsOn := make(map[string]string, 0)
	f := func(path []*RegistryNode) {
		node := path[len(path)-1]
		switch node.NodeType {
		case RegistryNodeType:
			if tiepuh := node.TypeElement(); tiepuh != nil {
				if tiepuh.Category == TypeCategoryHandle && len(tiepuh.Parent) > 0 {
					dependsOn[tiepuh.Name()] = tiepuh.Parent
				}
			}
		case RegistryNodeCommand:
			if command := node.CommandElement(); command != nil {
				if len(command.Params) > 0 && !config.IsGlobalProc(command.Name()) {
					facades[command.Params[0].Type] = true
				}
			}
		}
	}
	graph.DepthFirstSearch(config.Enabled(), f)
	needsFacade := make(map[string]bool, len(facades))
	for current := range facades {
		for len(current) > 0 {
			needsFacade[current] = true
			current = dependsOn[current]
		}
	}
	return needsFacade
}

const goSubTemplates = `{{define "docurl"}}https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/{{.}}.html{{end}}{{define "const"}}// These are API constants.
const ({{range .}}
	{{.Name.Go}} = {{.Value.Go}}{{end}}
)
{{end}}{{define "version"}}// {{.Name.Go}} is an implementation of the Vulkan Make Api Version
// defines. See
// {{template "docurl" "VK_MAKE_API_VERSION"}}
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
{{end}}{{define "headerversion"}}// {{.HeaderVersionName.Go}} is the version of the vk specification used to generate this.
// {{template "docurl" .Name.C}}
const {{.HeaderVersionName.Go}} = {{.Value.Go}}
{{end}}{{define "base"}}// {{.Name.Go}} basetype
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Type.Go}}
{{end}}{{define "handle"}}// {{.Name.Go}} is a Handle to a vulkan resource.{{if .HasParent}}
// {{.Name.Go}} is a child of {{.ParentName.Go}}.{{end}}{{if needsFacade .Name}}
//
// Use {{if .HasParent}}{{.ParentName.GoFacade}}.{{end}}Make{{.Name.GoFacade}} to create a facade around this object to invoke methods.{{end}}
// 
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Name.CGo}}

// Null{{.Name.Go}} is a typed Null value for the {{.Name.Go}} type.
var Null{{.Name.Go}} {{.Name.Go}}{{if and (hasProcAddr .Name) (not .HasParent)}}

// Make{{.Name.GoFacade}}Facade provides a facade interface to the handle. It loads the proc
// addresses for the provided {{.Name.Go}} handle.
func Make{{.Name.GoFacade}}(x {{.Name.Go}}) {{.Name.GoFacade}} {
	var addrs C.vksProcAddr
	C.{{procAddrFunc .Name}}(x, &addrs)
	return {{.Name.GoFacade}}{
		H:     x,
		procs: &addrs,
	}
}{{else if and (hasProcAddr .Name) (needsFacade .Name)}}

// Make{{.Name.GoFacade}}provides a facade interface to the handle. It requires
// the parent facade for the proc address to load the new proc addresses.
func (parent {{.ParentName.GoFacade}}) Make{{.Name.GoFacade}}(x {{.Name.Go}}) {{.Name.GoFacade}} {
	var addrs C.vksProcAddr
	C.{{procAddrFunc .Name}}(x, &addrs, parent.procs)
	return {{.Name.GoFacade}}{
		H:     x,
		procs: &addrs,
	}
}{{else if needsFacade .Name}}

// Make{{.Name.GoFacade}}provides a facade interface to the handle. It requires
// the parent facade to copy the proc addresses.
func (parent {{.ParentName.GoFacade}}) Make{{.Name.GoFacade}}(x {{.Name.Go}}) {{.Name.GoFacade}} {
	return {{.Name.GoFacade}}{
		H:     x,
		procs: parent.procs,
	}
}{{end}}{{if needsFacade .Name}}

// {{.Name.GoFacade}} is a {{.Name.Go}} handle with the proc addresses pointer. It allows
// the invocation of methods against the handle. In the vulkan spec, these would be the functions
// that take the handle as the first parameter.
//
// Use the H field to get the handle back.
type {{.Name.GoFacade}} struct {
	H     {{.Name.Go}}   // The vulkan Handle
	procs *C.vksProcAddr // The addresses for commands.
}{{end}}

{{end}}{{define "enum"}}// {{.Name.Go}} enum/enums
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Type.Go}}{{if gt (len .Values) 0}}

const ({{range .Values}}
	{{.Name.Go}} {{.Type.Go}} = {{.Value.Go}}{{end}}
)

var (
	reverse{{.Name.Go}} map[{{.Name.Go}}]string = map[{{.Name.Go}}]string{ {{range .Values}}{{if not .Alias}}
		{{.Name.Go}}: "{{.Name.Go}}",{{end}}{{end}}
	}
)
func (x {{.Name.Go}}) String() string {
	if s, ok := reverse{{.Name.Go}}[x]; ok {
		return s
	}
	return fmt.Sprintf("{{.Name.Go}}=%d", x)
}{{end}}
{{end}}{{define "bitmask"}}// {{.Name.Go}} bitmask
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Type.Go}}
{{end}}{{define "func"}}// {{.Name.Go}} function pointer
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Name.CGo}}
{{end}}{{define "union"}}// {{.Name.Go}} union
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Name.CGo}}
{{end}}`
const goCommandTemplate = `{{define "command"}}// {{.Name.Go}} command
// {{template "docurl" .Name.C}}
func {{if eq (isGlobal .Name) false}}(x {{.Parent.GoFacade}}){{end}}{{.Name.Go}}({{range ooParams .Name .Parameters}}{{.Name.Go}} {{.Type.Go}}, {{end}}) {{if ne .Return.Go "void"}}{{.Return.Go}} {{end}}{
	addrs := {{if isGlobal .Name}}&C.vksProcAddresses{{else}}x.procs{{end}}{{$cmd := .Name}}{{range $key, $val := .Parameters}}
	p{{$key}} := {{$val.Type.GoToC}}(&{{if or (isGlobal $cmd) (ne $key 0)}}{{$val.Name.Go}}{{else}}x.H{{end}}){{end}}
	{{if ne .Return.Go "void"}}ret := {{end}}{{.Name.CGo}}(addrs{{range $key, $val := .Parameters}}, *p{{$key}}{{end}})
	{{if ne .Return.Go "void"}}retPtr := {{.Return.CToGo}}(&ret)
	return *retPtr
{{end}}}
{{end}}`
const goStructTemplate = `{{define "struct"}}// {{.Name.Go}} provides a go interface for {{.Name.C}}.
// {{template "docurl" .Name.C}}
type {{.Name.Go}} {{.Name.CGo}}

// Sizeof{{.Name.Go}} is the memory size of a {{.Name.Go}}
var Sizeof{{.Name.Go}} int = int(unsafe.Sizeof({{.Name.Go}}{}))

// Free releases the memory allocated by AsCPtr.
// It does not free pointers stored in the structure.
func (x *{{.Name.Go}}) Free() {
	C.free(unsafe.Pointer(x))
}

// AsCPtr copies the object to the C heap and returns the pointer.
// Free must be explicitly called on the returned pointer.
func (x {{.Name.Go}}) AsCPtr() *{{.Name.Go}} {
	clone := (*{{.Name.Go}})(newCBlock(C.ulong(Sizeof{{.Name.Go}})))
	*clone = x
	return clone
}

// {{.Name.Go}}FreeCSlice releases the memory allocated by {{.Name.Go}}MakeCSlice.
// It does not free pointers stored inside the slice.
func {{.Name.Go}}FreeCSlice(x []{{.Name.Go}}) {
	if len(x) > 0 {
		C.free(unsafe.Pointer(&x[0]))
	}
}

// {{.Name.Go}}MakeCSlice allocates memory for the passed arguments on the C heap,
// copies their values to the allocated memory, and creates a slice around the
// C memory. {{.Name.Go}}FreeCSlice must be called on the returned slice.
func {{.Name.Go}}MakeCSlice(x ...{{.Name.Go}}) []{{.Name.Go}} {
	if len(x) == 0 {
		return nil
	}
	sz := Sizeof{{.Name.Go}} * len(x)
	dst := unsafe.Slice((*{{.Name.Go}})(newCBlock(C.ulong(sz))), len(x))
	copy(dst, x)
	return dst
}{{with $struct := .}}{{range .Members}}

// {{.Name.Go}} returns the value of {{.Name.C}} from {{$struct.Name.C}}
func (x {{$struct.Name.Go}}) {{.Name.Go}}() {{.Type.Go}} {
	ptr := {{.Type.CToGo}}(&x.{{.Name.CGo}}) 
	return *ptr
}{{if ne .Value nil}}

// WithDefault{{.Name.Go}} sets the value of {{.Name.Go}} to the value provided in the
// specification. This method only exists if there is a single value in the specification.
func (x {{$struct.Name.Go}}) WithDefault{{.Name.Go}}() {{$struct.Name.Go}} {
	return x.With{{.Name.Go}}({{.Value.Go}})
}{{end}}{{if or (eq $struct.ReadOnly false) (or (eq .Name.Go "PNext") (eq .Name.Go "SType"))}}

// With{{.Name.Go}} sets the value for the {{.Name.Go}} on the underlying C structure.
// It performs whatever conversions are necessary to match the C API.{{if ne .Length nil}}
// 
// The specification defines {{.Length.Name.Go}} as the length of this field.
// {{.Length.Name.Go}} is updated with the length of the new value.{{end}}
func (x {{$struct.Name.Go}}) With{{.Name.Go}}(y {{.Type.Go}}) {{$struct.Name.Go}} { {{- if .Copy}}
	ptr := {{.Type.GoToC}}(&y)
	copy(x.{{.Name.CGo}}[:], unsafe.Slice(*ptr, len(y))){{else}}
	x.{{.Name.CGo}} = *({{.Type.GoToC}}(&y)){{end}}
	return x{{if ne .Length nil}}.With{{.Length.Name.Go}}({{.Length.Type.Go}}(len(y))){{end}}
}{{end}}{{end}}
{{end}}{{end}}
{{define "structalias"}}//{{.Name.Go}} is an alias to {{.Alias.Go}}.
// {{template "docurl" .Name.C}}
//
// Deprecated: Most Aliases in the Vulkan spec are for compatibility purposes as extensions get
// promoted to features. If possible, update code to use the promoted name: {{.Alias.Go}}.
type {{.Name.Go}} = {{.Alias.Go}}{{end}}
`
const goPrimaryTemplate = `package {{.PackageName}}

//#cgo LDFLAGS: -lvulkan
//#include <stdlib.h>
//#include <string.h>
//#include "{{.Header}}"
import "C"
import (
	"bytes"
	"fmt"
	"unsafe"
)

// Init loads the Vulkan library.
func Init() Result {
	ret := C.vksDynamicLoad()
	ptr := (*Result)(&ret)
	return *ptr
}

// Destroy unloads the Vulkan Library.
func Destroy() {
	C.vksDynamicUnload()
}

// NewCString allocates the provided string on the C heap. FreeCString must be
// called when the string is no longer needed.
func NewCString(s string) *byte {
	b := nullTerminatedBuffer(s).Bytes()
	ptr := C.malloc(C.ulong(len(b)))
	C.memcpy(ptr, unsafe.Pointer(&b[0]), C.ulong(len(b)))
	return (*byte)(ptr)
}

// FreeCString releases the C heap memory allocated by NewCString.
func FreeCString(ptr *byte) {
	if ptr != nil {
		C.free(unsafe.Pointer(ptr))
	}
}

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

func newCBlock(sz C.ulong) unsafe.Pointer {
	ptr := C.malloc(sz)
	C.memset(ptr, 0, sz)
	return ptr
}

func nullTerminatedBuffer(s string) *bytes.Buffer {
	var str bytes.Buffer
	var isNullTerminated bool
	for _, c := range ([]byte)(s) {
		if c == 0 {
			isNullTerminated = true
		}
		str.WriteByte(c)
		if isNullTerminated {
			break
		}
	}
	if !isNullTerminated {
		str.WriteByte(0)
	}
	return &str
}
`
