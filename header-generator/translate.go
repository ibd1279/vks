package main

import (
	"fmt"
	"regexp"
	"strconv"
	"strings"

	strip "github.com/grokify/html-strip-tags-go"
)

type Translator interface {
	C() string
	CGo() string
	Go() string
}

type Converter interface {
	CToGo() string
	GoToC() string
}

// uint64_t vs unsigned long long vs. uint64
type ScalarTranslator struct {
	specType string // e.g. uint64_t
	cGoType  string // e.g. C.ulonglong
	goType   string // e.g. uint64
	cToGo    string // e.g. C.GoString
	goToC    string // e.g. C.CString
}

func (xl8r *ScalarTranslator) C() string   { return xl8r.specType }
func (xl8r *ScalarTranslator) CGo() string { return xl8r.cGoType }
func (xl8r *ScalarTranslator) Go() string  { return xl8r.goType }
func (xl8r *ScalarTranslator) CToGo() string {
	f := xl8r.cToGo
	if len(f) == 0 {
		f = xl8r.Go()
	}
	return fmt.Sprintf("func(x *%s) *%s { /* Scalar */ return (*%s)(unsafe.Pointer(x)) }", xl8r.CGo(), xl8r.Go(), f)
}
func (xl8r *ScalarTranslator) GoToC() string {
	f := xl8r.goToC
	if len(f) == 0 {
		f = xl8r.CGo()
	}
	return fmt.Sprintf("func(x *%s) *%s { /* Scalar */ return (*%s)(unsafe.Pointer(x)) }", xl8r.Go(), xl8r.CGo(), f)
}

var (
	Int8Translator          *ScalarTranslator = &ScalarTranslator{"int8_t", "C.schar", "int8", "", ""}
	Int16Translator                           = &ScalarTranslator{"int16_t", "C.short", "int16", "", ""}
	Int32Translator                           = &ScalarTranslator{"int32_t", "C.int", "int32", "", ""}
	Int64Translator                           = &ScalarTranslator{"int64_t", "C.longlong", "int64", "", ""}
	Uint8Translator                           = &ScalarTranslator{"uint8_t", "C.uchar", "byte", "", ""}
	Uint16Translator                          = &ScalarTranslator{"uint16_t", "C.ushort", "uint16", "", ""}
	Uint32Translator                          = &ScalarTranslator{"uint32_t", "C.uint", "uint32", "", ""}
	Uint64Translator                          = &ScalarTranslator{"uint64_t", "C.ulonglong", "uint64", "", ""}
	SizeTranslator                            = &ScalarTranslator{"size_t", "C.ulong", "uint32", "", ""}
	Float32Translator                         = &ScalarTranslator{"float", "C.float", "float32", "", ""}
	Float64Translator                         = &ScalarTranslator{"double", "C.double", "float64", "", ""}
	StringTranslator                          = &ScalarTranslator{"char", "C.char", "byte", "", ""}
	UnsafePointerTranslator                   = &ScalarTranslator{"void*", "unsafe.Pointer", "unsafe.Pointer", "", ""}

	scalarTranslatorList map[string]*ScalarTranslator = map[string]*ScalarTranslator{
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

func GetScalarTranslator(specType string) *ScalarTranslator {
	return scalarTranslatorList[specType]
}

// VkInstane vs C.VkInstance
type HandleTranslator struct {
	specValue string
}

func (xl8r *HandleTranslator) C() string   { return xl8r.specValue }
func (xl8r *HandleTranslator) CGo() string { return fmt.Sprintf("C.%s", xl8r.specValue) }
func (xl8r *HandleTranslator) Go() string  { return xl8r.specValue }
func (xl8r *HandleTranslator) CToGo() string {
	return fmt.Sprintf("/* Handle */ (*%s)", xl8r.Go())
}
func (xl8r *HandleTranslator) GoToC() string {
	return fmt.Sprintf("/* Handle */ (*%s)", xl8r.CGo())
}

var (
	handleTranslatorList map[string]*HandleTranslator = map[string]*HandleTranslator{}
)

func GetHandleTranslator(specType string) *HandleTranslator {
	return handleTranslatorList[specType]
}

// VK_SUCCESS vs VK_SUCCESS
type LiteralTranslator struct {
	specName string
}

func (xl8r *LiteralTranslator) C() string   { return xl8r.specName }
func (xl8r *LiteralTranslator) CGo() string { return xl8r.specName }
func (xl8r *LiteralTranslator) Go() string  { return xl8r.specName }

// 0x.8p-0 vs. 0x0.8fp-0
type ValueTranslator struct {
	specValue string
}

func (xl8r *ValueTranslator) C() string   { return xl8r.specValue }
func (xl8r *ValueTranslator) CGo() string { return xl8r.specValue }
func (xl8r *ValueTranslator) Go() string  { return xl8r.specValue }

// (1 << 3) vs (1 << 3)
type BitValueTranslator struct {
	bitPos int
}

func (xl8r *BitValueTranslator) C() string   { return xl8r.Go() }
func (xl8r *BitValueTranslator) CGo() string { return xl8r.Go() }
func (xl8r *BitValueTranslator) Go() string {
	return fmt.Sprintf("(1 << %d)", xl8r.bitPos)
}

// type vs. type_
type NameTranslator struct {
	orig Translator
}

func (xl8r *NameTranslator) C() string { return xl8r.orig.C() }
func (xl8r *NameTranslator) CGo() string {
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
func (xl8r *NameTranslator) Go() string {
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

// sType vs SType
type ExportTranslator struct {
	orig Translator
}

func (xl8r *ExportTranslator) C() string   { return xl8r.orig.C() }
func (xl8r *ExportTranslator) CGo() string { return xl8r.orig.CGo() }
func (xl8r *ExportTranslator) Go() string  { return strings.Title(xl8r.orig.Go()) }

type IdentifierTranslator struct {
	specValue string
}

func (xl8r *IdentifierTranslator) C() string   { return xl8r.specValue }
func (xl8r *IdentifierTranslator) CGo() string { return fmt.Sprintf("C.%s", xl8r.specValue) }
func (xl8r *IdentifierTranslator) Go() string  { return xl8r.specValue }
func (xl8r *IdentifierTranslator) CToGo() string {
	return fmt.Sprintf("/* Identifier */ (*%s)", xl8r.Go())
}
func (xl8r *IdentifierTranslator) GoToC() string {
	return fmt.Sprintf("/* Identifier */ (*%s)", xl8r.CGo())
}

// VkClearValue vs C.union_VkClearValue
type UnionTranslator struct {
	specValue string
}

func (xl8r *UnionTranslator) C() string   { return xl8r.specValue }
func (xl8r *UnionTranslator) CGo() string { return fmt.Sprintf("C.union_%s", xl8r.specValue) }
func (xl8r *UnionTranslator) Go() string  { return xl8r.specValue }

// VkClearAttachment vs C.struct_VkClearAttachment
type StructTranslator struct {
	specValue string
}

func (xl8r *StructTranslator) C() string   { return xl8r.specValue }
func (xl8r *StructTranslator) CGo() string { return fmt.Sprintf("C.struct_%s", xl8r.specValue) }
func (xl8r *StructTranslator) Go() string  { return xl8r.specValue }

// uint32_t* vs *uint32
type PointerTranslator struct {
	orig Translator
	conv Converter
}

func (xl8r *PointerTranslator) C() string   { return fmt.Sprintf("%s*", xl8r.orig.C()) }
func (xl8r *PointerTranslator) CGo() string { return fmt.Sprintf("*%s", xl8r.orig.CGo()) }
func (xl8r *PointerTranslator) Go() string  { return fmt.Sprintf("*%s", xl8r.orig.Go()) }
func (xl8r *PointerTranslator) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Pointer */ return (*%s)(unsafe.Pointer(x)) }", xl8r.CGo(), xl8r.Go(), xl8r.Go())
}
func (xl8r *PointerTranslator) GoToC() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Pointer */ return (*%s)(unsafe.Pointer(x)) }", xl8r.Go(), xl8r.CGo(), xl8r.CGo())
}

