# Pipeline with Vks

The previous section focused on creating a device using Vks. This part of the tutorial will focus on creating the pipeline with Vks. This will follow along to the [original vulkan-go tutorial that I wrote](https://github.com/ibd1279/vulkangotutorial/blob/main/tutorial/part07.md), except you will build the pipeline into the same file as the application.

This pipeline object contains all the parts that would need to be recreated should the window be resized, or the swapchain otherwise be labeled as suboptimal.

## SwapChain or Swapchain?

I call it a swapchain (one word), because that seems to be how Vulkan names it. The vulkan-tutorial calls it a swap chain. I also don't think it really matters, so name it however makes you comfortable, but this tutorial uses one word.

Using the swapchain requires an extension be enabled when you create the device.

First add this required extension to the application.

```golang
func main() {
	...
	app := App{
		...
		RequiredDeviceExtensions: []string{
			vks.VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
			vks.VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		},
	}
	...
}
```

Then add a new field to the application.

```golang
type App struct {
	...

	pipeline *Pipeline
}
```

Then add the cleanup call

```golang
func (app *App) destroy() {
	app.pipeline.Destroy(app)
	...
}
```

Then add the creation call to the setup.

```golang
func (app *App) setup() {
	...
	app.recreatePipeline()
}
```

Then populate the recreatePipeline method.

```golang
func (app *App) recreatePipeline() {
	app.pipeline = NewPipeline(app, app.pipeline)
}
```

At the bottom of the file, add the new pipeline structure.

```golang
//// Pipeline

type Pipeline struct {
	swapchain       vks.SwapchainKHR
	swapchainImages []vks.Image
	swapchainFormat vks.SurfaceFormatKHR
	swapchainExtent vks.Extent2D
}

func NewPipeline(app *App, oldPipeline *Pipeline) *Pipeline {
	pipeline := &Pipeline{}
	pipeline.createSwapchain(app, oldPipeline)
	return pipeline
}

func (pipeline *Pipeline) Destroy(app *App) {
}

func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	// Capture the old Swapchain.

	// Swapchain support.

	// Formats.

	// Present Mode.

	// Extent.

	// Image Count.

	// Queue Families and Share mode.

	// Create the swapchain.

	// Fetch the Swapchain Images.
}
```

With this structure in place, you can start populating the swapchain creation method.

Capturing the oldSwapchain will be helpful when you start dealing with resizing, but on the first call, this will always be nil.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	// Capture the old Swapchain.
	oldSwapchain := vks.NullSwapchainKHR
	if oldPipeline != nil {
		oldSwapchain = oldPipeline.swapchain
	}
	
	...
}
```

For getting the swapchain support, we can reuse a method we wrote during physical device selection. The goal of this swapchain is to present to our surface, so we want to use formats and modes related to the surface.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...

	// Swapchain support.
	caps, fmts, modes := app.describePhysicalDeviceSurface(app.physicalDevice)

	...
}
```

Then select the format and present mode for the swapchain images.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...
	
	// Formats.
	format := func() vks.SurfaceFormatKHR {
		for _, v := range fmts {
			if v.Format() == vks.VK_FORMAT_B8G8R8A8_SRGB &&
				v.ColorSpace() == vks.VK_COLOR_SPACE_SRGB_NONLINEAR_KHR {
				return v
			}
		}
		return fmts[0]
	}()

	// Present Mode.
	presentMode := func() vks.PresentModeKHR {
		for _, v := range modes {
			if v == vks.VK_PRESENT_MODE_MAILBOX_KHR {
				return v
			}
		}
		return vks.VK_PRESENT_MODE_FIFO_KHR
	}()
	
	...
}
```

You also need to define the extent and image count for the swapchain images. This is slightly more verbose because of the lack of tertiary operators in Go.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...
	
	// Extent.
	clamp := func(val, smallest, largest uint32) uint32 {
		min := func(a, b uint32) uint32 {
			if a < b {
				return a
			}
			return b
		}
		max := func(a, b uint32) uint32 {
			if a > b {
				return a
			}
			return b
		}
		return max(smallest, min(val, largest))
	}
	extent := func() vks.Extent2D {
		if caps.CurrentExtent().Width() != ^uint32(0) {
			return caps.CurrentExtent()
		}
		width, height := app.window.GetFramebufferSize()
		return vks.Extent2D{}.
			WithWidth(clamp(uint32(width), caps.MinImageExtent().Width(), caps.MaxImageExtent().Width())).
			WithHeight(clamp(uint32(height), caps.MinImageExtent().Height(), caps.MaxImageExtent().Height()))
	}()

	// Image Count.
	imgCount := func() uint32 {
		count := caps.MinImageCount() + 1
		if caps.MaxImageCount() > 0 {
			count = clamp(count, caps.MinImageCount(), caps.MaxImageCount())
		}
		return count
	}()

	...
}
```

