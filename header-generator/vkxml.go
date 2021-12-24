package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"sort"
	"strings"
)

// Registry contains the entire definition of one or more related APIs.
// Version 1.2.198, 2021-11-09 08:03:38Z
type Registry struct {
	XMLName xml.Name `xml:"registry"`
	// Comment contains arbitrary text, such as a copyright statement.
	Comment []string `xml:"comment,omitempty"`
	// Platforms defines platform names corresponding to platform-specific
	// API extensions.
	Platforms []PlatformElement `xml:"platforms>platform"`
	// Tags defines author IDs used for extensions and layers. Author IDs
	// are described in detail in the “Layers & Extensions” section of the
	// “Vulkan Documentation and Extensions: Procedures and Conventions”
	// document.
	Tags []TagElement `xml:"tags>tag"`
	// Types defines API types. Usually only one tag is used.
	Types []TypeElement `xml:"types>type"`
	// Enums defines API token names and values. Usually multiple tags are
	// used. Related groups may be tagged as an enumerated type
	// corresponding to a type tag, and resulting in a C enum declaration.
	// This ability is heavily used in the Vulkan API.
	Enums []EnumsElement `xml:"enums"`
	// Commands defines API commands (functions). Usually only one tag is
	// used.
	Commands []CommandElement `xml:"commands>command"`
	// Features defines API feature interfaces (API versions, more or less).
	// One tag per feature set.
	Features []FeatureElement `xml:"feature"`
	// Extensions defines API extension interfaces. Usually only one tag is
	// used, wrapping many extensions.
	Extensions []ExtensionElement `xml:"extensions>extension"`
}

