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
		log.Printf("%v", vks.VK_API_VERSION_1_3)
	}

	var count uint32
	if result := vks.EnumerateInstanceExtensionProperties(nil, &count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	instanceExtensions := make([]vks.ExtensionProperties, count)
	if result := vks.EnumerateInstanceExtensionProperties(nil, &count, instanceExtensions); !result.IsSuccess() {
		panic(result.AsErr())
	}
	for k, instExt := range instanceExtensions {
		name := vks.ToString(instExt.ExtensionName())
		log.Printf("Extension%02d %v (%v)", k, name, instExt.SpecVersion())
	}

	if result := vks.EnumerateInstanceLayerProperties(&count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	instanceLayers := make([]vks.LayerProperties, count)
	if result := vks.EnumerateInstanceLayerProperties(&count, instanceLayers); !result.IsSuccess() {
		panic(result.AsErr())
	}
	for k, instLay := range instanceLayers {
		name := vks.ToString(instLay.LayerName())
		log.Printf("Layer%02d %v (%v / %v)", k, name, instLay.ImplementationVersion(), instLay.SpecVersion())
	}

	appInfo := vks.ApplicationInfo{}.
		WithDefaultSType().
		WithApplication("Test", vks.MakeApiVersion(0, 1, 0, 0)).
		WithEngine("NoEngine", vks.MakeApiVersion(0, 1, 0, 0)).
		WithApiVersion(uint32(vks.VK_API_VERSION_1_3)).
		AsCPtr()
	createInfo := vks.InstanceCreateInfo{}.
		WithDefaultSType().
		WithPApplicationInfo(appInfo).
		WithFlags(vks.InstanceCreateFlags(vks.VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR)).
		WithLayers([]string{"VK_LAYER_KHRONOS_validation"}).
		WithExtensions([]string{"VK_KHR_surface", "VK_KHR_portability_enumeration", "VK_KHR_get_physical_device_properties2"}).
		AsCPtr()
	defer func() { createInfo.Free(); appInfo.Free() }()

	var vkInstance vks.Instance
	if err := vks.CreateInstance(createInfo, nil, &vkInstance).AsErr(); err != nil {
		panic(err)
	}
	instance := vks.MakeInstanceFacade(vkInstance)

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
