package main

import (
	"fmt"
	"log"
	"regexp"
	"strconv"
	"strings"

	strip "github.com/grokify/html-strip-tags-go"
)

// Translator interface provides a set of rules for how to translate the
// specification type informaiton into names for other situations.
type Translator interface {
	C() string   // C name.
	CGo() string // Cgo name.
	Go() string  // Go name.
}

// LiteralTranslator is used for values that are constants or expected to
// be the same in all outputs. For example 198 or (1 << 4) or VK_SUCCESS.
type LiteralTranslator struct {
	specName string
}

func (xl8r *LiteralTranslator) C() string   { return xl8r.specName }
func (xl8r *LiteralTranslator) CGo() string { return xl8r.specName }
func (xl8r *LiteralTranslator) Go() string  { return xl8r.specName }

// BitValueTranslator is outputs 1 shifted by enough bits to represent
// the required bitposition in the bit mask.
type BitValueTranslator struct {
	bitPos int
}

func (xl8r *BitValueTranslator) C() string   { return xl8r.Go() }
func (xl8r *BitValueTranslator) CGo() string { return xl8r.Go() }
func (xl8r *BitValueTranslator) Go() string  { return fmt.Sprintf("(1 << %d)", xl8r.bitPos) }

// ReservedWordTranslator deals with un exported names (think parameter names)
// that are reserved words, like type, range, and func
type ReservedWordTranslator struct {
	orig Translator
}

func (xl8r *ReservedWordTranslator) C() string { return xl8r.orig.C() }
func (xl8r *ReservedWordTranslator) CGo() string {
	name := xl8r.orig.CGo()
	if name == "type" {
		name = "_type"
	} else if name == "range" {
		name = "_range"
	} else if name == "func" {
		name = "_func"
	}
	return name
}
func (xl8r *ReservedWordTranslator) Go() string {
	name := xl8r.orig.Go()
	if name == "type" {
		name = "type_"
	} else if name == "range" {
		name = "range_"
	} else if name == "func" {
		name = "func_"
	}
	return name
}

// CamelCaseTranslator converts uppercase defines into More go friendly values.
// For example, VK_API_VERSION vs VkApiVersion.
type CamelCaseTranslator struct {
	specName string
}

func (xl8r *CamelCaseTranslator) C() string   { return xl8r.specName }
func (xl8r *CamelCaseTranslator) CGo() string { return xl8r.specName }
func (xl8r *CamelCaseTranslator) Go() string {
	src := xl8r.specName
	if strings.Contains(src, "_") {
		lower := strings.ToLower(src)
		parts := strings.Split(lower, "_")
		for h := 1; h < len(parts); h++ {
			parts[h] = strings.Title(parts[h])
		}
		return strings.Join(parts, "")
	}
	return src
}

// TypeDefTranslator provides the C. prefix for the CGo translation. It is
// generally used for things like Handles, where the typedef isn't a struct or
// a union.
type TypeDefTranslator struct {
	specName string
}

func (xl8r *TypeDefTranslator) C() string   { return xl8r.specName }
func (xl8r *TypeDefTranslator) CGo() string { return fmt.Sprintf("C.%s", xl8r.specName) }
func (xl8r *TypeDefTranslator) Go() string  { return xl8r.specName }

// StructTranslator provides the C.struct_ prefix for CGo translations. It is
// used for struct types. This could probably be replaced by typedef, as
// the vulkan header typedef's all the structs. Keeping it mostly for semantic
// reasons.
type StructTranslator struct {
	specName string
}

func (xl8r *StructTranslator) C() string   { return xl8r.specName }
func (xl8r *StructTranslator) CGo() string { return fmt.Sprintf("C.struct_%s", xl8r.specName) }
func (xl8r *StructTranslator) Go() string  { return xl8r.specName }

// ExportTranslator runs a series of rules over the Go name to prepare it
// to be part of the exported API. The rules are added to `TranslatorRules`
// in the order they are expected to be executed.
type ExportTranslator struct {
	orig Translator
}

func (xl8r *ExportTranslator) C() string   { return xl8r.orig.C() }
func (xl8r *ExportTranslator) CGo() string { return xl8r.orig.CGo() }
func (xl8r *ExportTranslator) Go() string {
	src := xl8r.orig.Go()
	for _, v := range TranslatorRules {
		src = v.Translate(src)
	}
	return src
}