// PlatformElement contains descriptions of platform IDs for platforms supported
// by window system-specific extensions to Vulkan.
type PlatformElement struct {
	// Name is required. The platform name. This must be a short alphanumeric
	// string corresponding to the platform name, valid as part of a C99
	// identifier. Lower-case is preferred.
	Name string `xml:"name,attr"`
	// Protect is required. This must be a C99 preprocessor token beginning
	// with VK_USE_PLATFORM_ followed by the platform name, converted to
	// upper case, followed by _ and the extension suffix of the
	// corresponding window system-specific extension supporting the platform.
	Protect string `xml:"protect,attr"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
}

// TagElement contains authorid tags describing reserved author IDs used by
// extension and layer authors.
type TagElement struct {
	// Name is required. The author ID, as registered with Khronos. A short,
	// upper-case string, usually an abbreviation of an author, project or
	// company name.
	Name string `xml:"name,attr"`
	// Author is required. The author name, such as a full company or project
	// name.
	Author string `xml:"author,attr"`
	// Contact is required. The contact who registered or is currently
	// responsible for extensions and layers using the ID, including sufficient
	// contact information to reach the contact such as individual name together
	// with email address, Github username, or other contact information.
	Contact string `xml:"contact,attr"`
}

// TypeElement contains information which can be used to generate C code
// corresponding to the type. In many cases, this is simply legal C code, with
// attributes or embedded tags denoting the type name and other types used in
// defining this type.
type TypeElement struct {
	// Requires is optional. Another type name this type requires to complete
	// its definition.
	Requires string `xml:"requires,attr"`
	// NameAttr is optional if the name is defined in the tag body. Name of
	// this type.
	NameAttr string `xml:"name,attr,omitempty"`
	// Alias is optional. Another type name which this type is an alias of. Must
	// match the name of another type element.
	Alias string `xml:"alias,attr,omitempty"`
	// Api is optional API names for which this definition is specialized, so
	// that different APIs may have different definitions for the same type.
	Api string `xml:"api,attr,omitempty"`
	// Category is optional. A string which indicates that this type contains a
	// more complex structured definition.
	Category TypeCategory `xml:"category,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,omitempty"`
	// Parent is only applicable if "category" is handle. Notes another type
	// with the handle category that acts as a parent object for this type.
	Parent string `xml:"parent,attr,omitempty"`
	// ReturnedOnly is only applicable if "category" is struct or union. Notes
	// that this struct/union is going to be filled in by the API, rather than
	// an application filling it out and passing it to the API.
	ReturnedOnly bool `xml:"returnedonly,attr,omitempty"`
	// StructExtends is only applicable if category is struct or union. This is
	// a comma-separated list of structures whose pNext can include this type.
	StructExtends string `xml:"structextends,attr,omitempty"`
	// AllowDuplicate is only applicable if category is "struct". If "true",
	// then structures whose pNext chains include this structure may include
	// more than one instance of it.
	AllowDuplicate bool `xml:"allowduplicate,attr,omitempty"`
	// ObjTypeEnum is only applicable at present if category is "handle".
	// Specifies the name of a VkObjectType enumerant which corresponds to this
	// type.
	ObjTypeEnum string `xml:"objtypeenum,attr,omitempty"`
	// StructMembers defining the members of the aggregate type, in order,
	// interleaved with any number of comment tags.
	StructMembers []TypeMemberElement `xml:"member"`
	// TypeTag is nested type tags contain other type names which are required
	// by the definition of this type.
	TypeTag string `xml:"type,omitempty"`
	// ApiEntry insert a platform calling convention macro here during header
	// generation, used mostly for function pointer types.
	ApiEntry string `xml:"apientry,omitempty"`
	// NameTag contains the name of this type (if not defined in the tag attributes).
	NameTag string `xml:"name"`
	// BitValues contains the name of the enumeration defining flag values for a
	// bitmask type. Ignored for other types.
	BitValues string `xml:"bitvalues,omitempty"`
	// Raw is if the category attribute is one of basetype, bitmask, define,
	// funcpointer, group, handle or include, or is not specified, type contains
	// text which is legal C code for a type declaration.
	Raw string `xml:",innerxml"`
}

func (typeElement TypeElement) Name() string {
	if len(typeElement.NameTag) > 0 {
		return typeElement.NameTag
	}
	return typeElement.NameAttr
}

func (typeElement TypeElement) SpecializedName() string {
	parts := []string{typeElement.Name()}
	if len(typeElement.Api) > 0 {
		parts = append(parts, typeElement.Api)
	}
	return strings.Join(parts, "::")
}

type TypeCategory string

var (
	TypeCategoryBasetype    TypeCategory = "basetype"
	TypeCategoryBitmask     TypeCategory = "bitmask"
	TypeCategoryDefine      TypeCategory = "define"
	TypeCategoryEnum        TypeCategory = "enum"
	TypeCategoryFuncpointer TypeCategory = "funcpointer"
	TypeCategoryGroup       TypeCategory = "group"
	TypeCategoryHandle      TypeCategory = "handle"
	TypeCategoryInclude     TypeCategory = "include"
	TypeCategoryStruct      TypeCategory = "struct"
	TypeCategoryUnion       TypeCategory = "union"

	TypeCategoryOrder map[TypeCategory]int = map[TypeCategory]int{
		TypeCategoryInclude:     0,
		TypeCategoryDefine:      1,
		TypeCategoryBasetype:    2,
		TypeCategoryHandle:      3,
		TypeCategoryEnum:        4,
		TypeCategoryGroup:       5,
		TypeCategoryBitmask:     6,
		TypeCategoryFuncpointer: 7,
		TypeCategoryStruct:      8,
		TypeCategoryUnion:       9,
	}

	TypeCategoryTerminal map[TypeCategory]bool = map[TypeCategory]bool{
		TypeCategoryInclude: true,
		TypeCategoryEnum:    true,
	}
)

type TypeMemberElement struct {
	// Values is only valid on the sType member of a struct. This is a
	// comma-separated list of enumerant values that are valid for the
	// structure type.
	Values string `xml:"values,attr,omitempty"`
	// Length is for array members. Length may be one or more of the following
	// things, separated by commas (one for each array indirection): another
	// member of that struct; "null-terminated" for a string; "1" to indicate
	// it is just a pointer (used for nested pointers); or an equation in math
	// markup for incorporation in the specification.
	Length string `xml:"len,attr,omitempty"`
	// AltLength is for array members where the length is a math equation. This
	// attribute should be specified with an equivalent equation using only C
	// builtin operators, C math library function names, and variables as
	// allowed for len. It must be a valid C99 expression whose result is equal
	// to len for all possible inputs.
	AltLength string `xml:"altlen,attr,omitempty"`
	// ExternSync denotes that the member should be externally synchronized
	// when accessed by Vulkan.
	ExternSync string `xml:"externsync,attr,omitempty"`
	// Optional is optional. This attribute determines whether this member can
	// be omitted by providing NULL (for pointers), VK_NULL_HANDLE
	// (for handles), or 0 (for other scalar types). If the member is a pointer
	// to one of those types, multiple values may be provided, separated by
	// commas - one for each pointer indirection.
	Optional string `xml:"optional,attr,omitempty"`
	// Selector is optional. If the member is a union, selector identifies
	// another member of the struct that is used to select which of that
	// union’s members are valid.
	Selector string `xml:"selector,attr,omitempty"`
	// Selection is optional. For a member of a union, selection identifies a
	// value of the selector that indicates this member is valid.
	Selection string `xml:"selection,attr,omitempty"`
	// NoAutoValidity prevents automatic validity language being generated for
	// the tagged item.
	NoAutoValidity string `xml:"noautovalidity,attr,omitempty"`
	// LimitType is only applicable for members of VkPhysicalDeviceProperties
	// and VkPhysicalDeviceProperties2, their substrucutres, and extensions.
	// Specifies the type of a device limit.
	LimitType TypeMemberLimitType `xml:"limittype,attr,omitempty"`
	// ObjectType is only applicable for members which are uint64_t values
	// representing a Vulkan obejct handle. Specifies the name of another member
	// which must be a VkObjectType or VkDebugReportObjectTypeEXT value
	// specifying the type of object the handle refers to.
	ObjectType string `xml:"objecttype,attr,omitempty"`
	// Type is optional. It contains text which is a valid type name found in
	// another type tag, and indicates that this type must be previously
	// defined for the definition of the command to succeed. Builtin C types
	// should not be wrapped in type tags.
	Type string `xml:"type"`
	// Name is required and contains the struct/union member name being
	// described.
	Name string `xml:"name"`
	// Enum is optional. It contains text which is a valid enumerant name found
	// in another type tag, and indicates that this enumerant must be previously
	// defined for the definition of the command to succeed.
	Enum string `xml:"enum,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,omitempty"`
	// Raw is the text elements of a member tag, with all other tags removed, is a
	// legal C declaration of a struct or union member.
	Raw string `xml:",innerxml"`
}

type TypeMemberLimitType string

var (
	TypeMemberLimitTypeMin     TypeMemberLimitType = "min"
	TypeMemberLimitTypeMax     TypeMemberLimitType = "max"
	TypeMemberLimitTypeBitmask TypeMemberLimitType = "bitmask"
	TypeMemberLimitTypeRange   TypeMemberLimitType = "range"
	TypeMemberLimitTypeStruct  TypeMemberLimitType = "struct"
	TypeMemberLimitTypeNoAuto  TypeMemberLimitType = "noauto"
)

// EnumsElement contains individual enum tags describing each of the token names
// used in the API. In some cases these correspond to a C enum, and in some cases
// they are simply compile time constants (e.g. #define).
type EnumsElement struct {
	// Name is optional. String naming the C enum type whose members are defined
	// by this enum group. If present, this attribute should match the name
	// attribute of a corresponding type tag.
	Name string `xml:"name,attr,omitempty"`
	// Type is optional. String describing the data type of the values of this
	// group of enums. At present the only accepted categories are enum and
	// bitmask
	Type EnumsElementType `xml:"type,attr,omitempty"`
	// Start is optional and defines the start of a reserved range of enumerants
	// for a particular vendor or purpose. start must be less than or equal to
	// end. These fields define formal enumerant allocations, and are made by
	// the Khronos Registrar on request from implementors following the enum
	// allocation policy
	Start int `xml:"start,attr,omitempty"`
	// end is optional and defines the end of a reserved range of enumerants
	// for a particular vendor or purpose. start must be less than or equal to
	// end.
	End int `xml:"end,attr,omitempty"`
	// Vendor is optional. String describing the vendor or purpose to whom a
	// reserved range of enumerants is allocated.
	Vendor string `xml:"vendor,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
	// BitWidth is optional. Bit width required for the generated enum value
	// type. If omitted, a default value of 32 is used.
	BitWidth int `xml:"bitwidth,attr,omitempty"`
	// Enums contains the tokens defined.
	Enums []EnumElement `xml:"enum,omitempty"`
}