The last piece of information you need is the queue families and share modes. If you need two queue families, you will need a non-exclusive sharing mode.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...
	
	// Queue Families and Share mode.
	qFamIdx, shareMode := func() ([]uint32, vks.SharingMode) {
		gIdx, pIdx := app.queueFamilies(app.physicalDevice)
		qfi := []uint32{uint32(gIdx), uint32(pIdx)}
		sm := vks.VK_SHARING_MODE_CONCURRENT
		if gIdx == pIdx {
			sm = vks.VK_SHARING_MODE_EXCLUSIVE
			qfi = qfi[:1]
		}
		return qfi, sm
	}()

	...
}
```

With all of the information collected, you can now create the swapchain handle and object.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...
	
	// Create the swapchain.
	swapchainInfo := vks.SwapchainCreateInfoKHR{}.
		WithDefaultSType().
		WithSurface(app.surface).
		WithMinImageCount(imgCount).
		WithImageFormat(format.Format()).
		WithImageColorSpace(format.ColorSpace()).
		WithImageExtent(extent).
		WithImageArrayLayers(1).
		WithImageUsage(vks.ImageUsageFlags(vks.VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)).
		WithImageSharingMode(shareMode).
		WithPQueueFamilyIndices(qFamIdx).
		WithPreTransform(caps.CurrentTransform()).
		WithCompositeAlpha(vks.VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR).
		WithPresentMode(presentMode).
		WithClipped(vks.VK_TRUE).
		WithOldSwapchain(oldSwapchain).
		AsCPtr()
	defer swapchainInfo.Free()

	var swapchain vks.SwapchainKHR
	if err := app.device.CreateSwapchainKHR(swapchainInfo, nil, &swapchain).AsErr(); err != nil {
		panic(err)
	}
	pipeline.swapchain = swapchain
	pipeline.swapchainFormat = format
	pipeline.swapchainExtent = extent

	...
}
```

Similar to the SurfaceKHR, there aren't any methods that take the SwapchainKHR as the primary (first) parameter. It is better suited to passing around the handle since it will be more frequently used.

At the very end of the method, fetch the swapchain images.

```golang
func (pipeline *Pipeline) createSwapchain(app *App, oldPipeline *Pipeline) {
	...
	
	// Fetch the Swapchain Images.
	var count uint32
	if err := app.device.GetSwapchainImagesKHR(pipeline.swapchain, &count, nil).AsErr(); err != nil {
		panic(err)
	}
	pipeline.swapchainImages = make([]vks.Image, count)
	if err := app.device.GetSwapchainImagesKHR(pipeline.swapchain, &count, pipeline.swapchainImages).AsErr(); err != nil {
		panic(err)
	}
}
```

The last step is to destroy the swapchain when the pipeline is destroyed.

```golang
func (pipeline *Pipeline) Destroy(app *App) {
	app.device.DestroySwapchainKHR(pipeline.swapchain, nil)
}
```

## Everything Else Pipeline

The rest of the pipeline is relatively simple compared to collecting the data and the choices made in the swapchain.

Start by adding all the additional fields you will need to the Pipeline structure.
 
```golang
type Pipeline struct {
	swapchain              vks.SwapchainKHR
	swapchainImages        []vks.Image
	swapchainFormat        vks.SurfaceFormatKHR
	swapchainExtent        vks.Extent2D
	swapchainImageViews    []vks.ImageView
	swapchainFramebuffers  []vks.Framebuffer
	renderPass             vks.RenderPass
	pipelineLayout         vks.PipelineLayout
	pipelines              []vks.Pipeline
}
```

Then update the NewPipeline method to call other methods to populate these fields.

```golang
func NewPipeline(app *App, oldPipeline *Pipeline) *Pipeline {
	pipeline := &Pipeline{}
	pipeline.createSwapchain(app, oldPipeline)
	pipeline.createImageViews(app)
	pipeline.createRenderPass(app)
	pipeline.createFramebuffers(app)
	pipeline.createPipelineLayout(app)
	pipeline.createPipelines(app)
	return pipeline
}
```

You will need to update the the Destroy method to handle these new fields as well.

