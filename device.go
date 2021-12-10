package vks

import "C"

func (x DeviceCreateInfo) WithLayers(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		// TODO deal with the leaked NewCString memory.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

func (x DeviceCreateInfo) WithExtensions(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		// TODO deal with the leaked NewCString memory.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}