type EnumsElementType string

var (
	EnumsElementEnum    EnumsElementType = "enum"
	EnumsElementBitmask EnumsElementType = "bitmask"
)

// EnumElement defines a single Vulkan (or other API) token.
type EnumElement struct {
	// Value is value in the form of a legal C expression when evaluated at
	// compile time in the generated header files. Exactly one of value and
	// bitpos must be present in an enum tag.
	Value string `xml:"value,attr,omitempty"`
	// BitPos is a literal integer bit position in a bitmask. Exactly one of
	// value and bitpos must be present in an enum tag.
	BitPos int `xml:"bitpos,attr,omitempty"`
	// Name is required. Enumerant name, a legal C preprocessor token name.
	Name string `xml:"name,attr"`
	// Api is optional API names for which this definition is specialized, so
	// that different APIs may have different values for the same token.
	Api string `xml:"api,attr,omitempty"`
	// Type may be used only when value is specified. If present the attribute
	// must be a C scalar type corresponding to the type of value, although only
	// uint32_t, uint64_t, and float are currently meaningful.
	TypeAttr string `xml:"type,attr,omitempty"`
	// Alias is optional. Name of another enumerant this is an alias of. An
	// enumerant alias is simply a different name for the exact same value or
	// bitpos.
	Alias string `xml:"alias,attr,omitempty"`
	// Protect is optional. An additional preprocessor token used to protect an
	// enum definition.
	Protect string `xml:"protect,attr,omitempty"`
}

func (enumElement EnumElement) SpecializedName() string {
	parts := []string{enumElement.Name}
	if len(enumElement.Api) > 0 {
		parts = append(parts, enumElement.Api)
	}
	return strings.Join(parts, "::")
}

func (enumElement EnumElement) Type() string {
	if len(enumElement.TypeAttr) > 0 {
		return enumElement.TypeAttr
	}
	return "uint32_t"
}