// ExportTranslatorRule provides an interface for defining different
// rules for the ExportTranslator. If a rule does apply, it should return
// the original, it returns the original string.
type ExportTranslatorRule interface {
	Translate(string) string
}

// TranslatorRules are the configured list of rules for the ExportTranslator
var TranslatorRules []ExportTranslatorRule

// exportTranslatorRuleDeprefix removes the provided prefix if a word has it.
type exportTranslatorRuleDeprefix struct {
	prefix string
}

func (etRule exportTranslatorRuleDeprefix) Translate(src string) string {
	if strings.HasPrefix(src, etRule.prefix) {
		return src[len(etRule.prefix):]
	}
	return src
}

// exportTranslatorRuleTitle makes the first character of a string uppercase.
type exportTranslatorRuleTitle struct{}

func (etRule exportTranslatorRuleTitle) Translate(src string) string {
	return strings.Title(src)
}

// Converter interface is an extension of the Translator interface. It
// provides code for converting from one type to another type.
type Converter interface {
	Translator
	CToGo() string
	GoToC() string
}

// ScalarConverter provides conversions for the common C scalar types to
// Go types.
type ScalarConverter struct {
	specType string // e.g. uint64_t
	cGoType  string // e.g. C.ulonglong
	goType   string // e.g. uint64
	cToGo    string // e.g. C.GoString
	goToC    string // e.g. C.CString
}

func (xl8r *ScalarConverter) C() string   { return xl8r.specType }
func (xl8r *ScalarConverter) CGo() string { return xl8r.cGoType }
func (xl8r *ScalarConverter) Go() string  { return xl8r.goType }
func (xl8r *ScalarConverter) CToGo() string {
	f := xl8r.cToGo
	if len(f) == 0 {
		f = xl8r.Go()
	}
	return fmt.Sprintf("func(x *%s) *%s { /* Scalar */ return (*%s)(unsafe.Pointer(x)) }", xl8r.CGo(), xl8r.Go(), f)
}
func (xl8r *ScalarConverter) GoToC() string {
	f := xl8r.goToC
	if len(f) == 0 {
		f = xl8r.CGo()
	}
	return fmt.Sprintf("func(x *%s) *%s { /* Scalar */ return (*%s)(unsafe.Pointer(x)) }", xl8r.Go(), xl8r.CGo(), f)
}

var (
	Int8Translator          *ScalarConverter = &ScalarConverter{"int8_t", "C.schar", "int8", "", ""}
	Int16Translator                          = &ScalarConverter{"int16_t", "C.short", "int16", "", ""}
	Int32Translator                          = &ScalarConverter{"int32_t", "C.int", "int32", "", ""}
	Int64Translator                          = &ScalarConverter{"int64_t", "C.longlong", "int64", "", ""}
	Uint8Translator                          = &ScalarConverter{"uint8_t", "C.uchar", "byte", "", ""}
	Uint16Translator                         = &ScalarConverter{"uint16_t", "C.ushort", "uint16", "", ""}
	Uint32Translator                         = &ScalarConverter{"uint32_t", "C.uint", "uint32", "", ""}
	Uint64Translator                         = &ScalarConverter{"uint64_t", "C.ulonglong", "uint64", "", ""}
	SizeTranslator                           = &ScalarConverter{"size_t", "C.ulong", "uint64", "", ""}
	Float32Translator                        = &ScalarConverter{"float", "C.float", "float32", "", ""}
	Float64Translator                        = &ScalarConverter{"double", "C.double", "float64", "", ""}
	StringTranslator                         = &ScalarConverter{"char", "C.char", "byte", "", ""}
	UnsafePointerTranslator                  = &ScalarConverter{"void*", "unsafe.Pointer", "unsafe.Pointer", "", ""}
)

// TypeDefConverter provides converter methods that convert from
// compatible types. For example a C.VkInstance to a go Instance.
// which is just a cast.
type TypeDefConverter struct {
	orig Translator
}