```golang
func (pipeline *Pipeline) Destroy(app *App) {
	for _, ppln := range pipeline.pipelines {
		app.device.DestroyPipeline(ppln, nil)
	}
	app.device.DestroyPipelineLayout(pipeline.pipelineLayout, nil)
	for _, framebuffer := range pipeline.swapchainFramebuffers {
		app.device.DestroyFramebuffer(framebuffer, nil)
	}
	app.device.DestroyRenderPass(pipeline.renderPass, nil)
	for _, imgView := range pipeline.swapchainImageViews {
		app.device.DestroyImageView(imgView, nil)
	}
	app.device.DestroySwapchainKHR(pipeline.swapchain, nil)
}
```

In order to keep things building, you'll need to create the empty methods for each one of those functions at the bottom of the file.

```golang
func (pipeline *Pipeline) createImageViews(app *App)     {}
func (pipeline *Pipeline) createRenderPass(app *App)     {}
func (pipeline *Pipeline) createFramebuffers(app *App)   {}
func (pipeline *Pipeline) createPipelineLayout(app *App) {}
func (pipeline *Pipeline) createPipelines(app *App)      {}
```

### ImageViews

Creating the image views is mostly declarative, with many of the default initializations being sufficient.

```golang
func (pipeline *Pipeline) createImageViews(app *App) {
	pipeline.swapchainImageViews = make([]vks.ImageView, len(pipeline.swapchainImages))
	for k, img := range pipeline.swapchainImages {
		imgViewInfo := vks.ImageViewCreateInfo{}.
			WithDefaultSType().
			WithImage(img).
			WithViewType(vks.VK_IMAGE_VIEW_TYPE_2D).
			WithFormat(pipeline.swapchainFormat.Format()).
			WithSubresourceRange(vks.ImageSubresourceRange{}.
				WithAspectMask(vks.ImageAspectFlags(vks.VK_IMAGE_ASPECT_COLOR_BIT)).
				WithLevelCount(1).
				WithLayerCount(1)).
			AsCPtr()
		defer imgViewInfo.Free()

		if err := app.device.CreateImageView(imgViewInfo, nil, &pipeline.swapchainImageViews[k]).AsErr(); err != nil {
			panic(err)
		}
	}
}
```

There isn't much complexity here, other than creating a single create info per swapchain images.

### Render Pass

Similar to the ImageViews, the render is mostly declarative values. Populate the method with the below.

```golang
func (pipeline *Pipeline) createRenderPass(app *App) {
	attachments := []vks.AttachmentDescription{
		vks.AttachmentDescription{}.
			WithFormat(pipeline.swapchainFormat.Format()).
			WithSamples(vks.VK_SAMPLE_COUNT_1_BIT).
			WithLoadOp(vks.VK_ATTACHMENT_LOAD_OP_CLEAR).
			WithStoreOp(vks.VK_ATTACHMENT_STORE_OP_STORE).
			WithStencilLoadOp(vks.VK_ATTACHMENT_LOAD_OP_DONT_CARE).
			WithStencilStoreOp(vks.VK_ATTACHMENT_STORE_OP_DONT_CARE).
			WithFinalLayout(vks.VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
	}
	subpasses := []vks.SubpassDescription{
		vks.SubpassDescription{}.
			WithPColorAttachments([]vks.AttachmentReference{
				vks.AttachmentReference{}.
					WithAttachment(0).
					WithLayout(vks.VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
			}),
	}
	dependencies := []vks.SubpassDependency{
		vks.SubpassDependency{}.
			WithSrcSubpass(vks.VK_SUBPASS_EXTERNAL).
			WithSrcStageMask(vks.PipelineStageFlags(vks.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)).
			WithDstStageMask(vks.PipelineStageFlags(vks.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)).
			WithDstAccessMask(vks.AccessFlags(vks.VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)),
	}
	rpInfo := vks.RenderPassCreateInfo{}.
		WithDefaultSType().
		WithPAttachments(attachments).
		WithPSubpasses(subpasses).
		WithPDependencies(dependencies).
		AsCPtr()
	defer rpInfo.Free()

	var renderPass vks.RenderPass
	if err := app.device.CreateRenderPass(rpInfo, nil, &renderPass).AsErr(); err != nil {
		panic(err)
	}
	pipeline.renderPass = renderPass
}
```

### Framebuffers

The trend of declaration continues through the Framebuffers.

