package vks

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

func (x VkApplicationInfo) WithApplication(name string, v VkApiVersion) VkApplicationInfo {
	n := NullTerminatedPointer(name)
	return x.
		WithPApplicationName(n).
		WithApplicationVersion(uint32(v))
}

func (x VkApplicationInfo) WithEngine(name string, v VkApiVersion) VkApplicationInfo {
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