func (xl8r *TypeDefConverter) C() string   { return xl8r.orig.C() }
func (xl8r *TypeDefConverter) CGo() string { return xl8r.orig.CGo() }
func (xl8r *TypeDefConverter) Go() string  { return xl8r.orig.Go() }
func (xl8r *TypeDefConverter) CToGo() string {
	return fmt.Sprintf("/* typedef */ (*%s)", xl8r.Go())
}
func (xl8r *TypeDefConverter) GoToC() string {
	return fmt.Sprintf("/* typedef */ (*%s)", xl8r.CGo())
}

// PointerConverter wraps an existing translator to be treated like a pointer.
// It is mostly used in struct memebers and command parameters.
type PointerConverter struct {
	orig Translator
}

func (xl8r *PointerConverter) C() string   { return fmt.Sprintf("%s*", xl8r.orig.C()) }
func (xl8r *PointerConverter) CGo() string { return fmt.Sprintf("*%s", xl8r.orig.CGo()) }
func (xl8r *PointerConverter) Go() string  { return fmt.Sprintf("*%s", xl8r.orig.Go()) }
func (xl8r *PointerConverter) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Pointer */ return (*%s)(unsafe.Pointer(x)) }", xl8r.CGo(), xl8r.Go(), xl8r.Go())
}
func (xl8r *PointerConverter) GoToC() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Pointer */ return (*%s)(unsafe.Pointer(x)) }", xl8r.Go(), xl8r.CGo(), xl8r.CGo())
}

// SliceConverter wraps an existing translator to be treated like a slice.
// It is mostly used in struct memebers and command parameters.
type SliceConverter struct {
	orig Translator
}

func (xl8r *SliceConverter) C() string   { return fmt.Sprintf("%s*", xl8r.orig.C()) }
func (xl8r *SliceConverter) CGo() string { return fmt.Sprintf("*%s", xl8r.orig.CGo()) }
func (xl8r *SliceConverter) Go() string  { return fmt.Sprintf("[]%s", xl8r.orig.Go()) }
func (xl8r *SliceConverter) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Slice */ slc := unsafe.Slice((*%s)(unsafe.Pointer(x)), (1 << 31)); return &slc }", xl8r.CGo(), xl8r.Go(), xl8r.orig.Go())
}
func (xl8r *SliceConverter) GoToC() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Slice */ if len(*x) > 0 { slc := (%s)(unsafe.Pointer(&((*x)[0]))); return &slc }; var ptr unsafe.Pointer; return (*%s)(unsafe.Pointer((&ptr))) }", xl8r.Go(), xl8r.CGo(), xl8r.CGo(), xl8r.CGo())
}

// ArrayConverter wraps an existing translator to be treated like an array.
// It is mostly used in struct members.
type ArrayConverter struct {
	orig Translator
	size Translator
}

func (xl8r *ArrayConverter) C() string { return fmt.Sprintf("%s[%s]", xl8r.orig.C(), xl8r.size.C()) }
func (xl8r *ArrayConverter) CGo() string {
	return fmt.Sprintf("[%s]%s", xl8r.size.CGo(), xl8r.orig.CGo())
}
func (xl8r *ArrayConverter) Go() string {
	return fmt.Sprintf("[]%s", xl8r.orig.Go())
}
func (xl8r *ArrayConverter) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Array */ slc := unsafe.Slice((*%s)(unsafe.Pointer(x)), %s); return &slc }", xl8r.CGo(), xl8r.Go(), xl8r.orig.Go(), xl8r.size.Go())
}
func (xl8r *ArrayConverter) GoToC() string {
	return fmt.Sprintf("func(x *%s) **%s { /* Array */ if len(*x) > 0 { slc := (*%s)(unsafe.Pointer(&((*x)[0]))); return &slc }; var ptr unsafe.Pointer; return (**%s)(unsafe.Pointer((&ptr))) }", xl8r.Go(), xl8r.orig.CGo(), xl8r.orig.CGo(), xl8r.orig.CGo())
}

var (
	cachedTranslatorMap map[string]Translator = map[string]Translator{
		"int8_t":   Int8Translator,
		"int16_t":  Int16Translator,
		"int32_t":  Int32Translator,
		"int64_t":  Int64Translator,
		"uint8_t":  Uint8Translator,
		"uint16_t": Uint16Translator,
		"uint32_t": Uint32Translator,
		"uint64_t": Uint64Translator,
		"size_t":   SizeTranslator,
		"float":    Float32Translator,
		"double":   Float64Translator,
		"char":     StringTranslator,
		"void*":    UnsafePointerTranslator,
	}
)