// CommandElement contains definitions of each of the functions (commands) used in
// the API.
type CommandElement struct {
	// Queues is optional. A string identifying the command queues this command
	// can be placed on. The format of the string is one or more of the terms
	// "compute", "transfer", and "graphics", with multiple terms separated by
	// commas (",").
	Queues string `xml:"queues,attr,omitempty"`
	// SuccessCodes is optional. A string describing possible successful return
	// codes from the command, as a comma-separated list of Vulkan result code
	// names.
	SuccessCodes string `xml:"successcodes,attr,omitempty"`
	// ErrorCodes is optional. A string describing possible error return codes
	// from the command, as a comma-separated list of Vulkan result code names.
	ErrorCodes string `xml:"errorcodes,attr,omitempty"`
	// RenderPass is optional. A string identifying whether the command can be
	// issued only inside a render pass ("inside"), only outside a render pass
	// ("outside"), or both ("both").
	RenderPass string `xml:"renderpass,attr,omitempty"`
	// CmdBufferLevel is optional. A string identifying the command buffer
	// levels that this command can be called by. The format of the string is
	// one or more of the terms "primary" and "secondary", with multiple terms
	// separated by commas (",").
	CmdBufferLevel string `xml:"cmdbufferlevel,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
	// NameAttr is required for Alias commands. A string naming the command defined
	// by the tag.
	NameAttr string `xml:"name,attr,omitempty"`
	// Alias is required for Alias commands. A string naming the command that
	// name is an alias of.
	AliasAttr string `xml:"alias,attr,omitempty"`
	// Proto is required. It is a tag defining the C function prototype of a
	// command, up to the function name and return type but not including
	// function parameters.
	Proto CommandProtoElement `xml:"proto"`
	// Params is optional. If a command takes not arguments, it will have no
	// params.
	Params []CommandParamElement `xml:"param,omitempty"`
	// AliasTag is optional, and is another way to Alias commands.
	AliasTag string `xml:"alias,omitempty"`
	// Description is optional. Unused text.
	Description string `xml:"description,omitempty"`
	// ImplicitExternSyncParams is optional. Contains a list of param tags, each
	// containing asciidoc source text describing an object which is not a
	// parameter of the command but is related to one, and which also requires
	// external synchronization.
	ImplicitExternSyncParams []string `xml:"implicitexternsyncparams>param,omitempty"`
}

func (commandElement CommandElement) Alias() string {
	if len(commandElement.AliasAttr) > 0 {
		return commandElement.AliasAttr
	}
	return commandElement.AliasTag
}

func (commandElement CommandElement) Name() string {
	if len(commandElement.NameAttr) > 0 && len(commandElement.Alias()) > 0 {
		return commandElement.NameAttr
	}
	return commandElement.Proto.Name
}

type CommandQueueType string

var (
	CommandQueueCompute  CommandQueueType = "compute"
	CommandQueueTransfer CommandQueueType = "transfer"
	CommandQueueGraphics CommandQueueType = "graphics"
)

type CommandRenderPassType string

var (
	CommandRenderPassInside  CommandRenderPassType = "inside"
	CommandRenderPassOutside CommandRenderPassType = "outside"
	CommandRenderPassBoth    CommandRenderPassType = "both"
)

type CommandBufferLevelType string

var (
	CommandBufferLevelPrimary   CommandBufferLevelType = "primary"
	CommandBufferLevelSecondary CommandBufferLevelType = "secondary"
)

// CommandProtoElement defines the return type and name of a command.
type CommandProtoElement struct {
	// Type is optional, and contains text which is a valid type name found in
	// a type tag. It indicates that this type must be previously defined for
	// the definition of the command to succeed. Builtin C types, and any
	// derived types which are expected to be found in other header files,
	// should not be wrapped in type tags.
	Type string `xml:"type"`
	// Name is required and contains the command name being described.
	Name string `xml:"name"`
	// Raw is the text elements of a proto tag. With all other tags removed,
	// it is legal C code describing the return type and name of a command.
	Raw string `xml:",innerxml"`
}

type CommandParamElement struct {
	// Length is for array parameters. Length may be one or more of the following
	// things, separated by commas (one for each array indirection): another
	// parameter of that command; "null-terminated" for a string; "1" to indicate
	// it is just a pointer (used for nested pointers); or an equation in math
	// markup for incorporation in the specification.
	Length string `xml:"len,attr,omitempty:"`
	// AltLength is for array parameters where the length is a math equation. This
	// attribute should be specified with an equivalent equation using only C
	// builtin operators, C math library function names, and variables as
	// allowed for len. It must be a valid C99 expression whose result is equal
	// to len for all possible inputs.
	AltLength string `xml:"altlen,attr,omitempty"`
	// Optional is optional. This attribute determines whether this parameter can
	// be omitted by providing NULL (for pointers), VK_NULL_HANDLE
	// (for handles), or 0 (for other scalar types). If the parameter is a pointer
	// to one of those types, multiple values may be provided, separated by
	// commas - one for each pointer indirection.
	Optional string `xml:"optional,attr,omitempty"`
	// Selector is optional. If the parameter is a union, selector identifies
	// another parameter of the command that is used to select which of that
	// union’s members are valid.
	Selector string `xml:"selector,attr,omitempty"`
	// NoAutoValidity prevents automatic validity language being generated for
	// the tagged item.
	NoAutoValidity string `xml:"noautovalidity,attr,omitempty"`
	// ExternSync denotes that the parameter should be externally synchronized
	// when accessed by Vulkan.
	ExternSync string `xml:"externsync,attr,omitempty"`
	// ObjectType is only applicable for members which are uint64_t values
	// representing a Vulkan obejct handle. Specifies the name of another member
	// which must be a VkObjectType or VkDebugReportObjectTypeEXT value
	// specifying the type of object the handle refers to.
	ObjectType string `xml:"objecttype,attr,omitempty"`
	// Type is optional, and contains text which is a valid type name found in
	// a type tag. It indicates that this type must be previously defined for
	// the definition of the command to succeed. Builtin C types, and any
	// derived types which are expected to be found in other header files,
	// should not be wrapped in type tags.
	Type string `xml:"type"`
	// Name is required and contains the parameter name being described.
	Name string `xml:"name"`
	// Raw is the text elements of a param tag. With all other tags removed,
	// it is legal C code describing the parameter.
	Raw string `xml:",innerxml"`
}

// A feature is the set of interfaces (enumerants and commands) defined by a
// particular API and version, such as Vulkan 1.0, and includes all profiles of
// that API and version.
type FeatureElement struct {
	// Api is required and contains the API names this feature is defined for,
	// such as "vulkan".
	Api string `xml:"api,attr"`
	// Name is required. Version name, used as the C preprocessor token under
	// which the version’s interfaces are protected against multiple inclusion.
	Name string `xml:"name,attr"`
	// Number is required. Feature version number, usually a string interpreted
	// as majorNumber.minorNumber.
	Number string `xml:"number,attr"`
	// SortOrder is optional. A decimal number which specifies an order relative
	// to other feature tags when calling output generators. Defaults to 0
	SortOrder int `xml:"sortorder,attr,omitempty"`
	// Protect is optional. An additional preprocessor token used to protect a
	// feature definition. Usually another feature or extension name.
	Protect string `xml:"protect,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
	// Require is optional and describes the set of interfaces that is required
	// for this feature.
	Requires []RequireElement `xml:"require,omitempty"`
	// Remove is optional and describes the set of interfaces to remove from
	// this feature.
	Removes []RequireElement `xml:"remove,omitempty"`
}

func (featureElement FeatureElement) SpecializedName() string {
	parts := []string{featureElement.Name}
	if len(featureElement.Api) > 0 {
		parts = append(parts, featureElement.Api)
	}
	return strings.Join(parts, "::")
}

