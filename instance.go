package vks

//import "C"

func SetApplication(arp *AutoReleasePool, name string, v ApiVersion) ConfigFunction[*ApplicationInfo] {
	n := NewCStr(arp, name)
	return func(in *ApplicationInfo) {
		in.SetPApplicationName(n)
		in.SetApplicationVersion(uint32(v))
	}
}

// Deprecated: Use SetApplication instead.
func (x ApplicationInfo) WithApplication(name string, v ApiVersion) ApplicationInfo {
	// Use SetApplication instead because it deals with the memory leak.
	n := NewCString(name)
	return x.
		WithPApplicationName(n).
		WithApplicationVersion(uint32(v))
}

func SetEngine(arp *AutoReleasePool, name string, v ApiVersion) ConfigFunction[*ApplicationInfo] {
	n := NewCStr(arp, name)
	return func(in *ApplicationInfo) {
		in.SetPEngineName(n)
		in.SetEngineVersion(uint32(v))
	}
}

// Deprecated: Use SetEngine instead.
func (x ApplicationInfo) WithEngine(name string, v ApiVersion) ApplicationInfo {
	// Use SetEngine instead because it deals with the memory leak.
	n := NewCString(name)
	return x.
		WithPEngineName(n).
		WithEngineVersion(uint32(v))
}

// TODO maybe make this more generic, for both Instance and Device?
func SetInstanceLayers(arp *AutoReleasePool, names []string) ConfigFunction[*InstanceCreateInfo] {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NewCStr(arp, names[h]))
	}
	return func(in *InstanceCreateInfo) {
		in.SetPpEnabledLayerNames(cNames)
		in.SetEnabledLayerCount(uint32(len(cNames)))
	}
}

// Deprecated: Use SetInstanceLayers instead.
func (x InstanceCreateInfo) WithLayers(names []string) InstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		// Use SetInstanceLayers instead because it deals with the memory leak.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

// TODO maybe make this more generic, for both Instance and Device?
func SetInstanceExtensions(arp *AutoReleasePool, names []string) ConfigFunction[*InstanceCreateInfo] {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NewCStr(arp, names[h]))
	}
	return func(in *InstanceCreateInfo) {
		in.SetPpEnabledExtensionNames(cNames)
		in.SetEnabledExtensionCount(uint32(len(cNames)))
	}
}

// Deprecated: Use SetInstanceExtensions instead.
func (x InstanceCreateInfo) WithExtensions(names []string) InstanceCreateInfo {
	var cNames []*byte
	for h := range names {
		// Use SetInstanceExtensions instead because it deals with the memory leak.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}
