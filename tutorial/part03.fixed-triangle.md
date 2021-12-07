# Displaying the Triangle

The previous section was focused on the pipeline shaders and fixed functions. This part is going to focus on submitting commands, coordination between the GPU and the CPU, and finally presenting the triangle.

## Commands and their Pool

Starting with the top of the file, add the command Pool to the application.

```golang
type App struct {
	...

	pipeline    *Pipeline
	commandPool vks.CommandPoolFacade
}
```

The pipeline is going to depend on the command pool, so destroy the command pool after the pipeline.

```golang
func (app *App) destroy() {
	app.pipeline.Destroy(app)
	app.device.DestroyCommandPool(app.commandPool.H, nil)
	...
}
```

Then you add the call to create the command pool just before the  pipeline creation.

```golang
func (app *App) setup() {
	...
	app.createCommandPool()
	app.recreatePipeline()
}
```

Then you implement the createCommandPool method.

```golang
func (app *App) createCommandPool() {
	gIdx, _ := app.queueFamilies(app.physicalDevice)
	pInfo := vks.CommandPoolCreateInfo{}.
		WithDefaultSType().
		WithQueueFamilyIndex(uint32(gIdx)).
		AsCPtr()
	defer pInfo.Free()

	var commandPool vks.CommandPool
	if err := app.device.CreateCommandPool(pInfo, nil, &commandPool).AsErr(); err != nil {
		panic(err)
	}
	app.commandPool = app.device.MakeCommandPoolFacade(commandPool)
}
```

Add the command buffers to the Pipeline

```golang
type Pipeline struct {
	...
	commandBuffers        []vks.CommandBuffer
}
```

Then add the call to create the command buffers.

```golang
func NewPipeline(app *App, oldPipeline *Pipeline) *Pipeline {
	...
	pipeline.createCommandBufrers(app)
	return pipeline
}
```

And clean up the buffers.

```golang
func (pipeline *Pipeline) Destroy(app *App) {
	app.device.FreeCommandBuffers(
		app.commandPool.H,
		uint32(len(pipeline.commandBuffers)),
		pipeline.commandBuffers,
	)
	...
}
```

And implement the method to create the command buffers. The first step is to allocate them from the command pool.

```golang
func (pipeline *Pipeline) createCommandBufrers(app *App) {
	cmdBufferCount := uint32(len(pipeline.swapchainFramebuffers))
	bInfo := vks.CommandBufferAllocateInfo{}.
		WithDefaultSType().
		WithCommandPool(app.commandPool.H).
		WithLevel(vks.VK_COMMAND_BUFFER_LEVEL_PRIMARY).
		WithCommandBufferCount(cmdBufferCount).
		AsCPtr()
	defer bInfo.Free()
	pipeline.commandBuffers = make([]vks.CommandBuffer, cmdBufferCount)
	if err := app.device.AllocateCommandBuffers(bInfo, pipeline.commandBuffers).AsErr(); err != nil {
		panic(err)
	}

	// Record commands.
}
```

Once the buffers are allocated, you can record commands to the buffers. Since the vertex data is already inside the shaders, you can keep this to "draw" for right now.

```golang
func (pipeline *Pipeline) createCommandBufrers(app *App) {
	...
	
	// Record commands.
	for k, cmdBufHndl := range pipeline.commandBuffers {
		cmdBuf := app.commandPool.MakeCommandBufferFacade(cmdBufHndl)
		cmdBeginInfo := vks.CommandBufferBeginInfo{}.
			WithDefaultSType().
			AsCPtr()
		defer cmdBeginInfo.Free()
		if err := cmdBuf.BeginCommandBuffer(cmdBeginInfo).AsErr(); err != nil {
			panic(err)
		}

		renderBeginInfo := vks.RenderPassBeginInfo{}.
			WithDefaultSType().
			WithRenderPass(pipeline.renderPass).
			WithFramebuffer(pipeline.swapchainFramebuffers[k]).
			WithRenderArea(vks.Rect2D{}.WithExtent(pipeline.swapchainExtent)).
			WithPClearValues([]vks.ClearValue{vks.ClearValue(vks.MakeClearColorValueFloat32(0, 0, 0, 1))}).
			AsCPtr()
		defer renderBeginInfo.Free()
		cmdBuf.CmdBeginRenderPass(renderBeginInfo, vks.VK_SUBPASS_CONTENTS_INLINE)
		cmdBuf.CmdBindPipeline(vks.VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelines[0])
		cmdBuf.CmdDraw(3, 1, 0, 0)
		cmdBuf.CmdEndRenderPass()
		if err := cmdBuf.EndCommandBuffer().AsErr(); err != nil {
			panic(err)
		}
	}
}
```

## Parallel Computing