// ExtensionElement is the set of interfaces defined by a particular API extension
// specification. Extension is similar to Feature, but instead of having version and
// profile attributes, instead has a supported attribute, which describes the set of
// API names which the extension can potentially be implemented against.
type ExtensionElement struct {
	// Name is required. Version name, used as the C preprocessor token under
	// which the version’s interfaces are protected against multiple inclusion.
	Name string `xml:"name,attr"`
	// Number is required. Extension version number.
	Number int `xml:"number,attr"`
	// SortOrder is optional. A decimal number which specifies an order relative
	// to other feature tags when calling output generators. Defaults to 0
	SortOrder int `xml:"sortorder,attr,omitempty"`
	// Author is optional. The author name, such as a full company name. If not
	// present, this can be taken from the corresponding tag attribute. However,
	// EXT and other multi-vendor extensions may not have a well-defined author
	// or contact in the tag.
	Author string `xml:"author,attr,omitempty"`
	// Contact is optional. The contact who registered or is currently
	// responsible for extensions and layers using the tag, including sufficient
	// contact information to reach the contact such as individual name together
	// with Github username (@username), Khronos internal Gitlab username
	// (gitlab:@username) if no public Github contact is available, or other
	// contact information.
	Contact string `xml:"contact,attr,omitempty"`
	// Type is required if the supported attribute is not 'disabled'. Must be
	// either 'device' or 'instance', if present.
	Type string `xml:"type,attr,omitempty"`
	// RequiresAttr is optional. Comma-separated list of extension names this
	// extension requires to be supported. Extensions whose type is 'instance'
	// must not require extensions whose type is 'device'.
	RequiresAttr string `xml:"requires,attr,omitempty"`
	// RequiresCore is optional. Core version of Vulkan required by the
	// extension. Defaults to 1.0
	RequiresCore string `xml:"requiresCore,attr,omitempty"`
	// Protect is optional. An additional preprocessor token used to protect a
	// feature definition. Usually another feature or extension name.
	Protect string `xml:"protect,attr,omitempty"`
	// Platform is optional. Indicates that the extension is specific to the
	// platform identified by the attribute value, and should be emitted
	// conditional on that platform being available, in a platform-specific
	// header, etc. The attribute value must be the same as one of the platform
	// name attribute values.
	Platform string `xml:"platform,attr,omitempty"`
	// Supported is required API names this extension is defined for. When the
	// extension tag is just reserving an extension number, use
	// supported="disabled" to indicate this extension should never be processed.
	Supported string `xml:"supported,attr,omitempty"`
	// PromotedTo is optional. A Vulkan version or a name of an extension that
	// this extension was promoted to.
	PromotedTo string `xml:"promotedto,attr,omitempty"`
	// DeprecatedBy is optional. A Vulkan version or a name of an extension that
	// deprecates this extension. It may be an empty string.
	DeprecatedBy string `xml:"deprecatedby,attr,omitempty"`
	// ObsoletedBy is optional. A Vulkan version or a name of an extension that
	// obsoletes this extension. It may be an empty string.
	ObsoletedBy string `xml:"obsoletedby,attr,omitempty"`
	// Provisional is optional. 'true' if this extension is released provisionally.
	Provisional bool `xml:"provisional,attr,omitempty"`
	// SpecialUse is optional. If present, must contain one or more tokens
	// separated by commas, indicating a special purpose of the extension.
	SpecialUse string `xml:"specialuse,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
	// Require is optional and describes the set of interfaces that is required
	// for this extension.
	Requires []RequireElement `xml:"require,omitempty"`
	// Remove is optional and describes the set of interfaces to remove from
	// this extension.
	Removes []RequireElement `xml:"remove,omitempty"`
}

type ExtensionSpecialUseType string

var (
	ExtensionSpecialUseCadsupport   ExtensionSpecialUseType = "cadSupport"
	ExtensionSpecialUseD3demulation ExtensionSpecialUseType = "d3demulation"
	ExtensionSpecialUseDebugging    ExtensionSpecialUseType = "debugging"
	ExtensionSpecialUseDevtools     ExtensionSpecialUseType = "devtools"
	ExtensionSpecialUseGlemulation  ExtensionSpecialUseType = "glemulation"
)

// RequireElement is a definition of some scope to require or remove from a feature
// or extension.
type RequireElement struct {
	// Profile is optional. String name of an API profile. Interfaces in the
	// tag are only required (or removed) if the specified profile is being
	// generated.
	Profile string `xml:"profile,attr,omitempty"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
	// Api is optional API names requiring or removing these interfaces.
	// Interfaces in the tag are only required (or removed) if the requested
	// API name matches the attribute. If not specified, interfaces are required
	// (or removed) for all APIs.
	Api string `xml:"api,attr,omitempty"`
	// Extension is optional, and only for require tags. String containing an
	// API extension name.
	Extension string `xml:"extension,attr,omitempty"`
	// Feature is optional, and only for require tags. String containing an API
	// feature name.
	Feature string `xml:"feature,attr,omitempty"`
	// Commands is the referenced commands.
	Commands []RequireReferenceElement `xml:"command,omitempty"`
	// Enums is the referenced or extended enums
	Enums []RequireEnumElement `xml:"enum,omitempty"`
	// Types is the referenced types.
	Types []RequireReferenceElement `xml:"type,omitempty"`
}

// RequiredReferenceElement is an reference to another element from inside the
// require or remove block.
type RequireReferenceElement struct {
	// Name is required. Name of the referenced entity.
	Name string `xml:"name,attr"`
	// Comment is optional. Arbitrary string.
	Comment string `xml:"comment,attr,omitempty"`
}

