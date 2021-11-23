package main

import (
	"fmt"
	"strings"
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
