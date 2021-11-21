# vks
vks is a Vulkan bridge for Go. The header generator folder contains the code that
is used to generate the vulkan bindings. It woks similar to c-for-go, but it uses
the vulkan specification published as XML to generate the output bindings.

## Memory management
Unlike c-for-go, this wrapper does very little memory management for you. Most of
the logic is for converting from Cgo types into go types, but ensuring that C
memory is allocated and released is up to the program, not to this binding.

## Completeness
Currently, the header is only generating implementatations for the features in
the "vulkan" API for 1.0, 1.1, and 1.2.

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
	if layProps, result := vks.EnumerateInstanceLayerProperties(); result.IsSuccess() {
		for k, v := range layProps {
			name := vks.ToString(v.LayerName())
			log.Printf("layer %d %v %d %t", k, name, len(name), "VK_LAYER_LUNARG_api_dump" == name)
		}
	}
	if extProps, result := vks.EnumerateInstanceExtensionProperties(""); result.IsSuccess() {
		for k, v := range extProps {
			name := vks.ToString(v.ExtensionName())
			log.Printf("extension %d %v %d %t", k, name, len(name), "VK_KHR_surface" == name)
		}
	}
	if version, result := vks.EnumerateInstanceVersion(); result.IsSuccess() {
		log.Printf("%v", version)
	}

	appInfo := new(vks.VkApplicationInfo).
		WithDefaultSType().
		WithApplication("Test", vks.MakeApiVersion(0, 1, 0, 0)).
		WithEngine("NoEngine", vks.MakeApiVersion(0, 1, 0, 0)).
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
			apiVersion := vks.Version(props.Properties().ApiVersion())
			devType := props.Properties().DeviceType()
			driverVersion := vks.Version(props.Properties().DriverVersion())
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