// RequireEnumElement is a reference or extension of an existing enum.
type RequireEnumElement struct {
	EnumElement
	// Extends is the name of a separately defined enumerated type (e.g. a type
	// tag with category="enum") to which the extension enumerant is added.
	Extends string `xml:"extends,attr,omitempty"`
	// ExtensionNumber is an extension number. The extension number in turn
	// specifies the starting value of a block (range) of values reserved for
	// enumerants defined by or associated with the corresponding extension tag
	// with the same number.
	ExtensionNumber int `xml:"extnumber,attr,omitempty"`
	// Offset is the offset within an extension block. If extnumber is not
	// present, the extension number defining that block is given by the number
	// attribute of the surrounding extension tag.
	Offset int `xml:"offset,attr,omitempty"`
	// Direction, if present, is the calculated enumerant value will be
	// negative, instead of positive. Negative enumerant values are normally
	// used only for Vulkan error codes. The attribute value must be specified
	// as dir="-".
	Direction string `xml:"dir,attr,omitempty"`
}

// DecodeRegistry Unmarshals a Registry from a reader.
func DecodeRegistry(r io.Reader) (*Registry, error) {
	xmlDecoder := xml.NewDecoder(r)
	var registry Registry
	if err := xmlDecoder.Decode(&registry); err != nil {
		return nil, err
	}
	return &registry, nil
}

// Graph generates a dependency graph based on the Vulkan XML.
func (registry *Registry) Graph() (RegistryGraph, *RegistryNode) {
	dictionary := make(RegistryGraph, len(registry.Types))
	constants := NewVirtualNode()

	// Define the dictionary of nodes.
	define := func(name string, nodeType RegistryNodeElementType, element interface{}) {
		if _, ok := dictionary[name]; ok {
			panic("name reused.")
		}
		dictionary[name] = &RegistryNode{NodeType: nodeType, Element: element}
	}
	for _, v := range registry.Platforms {
		define(v.Name, RegistryNodePlatform, v)
	}
	for _, v := range registry.Types {
		define(v.SpecializedName(), RegistryNodeType, v)
	}
	for _, v := range registry.Enums {
		for _, enum := range v.Enums {
			define(enum.SpecializedName(), RegistryNodeEnum, enum)
		}
	}
	for _, v := range registry.Commands {
		define(v.Name(), RegistryNodeCommand, v)
	}
	for _, v := range registry.Features {
		define(v.SpecializedName(), RegistryNodeFeature, v)
	}
	for _, v := range registry.Extensions {
		define(v.Name, RegistryNodeExtension, v)
	}

	// Link the nodes together.
	link := func(self *RegistryNode, parent string) {
		if node, ok := dictionary[parent]; ok {
			self.AddParent(node)
		}
	}
	for _, v := range registry.Types {
		self := dictionary[v.SpecializedName()]
		link(self, v.Requires)
		link(self, v.Alias)
		link(self, v.Parent)
		for _, member := range v.StructMembers {
			link(self, member.Type)
			link(self, member.Enum)
		}
		link(self, v.ObjTypeEnum)
		link(self, v.TypeTag)
	}
	for _, v := range registry.Enums {
		child := dictionary[v.Name]
		if child == nil {
			child = constants
		}
		self := &RegistryNode{
			NodeType: RegistryNodeEnums,
			Element:  v,
		}
		child.AddParent(self)
		for _, enum := range v.Enums {
			value := dictionary[enum.SpecializedName()]
			link(self, enum.SpecializedName())
			link(value, enum.Alias)
		}
	}
	for _, v := range registry.Commands {
		self := dictionary[v.Name()]
		link(self, v.Alias())
		link(self, v.Proto.Type)
		for _, param := range v.Params {
			link(self, param.Type)
		}
	}

	// Link the high level features.
	for _, v := range registry.Features {
		self := dictionary[v.SpecializedName()]
		for _, require := range v.Requires {
			for _, command := range require.Commands {
				link(self, command.Name)
			}
			for _, tiepuh := range require.Types {
				link(self, tiepuh.Name)
			}
			for _, enum := range require.Enums {
				if len(enum.Extends) > 0 {
					link(self, enum.Extends)
				}
				link(self, enum.EnumElement.SpecializedName())
			}
		}
		// TODO Removes to go here.
	}
	for _, v := range registry.Extensions {
		self := dictionary[v.Name]
		for _, require := range v.Requires {
			for _, command := range require.Commands {
				link(self, command.Name)
			}
			for _, tiepuh := range require.Types {
				link(self, tiepuh.Name)
			}
			for _, enum := range require.Enums {
				if len(enum.Extends) > 0 {
					link(self, enum.Extends)
				}
				link(self, enum.EnumElement.SpecializedName())
			}
		}
		// TODO Removes to go here.
		for _, require := range strings.Split(v.RequiresAttr, ",") {
			link(self, strings.TrimSpace(require))
		}
		link(self, v.Platform)
		requiresCore := "1.0"
		if len(v.RequiresCore) > 0 {
			requiresCore = v.RequiresCore
		}
		supported := "vulkan"
		if len(v.Supported) > 0 {
			supported = v.Supported
		}
		for _, feature := range registry.Features {
			if feature.Api == supported && feature.Number == requiresCore {
				link(self, feature.SpecializedName())
			}
		}
	}
	return dictionary, constants
}

// RegistryNode is a wrapper for registry nodes to allow them to be used as a graph
// rather than as an XML document.
type RegistryNode struct {
	// NodeType is the type of node, representing the type, command, enum, etc.
	// types present in the Vulkan Registry XML.
	NodeType RegistryNodeElementType
	// Element is a reference to the actual node in the registry. Use the helper
	// functions to return the node back into an XML Element.
	Element interface{}
	// Parents represent the nodes this node depends on. For example, the
	// VkResult type would have a parent of the VkResult enum. The VkResult enum
	// would have parents on the enum value of VK_SUCCESS, etc.
	Parents RegistryNodeParents
}

// RegistryNodeParents is a slice type that implements the interfaces required by
// sort.Sort. The implemented sorting order is based on RegistryNodeElementType.
// Type elements are subsorted based on the order defined in TypeCategoryOrder.
type RegistryNodeParents []*RegistryNode

