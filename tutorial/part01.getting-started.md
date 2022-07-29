# Getting Started with Vks

Vks is a Vulkan bridge for go. This tutorial assumes you are familiar with computer graphics and vulkan concepts. It primarily focuses on the differences between the [vulkan tutorial written in C++](https://vulkan-tutorial.com/Introduction) and using vks in Go.  It is roughly based on my [previous tutorial](https://virtsoftcrazy.blogspot.com/2021/10/vulcan-go-and-triangle-part-1.html), written using [vulkan-go](https://pkg.go.dev/github.com/vulkan-go/vulkan).

## Base Code

Start by creating the base skeleton. The rest of the tutorial is going to be adding code to this file.

```golang
package main

import (
	"log"
	"runtime"

	"github.com/ibd1279/vks"
)

func init() {
	runtime.LockOSThread()
}

const (
	WindowHeight = 600
	WindowWidth  = 800
)

func main() {
	log.Printf("Vulkan Header version: %v", vks.VK_HEADER_VERSION_COMPLETE)
	app := App{}
	app.Run()
}

type App struct{}

func (app *App) destroy() {}

func (app *App) setup() {}

func (app *App) mainLoop() {}

func (app *App) drawFrame() {}

func (app *App) recreatePipeline() {}

func (app *App) Run() {
	app.setup()
	defer app.destroy()
	app.mainLoop()
}
```

If you run this application now, it will immediately exit after displaying the version of the xml specification used to generate the current build of vks.

## Integrating GLFW

Add the import for GLFW:

```golang
import (
	...	
	"github.com/go-gl/glfw/v3.3/glfw"
	...
)
```

Don't forget to run go get before / afterwards.

Then update the app to store the window handle. GLFW will require certain extensions in order to use Vulkan, so you also add a slice for storing that list.

```golang
type App struct {
	RequiredInstanceExtensions []string
	window                     *glfw.Window
}
```

Now that you have the window object, you need to explicitly release it when the application is destroyed.

```golang
func (app *App) destroy() {
	app.window.Destroy()
	glfw.Terminate()
}
```

Inside the setup method, you add the call to the initWindow function.

```golang
func (app *App) setup() {
	app.initWindow()
}
```

And then add the initWindow method to the application.

```golang
func (app *App) initWindow() {
	// Initialize GLFW
	err := glfw.Init()
	if err != nil {
		panic(err)
	}

	// Tell GLFW we aren't using OpenGL.
	glfw.WindowHint(glfw.ClientAPI, glfw.NoAPI)

	// We aren't yet ready to handle resizable windows.
	glfw.WindowHint(glfw.Resizable, glfw.False)

	// Create the window object.
	app.window, err = glfw.CreateWindow(WindowWidth, WindowHeight, "vks example", nil, nil)
	if err != nil {
		panic(err)
	}

	app.RequiredInstanceExtensions = append(app.RequiredInstanceExtensions, app.window.GetRequiredInstanceExtensions()...)
}
```

Most of this is boilerplate GLFW. The interesting part is the appending of RequiredExtensions at the end of the function. This will be used later when you create the Vulkan instance.

You can now update the mainLoop to wait for the window to be closed before exiting.

```golang
func (app *App) mainLoop() {
	for !app.window.ShouldClose() {
		glfw.PollEvents()
		app.drawFrame()
	}
}
```

## InitVulkan and Dumping Support

Working down the file, you add the clean up call

```golang
func (app *App) destroy() {
	vks.Destroy()
	...
}
```

Then you add the initVulkan call to the setup.

```golang
func (app *App) setup() {
	...
	app.initVulkan()
}
```

And create the initVulkan method.

```golang
func (app *App) initVulkan() {
	// Load the vulkan Library.
	if ret := vks.Init(); !ret.IsSuccess() {
		panic(ret.AsErr())
	}

	// Display supported version.
	// NOTE: This could segfault for two reasons -- 1, the library failed to load properly or 2,
	// the instance of vulkan you currently have only supports 1.0.
	var version uint32
	if result := vks.EnumerateInstanceVersion(&version); result.IsSuccess() {
		log.Printf("Supported Instance Version %v", vks.ApiVersion(version))
	}

	var count uint32

	// Enumerate supported layers
	if err := vks.EnumerateInstanceLayerProperties(&count, nil).AsErr(); err != nil {
		panic(err)
	}
	layProps := make([]vks.LayerProperties, count)
	if err := vks.EnumerateInstanceLayerProperties(&count, layProps).AsErr(); err != nil {
		panic(err)
	}
	for h := range layProps {
		log.Printf("Supported Layer %s", vks.ToString(layProps[h].LayerName()))
	}

	// Enumerate supported extensions
	if err := vks.EnumerateInstanceExtensionProperties(nil, &count, nil).AsErr(); err != nil {
		panic(err)
	}
	extProps := make([]vks.ExtensionProperties, count)
	if err := vks.EnumerateInstanceExtensionProperties(nil, &count, extProps).AsErr(); err != nil {
		panic(err)
	}
	for h := range layProps {
		log.Printf("Supported Extension %s", vks.ToString(extProps[h].ExtensionName()))
	}
}
```

The comments describe most of the logic here. If you are unfamiliar with the enumeration pattern used in Vulkan, the [vulkan-tutorial](https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Instance#page_Checking-for-extension-support) provides details on what those calls are doing.

## Creating an Instance

Starting from the top of the file, add the required layers. For this tutorial, you will use the validation layers.

```golang
func main() {
	...
	app := App{
		RequiredLayers: []string{
			"VK_LAYER_KHRONOS_validation",
		},
	}
	...
}
```

Add fields on the struct for storing the instance and the newly added required layers.

```golang
type App struct {
	...
	RequiredLayers             []string
	instance                   vks.InstanceFacade
}
```

Add the clean up for the new field.

```golang
func (app *App) destroy() {
	app.instance.DestroyInstance(nil)
	...
}
```

Add the call to createInstance in the setup method.

```golang
func (app *App) setup() {
	...
	app.createInstance()
}
```

Then create the createInstance method.

```golang
func (app *App) createInstance() {
	appInfo := vks.ApplicationInfo{}.
		WithDefaultSType().
		WithApplication("vkslash", vks.MakeApiVersion(0, 1, 0, 0)).
		WithEngine("NoEngine", vks.MakeApiVersion(0, 1, 0, 0)).
		WithApiVersion(uint32(vks.VK_API_VERSION_1_2)).
		AsCPtr()
	createInfo := vks.InstanceCreateInfo{}.
		WithDefaultSType().
		WithPApplicationInfo(appInfo).
		WithLayers(app.RequiredLayers).
		WithExtensions(app.RequiredInstanceExtensions).
		AsCPtr()
	defer func() { createInfo.Free(); appInfo.Free() }()

	var instance vks.Instance
	if err := vks.CreateInstance(createInfo, nil, &instance).AsErr(); err != nil {
		panic(err)
	}
	app.instance = vks.MakeInstanceFacade(instance)
}
```

The first part of this function is populating some vks structs with values. Each of these structs ends with "AsCPtr()", because you need these memory values stored in the C heap, not the Go heap. For many structures, you can stay entirely in the Go space, but because shimmering (going back and forth between go and C heaps) is not allowed, it is sometimes easier to move a structure to C.

Because these two structures were allocated on the C heap, you add the defer call to free them.  This is important to call out: C allocated pointers are not garbage collected. You must explicitly release them when you are finished with them.

The interesting part of the call to vks.CreateInstance is that you must "make" an instance after you "create" an instance. This has to do with the difference between an InstanceHandle and an Instance.

And InstanceHandle is effectively just a pointer to a vulkan resource. They are opaque objects and you cannot do much with them individually besides pass them as parameters. An Instance on the other head is the combination of an InstanceHandle and the ProcAddresses for that instance. With the addition of those ProcAddresses, you can invoke methods and do other more useful things.

You can see more of this difference by looking at the related go docs for vks.InstanceHandle and vks.Instance.

Smaller details worth calling out include the usage of the required extensions in the createInfo struct, and that I've hard coded the validation layers to be enabled. For the purposes of this tutorial, I've skipped the [Vulkan-Tutorial bits on making this dynamic](https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Validation_layers).

## Creating a Surface

You need to create the surface before you select a physical device. This is because the surface can impact physical device selection and the usable capabilities.

Start by adding a field to the App struct. The field in this case will be a Handle, since Surfaces don't have additional commands.

```golang
type App struct {
	...
	surface            vks.SurfaceKHR
}
```

Then call cleanup on the new surface handle.

```golang
func (app *App) destroy() {
	app.instance.DestroySurfaceKHR(app.surface, nil)
	...
}
```

Then call createSurface in the setup method.

```golang
func (app *App) setup() {
	...
	app.createSurface()
}
```

Then create the createSurface method.

```golang
func (app *App) createSurface() {
	surface, err := app.window.CreateWindowSurface(app.instance.H, nil)
	if err != nil {
		panic(err)
	}
	app.surface = *(*vks.SurfaceKHR)(unsafe.Pointer(surface))
}
```

This function is pretty straight forward. The GLFW window is used to create a new Surface pointer. It comes into the application as a uintptr, which needs to be converted into a vulkan surface.

## Describing Physical Devices

You start by adding a pickPhysicalDevice method, although it won't be picking a physical device just yet.

```golang
func (app *App) setup() {
	...
	app.pickPhysicalDevice()
}
```

The method is going to list out information about the physical devices in the first pass.

```golang
func (app *App) pickPhysicalDevice() {
	var count uint32
	if err := app.instance.EnumeratePhysicalDevices(&count, nil).AsErr(); err != nil {
		panic(err)
	}
	if count == 0 {
		panic("No vulkan supported physical devices.")
	}
	pdHandles := make([]vks.PhysicalDevice, count)
	if err := app.instance.EnumeratePhysicalDevices(&count, pdHandles).AsErr(); err != nil {
		panic(err)
	}
	phyDevs := make([]vks.PhysicalDeviceFacade, count)
	for h := range pdHandles {
		phyDevs[h] = app.instance.MakePhysicalDeviceFacade(pdHandles[h])
	}

	// Dump the physical devices.
	for k, phyDev := range phyDevs {
		props, feats, qfProps := app.describePhysicalDevice(phyDev)
		log.Printf("Physical device avail %d: %s %s (wide lines support:%v)", k,
			vks.ToString(props.DeviceName()),
			props.DeviceType(),
			feats.WideLines(),
		)
		for k, v := range qfProps {
			log.Printf("\t%d - %d 0b%016b", k, v.QueueCount(), v.QueueFlags())
		}
	}

	// Select the physical device.
}
```

You need to define the describePhysicalDevice method to make this compile. While it is possible to use the Vulkan 1.0 version of physical device properties, this approach is more extensible. 

```golang
func (app *App) describePhysicalDevice(phyDev vks.PhysicalDeviceFacade) (vks.PhysicalDeviceProperties, vks.PhysicalDeviceFeatures, []vks.QueueFamilyProperties) {
	props := vks.PhysicalDeviceProperties2{}.
		WithDefaultSType().
		AsCPtr()
	feats := vks.PhysicalDeviceFeatures2{}.
		WithDefaultSType().
		AsCPtr()
	defer func() {
		props.Free()
		feats.Free()
	}()

	phyDev.GetPhysicalDeviceProperties2(props)
	phyDev.GetPhysicalDeviceFeatures2(feats)

	var count uint32
	phyDev.GetPhysicalDeviceQueueFamilyProperties2(&count, nil)
	qfProps := make([]vks.QueueFamilyProperties2, count)
	for h := range qfProps {
		qfProps[h] = qfProps[h].WithDefaultSType()
	}
	phyDev.GetPhysicalDeviceQueueFamilyProperties2(&count, qfProps)
	qf := make([]vks.QueueFamilyProperties, count)
	for h := range qfProps {
		qf[h] = qfProps[h].QueueFamilyProperties()
	}

	return props.Properties(), feats.Features(), qf
}
```

If you run the application now, you will see the physical devices on your machine listed, along with details about their queue families.

## Surface Intersection

As mentioned in the section on creating the surface, a specific surface can impact the capabilities of the physical device. Vulkan provides mechanisms for looking up the capabilities using the PhysicalDevice.

Start by adding a method to describing a PhysicalDevice and Surface combination.

```golang
func (app *App) describePhysicalDeviceSurface(phyDev vks.PhysicalDeviceFacade) (vks.SurfaceCapabilitiesKHR, []vks.SurfaceFormatKHR, []vks.PresentModeKHR) {
	// Get the surface Capabilities
	surf := vks.PhysicalDeviceSurfaceInfo2KHR{}.
		WithDefaultSType().
		WithSurface(app.surface).
		AsCPtr()
	caps := vks.SurfaceCapabilities2KHR{}.
		WithDefaultSType().
		AsCPtr()
	defer func() {
		surf.Free()
		caps.Free()
	}()

	if ret := phyDev.GetPhysicalDeviceSurfaceCapabilities2KHR(surf, caps); !ret.IsSuccess() {
		panic(ret.AsErr())
	}

	// Get the surface formats
	var count uint32
	if ret := phyDev.GetPhysicalDeviceSurfaceFormats2KHR(surf, &count, nil); !ret.IsSuccess() {
		panic(ret.AsErr())
	}
	fmts2 := make([]vks.SurfaceFormat2KHR, count)
	for h := range fmts2 {
		fmts2[h] = fmts2[h].WithDefaultSType()
	}
	if ret := phyDev.GetPhysicalDeviceSurfaceFormats2KHR(surf, &count, fmts2); !ret.IsSuccess() {
		panic(ret.AsErr())
	}
	fmts := make([]vks.SurfaceFormatKHR, count)
	for h := range fmts2 {
		fmts[h] = fmts2[h].SurfaceFormat()
	}

	// Get the present modes
	if ret := phyDev.GetPhysicalDeviceSurfacePresentModesKHR(app.surface, &count, nil); !ret.IsSuccess() {
		panic(ret.AsErr())
	}
	mode := make([]vks.PresentModeKHR, count)
	if ret := phyDev.GetPhysicalDeviceSurfacePresentModesKHR(app.surface, &count, mode); !ret.IsSuccess() {
		panic(ret.AsErr())
	}

	return caps.SurfaceCapabilities(), fmts, mode
}
```

The method is straightforward in collecting three pieces of information from the physical device, and returning the collection. If this appears more complicated than the previous version, that is because it is using the new extensible APIs introduced in Vulkan 1.1.

One more method is needed to select queue families from the physical device. One of the queue families you are looking for is to present, which is obviously impacted by the presentation surface.

```golang
func (app *App) queueFamilies(phyDev vks.PhysicalDeviceFacade) (g int, p int) {
	g, p = -1, -1

	_, _, qfProps := app.describePhysicalDevice(phyDev)
	for k, v := range qfProps {
		idx := uint32(k)

		if v.QueueFlags()&vks.QueueFlags(vks.VK_QUEUE_GRAPHICS_BIT) != 0 {
			g = int(idx)
		}

		var pSupport vks.Bool32
		if err := phyDev.GetPhysicalDeviceSurfaceSupportKHR(idx, app.surface, &pSupport).AsErr(); err != nil {
			panic(err)
		}
		if pSupport.IsTrue() {
			p = int(idx)
		}

		if p >= 0 && g >= 0 {
			break
		}
	}
	return g, p
}
```

For the graphics queue, you check that the queue flags includes the graphics bit. For the presentation support, you check that the presentation queue signals support for the surface.

These two functions can now be used in the pickPhysicalDevice method to select a capable physical device. The "algorithm" presented here is just "first listed physical device". A more advanced application could score the physical devices in some way in order to select the right one.

First, add the field to the App

```golang
type App struct {
	...
	physicalDevice     vks.PhysicalDeviceFacade
}
```

Then update the pickPhysicalDevice method to populate it.

```golang
func (app *App) pickPhysicalDevice() {
	...
	
	// Select the physical device.
	var idx int = -1
	for k, phyDev := range phyDevs {
		gIdx, pIdx := app.queueFamilies(phyDev)
		_, fmts, modes := app.describePhysicalDeviceSurface(phyDev)
		if gIdx >= 0 && pIdx >= 0 && len(fmts) > 0 && len(modes) > 0 {
			idx = k
			break
		}
	}
	if idx < 0 {
		panic(fmt.Errorf("unabled to find a suitable device."))
	}
	app.physicalDevice = phyDevs[idx]
}
```

You don't need to destroy physical devices. [Users generally get unhappy](https://kotaku.com/new-world-still-bricking-expensive-graphics-cards-say-1847782307) when you do.

If you build and run the application now, you may get some validation errors like the following.

> Attemped to call vkGetPhysicalDeviceSurfaceCapabilities2KHR() but its required extension VK_KHR_get_surface_capabilities2 has not been enabled

The fix for this is to enable the extension in the main function, were we allocate the App object.

```golang
func main() {
	...
	app := App{
		RequiredInstanceExtensions: []string{
			vks.VK_KHR_SURFACE_EXTENSION_NAME,
			vks.VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
		},
		RequiredLayers: []string{
			"VK_LAYER_KHRONOS_validation",
		},
	}
	...
}
```

You had added this field to the application when you added the GLFW window, and it was already included in the instance creation call.

## Non-physical Device

Once you have selected the physical device, you can create a logical device onto of it. I highly recommend that you read the [Vulkan specifications Devices introduction](https://www.khronos.org/registry/vulkan/specs/1.3-extensions/html/vkspec.html#devsandqueues-devices) for a better understanding of how the "VkDevice" fits into the Vulkan ecosystem. Then you can jump down to [Device Creation](https://www.khronos.org/registry/vulkan/specs/1.3-extensions/html/vkspec.html#devsandqueues-device-creation) to get an understanding of the API.

Start by adding the device field to App. We also add a RequiredDeviceExtensions field, for any device specific extensions we want to enable.

```golang
type App struct {
	...

	RequiredDeviceExtensions []string
	device                   vks.DeviceFacade
}
```

Then add the clean up call in the destroy method.

```golang
func (app *App) destroy() {
	app.device.DestroyDevice(nil)
	...
}
```

Then add the createLogicalDevice call in the setup method.

```golang
func (app *App) setup() {
	...
	app.createLogicalDevice()
}
```

And implement the new method.

```golang
func (app *App) createLogicalDevice() {
	gIdx, pIdx := app.queueFamilies(app.physicalDevice)
	queueFamilyIndices := []uint32{uint32(gIdx), uint32(pIdx)}
	if gIdx == pIdx {
		queueFamilyIndices = queueFamilyIndices[:1]
	}

	qInfos := make([]vks.DeviceQueueCreateInfo, len(queueFamilyIndices))
	for h, idx := range queueFamilyIndices {
		qInfos[h] = vks.DeviceQueueCreateInfo{}.
			WithDefaultSType().
			WithQueueFamilyIndex(idx).
			WithQueueCount(1).
			WithPQueuePriorities([]float32{1.0})
	}
	qInfos = vks.DeviceQueueCreateInfoMakeCSlice(qInfos...)

	dInfo := vks.DeviceCreateInfo{}.
		WithDefaultSType().
		WithPQueueCreateInfos(qInfos).
		WithLayers(app.RequiredLayers).
		WithExtensions(app.RequiredDeviceExtensions).
		AsCPtr()
	defer func() {
		dInfo.Free()
		vks.DeviceQueueCreateInfoFreeCSlice(qInfos)
	}()

	var device vks.Device
	if err := app.physicalDevice.CreateDevice(dInfo, nil, &device).AsErr(); err != nil {
		panic(err)
	}
	app.device = app.physicalDevice.MakeDeviceFacade(device)
}
```

The first block decides if we need two queues, or one. The second block populates the queue create infos for the right number of queues. The third block populates the DeviceCreateInfo with the queues and required layers and extensions. The fourth block creates the device and stores it on the app.

On my workstation, I get a validation error about "VK_KHR_portability_subset". Feel free to use the "RequiredDeviceExtensions" field to resolve the issue if you have the same.

```golang
func main() {
	...
	app := App{
		...
		RequiredDeviceExtensions: []string{
			"VK_KHR_portability_subset",
		},
	}
	...
}
```

## The Queues

When the device was created, it also created the queues that you need. Starting from the top again, add a field to capture the queues.

```golang
type App struct {
	...
	graphicsQueue            vks.QueueFacade
	presentationQueue        vks.QueueFacade
}
```

Then add the calls to populate the fields to the bottom of the createLogicalDevice method.

```golang
func (app *App) createLogicalDevice() {
	...
	
	var queue vks.Queue
	qIdx := 0 // graphics is the head
	app.device.GetDeviceQueue(queueFamilyIndices[qIdx], uint32(qIdx), &queue)
	app.graphicsQueue = app.device.MakeQueueFacade(queue)

	qIdx = len(queueFamilyIndices) - 1 // present is the tail.
	app.device.GetDeviceQueue(queueFamilyIndices[qIdx], uint32(qIdx), &queue)
	app.presentationQueue = app.device.MakeQueueFacade(queue)
}
```

If there happens to only be one queue family you are interested in, the head and the tail of the queueFamilyIndicies will be the same element. Additionally, we don't have to destroy queues when finished, so it isn't critical to keep the distinction clear.

## Conclusion

At the end of this step, you have created an application that creates a GLFW window, initializes Vulkan, dumps some debug information, creates a device, and creates the queues on those devices.  In the next section, we will build the pipeline.
