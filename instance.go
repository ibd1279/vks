package vks

import "C"

func (x ApplicationInfo) WithApplication(name string, v ApiVersion) ApplicationInfo {
	n := NullTerminatedPointer(name)
	return x.
		WithPApplicationName(n).
		WithApplicationVersion(uint32(v))
}

func (x ApplicationInfo) WithEngine(name string, v ApiVersion) ApplicationInfo {
	n := NullTerminatedPointer(name)
	return x.
		WithPEngineName(n).
		WithEngineVersion(uint32(v))
}

func (x InstanceCreateInfo) WithLayers(names []string) InstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

func (x InstanceCreateInfo) WithExtensions(names []string) InstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NullTerminatedPointer(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}