func (parents RegistryNodeParents) Len() int      { return len(parents) }
func (parents RegistryNodeParents) Swap(a, b int) { parents[a], parents[b] = parents[b], parents[a] }
func (parents RegistryNodeParents) Less(a, b int) bool {
	if parents[a].NodeType == parents[b].NodeType && parents[a].NodeType == RegistryNodeType {
		at, bt := parents[a].TypeElement(), parents[b].TypeElement()
		return TypeCategoryOrder[at.Category] < TypeCategoryOrder[bt.Category]
	}
	return parents[a].NodeType < parents[b].NodeType
}

// NewVirtualNode creates a new virtual node. A Virtual node is a node that doesn't
// reference a vulkan XML element. It is used to create a custom node that has
// hand selected parents.
func NewVirtualNode() *RegistryNode {
	return &RegistryNode{
		NodeType: RegistryNodeVirtual,
	}
}

// AddParent adds a parent to the RegistryNode. It effectively creates the edges
// in the graph.
func (registryNode *RegistryNode) AddParent(node *RegistryNode) {
	registryNode.Parents = append(registryNode.Parents, node)
}

// PlatformElement returns the Element as a Registry PlatformElement or nil.
func (registryNode RegistryNode) PlatformElement() *PlatformElement {
	if registryNode.NodeType == RegistryNodePlatform {
		if obj, ok := registryNode.Element.(PlatformElement); ok {
			return &obj
		}
	}
	return nil
}

// TypeElement returns the Element as a Registry TypeElement or nil.
func (registryNode RegistryNode) TypeElement() *TypeElement {
	if registryNode.NodeType == RegistryNodeType {
		if obj, ok := registryNode.Element.(TypeElement); ok {
			return &obj
		}
	}
	return nil
}

// EnumsElement returns the Element as a Registry EnumsElement or nil. This
// only works on nodes that are not part of the graph, but are part of how
// Enums are stored in the graph.
func (registryNode RegistryNode) EnumsElement() *EnumsElement {
	if registryNode.NodeType == RegistryNodeEnums {
		if obj, ok := registryNode.Element.(EnumsElement); ok {
			return &obj
		}
	}
	return nil
}

// EnumsParents returns a slice of the parents that could be successfully
// converted into a EnumsElement
func (registryNode RegistryNode) EnumsParents() (parents []struct {
	N *RegistryNode
	E EnumsElement
}) {
	for _, n := range registryNode.Parents {
		if e := n.EnumsElement(); e != nil {
			parents = append(parents, struct {
				N *RegistryNode
				E EnumsElement
			}{n, *e})
		}
	}
	return
}

// EnumElement returns the Element as a Registry EnumElement or nil.
func (registryNode RegistryNode) EnumElement() *EnumElement {
	if registryNode.NodeType == RegistryNodeEnum {
		if obj, ok := registryNode.Element.(EnumElement); ok {
			return &obj
		}
	}
	return nil
}

// EnumParents returns a slice of the parents that could be successfully
// converted into a EnumElement
func (registryNode RegistryNode) EnumParents() (parents []struct {
	N *RegistryNode
	E EnumElement
}) {
	for _, n := range registryNode.Parents {
		if e := n.EnumElement(); e != nil {
			parents = append(parents, struct {
				N *RegistryNode
				E EnumElement
			}{n, *e})
		}
	}
	return
}

// CommandElement returns the Element as a Registry CommandElement or nil.
func (registryNode RegistryNode) CommandElement() *CommandElement {
	if registryNode.NodeType == RegistryNodeCommand {
		if obj, ok := registryNode.Element.(CommandElement); ok {
			return &obj
		}
	}
	return nil
}

// FeatureElement returns the Element as a Registry FeatureElement or nil.
func (registryNode RegistryNode) FeatureElement() *FeatureElement {
	if registryNode.NodeType == RegistryNodeFeature {
		if obj, ok := registryNode.Element.(FeatureElement); ok {
			return &obj
		}
	}
	return nil
}

// ExtensionElement returns the Element as a Registry ExtensionElement or nil.
func (registryNode RegistryNode) ExtensionElement() *ExtensionElement {
	if registryNode.NodeType == RegistryNodeExtension {
		if obj, ok := registryNode.Element.(ExtensionElement); ok {
			return &obj
		}
	}
	return nil
}

// Name returns the name of the underlying element, in the format of
// "[category ]name", where category comes from the type element.
func (registryNode RegistryNode) Name() string {
	name := "UNKNOWN"
	switch registryNode.NodeType {
	case RegistryNodePlatform:
		if platform := registryNode.PlatformElement(); platform != nil {
			name = platform.Name
		}
		break
	case RegistryNodeType:
		if tiepuh := registryNode.TypeElement(); tiepuh != nil {
			name = strings.TrimSpace(fmt.Sprintf("%s %s", tiepuh.Category, tiepuh.SpecializedName()))
		}
		break
	case RegistryNodeEnums:
		if enums := registryNode.EnumsElement(); enums != nil {
			name = fmt.Sprintf("%s %s", enums.Type, enums.Name)
		}
	case RegistryNodeEnum:
		if enum := registryNode.EnumElement(); enum != nil {
			name = enum.SpecializedName()
		}
		break
	case RegistryNodeCommand:
		if command := registryNode.CommandElement(); command != nil {
			name = command.Name()
		}
		break
	case RegistryNodeFeature:
		if feature := registryNode.FeatureElement(); feature != nil {
			name = feature.SpecializedName()
		}
		break
	case RegistryNodeExtension:
		if extension := registryNode.ExtensionElement(); extension != nil {
			name = extension.Name
		}
		break
	case RegistryNodeVirtual:
		name = "/virtual/"
	}
	return name
}