// uint32_t* vs []uint32
type SliceTranslator struct {
	orig Translator
	conv Converter
}

func (xl8r *SliceTranslator) C() string   { return fmt.Sprintf("%s*", xl8r.orig.C()) }
func (xl8r *SliceTranslator) CGo() string { return fmt.Sprintf("*%s", xl8r.orig.CGo()) }
func (xl8r *SliceTranslator) Go() string  { return fmt.Sprintf("[]%s", xl8r.orig.Go()) }
func (xl8r *SliceTranslator) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Slice */ slc := unsafe.Slice((*%s)(unsafe.Pointer(x)), (1 << 31)); return &slc }", xl8r.CGo(), xl8r.Go(), xl8r.orig.Go())
}
func (xl8r *SliceTranslator) GoToC() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Slice */ if len(*x) > 0 { slc := (%s)(unsafe.Pointer(&((*x)[0]))); return &slc }; var ptr unsafe.Pointer; return (*%s)(unsafe.Pointer((&ptr))) }", xl8r.Go(), xl8r.CGo(), xl8r.CGo(), xl8r.CGo())
}

// uint32_t[4] vs [4]uint32
type ArrayTranslator struct {
	orig Translator
	size Translator
	conv Converter
}

func (xl8r *ArrayTranslator) C() string { return fmt.Sprintf("%s[%s]", xl8r.orig.C(), xl8r.size.C()) }
func (xl8r *ArrayTranslator) CGo() string {
	return fmt.Sprintf("[%s]%s", xl8r.size.CGo(), xl8r.orig.CGo())
}
func (xl8r *ArrayTranslator) Go() string {
	return fmt.Sprintf("[]%s", xl8r.orig.Go())
}
func (xl8r *ArrayTranslator) CToGo() string {
	return fmt.Sprintf("func(x *%s) *%s { /* Array */ slc := unsafe.Slice((*%s)(unsafe.Pointer(x)), %s); return &slc }", xl8r.CGo(), xl8r.Go(), xl8r.orig.Go(), xl8r.size.Go())
}
func (xl8r *ArrayTranslator) GoToC() string {
	return fmt.Sprintf("func(x *%s) **%s { /* Array */ if len(*x) > 0 { slc := (*%s)(unsafe.Pointer(&((*x)[0]))); return &slc }; var ptr unsafe.Pointer; return (**%s)(unsafe.Pointer((&ptr))) }", xl8r.Go(), xl8r.orig.CGo(), xl8r.orig.CGo(), xl8r.orig.CGo())
}

/* const VK_MAX_PHYSICAL_DEVICE_NAME_SIZE uint32 256 */
type ConstData struct {
	Name  Translator // e.g. VK_MAX_PHYSICAL_DEVICE_NAME_SIZE
	Value Translator // e.g. 256
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
	Name  Translator // e.g. x
	Type  Translator // e.g. int32
	Value Translator
	Copy  bool
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
	if len(tiepuh.Alias) > 0 {
		data.Alias = &StructTranslator{tiepuh.Alias}
		return &struct {
			Template string
			Data     StructData
		}{"structalias", data}
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
