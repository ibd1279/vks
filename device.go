package vks

import "C"

func (x DeviceCreateInfo) WithLayers(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

func (x DeviceCreateInfo) WithExtentions(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}