```golang
func (pipeline *Pipeline) createFramebuffers(app *App) {
	pipeline.swapchainFramebuffers = make([]vks.Framebuffer, len(pipeline.swapchainImageViews))
	for k, imgView := range pipeline.swapchainImageViews {
		attachments := []vks.ImageView{imgView}
		fInfo := vks.FramebufferCreateInfo{}.
			WithDefaultSType().
			WithRenderPass(pipeline.renderPass).
			WithPAttachments(attachments).
			WithWidth(pipeline.swapchainExtent.Width()).
			WithHeight(pipeline.swapchainExtent.Height()).
			WithLayers(1).
			AsCPtr()
		defer fInfo.Free()
		if err := app.device.CreateFramebuffer(fInfo, nil, &pipeline.swapchainFramebuffers[k]).AsErr(); err != nil {
			panic(err)
		}
	}
}
```

## Pipeline Layout

You implement more of the same for the pipeline layout.

```golang
func (pipeline *Pipeline) createPipelineLayout(app *App) {
	lInfo := vks.PipelineLayoutCreateInfo{}.
		WithDefaultSType().
		AsCPtr()
	defer lInfo.Free()

	var pipelineLayout vks.PipelineLayout
	if err := app.device.CreatePipelineLayout(lInfo, nil, &pipelineLayout).AsErr(); err != nil {
		panic(err)
	}
	pipeline.pipelineLayout = pipelineLayout
}
```

This method becomes more complicated when you start exploring push constants and other more advanced concepts.

## Compiling Shaders

At this point, you have to take a quick break and compile your shaders. As this part isn't very Go related, I highly recommend reading the [Vulkan-Tutorial page on this topic](https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules). Return here when you get to the section on Loading a shader, as that gets back into language specific code.

