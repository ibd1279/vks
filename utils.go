package vks

import (
	"bytes"
	"fmt"
)

// IsSuccess checks if the VkResult is equal to the VK_SUCCESS value.
func (result Result) IsSuccess() bool {
	return result == VK_SUCCESS
}

// IsWarn checks if the VkResult is a positive value. See
// https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkResult.html#_description
// for more details on success (not an error) codes.
func (result Result) IsWarn() bool {
	return result > 0
}

// IsError checks if the VkResult is a negative value. see
// https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkResult.html#_description
// for more details on error codes.
func (result Result) IsError() bool {
	return result < 0
}

// AsError converts the VkResult into a Go error. VK_SUCCESS returns nil.
func (result Result) AsErr() error {
	switch result {
	case VK_SUCCESS:
		return nil
	default:
		return fmt.Errorf("error: %v", result)
	}
}

// OrPanic panics if the VkResult is not VK_SUCCESS
func (result Result) OrPanic() {
	if err := result.AsErr(); err != nil {
		panic(err)
	}
}

// IsTrue is shorthand for comparing the bool to VK_TRUE.
func (bool32 Bool32) IsTrue() bool {
	return bool32 == VK_TRUE
}

// IsFalse is shorthand for comparing the bool to VK_FALSE.
func (bool32 Bool32) IsFalse() bool {
	return bool32 == VK_FALSE
}

// String returns the string representation of the bool.
func (bool32 Bool32) String() string {
	return fmt.Sprintf("%t", bool32.IsTrue())
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
