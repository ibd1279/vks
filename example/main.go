package main

import (
	"log"
	"runtime"

	"github.com/ibd1279/vks"
)

func init() {
	// Not really required in this example (no glfw dependency) but here because it is
	// normally needed when you deal with windowing.
	runtime.LockOSThread()
}

func main() {
	// Create an ARP to deal with the memory that Go cannot manage.
	arp := vks.NewAutoReleaser()
	defer arp.Release()

	// This loads the dylib and the associated memory addresses.
	if ret := vks.Init(); !ret.IsSuccess() {
		panic(ret.AsErr())
	}
	defer vks.Destroy()

	// Example of getting the vulkan version number.
	var version uint32
	if result := vks.EnumerateInstanceVersion(&version); result.IsSuccess() {
		log.Printf("%v - API version", vks.ApiVersion(version))
		log.Printf("%v - vk.xml version", vks.VK_HEADER_VERSION_COMPLETE)
		log.Printf("%v - Vulkan Major Version", vks.VK_API_VERSION_1_3)
	}

	// Example of a simple (vulkan 1.0) enumeration call.
	var count uint32
	if result := vks.EnumerateInstanceExtensionProperties(nil, &count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	instanceExtensions := make([]vks.ExtensionProperties, count)
	if result := vks.EnumerateInstanceExtensionProperties(nil, &count, instanceExtensions); !result.IsSuccess() {
		panic(result.AsErr())
	}
	for k, instExt := range instanceExtensions {
		// Example of converting Vulkan C strings into Go strings
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

	// Example of creating an info object used in many of the vulkan create calls.
	appInfo := vks.CPtr(arp, &vks.ApplicationInfo{},
		vks.SetDefaultSType,
		vks.SetApplication(arp, "Test", vks.MakeApiVersion(0, 1, 0, 0)),
		vks.SetEngine(arp, "NoEngine", vks.MakeApiVersion(0, 1, 0, 0)),
		func(in *vks.ApplicationInfo) {
			in.SetApiVersion(uint32(vks.VK_API_VERSION_1_3))
		},
	)
	createInfo := vks.CPtr(arp, &vks.InstanceCreateInfo{},
		vks.SetDefaultSType,
		vks.SetInstanceLayers(arp, []string{"VK_LAYER_KHRONOS_validation"}),
		vks.SetInstanceExtensions(arp, []string{
			"VK_KHR_surface",
			"VK_KHR_portability_enumeration",
			"VK_KHR_get_physical_device_properties2"}),
		func(in *vks.InstanceCreateInfo) {
			in.SetPApplicationInfo(appInfo)
			in.SetFlags(vks.InstanceCreateFlags(vks.VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR))
		},
	)

	// Example of converting a create result into a facade. The facade loads the C function
	// addresses for the underlying facade object (usually an instance or a device).
	var vkInstance vks.Instance
	if err := vks.CreateInstance(createInfo, nil, &vkInstance).AsErr(); err != nil {
		panic(err)
	}
	instance := vks.MakeInstanceFacade(vkInstance)
	defer instance.DestroyInstance(nil)

	if result := instance.EnumeratePhysicalDevices(&count, nil); !result.IsSuccess() {
		panic(result.AsErr())
	}
	phyDevs := make([]vks.PhysicalDevice, count)
	if result := instance.EnumeratePhysicalDevices(&count, phyDevs); !result.IsSuccess() {
		panic(result.AsErr())
	}

	for k, phyDev := range phyDevs {
		phyDev := instance.MakePhysicalDeviceFacade(phyDev)

		// Example of how to use the PNext pointers in the vulkan SDK.
		driverProps := vks.CPtr(arp, &vks.PhysicalDeviceDriverProperties{},
			vks.SetDefaultSType,
		)
		props := vks.CPtr(arp, &vks.PhysicalDeviceProperties2{},
			vks.SetDefaultSType,
			vks.SetPNext[*vks.PhysicalDeviceProperties2](driverProps),
		)

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

}