Compile the [shader.vert](https://github.com/ibd1279/vulkangotutorial/blob/5632dcbc7dd4ccc9eebd466aafef13430c8612cf/shaders/shader.vert) and [shader.frag](https://github.com/ibd1279/vulkangotutorial/blob/5632dcbc7dd4ccc9eebd466aafef13430c8612cf/shaders/shader.frag) files into .spv files, and put those .spv files in the "shaders" subdirectory.

```
glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv
```

## The Pipeline(s)

It is easiest to think of building this simple pipeline as three distinct parts: Loading the shader byte code, declaring all the fixed functions, and finally creating the pipeline. As mentioned in the other tutorials, pipelines are rather immutable once created. If you need to change something later (like the view port as an example), you will need to recreate the pipelines.  There are a couple of things that can be dynamically changed, but it is generally better to approach the pipelines as immutable after creation.

### Shader Modules

Shader modules are based on 32-bit words. As a result, you will need to load the bytes and convert them into 32-bit words. In my original tutorial, I implemented it [in a way that avoids unsafe](https://github.com/ibd1279/vulkangotutorial/blob/main/tutorial/part01.md#32-bit-words). In this tutorial, I'm going to embrace the unsafe conversion.

```golang
func (pipeline *Pipeline) createPipelines(app *App) {
	// Function for loading a shader.
	loadShader := func(fn string) vks.ShaderModule {
		b, err := ioutil.ReadFile(fn)
		if err != nil {
			panic(err)
		}
		words := unsafe.Slice((*uint32)(unsafe.Pointer(&b[0])), len(b)/4)
		sInfo := vks.ShaderModuleCreateInfo{}.
			WithDefaultSType().
			WithPCode(words).
			WithCodeSize(uint64(len(b))).
			AsCPtr()
		defer sInfo.Free()
		var module vks.ShaderModule
		if err := app.device.CreateShaderModule(sInfo, nil, &module).AsErr(); err != nil {
			panic(err)
		}
		return module
	}

	// Create the ShaderStage info objects.

	// Create the pipelines.
}
```

The declaration of words is the most interesting part here. I recommend reading the documentation for [unsafe.Slice](https://pkg.go.dev/unsafe#Slice) to understand what is happening with the declaration of "words".

## Shader Stages

Shader stages is typical create info declaration.

```golang
func (pipeline *Pipeline) createPipelines(app *App) {
	...

	// Create the ShaderStage info objects.
	vertShader := loadShader("shaders/vert.spv")
	fragShader := loadShader("shaders/frag.spv")
	name := vks.NewCString("main")
	shaderStages := vks.PipelineShaderStageCreateInfoMakeCSlice(
		vks.PipelineShaderStageCreateInfo{}.
			WithDefaultSType().
			WithStage(vks.VK_SHADER_STAGE_VERTEX_BIT).
			WithModule(vertShader).
			WithPName(name),
		vks.PipelineShaderStageCreateInfo{}.
			WithDefaultSType().
			WithStage(vks.VK_SHADER_STAGE_FRAGMENT_BIT).
			WithModule(fragShader).
			WithPName(name),
	)
	defer func() {
		vks.PipelineShaderStageCreateInfoFreeCSlice(shaderStages)
		app.device.DestroyShaderModule(vertShader, nil)
		app.device.DestroyShaderModule(fragShader, nil)
		vks.FreeCString(name)
	}()

	// Create the pipelines.
}
```

### Fixed Functions and the Pipeline(s)

The `CreateGraphicsPipelines` command works on plurals, and expects to receive a slice of pipeline create info, even if we are only creating a single pipeline for this tutorial.

Otherwise, this is mostly declaring the fixed functions for the pipeline, and more detail can be found in the [Vulkan-Tutorial on  Fixed functions](https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions).

```golang
func (pipeline *Pipeline) createPipelines(app *App) {
	...

	// Create the pipelines.
	vtxInputState := vks.PipelineVertexInputStateCreateInfo{}.
		WithDefaultSType().
		AsCPtr()
	inputAssemblyState := vks.PipelineInputAssemblyStateCreateInfo{}.
		WithDefaultSType().
		WithTopology(vks.VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST).
		WithPrimitiveRestartEnable(vks.VK_FALSE).
		AsCPtr()
	viewportState := vks.PipelineViewportStateCreateInfo{}.
		WithDefaultSType().
		WithPViewports([]vks.Viewport{
			vks.Viewport{}.
				WithWidth(float32(pipeline.swapchainExtent.Width())).
				WithHeight(float32(pipeline.swapchainExtent.Height())).
				WithMaxDepth(1.0),
		}).
		WithPScissors([]vks.Rect2D{
			vks.Rect2D{}.
				WithExtent(pipeline.swapchainExtent),
		}).
		AsCPtr()
	rasterizationState := vks.PipelineRasterizationStateCreateInfo{}.
		WithDefaultSType().
		WithPolygonMode(vks.VK_POLYGON_MODE_FILL).
		WithLineWidth(1.0).
		WithCullMode(vks.CullModeFlags(vks.VK_CULL_MODE_BACK_BIT)).
		WithFrontFace(vks.VK_FRONT_FACE_CLOCKWISE).
		AsCPtr()
	multisampleState := vks.PipelineMultisampleStateCreateInfo{}.
		WithDefaultSType().
		WithRasterizationSamples(vks.VK_SAMPLE_COUNT_1_BIT).
		AsCPtr()
	colorBlendState := vks.PipelineColorBlendStateCreateInfo{}.
		WithDefaultSType().
		WithLogicOp(vks.VK_LOGIC_OP_COPY).
		WithPAttachments([]vks.PipelineColorBlendAttachmentState{
			vks.PipelineColorBlendAttachmentState{}.
				WithColorWriteMask(vks.ColorComponentFlags(vks.VK_COLOR_COMPONENT_R_BIT | vks.VK_COLOR_COMPONENT_G_BIT | vks.VK_COLOR_COMPONENT_B_BIT | vks.VK_COLOR_COMPONENT_A_BIT)),
		}).
		AsCPtr()
	pInfos := vks.GraphicsPipelineCreateInfoMakeCSlice(
		vks.GraphicsPipelineCreateInfo{}.
			WithDefaultSType().
			WithPStages(shaderStages).
			WithPVertexInputState(vtxInputState).
			WithPInputAssemblyState(inputAssemblyState).
			WithPViewportState(viewportState).
			WithPRasterizationState(rasterizationState).
			WithPMultisampleState(multisampleState).
			WithPColorBlendState(colorBlendState).
			WithLayout(pipeline.pipelineLayout).
			WithRenderPass(pipeline.renderPass),
	)
	defer func() {
		vks.GraphicsPipelineCreateInfoFreeCSlice(pInfos)
		vtxInputState.Free()
		inputAssemblyState.Free()
		viewportState.Free()
		rasterizationState.Free()
		multisampleState.Free()
		colorBlendState.Free()
	}()

	pipelines := make([]vks.Pipeline, len(pInfos))
	if err := app.device.CreateGraphicsPipelines(vks.NullPipelineCache, uint32(len(pInfos)), pInfos, nil, pipelines).AsErr(); err != nil {
		panic(err)
	}
	pipeline.pipelines = pipelines
}
```

## Conclusion

At this point, you have a majority of the pipeline built. In the next, and final step of this tutorial, you will add the command buffers, deal with the multi-threading issues, and finally display the triangle on the screen.