package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"log"
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
	// Deprecated is optional. Indicates that this type has been
	// deprecated. Possible values are:
	// * `true` - deprecated, but no explanation given.
	// * `aliased` - an old name not following Vulkan conventions. The
	//   equivalent alias following Vulkan conventions should be used
	//   instead.
	Deprecated TypeDeprecatedType `xml:"deprecated",attr,omitempty"`
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

func (typeElement TypeElement) SpecializedName(apiName string) string {
	parts := []string{typeElement.Name()}
	if len(typeElement.Api) > 0 && strings.Compare(apiName, typeElement.Api) != 0 {
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

type TypeDeprecatedType string

var (
	TypeDepreactedTrue    TypeDeprecatedType = "true"
	TypeDeprecatedAliased TypeDeprecatedType = "aliased"
)

type TypeMemberElement struct {
	// Api is optional API names for which this definition is specialized,
	// so that different APIs may have different definitions for the same
	// type. This definition is only used if the requested API name matches
	// the attribute. May be used to address subtle incompatibilities.
	Api string `xml:"api,attr,omitempty"`
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
	// Deprecated is optional. Indicates that this member has been
	// deprecated. Possible values are:
	//   * "true" - deprecated, but no explanation given.
	//   * "ignored" - functionality described by this member no longer operates.
	Deprecated TypeMemberDeprecatedType `xml:"deprecated,attr,omitempty"`
	// ExternSync denotes that the member should be externally synchronized
	// when accessed by Vulkan.
	ExternSync string `xml:"externsync,attr,omitempty"`
	// Optional is optional. A value of "true" specifies that this member
	// can be omitted by providing NULL (for pointers), VK_NULL_HANDLE (for
	// handles), or 0 (for other scalar types). If not present, the value
	// is assumed to be "false" (the member must not be omitted). If the
	// member is a pointer to one of those types, multiple values may be
	// provided, separated by commas - one for each pointer indirection.
	// Structure members with name pNext must always be specified with
	// optional="true", since there is no requirement that any member of a
	// pNext chain have a following member in the chain.
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
	// Stride is optional. if the member is an array, stride specifies the
	// name of another member containing the byte stride between
	// consecutive elements in the array. Is assumed tightly packed if
	// omitted.
	Stride string `xml:"stride,attr,omitempty"`
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

type TypeMemberDeprecatedType string

var (
	TypeMemberDepreactedTrue    TypeMemberDeprecatedType = "true"
	TypeMemberDeprecatedIgnored TypeMemberDeprecatedType = "ignored"
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
	BitPos *int `xml:"bitpos,attr,omitempty"`
	// Name is required. Enumerant name, a legal C preprocessor token name.
	Name string `xml:"name,attr"`
	// Api is optional API names for which this definition is specialized, so
	// that different APIs may have different values for the same token.
	Api string `xml:"api,attr,omitempty"`
	// Deprecated is optional. Indicates that this enum has been
	// deprecated. Possible values are:
	//   * "true" - deprecated, but no explanation given.
	//   * "ignored" - functionality described by this enum no longer operates.
	//   * "aliased" - an old name not following Vulkan conventions. The
	//     equivalent alias following Vulkan conventions should be used
	//     instead.
	Deprecated EnumDeprecatedType `xml:"deprecated,attr,omitempty"`
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

	// Intentionally ignoring comment and unused tags at this time.
}

type EnumDeprecatedType string

var (
	EnumDepreactedTrue    EnumDeprecatedType = "true"
	EnumDeprecatedIgnored EnumDeprecatedType = "ignored"
	EnumDeprecatedAliased EnumDeprecatedType = "aliased"
)

func (enumElement EnumElement) SpecializedName(apiName string) string {
	parts := []string{enumElement.Name}
	if len(enumElement.Api) > 0 && strings.Compare(apiName, enumElement.Api) != 0 {
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
	// Tasks is optional. A string identifying the tasks this command
	// performs, as described in the “Queue Operation” section of the
	// Vulkan API Specification. The format of the string is one or more of
	// the terms "action", "synchronization", "state", and "indirection",
	// with multiple terms separated by commas (",").
	Tasks string `xml:"tasks,attr,omitempty"`
	// Queues is optional. A string identifying the command queues this
	// command can be placed on. The format of the string is one or more of
	// the terms "compute", "decode", "encode", "graphics", "transfer",
	// "sparse_binding", and "opticalflow", with multiple terms separated
	// by commas (",").
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
	// VideoCoding is optional. A string identifying whether the command
	// can be issued only inside a video coding scope ("inside"), only
	// outside a video coding scope ("outside"), or both ("both"); the
	// default is "outside" for commands that do not specify it.
	VideoCoding string `xml:"videocoding,attr,omitempty"`
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
	// Apis is optional API names for which this definition is specialized,
	// so that different APIs may have different values for the same token.
	// This definition is only used if the requested API name matches the
	// attribute. May be used to address subtle incompatibilities.
	Api string `xml:"api,attr,omitempty"`
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

func (commandElement CommandElement) SpecializedName(apiName string) string {
	parts := []string{commandElement.Name()}
	if len(commandElement.Api) > 0 && strings.Compare(apiName, commandElement.Api) != 0 {
		parts = append(parts, commandElement.Api)
	}
	return strings.Join(parts, "::")
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
	// Api is optional. API names for which this definition is specialized,
	// so that different APIs may have different definitions for the same
	// type. This definition is only used if the requested API name matches
	// the attribute. May be used to address subtle incompatibilities.
	Api string `xml:"api,attr,omitempty"`
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
	// ValidStructs is optional. Allowed only when the parameter type is a
	// pointer to an abstract VkBaseInStructure or VkBaseOutStructure type.
	// This is a comma-separated list of structures which can either be
	// passed as the parameter, or can appear anywhere in the pNext chain
	// of the parameter.
	ValidStructs string `xml:"validstructs,attr,omitempty"`
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
	parts := []string{featureElement.Name, featureElement.Api}
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
	// depends is optional. String containing a boolean expression of one
	// or more API core version and extension names. The extension requires
	// the expression in the string to be satisfied to use any
	// functionality it defines (for instance extensions), or to use any
	// device-level functionality it provides (for device extensions).
	// Supported operators include , for logical OR, + for logical AND, and
	// ( ) for grouping. , and + are of equal precedence, and lower than (
	// ). Expressions must be evaluated left-to-right for operators of the
	// same precedence. Terms which are core version names are true if the
	// corresponding API version is supported. Terms which are extension
	// names are true if the corresponding extension is enabled.
	Depends string `xml:"depends,attr,omitempty"`
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
	// Ratified is optional. A comma-separated list of API names for which
	// this extension has been ratified by the Khronos Board of Promoters.
	// Defaults to the empty string if not specified.
	Ratified string `xml:"ratified,attr,omitempty"`
	// PromotedTo is  optional. A Vulkan version or a name of an extension
	// that this extension was promoted to. E.g. "VK_VERSION_1_1", or
	// "VK_KHR_draw_indirect_count". As discussed in the “Extending Vulkan”
	// chapter of the Vulkan API Specification, the promotedto relationship
	// is not a promise of exact API-level compatibility, and replacing use
	// of one interface with the other must not be done purely
	// mechanically.
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
	// Depends is optional, and only for require tags. String containing a
	// boolean expression of one or more API core version and extension
	// names. The syntax of this string is identical to that of the
	// extension depends attribute. Interfaces in the tag are only required
	// if the expression is satisfied.
	Depends string `xml:"depends,attr,omitempty"`
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
	Offset *int `xml:"offset,attr,omitempty"`
	// Direction, if present, is the calculated enumerant value will be
	// negative, instead of positive. Negative enumerant values are normally
	// used only for Vulkan error codes. The attribute value must be specified
	// as dir="-".
	Direction string `xml:"dir,attr,omitempty"`
}

// IsReference returns true if the enum lacks anything to define its own value.
func (enumElement RequireEnumElement) IsReference() bool {
	if len(enumElement.Value) > 0 ||
		enumElement.BitPos != nil ||
		enumElement.Offset != nil ||
		len(enumElement.Alias) > 0 {
		return false
	}
	return true
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

// ForApi filters the registry only returns the items matching the given
// apiName. For example, `vulkan` and `vulkansc` are both different API names.
// Features that don't match the given API name are not in the returned
// registry.
func (registry *Registry) ForApi(apiName string) *Registry {
	partOfApi := func(apiAttr string) bool {
		if len(apiAttr) == 0 {
			return true
		}
		apiList := strings.Split(apiAttr, ",")
		for _, v := range apiList {
			if strings.Compare(v, apiName) == 0 {
				return true
			}
		}
		return false
	}

	reg2 := &Registry{
		Platforms:  registry.Platforms,
		Tags:       registry.Tags,
		Types:      make([]TypeElement, 0, len(registry.Types)),
		Enums:      make([]EnumsElement, 0, len(registry.Enums)),
		Commands:   make([]CommandElement, 0, len(registry.Commands)),
		Features:   make([]FeatureElement, 0, len(registry.Features)),
		Extensions: make([]ExtensionElement, 0, len(registry.Extensions)),
	}

	for _, v := range registry.Types {
		if partOfApi(v.Api) {
			ct := v
			ct.StructMembers = make([]TypeMemberElement, 0, len(v.StructMembers))
			for _, member := range v.StructMembers {
				if partOfApi(member.Api) {
					ct.StructMembers = append(ct.StructMembers, member)
				}
			}
			reg2.Types = append(reg2.Types, ct)
		}
	}
	for _, v := range registry.Enums {
		ce := v
		ce.Enums = make([]EnumElement, 0, len(v.Enums))
		for _, enum := range v.Enums {
			if partOfApi(enum.Api) {
				ce.Enums = append(ce.Enums, enum)
			}
		}
		reg2.Enums = append(reg2.Enums, ce)
	}
	for _, v := range registry.Commands {
		if partOfApi(v.Api) {
			cc := v
			cc.Params = make([]CommandParamElement, 0, len(v.Params))
			for _, param := range v.Params {
				if partOfApi(param.Api) {
					cc.Params = append(cc.Params, param)
				}
			}
			reg2.Commands = append(reg2.Commands, cc)
		}
	}
	for _, v := range registry.Features {
		if partOfApi(v.Api) {
			cf := v
			cf.Requires = make([]RequireElement, 0, len(v.Requires))
			for _, req := range v.Requires {
				if partOfApi(req.Api) {
					cr := req
					cr.Enums = make([]RequireEnumElement, 0, len(req.Enums))
					for _, enum := range req.Enums {
						if partOfApi(enum.Api) {
							cr.Enums = append(cr.Enums, enum)
						}
					}
					cf.Requires = append(cf.Requires, cr)
				}
			}
			reg2.Features = append(reg2.Features, cf)
		}
	}
	for _, v := range registry.Extensions {
		if partOfApi(v.Supported) {
			ce := v
			ce.Requires = make([]RequireElement, 0, len(v.Requires))
			for _, req := range v.Requires {
				if partOfApi(req.Api) {
					cr := req
					cr.Enums = make([]RequireEnumElement, 0, len(req.Enums))
					for _, enum := range req.Enums {
						if partOfApi(enum.Api) {
							cr.Enums = append(cr.Enums, enum)
						}
					}
					ce.Requires = append(ce.Requires, cr)
				}
			}
			reg2.Extensions = append(reg2.Extensions, ce)
		}
	}
	return reg2
}

// GenerateNodes generates a registry map with just the nodes. No parenting and
// linkage has been added to them yet. This is stage catches anything that
// reuses the names and resolves entries that are references from entries that
// are declarations.
func (registry *Registry) GenerateNodes() RegistryGraph {
	graph := make(RegistryGraph, len(registry.Types))

	define := func(name string, nodeType RegistryNodeElementType, element interface{}) {
		newNode := RegistryNode{NodeType: nodeType, Element: element}
		if oldNode, ok := graph[name]; ok {
			if !newNode.Equals(*oldNode) {
				panic(fmt.Errorf("name '%v' reused.\n%#v\n%#v", name, oldNode, &newNode))
			}
		}
		graph[name] = &newNode
	}

	// Filter API centric elements by the API we are generating.
	for _, v := range registry.Platforms {
		define(v.Name, RegistryNodePlatform, v)
	}
	for _, v := range registry.Types {
		define(v.Name(), RegistryNodeType, v)
	}
	for _, v := range registry.Enums {
		for _, enum := range v.Enums {
			define(enum.Name, RegistryNodeEnum, enum)
		}
	}
	for _, v := range registry.Commands {
		define(v.Name(), RegistryNodeCommand, v)
	}
	for _, v := range registry.Features {
		define(v.Name, RegistryNodeFeature, v)
		for _, requires := range v.Requires {
			for _, enum := range requires.Enums {
				if !enum.IsReference() {
					define(enum.Name, RegistryNodeEnum, enum)
				}
			}
		}
	}
	for _, v := range registry.Extensions {
		define(v.Name, RegistryNodeExtension, v)
		for _, requires := range v.Requires {
			for _, enum := range requires.Enums {
				if !enum.IsReference() {
					define(enum.Name, RegistryNodeEnum, enum)
				}
			}
		}
	}

	return graph
}

// Graph generates a dependency graph based on the Vulkan XML. The enabled map allows for resolving
// some of the extension dependencies around features
func (registry *Registry) Graph(enabledMap map[string]bool) (RegistryGraph, *RegistryNode) {
	dictionary := registry.GenerateNodes()
	constants := NewVirtualNode()

	// Link the nodes together.
	link := func(self *RegistryNode, parent string) {
		if len(parent) == 0 {
			return
		}

		if node, ok := dictionary[parent]; ok {
			self.AddParent(node)
		} else {
			self.AddMissingParent(parent)
		}
	}

	for _, v := range registry.Types {
		self := dictionary[v.Name()]
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
			value := dictionary[enum.Name]
			link(self, enum.Name)
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
				link(self, enum.EnumElement.Name)
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
				link(self, enum.EnumElement.Name)
			}
		}
		// TODO Removes to go here.

		link(self, v.Platform)

		if strings.IndexAny(v.Depends, "+") > -1 && strings.IndexAny(v.Depends, ",") > -1 {
			log.Printf("Not auto-resolving dependency for %s: complex depends attribute '%s'", v.Name, v.Depends)
			continue
		} else if strings.IndexAny(v.Depends, "()") > -1 {
			log.Printf("Not auto-resolving dependency for %s: complex depends attribute '%s'", v.Name, v.Depends)
			continue
		} else if strings.IndexAny(v.Depends, "+") < 0 && strings.IndexAny(v.Depends, ",") > -1 {
			requirements := strings.FieldsFunc(v.Depends, func(c rune) bool {
				if c == ',' {
					return true
				}
				return false
			})
			var chosen string
			for h := len(requirements) - 1; h >= 0; h-- {
				if enabledMap[requirements[h]] {
					chosen = strings.TrimSpace(requirements[h])
					break
				}
			}
			if len(chosen) > 0 {
				link(self, chosen)
			}
		} else if strings.IndexAny(v.Depends, "+") > -1 && strings.IndexAny(v.Depends, ",") < 0 {
			requirements := strings.FieldsFunc(v.Depends, func(c rune) bool {
				if c == '+' {
					return true
				}
				return false
			})
			for _, require := range requirements {
				link(self, strings.TrimSpace(require))
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
	// List of parent names that couldn't be found in the dictionary for
	// the feature being build.
	MissingParents []string
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

// AddMissingParent adds the name of a missing parent to the object. It record
// that something was missing to enable trimming "incomplete" parts of the API
// out.
func (registryNode *RegistryNode) AddMissingParent(parent string) {
	registryNode.MissingParents = append(registryNode.MissingParents, parent)
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

// RequireEnumElement returns the Element as a Registry RequireEnumElement or
// nil.
func (registryNode RegistryNode) RequireEnumElement() *RequireEnumElement {
	if registryNode.NodeType == RegistryNodeEnum {
		if obj, ok := registryNode.Element.(RequireEnumElement); ok {
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
			name = strings.TrimSpace(fmt.Sprintf("%s %s", tiepuh.Category, tiepuh.Name))
		}
		break
	case RegistryNodeEnums:
		if enums := registryNode.EnumsElement(); enums != nil {
			name = fmt.Sprintf("%s %s", enums.Type, enums.Name)
		}
	case RegistryNodeEnum:
		if enum := registryNode.EnumElement(); enum != nil {
			name = enum.Name
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

// Equals compares two needs and tests that they are equal in type and value.
func (registryNode RegistryNode) Equals(other RegistryNode) bool {
	if registryNode.NodeType != other.NodeType {
		return false
	}
	var result bool
	switch registryNode.NodeType {
	case RegistryNodePlatform:
		a, b := registryNode.PlatformElement(), other.PlatformElement()
		result = *a == *b
	case RegistryNodeType:
		a, b := registryNode.TypeElement(), other.TypeElement()
		result = (a.Requires == b.Requires &&
			a.Name() == b.Name() &&
			a.Alias == b.Alias &&
			a.Api == b.Api &&
			a.Category == b.Category &&
			a.Parent == b.Parent &&
			a.ReturnedOnly == b.ReturnedOnly &&
			a.StructExtends == b.StructExtends &&
			a.AllowDuplicate == b.AllowDuplicate &&
			a.ObjTypeEnum == b.ObjTypeEnum &&
			a.TypeTag == b.TypeTag &&
			a.ApiEntry == b.ApiEntry &&
			a.BitValues == b.BitValues &&
			a.Raw == b.Raw)
	case RegistryNodeEnums:
		a, b := registryNode.EnumsElement(), other.EnumsElement()
		result = (a.Name == b.Name &&
			a.Type == b.Type &&
			a.BitWidth == b.BitWidth)
	case RegistryNodeEnum:
		a, b := registryNode.EnumElement(), other.EnumElement()
		if a == nil && b == nil {
			a, b := registryNode.RequireEnumElement(), other.RequireEnumElement()
			bitPos := a.BitPos == b.BitPos
			if a.BitPos != nil && b.BitPos != nil {
				bitPos = *a.BitPos == *b.BitPos
			}
			result = (a.Value == b.Value &&
				bitPos &&
				a.Name == b.Name &&
				a.Api == b.Api &&
				a.Deprecated == b.Deprecated &&
				a.Type() == b.Type() &&
				a.Alias == b.Alias &&
				a.Protect == b.Protect)
		} else {
			bitPos := a.BitPos == b.BitPos
			if a.BitPos != nil && b.BitPos != nil {
				bitPos = *a.BitPos == *b.BitPos
			}
			result = (a.Value == b.Value &&
				bitPos &&
				a.Name == b.Name &&
				a.Api == b.Api &&
				a.Deprecated == b.Deprecated &&
				a.Type() == b.Type() &&
				a.Alias == b.Alias &&
				a.Protect == b.Protect)
		}
	case RegistryNodeCommand:
		//a, b := registryNode.CommandElement(), other.CommandElement()
		result = false
	case RegistryNodeFeature:
		//a, b := registryNode.FeatureElement(), other.FeatureElement()
		result = false
	case RegistryNodeExtension:
		//a, b := registryNode.ExtensionElement(), other.ExtensionElement()
		result = false
	case RegistryNodeVirtual:
		result = false
	}
	return result
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
		log.Printf("Unknown feature %s", name)
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
		graph.applyRequiresElement(req, extension.Number, constants)
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
					value := enumValue(extNumber, *enum.Offset)
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