This next part follows along with [my vulkan-go tutorial on Synchronization](https://github.com/ibd1279/vulkangotutorial/blob/main/tutorial/part10.md#synchronization), which is based on the [Vulkan-tutorial in C++'s Synchronization](https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Rendering_and_presentation#page_Synchronization).

You will be creating semaphores and fences to ensure that the image being presented isn't being generated at the same time.

Start by setting a value for the number of frames that are allowed to be in flight

```golang
func main() {
	...
	app := App{
		...
		MaxFramesInFlight: 2,
	}
	...
}
```

Then extend the application to contain the synchronization objects.

```golang
type App struct {
	...

	imageAvailableSemaphores []vks.Semaphore
	renderFinishedSemaphores []vks.Semaphore
	inFlightFences           []vks.Fence
	imagesInFlight           []vks.Fence
	currentFrame             uint
	MaxFramesInFlight        uint
}
```

Add the cleanup logic into the Destroy method. Take note of the `DeviceWaitIdle` that was added before the pipeline destruction.

```golang
func (app *App) destroy() {
	app.device.DeviceWaitIdle()
	app.pipeline.Destroy(app)
	for _, fence := range app.inFlightFences {
		app.device.DestroyFence(fence, nil)
	}
	for _, semaphore := range app.renderFinishedSemaphores {
		app.device.DestroySemaphore(semaphore, nil)
	}
	for _, semaphore := range app.imageAvailableSemaphores {
		app.device.DestroySemaphore(semaphore, nil)
	}
	...
}
```

Extend the setup method.

```golang
func (app *App) setup() {
	...
	app.createSyncObjects()
	app.recreatePipeline()
}
```

The size of the imagesInFlight slice is based on the number of swapchain images, so you need to resize it when the pipeline is recreated.

```golang
func (app *App) recreatePipeline() {
	app.pipeline = NewPipeline(app, app.pipeline)
	app.imagesInFlight = make([]vks.Fence, len(app.pipeline.swapchainImages))
}
```

And implement the new createSyncObjects method.

```golang
func (app *App) createSyncObjects() {
	sInfo := vks.SemaphoreCreateInfo{}.
		WithDefaultSType().
		AsCPtr()
	fInfo := vks.FenceCreateInfo{}.
		WithDefaultSType().
		WithFlags(vks.FenceCreateFlags(vks.VK_FENCE_CREATE_SIGNALED_BIT)).
		AsCPtr()
	defer func() {
		sInfo.Free()
		fInfo.Free()
	}()

	app.imageAvailableSemaphores = make([]vks.Semaphore, app.MaxFramesInFlight)
	app.renderFinishedSemaphores = make([]vks.Semaphore, app.MaxFramesInFlight)
	app.inFlightFences = make([]vks.Fence, app.MaxFramesInFlight)
	for h := uint(0); h < app.MaxFramesInFlight; h++ {
		if err := app.device.CreateSemaphore(sInfo, nil, &app.imageAvailableSemaphores[h]).AsErr(); err != nil {
			panic(err)
		}
		if err := app.device.CreateSemaphore(sInfo, nil, &app.renderFinishedSemaphores[h]).AsErr(); err != nil {
			panic(err)
		}
		if err := app.device.CreateFence(fInfo, nil, &app.inFlightFences[h]).AsErr(); err != nil {
			panic(err)
		}
	}
}
```

## Drawing Frames

Now you can put all these different parts together, similar to [Part 10 of the vulkan-go tutorial](https://github.com/ibd1279/vulkangotutorial/blob/main/tutorial/part11.md#vulcan-go-and-a-triangle-part-11). 

Implement the drawFrame function, which has existed since the beginning, but been empty.

```golang
func (app *App) drawFrame() {
	// Get context.
	app.device.WaitForFences(1, app.inFlightFences[app.currentFrame:], vks.VK_TRUE, ^uint64(0))
	var imgIdx uint32
	result := app.device.AcquireNextImageKHR(
		app.pipeline.swapchain,
		^uint64(0),
		app.imageAvailableSemaphores[app.currentFrame],
		vks.NullFence,
		&imgIdx,
	)
	if result == vks.VK_ERROR_OUT_OF_DATE_KHR {
		app.recreatePipeline()
		return
	} else if result != vks.VK_SUCCESS && result != vks.VK_SUBOPTIMAL_KHR {
		panic(result.AsErr())
	}

	// avoid race conditions.
	if app.imagesInFlight[imgIdx] != vks.NullFence {
		app.device.WaitForFences(1, app.imagesInFlight[imgIdx:], vks.VK_TRUE, ^uint64(0))
	}
	app.imagesInFlight[imgIdx] = app.inFlightFences[app.currentFrame]

	// submit graphics work.
	sInfos := vks.SubmitInfoMakeCSlice(
		vks.SubmitInfo{}.
			WithDefaultSType().
			WithPWaitSemaphores([]vks.Semaphore{
				app.imageAvailableSemaphores[app.currentFrame],
			}).
			WithPWaitDstStageMask([]vks.PipelineStageFlags{
				vks.PipelineStageFlags(vks.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT),
			}).
			WithPCommandBuffers([]vks.CommandBuffer{
				app.pipeline.commandBuffers[imgIdx],
			}).
			WithPSignalSemaphores([]vks.Semaphore{
				app.renderFinishedSemaphores[app.currentFrame],
			}),
	)
	defer vks.SubmitInfoFreeCSlice(sInfos)
	app.device.ResetFences(1, app.inFlightFences[app.currentFrame:])
	if err := app.graphicsQueue.QueueSubmit(1, sInfos, app.inFlightFences[app.currentFrame]).AsErr(); err != nil {
		panic(err)
	}

	// submit the present work.
	pInfo := vks.PresentInfoKHR{}.
		WithDefaultSType().
		WithPWaitSemaphores([]vks.Semaphore{
			app.renderFinishedSemaphores[app.currentFrame],
		}).
		WithPSwapchains([]vks.SwapchainKHR{app.pipeline.swapchain}).
		WithPImageIndices([]uint32{imgIdx}).
		AsCPtr()
	defer pInfo.Free()
	result = app.presentationQueue.QueuePresentKHR(pInfo)
	if result == vks.VK_ERROR_OUT_OF_DATE_KHR || result == vks.VK_SUBOPTIMAL_KHR {
		app.recreatePipeline()
	} else if !result.IsSuccess() {
		panic(result.AsErr())
	}

	// move forward.
	app.currentFrame = (app.currentFrame + 1) % app.MaxFramesInFlight
}
```