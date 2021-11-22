package vks

import (
	"fmt"
)
import "C"

// EnumerateInstanceLayerProperties is a Go like wrapper around
// VkEnumerateInstanceLayerProperties.
func EnumerateInstanceLayerProperties() ([]VkLayerProperties, VkResult) {
	var count uint32
	if ret := VkEnumerateInstanceLayerProperties(&count, nil); !ret.IsSuccess() {
		return nil, ret
	}
	layProps := make([]VkLayerProperties, count)
	if ret := VkEnumerateInstanceLayerProperties(&count, layProps); !ret.IsSuccess() {
		return nil, ret
	}
	return layProps, VK_SUCCESS
}

// EnumerateInstanceExtensionProperties is a Go like wrapper around
// VkEnumerateInstanceExtensionProperties.
func EnumerateInstanceExtensionProperties(layer string) ([]VkExtensionProperties, VkResult) {
	var pLayerName *byte
	if len(layer) > 0 {
		pLayerName = NullTerminatedPointer(layer)
	}
	var count uint32
	if ret := VkEnumerateInstanceExtensionProperties(pLayerName, &count, nil); !ret.IsSuccess() {
		return nil, ret
	}
	extProps := make([]VkExtensionProperties, count)
	if ret := VkEnumerateInstanceExtensionProperties(pLayerName, &count, extProps); !ret.IsSuccess() {
		return nil, ret
	}
	return extProps, VK_SUCCESS
}

// EnumeratePhysicalDevices is a Go like wrapper around
// VkEnumeratePhysicalDevices
func EnumeratePhysicalDevices(instance VkInstance) ([]VkPhysicalDevice, VkResult) {
	var count uint32
	if ret := VkEnumeratePhysicalDevices(instance, &count, nil); !ret.IsSuccess() {
		return nil, ret
	}
	phyDevs := make([]VkPhysicalDevice, count)
	if ret := VkEnumeratePhysicalDevices(instance, &count, phyDevs); !ret.IsSuccess() {
		return nil, ret
	}
	return phyDevs, VK_SUCCESS
}

// Version is an implementation of the Vulkan Make Api Version defines.
type Version uint32

var (
	VK_API_VERSION_1_0 Version = MakeApiVersion(0, 1, 0, 0)
	VK_API_VERSION_1_1         = MakeApiVersion(0, 1, 1, 0)
	VK_API_VERSION_1_2         = MakeApiVersion(0, 1, 2, 0)
)

// EnumerateInstanceVersion wraps a call to VkEnumerateInstanceVersion to hide
// the uint32 cast.
func EnumerateInstanceVersion() (Version, VkResult) {
	var v uint32
	ret := VkEnumerateInstanceVersion(&v)
	return Version(v), ret
}

// MakeApiVersion creates a Version based on the provided Variant, maJor, miNor,
// and Patch.
func MakeApiVersion(v, j, n, p int) Version {
	return Version((uint32(v) << 29) | (uint32(j) << 22) | (uint32(n) << 12) | (uint32(p)))
}

func (v Version) Major() int   { return int((v >> 22) & 0x7F) }
func (v Version) Minor() int   { return int((v >> 12) & 0x3FF) }
func (v Version) Patch() int   { return int(v & 0xFFF) }
func (v Version) Variant() int { return int(v >> 29) }
func (v Version) String() string {
	return fmt.Sprintf("%d[v.%d.%d.%d/%d]", v, v.Major(), v.Minor(), v.Patch(), v.Variant())
}

func (x VkApplicationInfo) WithApplication(name string, v Version) VkApplicationInfo {
	n := NullTerminatedPointer(name)
	return x.
		WithPApplicationName(n).
		WithApplicationVersion(uint32(v))
}

func (x VkApplicationInfo) WithEngine(name string, v Version) VkApplicationInfo {
	n := NullTerminatedPointer(name)
	return x.
		WithPEngineName(n).
		WithEngineVersion(uint32(v))
}

func (x VkInstanceCreateInfo) WithLayers(names []string) VkInstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

func (x VkInstanceCreateInfo) WithExtensions(names []string) VkInstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}
