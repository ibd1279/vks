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

	var vkInstance vks.InstanceHandle
	if err := vks.CreateInstance(createInfo, nil, &vkInstance).AsErr(); err != nil {
		panic(err)
	}
	instance := vks.MakeInstance(vkInstance)

	var count uint32
	if result := instance.EnumeratePhysicalDevices(&count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	phyDevs := make([]vks.PhysicalDeviceHandle, count)
	if result := instance.EnumeratePhysicalDevices(&count, phyDevs); !result.IsSuccess() {
		panic(result.AsErr())
	}

	for k, phyDev := range phyDevs {
		phyDev := instance.MakePhysicalDevice(phyDev)
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
