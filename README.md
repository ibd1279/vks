# vks
vks is a Vulkan bridge for Go. The header generator folder contains the code that
is used to generate the vulkan bindings. It woks similar to c-for-go, but it uses
the vulkan specification published as XML to generate the output bindings.

## Memory management
Unlike c-for-go, this wrapper does very little memory management for you. Most of
the logic is for converting from Cgo types into go types. Ensuring that C
memory is allocated and released is up to the program, not to this binding.

A helper method for AsCPtr() exists on structures that makes it easier to copy
the structure to C. The program must call free on that structure to release the
C memory.

## Completeness
A list of the enabled features and extensions is available in the [vkxml.yml](https://github.com/ibd1279/vks/blob/main/vkxml.yml) file. 

The project doesn't do much to make the Vulkan APIs much easier to use. Commands
and types have the Vk prefix stripped, but not the constants. Facade objects are
used to access the methods, as they combine the function poitners with the handle
required to invoke the function.

## Example application
```go
package main

import (
	"log"
	"unsafe"

	"github.com/ibd1279/vks"
)

func main() {
	if ret := vks.Init(); !ret.IsSuccess() {
		panic(ret.AsErr())
	}
	defer vks.Destroy()
	var version uint32
	if result := vks.EnumerateInstanceVersion(&version); result.IsSuccess() {
		log.Printf("%v", vks.ApiVersion(version))
		log.Printf("%v", vks.VK_HEADER_VERSION_COMPLETE)
	}

	appInfo := vks.ApplicationInfo{}.
		WithDefaultSType().
		WithApplication("Test", vks.MakeApiVersion(0, 1, 0, 0)).
		WithEngine("NoEngine", vks.MakeApiVersion(0, 1, 0, 0)).
		WithApiVersion(uint32(vks.VK_API_VERSION_1_2)).
		AsCPtr()
	createInfo := vks.InstanceCreateInfo{}.
		WithDefaultSType().
		WithPApplicationInfo(appInfo).
		WithLayers([]string{"VK_LAYER_KHRONOS_validation"}).
		WithExtensions([]string{"VK_KHR_surface"}).
		AsCPtr()
	defer func() { createInfo.Free(); appInfo.Free() }()

	var vkInstance vks.Instance
	if err := vks.CreateInstance(createInfo, nil, &vkInstance).AsErr(); err != nil {
		panic(err)
	}
	instance := vks.MakeInstanceFacade(vkInstance)

	var count uint32
	if result := instance.EnumeratePhysicalDevices(&count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	phyDevs := make([]vks.PhysicalDevice, count)
	if result := instance.EnumeratePhysicalDevices(&count, phyDevs); !result.IsSuccess() {
		panic(result.AsErr())
	}

	for k, phyDev := range phyDevs {
		phyDev := instance.MakePhysicalDeviceFacade(phyDev)
		driverProps := vks.PhysicalDeviceDriverProperties{}.
			WithDefaultSType().
			AsCPtr()
		props := vks.PhysicalDeviceProperties2{}.
			WithDefaultSType().
			WithPNext(unsafe.Pointer(driverProps)).
			AsCPtr()
		defer func() {
			driverProps.Free()
			props.Free()
		}()

		phyDev.GetPhysicalDeviceProperties2(props)

		name := vks.ToString(props.Properties().DeviceName())
		apiVersion := vks.ApiVersion(props.Properties().ApiVersion())
		devType := props.Properties().DeviceType()
		driverVersion := vks.ApiVersion(props.Properties().DriverVersion())
		vendorId := props.Properties().VendorID()
		driverInfo := vks.ToString(driverProps.DriverInfo())
		driverName := vks.ToString(driverProps.DriverName())

		log.Printf("physical device %d %s %s - %s - %d %s %s %s", k, name, devType,
			apiVersion,
			vendorId, driverName, driverVersion, driverInfo)
	}

	instance.DestroyInstance(nil)
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
* ~~add renaming support to header-generator (to remove all the Vk prefixes on types and commands).~~
* ~~add dynamic linking, and using get\*ProcAddr() to remove some
  of the overhead.~~
* add WSI support, and putting certain extension data in different files (with
  different build tags).
* Put better documentation into the output. Hoping to find a way to pull some of the
  Vulkan docs into the file.
* ~~Re-organize the output to be more OO -- this may go with the get\*ProcAddr, as
  tracking handles to their parent instance may be part of that.~~