// Terminal returns true when a node element is considered to be a terminal element.
// This means that while it may have additional parents, those children are
// effectively part of the terminal node. Mostly applies to the enum type.
func (registryNode RegistryNode) Terminal() bool {
	if registryNode.NodeType == RegistryNodeType {
		if tiepuh := registryNode.TypeElement(); tiepuh != nil {
			if TypeCategoryTerminal[tiepuh.Category] {
				return true
			}
		}
	}
	return false
}

// String exists to implement Stringer.
func (registryNode RegistryNode) String() string {
	return fmt.Sprintf("%s(%v)", registryNode.Name(), registryNode.NodeType)
}

// RegistryNodeElementType is the type of a registry node.
type RegistryNodeElementType int

func (nodeType RegistryNodeElementType) String() string {
	switch nodeType {
	case RegistryNodePlatform:
		return "platform"
	case RegistryNodeType:
		return "type"
	case RegistryNodeEnums:
		return "enums"
	case RegistryNodeEnum:
		return "enum"
	case RegistryNodeCommand:
		return "command"
	case RegistryNodeFeature:
		return "feature"
	case RegistryNodeExtension:
		return "extension"
	case RegistryNodeVirtual:
		return "virtual"
	}
	return "UNKNOWN"
}

const (
	RegistryNodePlatform RegistryNodeElementType = iota
	RegistryNodeType
	RegistryNodeEnums
	RegistryNodeEnum
	RegistryNodeCommand
	RegistryNodeFeature
	RegistryNodeExtension
	RegistryNodeVirtual
)

type RegistryGraph map[string]*RegistryNode

// ApplyFeatureExtensions processes the require and remove blocks for features in the XML.
// The format for calculating enum extension values is defined in
// https://www.khronos.org/registry/vulkan/specs/1.3/styleguide.html#_assigning_extension_token_values
// The edges in the graph are updated by this method, but the graph map itself is not.
func (graph RegistryGraph) ApplyFeatureExtensions(name string, constants *RegistryNode) {
	node := graph[name]
	if node == nil {
		return
	}
	feature := node.FeatureElement()

	for _, req := range feature.Requires {
		graph.applyRequiresElement(req, 0, constants)
	}
}

// ApplyExtensionExtensions processes the require and remove blocks for features in the XML.
// The format for calculating enum extension values is defined in
// https://www.khronos.org/registry/vulkan/specs/1.3/styleguide.html#_assigning_extension_token_values
// The edges in the graph are updated by this method, but the graph map itself is not.
// The included map is used to check if a feature or extension required by the feature block
// is included.
func (graph RegistryGraph) ApplyExtensionExtensions(name string, included map[string]bool, constants *RegistryNode) {
	node := graph[name]
	if node == nil {
		return
	}
	extension := node.ExtensionElement()

	for _, req := range extension.Requires {
		if len(req.Feature) == 0 || included[req.Feature] {
			graph.applyRequiresElement(req, extension.Number, constants)
		}
	}
}

func (graph RegistryGraph) applyRequiresElement(element RequireElement, extensionNumber int, constants *RegistryNode) {
	for _, enum := range element.Enums {
		grandParent := &RegistryNode{
			NodeType: RegistryNodeEnum,
		}
		if len(enum.Extends) > 0 {
			parent := graph[enum.Extends].EnumsParents()[0]
			if parent.E.Type == EnumsElementBitmask {
			} else {
				baseValue := 1_000_000_000
				rangeSize := 1_000
				enumValue := func(extNumber, offset int) int {
					return baseValue + (extNumber-1)*rangeSize + offset
				}
				if len(enum.Alias) == 0 && len(enum.Value) == 0 {
					extNumber := extensionNumber
					if enum.ExtensionNumber > 0 {
						extNumber = enum.ExtensionNumber
					}
					value := enumValue(extNumber, enum.Offset)
					enum.EnumElement.Value = fmt.Sprintf("%d", value)
				}
			}
			grandParent.Element = enum.EnumElement
			parent.N.AddParent(grandParent)
		} else if len(enum.Value) > 0 {
			parent := constants.EnumsParents()[0]
			grandParent.Element = enum.EnumElement
			parent.N.AddParent(grandParent)
		}
	}
}

// DepthFirstSearch performs a depth first search on the graph, calling `collect` for
// any node with all the children visited. It is used to flatten / order the dependency
// graph for generating the output file.
func (graph RegistryGraph) DepthFirstSearch(starts []string, collect func([]*RegistryNode)) {
	tracker := map[*RegistryNode]bool{}
	root := NewVirtualNode()
	for _, start := range starts {
		if node, ok := graph[start]; ok {
			root.AddParent(node)
		}
	}

	stack := []RegistryNodeParents{RegistryNodeParents{root}}
	for len(stack) > 0 {
		currentPath := stack[0]
		current := currentPath[len(currentPath)-1]

		var unvisitedParents RegistryNodeParents
		if !current.Terminal() {
			for _, v := range current.Parents {
				var seen bool
				for h := range currentPath {
					if currentPath[h] == v {
						seen = true
					}
				}
				if !seen && !tracker[v] {
					unvisitedParents = append(unvisitedParents, v)
				}
			}
		}

		if !tracker[current] && len(unvisitedParents) == 0 {
			collect(currentPath)
			stack = stack[1:]
			tracker[current] = true
		} else if !tracker[current] {
			sort.Sort(unvisitedParents)
			head := make([]RegistryNodeParents, len(unvisitedParents), len(stack)+len(unvisitedParents))
			for k, v := range unvisitedParents {
				head[k] = append(currentPath, v)
			}
			stack = append(head, stack...)
		} else {
			stack = stack[1:]
		}
	}
}
