package vks

import (
	"bytes"
	"fmt"
)

// IsSuccess checks if the VkResult is equal to the VK_SUCCESS value.
func (result VkResult) IsSuccess() bool {
	return result == VK_SUCCESS
}

// AsError converts the VkResult into a Go error. VK_SUCCESS returns nil.
func (result VkResult) AsErr() error {
	switch result {
	case VK_SUCCESS:
		return nil
	default:
		return fmt.Errorf("error %d", result)
	}
}

// OrPanic panics if the VkResult is not VK_SUCCESS
func (result VkResult) OrPanic() {
	if err := result.AsErr(); err != nil {
		panic(err)
	}
}

// ToString converts a null terminated slice of bytes into a Go string.
func ToString(b []byte) string {
	var str bytes.Buffer
	for _, c := range b {
		if c == '\x00' {
			return str.String()
		}
		str.WriteByte(c)
	}
	return str.String()
}

// NullTerminatedString converts a string into a null terminated string. If
// the string is already null terminated, nothing is changed.
func NullTerminatedString(s string) string {
	str := nullTerminatedBuffer(s)
	return str.String()
}

// NullTerminatedSlice converts a string into a null terminated slice of bytes.
func NullTerminatedSlice(s string) []byte {
	str := nullTerminatedBuffer(s)
	return str.Bytes()
}

// NullTerminatedPointer converts a string into a null terminated byte pointer.
func NullTerminatedPointer(s string) *byte {
	str := nullTerminatedBuffer(s)
	b := str.Bytes()
	return &b[0]
}

func nullTerminatedBuffer(s string) bytes.Buffer {
	var str bytes.Buffer
	var isNullTerminated bool
	for _, c := range ([]byte)(s) {
		if c == 0 {
			isNullTerminated = true
		}
		str.WriteByte(c)
		if isNullTerminated {
			break
		}
	}
	if !isNullTerminated {
		str.WriteByte(0)
	}
	return str
}
