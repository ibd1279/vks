package vks

import "C"
import "unsafe"

// WithLayers is a helper function that converts to the layer names
// into C strings before adding the count and bytes to the
// DeviceCreateInfo.
//
// Deprecated: This was deprecated in newer versions of vulkan.
func SetDeviceLayers(arp *AutoReleasePool, names []string) ConfigFunction[*DeviceCreateInfo] {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NewCStr(arp, names[h]))
	}
	return func(in *DeviceCreateInfo) {
		in.SetPpEnabledLayerNames(cNames)
		in.SetEnabledLayerCount(uint32(len(cNames)))
	}
}

// WithLayers is a helper function that converts to the layer names
// into C strings before adding the count and bytes to the
// DeviceCreateInfo.
//
// Deprecated: This was deprecated in newer versions of vulkan.
//
// Deprecated: Use SetDeviceLayers instead.
func (x DeviceCreateInfo) WithLayers(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		// Use SetDeviceLayers instead to deal with the memory leak.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledLayerNames(cNames).
		WithEnabledLayerCount(uint32(len(cNames)))
}

// WithExtensions is a helper function that converts to the extension names
// into C strings before adding the count and bytes to the
// DeviceCreateInfo.
func SetDeviceExtensions(arp *AutoReleasePool, names []string) ConfigFunction[*DeviceCreateInfo] {
	var cNames []*byte
	for h := range names {
		cNames = append(cNames, NewCStr(arp, names[h]))
	}
	return func(in *DeviceCreateInfo) {
		in.SetPpEnabledExtensionNames(cNames)
		in.SetEnabledExtensionCount(uint32(len(cNames)))
	}
}

// WithExtensions is a helper function that converts to the extension names
// into C strings before adding the count and bytes to the
// DeviceCreateInfo.
//
// Deprecated: Use SetDeviceExtensions instead.
func (x DeviceCreateInfo) WithExtensions(names []string) DeviceCreateInfo {
	var cNames []*byte
	for h := range names {
		// Use SetDeviceExtensions instead to deal with the memory leak.
		cNames = append(cNames, NewCString(names[h]))
	}
	return x.
		WithPpEnabledExtensionNames(cNames).
		WithEnabledExtensionCount(uint32(len(cNames)))
}

// CopyToMemory copies data to device memory from the provided slice. See
// MapMemory for possible errors.  Returns the number of bytes copied.
//
// This was originally part of the header generator template. Moving it out of
// there since it didn't really make sense as part of that logic.
func (device DeviceFacade) CopyToMemory(memory DeviceMemory, offset, size DeviceSize, flags MemoryMapFlags, src []byte) (int, error) {
	var pData unsafe.Pointer
	if err := device.MapMemory(memory, offset, size, flags, &pData).AsErr(); err != nil {
		return 0, err
	}
	dst := unsafe.Slice((*byte)(pData), size)
	n := copy(dst, src)
	device.UnmapMemory(memory)
	return n, nil
}

// CopyFromMemory copies data from device memory into the provided slice. See
// MapMemory for possible errors.  Returns the number of bytes copied.
//
// This was originally part of the header generator template. Moving it out of
// there since it didn't really make sense as part of that logic.
func (device DeviceFacade) CopyFromMemory(memory DeviceMemory, offset, size DeviceSize, flags MemoryMapFlags, dst []byte) (int, error) {
	var pData unsafe.Pointer
	if err := device.MapMemory(memory, offset, size, flags, &pData).AsErr(); err != nil {
		return 0, err
	}
	src := unsafe.Slice((*byte)(pData), size)
	n := copy(dst, src)
	device.UnmapMemory(memory)
	return n, nil
}

// Should this be under Device? seems like unions need a better solution all
// around.
//
// TODO move this out to a unions.go file once, or figure out how to
// incorporate into the header generator
func MakeClearColorValueFloat32(r, g, b, a float32) ClearColorValue {
	bsz := unsafe.Sizeof(float32(0)) * 4
	orig := []float32{r, g, b, a}
	src := unsafe.Slice((*byte)(unsafe.Pointer(&orig[0])), bsz)
	var dst ClearColorValue
	copy(dst[:], src)
	return dst
}

func MakeClearColorValueUint32(r, g, b, a uint32) ClearColorValue {
	bsz := unsafe.Sizeof(uint32(0)) * 4
	orig := []uint32{r, g, b, a}
	src := unsafe.Slice((*byte)(unsafe.Pointer(&orig[0])), bsz)
	var dst ClearColorValue
	copy(dst[:], src)
	return dst
}

func MakeClearColorValueInt32(r, g, b, a int32) ClearColorValue {
	bsz := unsafe.Sizeof(int32(0)) * 4
	orig := []int32{r, g, b, a}
	src := unsafe.Slice((*byte)(unsafe.Pointer(&orig[0])), bsz)
	var dst ClearColorValue
	copy(dst[:], src)
	return dst
}

func (x ClearColorValue) AsClearValue() ClearValue {
	bsz := unsafe.Sizeof(int32(0)) * 4
	src := unsafe.Slice((*byte)(unsafe.Pointer(&x[0])), bsz)
	var dst ClearValue
	copy(dst[:], src)
	return dst
}
