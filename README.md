# vks
vks is a Vulkan bridge for Go. The header generator folder contains the code that
is used to generate the vulkan bindings. It woks similar to c-for-go, but it uses
the vulkan specification published as XML to generate the output bindings.

## Memory management
Unlike c-for-go, this wrapper does very little memory management for you. Most of
the logic is for converting from Cgo types into go types, but ensuring that C
memory is allocated and released is up to the program, not to this binding.

## Completeness
A list of the enabled features and extensions is available in the [vkxml.yml](https://github.com/ibd1279/vks/blob/main/vkxml.yml) file. 

Nothing is done to "fix" the names of functions or types. That means
typing "vks.Vk" frequently. There are a couple of functions where I've taken a first
stab at providing a more go-friendly wrapper ontop of the function, but for the
most part, everything is working directly with the vulkan API.

it also currently depends on the vulkan prototypes and linking with the vulkan library
when building.

## Example application
```go
package main

import (
	"log"
	"unsafe"

	"github.com/ibd1279/vks"
)

func main() {
	var version uint32
	if result := vks.VkEnumerateInstanceVersion(&version); result.IsSuccess() {
		log.Printf("%v", version)
	}

	appInfo := new(vks.VkApplicationInfo).
		WithDefaultSType().
		WithApplication("Test", vks.MakeVkApiVersion(0, 1, 0, 0)).
		WithEngine("NoEngine", vks.MakeVkApiVersion(0, 1, 0, 0)).
		WithApiVersion(uint32(vks.VK_API_VERSION_1_2)).
		AsCPtr()
	createInfo := new(vks.VkInstanceCreateInfo).
		WithDefaultSType().
		WithPApplicationInfo(appInfo).
		WithLayers([]string{"VK_LAYER_KHRONOS_validation"}).
		WithExtensions([]string{"VK_KHR_surface"}).
		AsCPtr()
	defer func() { createInfo.Free(); appInfo.Free() }()

	var instance vks.VkInstance
	if err := vks.VkCreateInstance(createInfo, nil, &instance).AsErr(); err != nil {
		panic(err)
	}

	if phyDevs, result := vks.EnumeratePhysicalDevices(instance); result.IsSuccess() {
		for k, phyDev := range phyDevs {
			driverProps := new(vks.VkPhysicalDeviceDriverProperties).
				WithDefaultSType().
				AsCPtr()
			defer func() { driverProps.Free() }()

			props := new(vks.VkPhysicalDeviceProperties2).
				WithDefaultSType().
				WithPNext(unsafe.Pointer(driverProps)).
				AsPtr()

			vks.VkGetPhysicalDeviceProperties2(phyDev, props)

			name := vks.ToString(props.Properties().DeviceName())
			apiVersion := vks.VkApiVersion(props.Properties().ApiVersion())
			devType := props.Properties().DeviceType()
			driverVersion := vks.VkApiVersion(props.Properties().DriverVersion())
			vendorId := props.Properties().VendorID()
			driverInfo := vks.ToString(driverProps.DriverInfo())
			driverName := vks.ToString(driverProps.DriverName())

			log.Printf("physical device %d %s %s - %s - %d %s %s %s", k, name, devType,
				apiVersion,
				vendorId, driverName, driverVersion, driverInfo)
		}
	}

	vks.VkDestroyInstance(instance, nil)
}
```

## Expected usage.

The expected usage is to configure a vkxml.yml file for a project, and use
the header-generator program to expose the vulkan functionality you need to
your program.

That being said, I intend to keep the (in my opinion) most common
extensions enabled and available for vks, partially for testing, and partially
as a living example of expected usage.

## Next steps

Listed in no particular order:
* add renaming support to header-generator (to remove all the Vk prefixes on types).
* add dynamic linking, and using get\*ProcAddr() to remove some
  of the overhead.
* add WSI support, and putting certain extension data in different files (with
  different build tags).
* Put better documentation into the output. Hoping to find a way to pull some of the
  Vulkan docs into the file.
* Re-organize the output to be more OO -- this may go with the get\*ProcAddr, as
  tracking handles to their parent instance may be part of that.