func CachedConverter(specType string) (Converter, bool) {
	if entry, ok := CachedTranslator(specType); ok {
		if converter, ok := entry.(Converter); ok {
			return converter, true
		}
	}
	return nil, false
}

func CachedTranslator(specType string) (Translator, bool) {
	entry, ok := cachedTranslatorMap[specType]
	return entry, ok
}

func GetScalarConverter(specType string) Converter {
	if converter, ok := CachedConverter(specType); ok {
		return converter
	}
	panic(fmt.Errorf("Unknown Scalar type requested: %v", specType))
}

func GetCommandTranslator(specType string) Translator {
	if entry, ok := CachedTranslator(specType); ok {
		return entry
	}
	entry := &ExportTranslator{&TypeDefTranslator{specType}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetDefineTranslator(specType string) Translator {
	if entry, ok := CachedTranslator(specType); ok {
		return entry
	}
	entry := &ExportTranslator{&CamelCaseTranslator{specType}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetBaseConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetHandleConverter(specType string) Converter {
	if converter, ok := CachedConverter(specType); ok {
		return converter
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetEnumNameConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetEnumValueConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetFuncConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetStructConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&StructTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func GetUnionConverter(specType string) Converter {
	if entry, ok := CachedConverter(specType); ok {
		return entry
	}
	entry := &TypeDefConverter{&ExportTranslator{&TypeDefTranslator{specType}}}
	cachedTranslatorMap[specType] = entry
	return entry
}

func CacheConverters(path []*RegistryNode) {
	node := path[len(path)-1]
	switch node.NodeType {
	case RegistryNodeType:
		if tiepuh := node.TypeElement(); tiepuh != nil {
			switch tiepuh.Category {
			case TypeCategoryDefine:
				GetDefineTranslator(tiepuh.Name())
			case TypeCategoryBasetype:
				GetBaseConverter(tiepuh.Name())
			case TypeCategoryHandle:
				GetHandleConverter(tiepuh.Name())
			case TypeCategoryEnum:
				GetEnumNameConverter(tiepuh.Name())
			case TypeCategoryBitmask:
				GetEnumNameConverter(tiepuh.Name())
			case TypeCategoryFuncpointer:
				GetFuncConverter(tiepuh.Name())
			case TypeCategoryStruct:
				GetStructConverter(tiepuh.Name())
			case TypeCategoryUnion:
				GetUnionConverter(tiepuh.Name())
			}
		}
	case RegistryNodeCommand:
		if command := node.CommandElement(); command != nil {
			GetCommandTranslator(command.Name())
		}
	}
}

/* const VK_MAX_PHYSICAL_DEVICE_NAME_SIZE uint32 256 */
type ConstData struct {
	Name  Translator // e.g. VK_MAX_PHYSICAL_DEVICE_NAME_SIZE
	Value Translator // e.g. 256
}

/* type VkVersion uint32 */
type DefineData struct {
	Name              Translator // e.g. VkVersion
	HeaderVersionName Translator // e.g. VK_HEADER_VERSION
	Value             Translator // e.g. 0, 1, 2 or 198
}

/* type VkBool32 uint32 */
type BaseData struct {
	Type Translator // e.g. uint32
	Name Translator // e.g. VkBool32
}

/* type VkInstance uintptr */
type HandleData struct {
	Name Translator // e.g. VkInstance
}

/* type VkResult uint32 */
type EnumData struct {
	Type   Translator // e.g. uint32
	Name   Translator // e.g. VkResult or VK_SUCCESS
	Values []EnumValueData
}

// const (VK_SUCCESS VkResult = 0)
type EnumValueData struct {
	Type  Translator // e.g. uint32
	Name  Translator // e.g. VkResult or VK_SUCCESS
	Value Translator // e.g. 0
	Alias bool
}

// type PFN_vkFreeFunction unsafe.Pointer
type FuncData struct {
	Type Translator // e.g. unsafe.Pointer
	Name Translator // e.g. PFN_vkFreeFunction
}

// type VkOffset2D struct {x int32; y int32}
type StructData struct {
	Name     Translator // e.g. VkOffset2D
	Members  []StructMemberData
	ReadOnly bool
	Alias    Translator
}

// x int32
type StructMemberData struct {
	Name   Translator // e.g. x
	Type   Translator // e.g. int32
	Value  Translator
	Copy   bool
	Length interface{}
}

// type VkClearValue C.VkClearValue
type UnionData struct {
	Name    Translator // e.g. VkClearValue
	Members []UnionMemberData
}

// VkClearColorValue color
type UnionMemberData struct {
	Name Translator // e.g. color
	Type Translator // e.g. VkClearColorValue
}

// func VkGetInstanceProcAddr(instance VkInstance, pName string) PFN_vkVoidFunction
type CommandData struct {
	Name       Translator // e.g. VkGetInstanceProcAddr
	Return     Translator // e.g. PFN_vkVoidFunction
	Parameters []CommandParamData
}

// ... (instance VkInstance, pName string) ...
type CommandParamData struct {
	Name Translator // e.g. pName
	Type Translator // e.g. string
}

func CommandToData(node *RegistryNode, command CommandElement) (bool, interface{}) {
	if len(command.Alias()) > 0 {
		// TODO add support for Aliases later.
		return false, nil
	} else {
		var returnTranslator Translator
		// Cheating a little bit as the spec does not currently have any
		// commands that return a pointer
		if tXl8r, ok := CachedTranslator(command.Proto.Type); ok {
			returnTranslator = tXl8r
		} else {
			returnTranslator = helperMemberTypeTranslator(command.Proto.Type, "", command.Proto.Raw, "")
		}
		data := CommandData{
			Name:   GetCommandTranslator(command.Name()),
			Return: returnTranslator,
		}

		for _, v := range command.Params {
			data.Parameters = append(data.Parameters, CommandParamData{
				Name: &ReservedWordTranslator{&LiteralTranslator{v.Name}},
				Type: helperMemberTypeTranslator(v.Type, v.Length, v.Raw, ""),
			})
		}

		return true, &struct {
			Template string
			Data     CommandData
		}{"command", data}
	}
	return false, nil
}

func TypeToData(node *RegistryNode, tiepuh TypeElement) (bool, interface{}) {
	switch tiepuh.Category {
	case TypeCategoryDefine:
		tmp := defineTypeToData(node, tiepuh)
		return tmp != nil, defineTypeToData(node, tiepuh)
	case TypeCategoryBasetype:
		tmp := baseTypeToData(node, tiepuh)
		return tmp != nil, baseTypeToData(node, tiepuh)
	case TypeCategoryHandle:
		tmp := handleTypeToData(node, tiepuh)
		return tmp != nil, handleTypeToData(node, tiepuh)
	case TypeCategoryEnum:
		tmp := enumTypeToData(node, tiepuh)
		return tmp != nil, enumTypeToData(node, tiepuh)
	case TypeCategoryBitmask:
		tmp := enumTypeToData(node, tiepuh)
		return tmp != nil, enumTypeToData(node, tiepuh)
	case TypeCategoryFuncpointer:
		tmp := funcTypeToData(node, tiepuh)
		return tmp != nil, funcTypeToData(node, tiepuh)
	case TypeCategoryStruct:
		tmp := structTypeToData(node, tiepuh)
		return tmp != nil, structTypeToData(node, tiepuh)
	case TypeCategoryUnion:
		tmp := unionTypeToData(node, tiepuh)
		return tmp != nil, unionTypeToData(node, tiepuh)
	}
	return false, nil
}

var (
	// list of defines that are intentionally ignored because they don't apply
	// to go, or are covered by the implementation of another define.
	defineSkip map[string]bool = map[string]bool{
		"VK_MAKE_VERSION":                   true, // deprecated
		"VK_VERSION_MAJOR":                  true, // deprecated
		"VK_VERSION_MINOR":                  true, // deprecated
		"VK_VERSION_PATCH":                  true, // deprecated
		"VK_API_VERSION":                    true, // deprecated
		"VK_DEFINE_HANDLE":                  true, // doesn't apply to go.
		"VK_USE_64_BIT_PTR_DEFINES":         true, // doesn't apply to go.
		"VK_NULL_HANDLE":                    true, // in the handle template.
		"VK_DEFINE_NON_DISPATCHABLE_HANDLE": true, // doesn't apply to go?
		"VK_API_VERSION_VARIANT":            true, // in the version template.
		"VK_API_VERSION_MAJOR":              true, // in the version template.
		"VK_API_VERSION_MINOR":              true, // in the version template.
		"VK_API_VERSION_PATCH":              true, // in the version template.
		"VK_API_VERSION_1_0":                true, // in the version template.
		"VK_API_VERSION_1_1":                true, // in the version template.
		"VK_API_VERSION_1_2":                true, // in the version template.
		"VK_MAKE_API_VERSION":               true, // in the version template.
	}
	headerVersionRegexp *regexp.Regexp = regexp.MustCompile("(\\d+)$")
	vulkanVersionRegexp *regexp.Regexp = regexp.MustCompile("\\((\\d+, \\d+, \\d+), VK_HEADER_VERSION\\)$")
)

func defineTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     DefineData
} {
	if defineSkip[tiepuh.Name()] {
		return nil
	}

	t := "version"
	data := DefineData{
		Name:              GetDefineTranslator("VK_API_VERSION"),
		HeaderVersionName: GetDefineTranslator("VK_HEADER_VERSION"),
	}
	switch tiepuh.Name() {
	case "VK_HEADER_VERSION":
		vals := headerVersionRegexp.FindStringSubmatch(tiepuh.Raw)
		if len(vals) < 2 {
			log.Printf("encountered a %s that could not be parsed / %s", tiepuh.Name(), tiepuh.Raw)
			return nil
		}
		data.Value = &LiteralTranslator{vals[1]}
		t = "headerversion"
	case "VK_HEADER_VERSION_COMPLETE":
		vals := vulkanVersionRegexp.FindStringSubmatch(tiepuh.Raw)
		if len(vals) < 2 {
			log.Printf("encountered a %s that could not be parsed / %s", tiepuh.Name(), tiepuh.Raw)
			return nil
		}
		data.Value = &LiteralTranslator{vals[1]}
	default:
		log.Printf("Encountered a define that is not handled %s", tiepuh.Name())
		return nil
	}

	return &struct {
		Template string
		Data     DefineData
	}{t, data}
}

func baseTypeToData(node *RegistryNode, tiepuh TypeElement) *struct {
	Template string
	Data     BaseData
} {
	if len(tiepuh.TypeTag) == 0 {
		return nil
	}
	data := BaseData{
		Type: GetScalarConverter(tiepuh.TypeTag),
		Name: GetBaseConverter(tiepuh.Name()),
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
	if t, ok := CachedConverter(tiepuh.Name()); ok {
		data.Name = t
	} else {
		data.Name = GetHandleConverter(tiepuh.Name())
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
		Name: GetEnumNameConverter(tiepuh.Name()),
	}

	templateName := "enum"
	var bitmask bool
	if tiepuh.Category == TypeCategoryBitmask {
		templateName = "bitmask"
		if len(tiepuh.TypeTag) > 0 {
			if t, ok := CachedTranslator(tiepuh.TypeTag); ok {
				ed.Type = t
			}
		}
		bitmask = true
	}
	for _, v := range node.EnumsParents() {
		var largestValue, smallestValue float64
		if v.E.Type == EnumsElementBitmask {
			bitmask = true
		}
		for _, v := range v.N.EnumParents() {
			evd := EnumValueData{
				Type: ed.Name,
				Name: GetEnumValueConverter(v.E.Name),
			}
			var valueTranslator Translator
			if len(v.E.Alias) > 0 {
				evd.Alias = true
				if tXl8r, ok := CachedTranslator(v.E.Alias); ok {
					valueTranslator = tXl8r
				} else {
					valueTranslator = &LiteralTranslator{v.E.Alias}
				}
			} else if bitmask {
				if v.E.BitPos == 0 && len(v.E.Value) > 0 {
					valueTranslator = &LiteralTranslator{v.E.Value}
				} else {
					if float64(uint64(1)<<v.E.BitPos) > largestValue {
						largestValue = float64(uint64(1) << v.E.BitPos)
					} else if float64(uint64(1)<<v.E.BitPos) < smallestValue {
						smallestValue = float64(uint64(1) << v.E.BitPos)
					}
					valueTranslator = &BitValueTranslator{v.E.BitPos}
				}
			} else {
				valueTranslator = &LiteralTranslator{v.E.Value}
				val, _ := strconv.ParseFloat(v.E.Value, 64)
				if val > largestValue {
					largestValue = val
				} else if val < smallestValue {
					smallestValue = val
				}
			}
			evd.Value = valueTranslator
			ed.Values = append(ed.Values, evd)
		}

		// I should look up how golang deals with constants to avoid this noise.
		if smallestValue < -0.5 && largestValue < float64((^uint32(0))>>1) {
			ed.Type = Int32Translator
		} else if smallestValue > -0.5 && largestValue > float64(^uint32(0)) {
			ed.Type = Uint64Translator
		} else if smallestValue < -0.5 && largestValue > float64((^uint32(0))>>1) {
			ed.Type = Int64Translator
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
		Type: GetScalarConverter("void*"),
		Name: GetFuncConverter(tiepuh.Name()),
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
		Name:     GetStructConverter(tiepuh.Name()),
		ReadOnly: tiepuh.ReturnedOnly,
	}
	if len(tiepuh.Alias) > 0 {
		data.Alias = GetStructConverter(tiepuh.Alias)
		return &struct {
			Template string
			Data     StructData
		}{"structalias", data}
	}

	lengthLookup := make(map[string]*StructMemberData, 0)
	for _, v := range tiepuh.StructMembers {
		subData := StructMemberData{
			Name: &ExportTranslator{&ReservedWordTranslator{&LiteralTranslator{v.Name}}},
			Type: helperMemberTypeTranslator(v.Type, v.Length, v.Raw, v.Comment),
		}
		if len(v.Values) > 0 && len(strings.Split(v.Values, ",")) == 1 && v.Name == "sType" {
			subData.Value = &LiteralTranslator{v.Values}
		} else if strings.ContainsAny(subData.Type.CGo(), "[]") {
			subData.Copy = true
		}
		data.Members = append(data.Members, subData)
		lengthLookup[v.Name] = &data.Members[len(data.Members)-1]
	}
	for k, v := range tiepuh.StructMembers {
		if len(v.Length) > 0 && v.Length != "1" && len(strings.Split(v.Length, ",")) == 1 {
			if ptr, ok := lengthLookup[v.Length]; ok && data.Members[k].Type.Go() != "unsafe.Pointer" {
				data.Members[k].Length = ptr
			}
		}
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
		Name: GetUnionConverter(tiepuh.Name()),
	}
	for _, v := range tiepuh.StructMembers {
		data.Members = append(data.Members, UnionMemberData{
			Name: &ExportTranslator{&LiteralTranslator{v.Name}},
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

	var typeConverter Converter
	if cXl8r, ok := CachedConverter(specType); ok {
		typeConverter = cXl8r
	} else if tXl8r, ok := CachedTranslator(specType); ok {
		//log.Printf("Investigate why %v does not have a converter.", specType)
		typeConverter = &TypeDefConverter{tXl8r}
	} else {
		//log.Printf("Investigate why %v does not have a cached converter or translator.", specType)
		typeConverter = &TypeDefConverter{&TypeDefTranslator{specType}}
	}

	for h := 0; h < pointerCount; h++ {
		typeConverter = &PointerConverter{typeConverter}
	}
	for h := 0; h < sliceCount; h++ {
		typeConverter = &SliceConverter{typeConverter}
	}
	if len(arraySize) > 0 {
		typeConverter = &ArrayConverter{
			typeConverter,
			&LiteralTranslator{arraySize},
		}
	}

	return typeConverter
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
					Name:  GetBaseConverter(v.E.Name),
					Value: &LiteralTranslator{value},
				}
			} else if len(v.E.Name) > 0 && len(v.E.Alias) > 0 {
				data = &ConstData{
					Name:  GetBaseConverter(v.E.Name),
					Value: GetBaseConverter(v.E.Alias),
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
