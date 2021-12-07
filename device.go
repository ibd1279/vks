package vks

import "C"
import "unsafe"

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
