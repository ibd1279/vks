#include <dlfcn.h>
#include "vk_wrapper.h"

// Global for proc addresses.
vksProcAddr vksProcAddresses;
void *vulkanHandle = NULL;

// default dynamic loader?
VkResult vksDynamicLoad() {
	// TODO Start: Abstract this block to be specialized as needed.
	vulkanHandle = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
	if (vulkanHandle == NULL) {
		return VK_ERROR_UNKNOWN;
	}
	vksProcAddresses.pvkGetInstanceProcAddr = dlsym(vulkanHandle, "vkGetInstanceProcAddr");
	// TODO end

	vksProcAddresses.pvkEnumerateInstanceVersion = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceVersion");
	vksProcAddresses.pvkEnumerateInstanceExtensionProperties = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
	vksProcAddresses.pvkEnumerateInstanceLayerProperties = vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
	vksProcAddresses.pvkCreateInstance = vkGetInstanceProcAddr(NULL, "vkCreateInstance");
	return VK_SUCCESS;
}

void vksDynamicUnload() {
	dlclose(vulkanHandle);
}

// call getProcAddress for all the commands.
void vksInitializeInstanceProcAddrs(VkInstance instance) {
	void* ptr = NULL;
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceImageFormatProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyDescriptorPool");
	if (ptr) {
		vksProcAddresses.pvkDestroyDescriptorPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetDescriptorPool");
	if (ptr) {
		vksProcAddresses.pvkResetDescriptorPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkAllocateDescriptorSets");
	if (ptr) {
		vksProcAddresses.pvkAllocateDescriptorSets = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkFreeDescriptorSets");
	if (ptr) {
		vksProcAddresses.pvkFreeDescriptorSets = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkUpdateDescriptorSets");
	if (ptr) {
		vksProcAddresses.pvkUpdateDescriptorSets = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDescriptorPool");
	if (ptr) {
		vksProcAddresses.pvkCreateDescriptorPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyDescriptorSetLayout");
	if (ptr) {
		vksProcAddresses.pvkDestroyDescriptorSetLayout = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDescriptorSetLayout");
	if (ptr) {
		vksProcAddresses.pvkCreateDescriptorSetLayout = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroySampler");
	if (ptr) {
		vksProcAddresses.pvkDestroySampler = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateSampler");
	if (ptr) {
		vksProcAddresses.pvkCreateSampler = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyPipelineLayout");
	if (ptr) {
		vksProcAddresses.pvkDestroyPipelineLayout = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreatePipelineLayout");
	if (ptr) {
		vksProcAddresses.pvkCreatePipelineLayout = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyPipeline");
	if (ptr) {
		vksProcAddresses.pvkDestroyPipeline = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateComputePipelines");
	if (ptr) {
		vksProcAddresses.pvkCreateComputePipelines = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateGraphicsPipelines");
	if (ptr) {
		vksProcAddresses.pvkCreateGraphicsPipelines = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkMergePipelineCaches");
	if (ptr) {
		vksProcAddresses.pvkMergePipelineCaches = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPipelineCacheData");
	if (ptr) {
		vksProcAddresses.pvkGetPipelineCacheData = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyPipelineCache");
	if (ptr) {
		vksProcAddresses.pvkDestroyPipelineCache = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreatePipelineCache");
	if (ptr) {
		vksProcAddresses.pvkCreatePipelineCache = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyShaderModule");
	if (ptr) {
		vksProcAddresses.pvkDestroyShaderModule = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateShaderModule");
	if (ptr) {
		vksProcAddresses.pvkCreateShaderModule = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyImageView");
	if (ptr) {
		vksProcAddresses.pvkDestroyImageView = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateImageView");
	if (ptr) {
		vksProcAddresses.pvkCreateImageView = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateFramebuffer");
	if (ptr) {
		vksProcAddresses.pvkCreateFramebuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyFramebuffer");
	if (ptr) {
		vksProcAddresses.pvkDestroyFramebuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateRenderPass");
	if (ptr) {
		vksProcAddresses.pvkCreateRenderPass = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyRenderPass");
	if (ptr) {
		vksProcAddresses.pvkDestroyRenderPass = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetRenderAreaGranularity");
	if (ptr) {
		vksProcAddresses.pvkGetRenderAreaGranularity = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetImageSubresourceLayout");
	if (ptr) {
		vksProcAddresses.pvkGetImageSubresourceLayout = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyImage");
	if (ptr) {
		vksProcAddresses.pvkDestroyImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateImage");
	if (ptr) {
		vksProcAddresses.pvkCreateImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyBufferView");
	if (ptr) {
		vksProcAddresses.pvkDestroyBufferView = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateBufferView");
	if (ptr) {
		vksProcAddresses.pvkCreateBufferView = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyBuffer");
	if (ptr) {
		vksProcAddresses.pvkDestroyBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateBuffer");
	if (ptr) {
		vksProcAddresses.pvkCreateBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetQueryPoolResults");
	if (ptr) {
		vksProcAddresses.pvkGetQueryPoolResults = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyQueryPool");
	if (ptr) {
		vksProcAddresses.pvkDestroyQueryPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateQueryPool");
	if (ptr) {
		vksProcAddresses.pvkCreateQueryPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetEvent");
	if (ptr) {
		vksProcAddresses.pvkResetEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkSetEvent");
	if (ptr) {
		vksProcAddresses.pvkSetEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetEventStatus");
	if (ptr) {
		vksProcAddresses.pvkGetEventStatus = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyEvent");
	if (ptr) {
		vksProcAddresses.pvkDestroyEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateEvent");
	if (ptr) {
		vksProcAddresses.pvkCreateEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroySemaphore");
	if (ptr) {
		vksProcAddresses.pvkDestroySemaphore = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateSemaphore");
	if (ptr) {
		vksProcAddresses.pvkCreateSemaphore = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkWaitForFences");
	if (ptr) {
		vksProcAddresses.pvkWaitForFences = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetFenceStatus");
	if (ptr) {
		vksProcAddresses.pvkGetFenceStatus = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetFences");
	if (ptr) {
		vksProcAddresses.pvkResetFences = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyFence");
	if (ptr) {
		vksProcAddresses.pvkDestroyFence = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateFence");
	if (ptr) {
		vksProcAddresses.pvkCreateFence = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkQueueBindSparse");
	if (ptr) {
		vksProcAddresses.pvkQueueBindSparse = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateCommandPool");
	if (ptr) {
		vksProcAddresses.pvkCreateCommandPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyCommandPool");
	if (ptr) {
		vksProcAddresses.pvkDestroyCommandPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetCommandPool");
	if (ptr) {
		vksProcAddresses.pvkResetCommandPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSparseImageFormatProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetImageSparseMemoryRequirements");
	if (ptr) {
		vksProcAddresses.pvkGetImageSparseMemoryRequirements = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements");
	if (ptr) {
		vksProcAddresses.pvkGetImageMemoryRequirements = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetBufferMemoryRequirements");
	if (ptr) {
		vksProcAddresses.pvkGetBufferMemoryRequirements = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkBindImageMemory");
	if (ptr) {
		vksProcAddresses.pvkBindImageMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkBindBufferMemory");
	if (ptr) {
		vksProcAddresses.pvkBindBufferMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkAllocateCommandBuffers");
	if (ptr) {
		vksProcAddresses.pvkAllocateCommandBuffers = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkFreeCommandBuffers");
	if (ptr) {
		vksProcAddresses.pvkFreeCommandBuffers = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkBeginCommandBuffer");
	if (ptr) {
		vksProcAddresses.pvkBeginCommandBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEndCommandBuffer");
	if (ptr) {
		vksProcAddresses.pvkEndCommandBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetCommandBuffer");
	if (ptr) {
		vksProcAddresses.pvkResetCommandBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceMemoryCommitment");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceMemoryCommitment = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkInvalidateMappedMemoryRanges");
	if (ptr) {
		vksProcAddresses.pvkInvalidateMappedMemoryRanges = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkFlushMappedMemoryRanges");
	if (ptr) {
		vksProcAddresses.pvkFlushMappedMemoryRanges = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkUnmapMemory");
	if (ptr) {
		vksProcAddresses.pvkUnmapMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkMapMemory");
	if (ptr) {
		vksProcAddresses.pvkMapMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkFreeMemory");
	if (ptr) {
		vksProcAddresses.pvkFreeMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkAllocateMemory");
	if (ptr) {
		vksProcAddresses.pvkAllocateMemory = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDeviceWaitIdle");
	if (ptr) {
		vksProcAddresses.pvkDeviceWaitIdle = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkQueueWaitIdle");
	if (ptr) {
		vksProcAddresses.pvkQueueWaitIdle = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkQueueSubmit");
	if (ptr) {
		vksProcAddresses.pvkQueueSubmit = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceQueue");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceQueue = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBindPipeline");
	if (ptr) {
		vksProcAddresses.pvkCmdBindPipeline = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetViewport");
	if (ptr) {
		vksProcAddresses.pvkCmdSetViewport = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetScissor");
	if (ptr) {
		vksProcAddresses.pvkCmdSetScissor = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetLineWidth");
	if (ptr) {
		vksProcAddresses.pvkCmdSetLineWidth = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumerateDeviceLayerProperties");
	if (ptr) {
		vksProcAddresses.pvkEnumerateDeviceLayerProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetBlendConstants");
	if (ptr) {
		vksProcAddresses.pvkCmdSetBlendConstants = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetDepthBounds");
	if (ptr) {
		vksProcAddresses.pvkCmdSetDepthBounds = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetStencilCompareMask");
	if (ptr) {
		vksProcAddresses.pvkCmdSetStencilCompareMask = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetStencilWriteMask");
	if (ptr) {
		vksProcAddresses.pvkCmdSetStencilWriteMask = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetStencilReference");
	if (ptr) {
		vksProcAddresses.pvkCmdSetStencilReference = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBindDescriptorSets");
	if (ptr) {
		vksProcAddresses.pvkCmdBindDescriptorSets = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBindIndexBuffer");
	if (ptr) {
		vksProcAddresses.pvkCmdBindIndexBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBindVertexBuffers");
	if (ptr) {
		vksProcAddresses.pvkCmdBindVertexBuffers = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDraw");
	if (ptr) {
		vksProcAddresses.pvkCmdDraw = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDrawIndexed");
	if (ptr) {
		vksProcAddresses.pvkCmdDrawIndexed = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDrawIndirect");
	if (ptr) {
		vksProcAddresses.pvkCmdDrawIndirect = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDrawIndexedIndirect");
	if (ptr) {
		vksProcAddresses.pvkCmdDrawIndexedIndirect = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDispatch");
	if (ptr) {
		vksProcAddresses.pvkCmdDispatch = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDispatchIndirect");
	if (ptr) {
		vksProcAddresses.pvkCmdDispatchIndirect = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdCopyBuffer");
	if (ptr) {
		vksProcAddresses.pvkCmdCopyBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdCopyImage");
	if (ptr) {
		vksProcAddresses.pvkCmdCopyImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBlitImage");
	if (ptr) {
		vksProcAddresses.pvkCmdBlitImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdCopyBufferToImage");
	if (ptr) {
		vksProcAddresses.pvkCmdCopyBufferToImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdCopyImageToBuffer");
	if (ptr) {
		vksProcAddresses.pvkCmdCopyImageToBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdUpdateBuffer");
	if (ptr) {
		vksProcAddresses.pvkCmdUpdateBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdFillBuffer");
	if (ptr) {
		vksProcAddresses.pvkCmdFillBuffer = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdClearColorImage");
	if (ptr) {
		vksProcAddresses.pvkCmdClearColorImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdClearDepthStencilImage");
	if (ptr) {
		vksProcAddresses.pvkCmdClearDepthStencilImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdClearAttachments");
	if (ptr) {
		vksProcAddresses.pvkCmdClearAttachments = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdResolveImage");
	if (ptr) {
		vksProcAddresses.pvkCmdResolveImage = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetEvent");
	if (ptr) {
		vksProcAddresses.pvkCmdSetEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdResetEvent");
	if (ptr) {
		vksProcAddresses.pvkCmdResetEvent = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdWaitEvents");
	if (ptr) {
		vksProcAddresses.pvkCmdWaitEvents = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdPipelineBarrier");
	if (ptr) {
		vksProcAddresses.pvkCmdPipelineBarrier = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBeginQuery");
	if (ptr) {
		vksProcAddresses.pvkCmdBeginQuery = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdEndQuery");
	if (ptr) {
		vksProcAddresses.pvkCmdEndQuery = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdResetQueryPool");
	if (ptr) {
		vksProcAddresses.pvkCmdResetQueryPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdWriteTimestamp");
	if (ptr) {
		vksProcAddresses.pvkCmdWriteTimestamp = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdCopyQueryPoolResults");
	if (ptr) {
		vksProcAddresses.pvkCmdCopyQueryPoolResults = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdPushConstants");
	if (ptr) {
		vksProcAddresses.pvkCmdPushConstants = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBeginRenderPass");
	if (ptr) {
		vksProcAddresses.pvkCmdBeginRenderPass = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdNextSubpass");
	if (ptr) {
		vksProcAddresses.pvkCmdNextSubpass = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdEndRenderPass");
	if (ptr) {
		vksProcAddresses.pvkCmdEndRenderPass = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdExecuteCommands");
	if (ptr) {
		vksProcAddresses.pvkCmdExecuteCommands = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumerateInstanceLayerProperties");
	if (ptr) {
		vksProcAddresses.pvkEnumerateInstanceLayerProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
	if (ptr) {
		vksProcAddresses.pvkEnumerateDeviceExtensionProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumerateInstanceExtensionProperties");
	if (ptr) {
		vksProcAddresses.pvkEnumerateInstanceExtensionProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyDevice");
	if (ptr) {
		vksProcAddresses.pvkDestroyDevice = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDevice");
	if (ptr) {
		vksProcAddresses.pvkCreateDevice = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceProcAddr = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetInstanceProcAddr");
	if (ptr) {
		vksProcAddresses.pvkGetInstanceProcAddr = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceMemoryProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceQueueFamilyProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceFormatProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceFeatures = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
	if (ptr) {
		vksProcAddresses.pvkEnumeratePhysicalDevices = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyInstance");
	if (ptr) {
		vksProcAddresses.pvkDestroyInstance = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateInstance");
	if (ptr) {
		vksProcAddresses.pvkCreateInstance = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetDepthBias");
	if (ptr) {
		vksProcAddresses.pvkCmdSetDepthBias = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkUpdateDescriptorSetWithTemplate");
	if (ptr) {
		vksProcAddresses.pvkUpdateDescriptorSetWithTemplate = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroySamplerYcbcrConversion");
	if (ptr) {
		vksProcAddresses.pvkDestroySamplerYcbcrConversion = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDescriptorUpdateTemplate");
	if (ptr) {
		vksProcAddresses.pvkCreateDescriptorUpdateTemplate = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalFenceProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceExternalFenceProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateSamplerYcbcrConversion");
	if (ptr) {
		vksProcAddresses.pvkCreateSamplerYcbcrConversion = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceQueue2");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceQueue2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkTrimCommandPool");
	if (ptr) {
		vksProcAddresses.pvkTrimCommandPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSparseImageFormatProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceMemoryProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceQueueFamilyProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceImageFormatProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceFormatProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceProperties2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceFeatures2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetImageSparseMemoryRequirements2");
	if (ptr) {
		vksProcAddresses.pvkGetImageSparseMemoryRequirements2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetBufferMemoryRequirements2");
	if (ptr) {
		vksProcAddresses.pvkGetBufferMemoryRequirements2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceExternalSemaphoreProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements2");
	if (ptr) {
		vksProcAddresses.pvkGetImageMemoryRequirements2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroyDescriptorUpdateTemplate");
	if (ptr) {
		vksProcAddresses.pvkDestroyDescriptorUpdateTemplate = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
	if (ptr) {
		vksProcAddresses.pvkEnumeratePhysicalDeviceGroups = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDispatchBase");
	if (ptr) {
		vksProcAddresses.pvkCmdDispatchBase = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdSetDeviceMask");
	if (ptr) {
		vksProcAddresses.pvkCmdSetDeviceMask = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceGroupPeerMemoryFeatures");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceGroupPeerMemoryFeatures = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDescriptorSetLayoutSupport");
	if (ptr) {
		vksProcAddresses.pvkGetDescriptorSetLayoutSupport = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkBindImageMemory2");
	if (ptr) {
		vksProcAddresses.pvkBindImageMemory2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkBindBufferMemory2");
	if (ptr) {
		vksProcAddresses.pvkBindBufferMemory2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkEnumerateInstanceVersion");
	if (ptr) {
		vksProcAddresses.pvkEnumerateInstanceVersion = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalBufferProperties");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceExternalBufferProperties = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkWaitSemaphores");
	if (ptr) {
		vksProcAddresses.pvkWaitSemaphores = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkResetQueryPool");
	if (ptr) {
		vksProcAddresses.pvkResetQueryPool = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetSemaphoreCounterValue");
	if (ptr) {
		vksProcAddresses.pvkGetSemaphoreCounterValue = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdBeginRenderPass2");
	if (ptr) {
		vksProcAddresses.pvkCmdBeginRenderPass2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdEndRenderPass2");
	if (ptr) {
		vksProcAddresses.pvkCmdEndRenderPass2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetBufferDeviceAddress");
	if (ptr) {
		vksProcAddresses.pvkGetBufferDeviceAddress = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetBufferOpaqueCaptureAddress");
	if (ptr) {
		vksProcAddresses.pvkGetBufferOpaqueCaptureAddress = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceMemoryOpaqueCaptureAddress");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceMemoryOpaqueCaptureAddress = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdNextSubpass2");
	if (ptr) {
		vksProcAddresses.pvkCmdNextSubpass2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateRenderPass2");
	if (ptr) {
		vksProcAddresses.pvkCreateRenderPass2 = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDrawIndexedIndirectCount");
	if (ptr) {
		vksProcAddresses.pvkCmdDrawIndexedIndirectCount = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCmdDrawIndirectCount");
	if (ptr) {
		vksProcAddresses.pvkCmdDrawIndirectCount = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkSignalSemaphore");
	if (ptr) {
		vksProcAddresses.pvkSignalSemaphore = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfacePresentModesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfaceFormatsKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfaceCapabilitiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfaceSupportKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
	if (ptr) {
		vksProcAddresses.pvkDestroySurfaceKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfaceCapabilities2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormats2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceSurfaceFormats2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDevicePresentRectanglesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDevicePresentRectanglesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkAcquireNextImage2KHR");
	if (ptr) {
		vksProcAddresses.pvkAcquireNextImage2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceGroupSurfacePresentModesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceGroupSurfacePresentModesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDeviceGroupPresentCapabilitiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetDeviceGroupPresentCapabilitiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkQueuePresentKHR");
	if (ptr) {
		vksProcAddresses.pvkQueuePresentKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkAcquireNextImageKHR");
	if (ptr) {
		vksProcAddresses.pvkAcquireNextImageKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetSwapchainImagesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetSwapchainImagesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkDestroySwapchainKHR");
	if (ptr) {
		vksProcAddresses.pvkDestroySwapchainKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR");
	if (ptr) {
		vksProcAddresses.pvkCreateSwapchainKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDisplayPlaneSurfaceKHR");
	if (ptr) {
		vksProcAddresses.pvkCreateDisplayPlaneSurfaceKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDisplayPlaneCapabilitiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetDisplayPlaneCapabilitiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateDisplayModeKHR");
	if (ptr) {
		vksProcAddresses.pvkCreateDisplayModeKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDisplayModePropertiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetDisplayModePropertiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDisplayPlaneSupportedDisplaysKHR");
	if (ptr) {
		vksProcAddresses.pvkGetDisplayPlaneSupportedDisplaysKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceDisplayPlanePropertiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceDisplayPropertiesKHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDisplayPlaneCapabilities2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetDisplayPlaneCapabilities2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceDisplayProperties2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceDisplayProperties2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetPhysicalDeviceDisplayPlaneProperties2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkGetDisplayModeProperties2KHR");
	if (ptr) {
		vksProcAddresses.pvkGetDisplayModeProperties2KHR = ptr;
	}
	ptr = vkGetInstanceProcAddr(instance, "vkCreateSharedSwapchainsKHR");
	if (ptr) {
		vksProcAddresses.pvkCreateSharedSwapchainsKHR = ptr;
	}
}

VkResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceImageFormatProperties)vksProcAddresses.pvkGetPhysicalDeviceImageFormatProperties)(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties));
	return ret;
}
void vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorPool)vksProcAddresses.pvkDestroyDescriptorPool)(device, descriptorPool, pAllocator));
}
VkResult vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
	VkResult ret = (((PFN_vkResetDescriptorPool)vksProcAddresses.pvkResetDescriptorPool)(device, descriptorPool, flags));
	return ret;
}
VkResult vkAllocateDescriptorSets(VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {
	VkResult ret = (((PFN_vkAllocateDescriptorSets)vksProcAddresses.pvkAllocateDescriptorSets)(device, pAllocateInfo, pDescriptorSets));
	return ret;
}
VkResult vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets) {
	VkResult ret = (((PFN_vkFreeDescriptorSets)vksProcAddresses.pvkFreeDescriptorSets)(device, descriptorPool, descriptorSetCount, pDescriptorSets));
	return ret;
}
void vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies) {
	(((PFN_vkUpdateDescriptorSets)vksProcAddresses.pvkUpdateDescriptorSets)(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies));
}
VkResult vkCreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {
	VkResult ret = (((PFN_vkCreateDescriptorPool)vksProcAddresses.pvkCreateDescriptorPool)(device, pCreateInfo, pAllocator, pDescriptorPool));
	return ret;
}
void vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorSetLayout)vksProcAddresses.pvkDestroyDescriptorSetLayout)(device, descriptorSetLayout, pAllocator));
}
VkResult vkCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) {
	VkResult ret = (((PFN_vkCreateDescriptorSetLayout)vksProcAddresses.pvkCreateDescriptorSetLayout)(device, pCreateInfo, pAllocator, pSetLayout));
	return ret;
}
void vkDestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySampler)vksProcAddresses.pvkDestroySampler)(device, sampler, pAllocator));
}
VkResult vkCreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {
	VkResult ret = (((PFN_vkCreateSampler)vksProcAddresses.pvkCreateSampler)(device, pCreateInfo, pAllocator, pSampler));
	return ret;
}
void vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipelineLayout)vksProcAddresses.pvkDestroyPipelineLayout)(device, pipelineLayout, pAllocator));
}
VkResult vkCreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {
	VkResult ret = (((PFN_vkCreatePipelineLayout)vksProcAddresses.pvkCreatePipelineLayout)(device, pCreateInfo, pAllocator, pPipelineLayout));
	return ret;
}
void vkDestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipeline)vksProcAddresses.pvkDestroyPipeline)(device, pipeline, pAllocator));
}
VkResult vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	VkResult ret = (((PFN_vkCreateComputePipelines)vksProcAddresses.pvkCreateComputePipelines)(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines));
	return ret;
}
VkResult vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	VkResult ret = (((PFN_vkCreateGraphicsPipelines)vksProcAddresses.pvkCreateGraphicsPipelines)(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines));
	return ret;
}
VkResult vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches) {
	VkResult ret = (((PFN_vkMergePipelineCaches)vksProcAddresses.pvkMergePipelineCaches)(device, dstCache, srcCacheCount, pSrcCaches));
	return ret;
}
VkResult vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) {
	VkResult ret = (((PFN_vkGetPipelineCacheData)vksProcAddresses.pvkGetPipelineCacheData)(device, pipelineCache, pDataSize, pData));
	return ret;
}
void vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipelineCache)vksProcAddresses.pvkDestroyPipelineCache)(device, pipelineCache, pAllocator));
}
VkResult vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {
	VkResult ret = (((PFN_vkCreatePipelineCache)vksProcAddresses.pvkCreatePipelineCache)(device, pCreateInfo, pAllocator, pPipelineCache));
	return ret;
}
void vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyShaderModule)vksProcAddresses.pvkDestroyShaderModule)(device, shaderModule, pAllocator));
}
VkResult vkCreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
	VkResult ret = (((PFN_vkCreateShaderModule)vksProcAddresses.pvkCreateShaderModule)(device, pCreateInfo, pAllocator, pShaderModule));
	return ret;
}
void vkDestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyImageView)vksProcAddresses.pvkDestroyImageView)(device, imageView, pAllocator));
}
VkResult vkCreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView) {
	VkResult ret = (((PFN_vkCreateImageView)vksProcAddresses.pvkCreateImageView)(device, pCreateInfo, pAllocator, pView));
	return ret;
}
VkResult vkCreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {
	VkResult ret = (((PFN_vkCreateFramebuffer)vksProcAddresses.pvkCreateFramebuffer)(device, pCreateInfo, pAllocator, pFramebuffer));
	return ret;
}
void vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyFramebuffer)vksProcAddresses.pvkDestroyFramebuffer)(device, framebuffer, pAllocator));
}
VkResult vkCreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
	VkResult ret = (((PFN_vkCreateRenderPass)vksProcAddresses.pvkCreateRenderPass)(device, pCreateInfo, pAllocator, pRenderPass));
	return ret;
}
void vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyRenderPass)vksProcAddresses.pvkDestroyRenderPass)(device, renderPass, pAllocator));
}
void vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) {
	(((PFN_vkGetRenderAreaGranularity)vksProcAddresses.pvkGetRenderAreaGranularity)(device, renderPass, pGranularity));
}
void vkGetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {
	(((PFN_vkGetImageSubresourceLayout)vksProcAddresses.pvkGetImageSubresourceLayout)(device, image, pSubresource, pLayout));
}
void vkDestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyImage)vksProcAddresses.pvkDestroyImage)(device, image, pAllocator));
}
VkResult vkCreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage) {
	VkResult ret = (((PFN_vkCreateImage)vksProcAddresses.pvkCreateImage)(device, pCreateInfo, pAllocator, pImage));
	return ret;
}
void vkDestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyBufferView)vksProcAddresses.pvkDestroyBufferView)(device, bufferView, pAllocator));
}
VkResult vkCreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView) {
	VkResult ret = (((PFN_vkCreateBufferView)vksProcAddresses.pvkCreateBufferView)(device, pCreateInfo, pAllocator, pView));
	return ret;
}
void vkDestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyBuffer)vksProcAddresses.pvkDestroyBuffer)(device, buffer, pAllocator));
}
VkResult vkCreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {
	VkResult ret = (((PFN_vkCreateBuffer)vksProcAddresses.pvkCreateBuffer)(device, pCreateInfo, pAllocator, pBuffer));
	return ret;
}
VkResult vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
	VkResult ret = (((PFN_vkGetQueryPoolResults)vksProcAddresses.pvkGetQueryPoolResults)(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags));
	return ret;
}
void vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyQueryPool)vksProcAddresses.pvkDestroyQueryPool)(device, queryPool, pAllocator));
}
VkResult vkCreateQueryPool(VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {
	VkResult ret = (((PFN_vkCreateQueryPool)vksProcAddresses.pvkCreateQueryPool)(device, pCreateInfo, pAllocator, pQueryPool));
	return ret;
}
VkResult vkResetEvent(VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkResetEvent)vksProcAddresses.pvkResetEvent)(device, event));
	return ret;
}
VkResult vkSetEvent(VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkSetEvent)vksProcAddresses.pvkSetEvent)(device, event));
	return ret;
}
VkResult vkGetEventStatus(VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkGetEventStatus)vksProcAddresses.pvkGetEventStatus)(device, event));
	return ret;
}
void vkDestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyEvent)vksProcAddresses.pvkDestroyEvent)(device, event, pAllocator));
}
VkResult vkCreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {
	VkResult ret = (((PFN_vkCreateEvent)vksProcAddresses.pvkCreateEvent)(device, pCreateInfo, pAllocator, pEvent));
	return ret;
}
void vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySemaphore)vksProcAddresses.pvkDestroySemaphore)(device, semaphore, pAllocator));
}
VkResult vkCreateSemaphore(VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {
	VkResult ret = (((PFN_vkCreateSemaphore)vksProcAddresses.pvkCreateSemaphore)(device, pCreateInfo, pAllocator, pSemaphore));
	return ret;
}
VkResult vkWaitForFences(VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
	VkResult ret = (((PFN_vkWaitForFences)vksProcAddresses.pvkWaitForFences)(device, fenceCount, pFences, waitAll, timeout));
	return ret;
}
VkResult vkGetFenceStatus(VkDevice device, VkFence fence) {
	VkResult ret = (((PFN_vkGetFenceStatus)vksProcAddresses.pvkGetFenceStatus)(device, fence));
	return ret;
}
VkResult vkResetFences(VkDevice device, uint32_t fenceCount, VkFence* pFences) {
	VkResult ret = (((PFN_vkResetFences)vksProcAddresses.pvkResetFences)(device, fenceCount, pFences));
	return ret;
}
void vkDestroyFence(VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyFence)vksProcAddresses.pvkDestroyFence)(device, fence, pAllocator));
}
VkResult vkCreateFence(VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence) {
	VkResult ret = (((PFN_vkCreateFence)vksProcAddresses.pvkCreateFence)(device, pCreateInfo, pAllocator, pFence));
	return ret;
}
VkResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence) {
	VkResult ret = (((PFN_vkQueueBindSparse)vksProcAddresses.pvkQueueBindSparse)(queue, bindInfoCount, pBindInfo, fence));
	return ret;
}
VkResult vkCreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {
	VkResult ret = (((PFN_vkCreateCommandPool)vksProcAddresses.pvkCreateCommandPool)(device, pCreateInfo, pAllocator, pCommandPool));
	return ret;
}
void vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyCommandPool)vksProcAddresses.pvkDestroyCommandPool)(device, commandPool, pAllocator));
}
VkResult vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
	VkResult ret = (((PFN_vkResetCommandPool)vksProcAddresses.pvkResetCommandPool)(device, commandPool, flags));
	return ret;
}
void vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) {
	(((PFN_vkGetPhysicalDeviceSparseImageFormatProperties)vksProcAddresses.pvkGetPhysicalDeviceSparseImageFormatProperties)(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties));
}
void vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {
	(((PFN_vkGetImageSparseMemoryRequirements)vksProcAddresses.pvkGetImageSparseMemoryRequirements)(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements));
}
void vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) {
	(((PFN_vkGetImageMemoryRequirements)vksProcAddresses.pvkGetImageMemoryRequirements)(device, image, pMemoryRequirements));
}
void vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) {
	(((PFN_vkGetBufferMemoryRequirements)vksProcAddresses.pvkGetBufferMemoryRequirements)(device, buffer, pMemoryRequirements));
}
VkResult vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	VkResult ret = (((PFN_vkBindImageMemory)vksProcAddresses.pvkBindImageMemory)(device, image, memory, memoryOffset));
	return ret;
}
VkResult vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	VkResult ret = (((PFN_vkBindBufferMemory)vksProcAddresses.pvkBindBufferMemory)(device, buffer, memory, memoryOffset));
	return ret;
}
VkResult vkAllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
	VkResult ret = (((PFN_vkAllocateCommandBuffers)vksProcAddresses.pvkAllocateCommandBuffers)(device, pAllocateInfo, pCommandBuffers));
	return ret;
}
void vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
	(((PFN_vkFreeCommandBuffers)vksProcAddresses.pvkFreeCommandBuffers)(device, commandPool, commandBufferCount, pCommandBuffers));
}
VkResult vkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo) {
	VkResult ret = (((PFN_vkBeginCommandBuffer)vksProcAddresses.pvkBeginCommandBuffer)(commandBuffer, pBeginInfo));
	return ret;
}
VkResult vkEndCommandBuffer(VkCommandBuffer commandBuffer) {
	VkResult ret = (((PFN_vkEndCommandBuffer)vksProcAddresses.pvkEndCommandBuffer)(commandBuffer));
	return ret;
}
VkResult vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
	VkResult ret = (((PFN_vkResetCommandBuffer)vksProcAddresses.pvkResetCommandBuffer)(commandBuffer, flags));
	return ret;
}
void vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) {
	(((PFN_vkGetDeviceMemoryCommitment)vksProcAddresses.pvkGetDeviceMemoryCommitment)(device, memory, pCommittedMemoryInBytes));
}
VkResult vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
	VkResult ret = (((PFN_vkInvalidateMappedMemoryRanges)vksProcAddresses.pvkInvalidateMappedMemoryRanges)(device, memoryRangeCount, pMemoryRanges));
	return ret;
}
VkResult vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
	VkResult ret = (((PFN_vkFlushMappedMemoryRanges)vksProcAddresses.pvkFlushMappedMemoryRanges)(device, memoryRangeCount, pMemoryRanges));
	return ret;
}
void vkUnmapMemory(VkDevice device, VkDeviceMemory memory) {
	(((PFN_vkUnmapMemory)vksProcAddresses.pvkUnmapMemory)(device, memory));
}
VkResult vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
	VkResult ret = (((PFN_vkMapMemory)vksProcAddresses.pvkMapMemory)(device, memory, offset, size, flags, ppData));
	return ret;
}
void vkFreeMemory(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkFreeMemory)vksProcAddresses.pvkFreeMemory)(device, memory, pAllocator));
}
VkResult vkAllocateMemory(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
	VkResult ret = (((PFN_vkAllocateMemory)vksProcAddresses.pvkAllocateMemory)(device, pAllocateInfo, pAllocator, pMemory));
	return ret;
}
VkResult vkDeviceWaitIdle(VkDevice device) {
	VkResult ret = (((PFN_vkDeviceWaitIdle)vksProcAddresses.pvkDeviceWaitIdle)(device));
	return ret;
}
VkResult vkQueueWaitIdle(VkQueue queue) {
	VkResult ret = (((PFN_vkQueueWaitIdle)vksProcAddresses.pvkQueueWaitIdle)(queue));
	return ret;
}
VkResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence) {
	VkResult ret = (((PFN_vkQueueSubmit)vksProcAddresses.pvkQueueSubmit)(queue, submitCount, pSubmits, fence));
	return ret;
}
void vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
	(((PFN_vkGetDeviceQueue)vksProcAddresses.pvkGetDeviceQueue)(device, queueFamilyIndex, queueIndex, pQueue));
}
void vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
	(((PFN_vkCmdBindPipeline)vksProcAddresses.pvkCmdBindPipeline)(commandBuffer, pipelineBindPoint, pipeline));
}
void vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports) {
	(((PFN_vkCmdSetViewport)vksProcAddresses.pvkCmdSetViewport)(commandBuffer, firstViewport, viewportCount, pViewports));
}
void vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors) {
	(((PFN_vkCmdSetScissor)vksProcAddresses.pvkCmdSetScissor)(commandBuffer, firstScissor, scissorCount, pScissors));
}
void vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
	(((PFN_vkCmdSetLineWidth)vksProcAddresses.pvkCmdSetLineWidth)(commandBuffer, lineWidth));
}
VkResult vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateDeviceLayerProperties)vksProcAddresses.pvkEnumerateDeviceLayerProperties)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
void vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, float blendConstants[4]) {
	(((PFN_vkCmdSetBlendConstants)vksProcAddresses.pvkCmdSetBlendConstants)(commandBuffer, blendConstants));
}
void vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
	(((PFN_vkCmdSetDepthBounds)vksProcAddresses.pvkCmdSetDepthBounds)(commandBuffer, minDepthBounds, maxDepthBounds));
}
void vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
	(((PFN_vkCmdSetStencilCompareMask)vksProcAddresses.pvkCmdSetStencilCompareMask)(commandBuffer, faceMask, compareMask));
}
void vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
	(((PFN_vkCmdSetStencilWriteMask)vksProcAddresses.pvkCmdSetStencilWriteMask)(commandBuffer, faceMask, writeMask));
}
void vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
	(((PFN_vkCmdSetStencilReference)vksProcAddresses.pvkCmdSetStencilReference)(commandBuffer, faceMask, reference));
}
void vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets) {
	(((PFN_vkCmdBindDescriptorSets)vksProcAddresses.pvkCmdBindDescriptorSets)(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets));
}
void vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
	(((PFN_vkCmdBindIndexBuffer)vksProcAddresses.pvkCmdBindIndexBuffer)(commandBuffer, buffer, offset, indexType));
}
void vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets) {
	(((PFN_vkCmdBindVertexBuffers)vksProcAddresses.pvkCmdBindVertexBuffers)(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets));
}
void vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
	(((PFN_vkCmdDraw)vksProcAddresses.pvkCmdDraw)(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance));
}
void vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
	(((PFN_vkCmdDrawIndexed)vksProcAddresses.pvkCmdDrawIndexed)(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance));
}
void vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndirect)vksProcAddresses.pvkCmdDrawIndirect)(commandBuffer, buffer, offset, drawCount, stride));
}
void vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndexedIndirect)vksProcAddresses.pvkCmdDrawIndexedIndirect)(commandBuffer, buffer, offset, drawCount, stride));
}
void vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	(((PFN_vkCmdDispatch)vksProcAddresses.pvkCmdDispatch)(commandBuffer, groupCountX, groupCountY, groupCountZ));
}
void vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
	(((PFN_vkCmdDispatchIndirect)vksProcAddresses.pvkCmdDispatchIndirect)(commandBuffer, buffer, offset));
}
void vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions) {
	(((PFN_vkCmdCopyBuffer)vksProcAddresses.pvkCmdCopyBuffer)(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions));
}
void vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions) {
	(((PFN_vkCmdCopyImage)vksProcAddresses.pvkCmdCopyImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter) {
	(((PFN_vkCmdBlitImage)vksProcAddresses.pvkCmdBlitImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter));
}
void vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions) {
	(((PFN_vkCmdCopyBufferToImage)vksProcAddresses.pvkCmdCopyBufferToImage)(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions) {
	(((PFN_vkCmdCopyImageToBuffer)vksProcAddresses.pvkCmdCopyImageToBuffer)(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions));
}
void vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData) {
	(((PFN_vkCmdUpdateBuffer)vksProcAddresses.pvkCmdUpdateBuffer)(commandBuffer, dstBuffer, dstOffset, dataSize, pData));
}
void vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
	(((PFN_vkCmdFillBuffer)vksProcAddresses.pvkCmdFillBuffer)(commandBuffer, dstBuffer, dstOffset, size, data));
}
void vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
	(((PFN_vkCmdClearColorImage)vksProcAddresses.pvkCmdClearColorImage)(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges));
}
void vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
	(((PFN_vkCmdClearDepthStencilImage)vksProcAddresses.pvkCmdClearDepthStencilImage)(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges));
}
void vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects) {
	(((PFN_vkCmdClearAttachments)vksProcAddresses.pvkCmdClearAttachments)(commandBuffer, attachmentCount, pAttachments, rectCount, pRects));
}
void vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions) {
	(((PFN_vkCmdResolveImage)vksProcAddresses.pvkCmdResolveImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	(((PFN_vkCmdSetEvent)vksProcAddresses.pvkCmdSetEvent)(commandBuffer, event, stageMask));
}
void vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	(((PFN_vkCmdResetEvent)vksProcAddresses.pvkCmdResetEvent)(commandBuffer, event, stageMask));
}
void vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
	(((PFN_vkCmdWaitEvents)vksProcAddresses.pvkCmdWaitEvents)(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers));
}
void vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
	(((PFN_vkCmdPipelineBarrier)vksProcAddresses.pvkCmdPipelineBarrier)(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers));
}
void vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
	(((PFN_vkCmdBeginQuery)vksProcAddresses.pvkCmdBeginQuery)(commandBuffer, queryPool, query, flags));
}
void vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
	(((PFN_vkCmdEndQuery)vksProcAddresses.pvkCmdEndQuery)(commandBuffer, queryPool, query));
}
void vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	(((PFN_vkCmdResetQueryPool)vksProcAddresses.pvkCmdResetQueryPool)(commandBuffer, queryPool, firstQuery, queryCount));
}
void vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
	(((PFN_vkCmdWriteTimestamp)vksProcAddresses.pvkCmdWriteTimestamp)(commandBuffer, pipelineStage, queryPool, query));
}
void vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
	(((PFN_vkCmdCopyQueryPoolResults)vksProcAddresses.pvkCmdCopyQueryPoolResults)(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags));
}
void vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues) {
	(((PFN_vkCmdPushConstants)vksProcAddresses.pvkCmdPushConstants)(commandBuffer, layout, stageFlags, offset, size, pValues));
}
void vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
	(((PFN_vkCmdBeginRenderPass)vksProcAddresses.pvkCmdBeginRenderPass)(commandBuffer, pRenderPassBegin, contents));
}
void vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
	(((PFN_vkCmdNextSubpass)vksProcAddresses.pvkCmdNextSubpass)(commandBuffer, contents));
}
void vkCmdEndRenderPass(VkCommandBuffer commandBuffer) {
	(((PFN_vkCmdEndRenderPass)vksProcAddresses.pvkCmdEndRenderPass)(commandBuffer));
}
void vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
	(((PFN_vkCmdExecuteCommands)vksProcAddresses.pvkCmdExecuteCommands)(commandBuffer, commandBufferCount, pCommandBuffers));
}
VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateInstanceLayerProperties)vksProcAddresses.pvkEnumerateInstanceLayerProperties)(pPropertyCount, pProperties));
	return ret;
}
VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateDeviceExtensionProperties)vksProcAddresses.pvkEnumerateDeviceExtensionProperties)(physicalDevice, pLayerName, pPropertyCount, pProperties));
	return ret;
}
VkResult vkEnumerateInstanceExtensionProperties(char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateInstanceExtensionProperties)vksProcAddresses.pvkEnumerateInstanceExtensionProperties)(pLayerName, pPropertyCount, pProperties));
	return ret;
}
void vkDestroyDevice(VkDevice device, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDevice)vksProcAddresses.pvkDestroyDevice)(device, pAllocator));
}
VkResult vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
	VkResult ret = (((PFN_vkCreateDevice)vksProcAddresses.pvkCreateDevice)(physicalDevice, pCreateInfo, pAllocator, pDevice));
	return ret;
}
PFN_vkVoidFunction vkGetDeviceProcAddr(VkDevice device, char* pName) {
	PFN_vkVoidFunction ret = (((PFN_vkGetDeviceProcAddr)vksProcAddresses.pvkGetDeviceProcAddr)(device, pName));
	return ret;
}
PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, char* pName) {
	PFN_vkVoidFunction ret = (((PFN_vkGetInstanceProcAddr)vksProcAddresses.pvkGetInstanceProcAddr)(instance, pName));
	return ret;
}
void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
	(((PFN_vkGetPhysicalDeviceMemoryProperties)vksProcAddresses.pvkGetPhysicalDeviceMemoryProperties)(physicalDevice, pMemoryProperties));
}
void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) {
	(((PFN_vkGetPhysicalDeviceQueueFamilyProperties)vksProcAddresses.pvkGetPhysicalDeviceQueueFamilyProperties)(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties));
}
void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) {
	(((PFN_vkGetPhysicalDeviceProperties)vksProcAddresses.pvkGetPhysicalDeviceProperties)(physicalDevice, pProperties));
}
void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) {
	(((PFN_vkGetPhysicalDeviceFormatProperties)vksProcAddresses.pvkGetPhysicalDeviceFormatProperties)(physicalDevice, format, pFormatProperties));
}
void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
	(((PFN_vkGetPhysicalDeviceFeatures)vksProcAddresses.pvkGetPhysicalDeviceFeatures)(physicalDevice, pFeatures));
}
VkResult vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) {
	VkResult ret = (((PFN_vkEnumeratePhysicalDevices)vksProcAddresses.pvkEnumeratePhysicalDevices)(instance, pPhysicalDeviceCount, pPhysicalDevices));
	return ret;
}
void vkDestroyInstance(VkInstance instance, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyInstance)vksProcAddresses.pvkDestroyInstance)(instance, pAllocator));
}
VkResult vkCreateInstance(VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
	VkResult ret = (((PFN_vkCreateInstance)vksProcAddresses.pvkCreateInstance)(pCreateInfo, pAllocator, pInstance));
	vksInitializeInstanceProcAddrs(*pInstance);
	return ret;
}
void vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
	(((PFN_vkCmdSetDepthBias)vksProcAddresses.pvkCmdSetDepthBias)(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor));
}
void vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData) {
	(((PFN_vkUpdateDescriptorSetWithTemplate)vksProcAddresses.pvkUpdateDescriptorSetWithTemplate)(device, descriptorSet, descriptorUpdateTemplate, pData));
}
void vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySamplerYcbcrConversion)vksProcAddresses.pvkDestroySamplerYcbcrConversion)(device, ycbcrConversion, pAllocator));
}
VkResult vkCreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
	VkResult ret = (((PFN_vkCreateDescriptorUpdateTemplate)vksProcAddresses.pvkCreateDescriptorUpdateTemplate)(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate));
	return ret;
}
void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
	(((PFN_vkGetPhysicalDeviceExternalFenceProperties)vksProcAddresses.pvkGetPhysicalDeviceExternalFenceProperties)(physicalDevice, pExternalFenceInfo, pExternalFenceProperties));
}
VkResult vkCreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
	VkResult ret = (((PFN_vkCreateSamplerYcbcrConversion)vksProcAddresses.pvkCreateSamplerYcbcrConversion)(device, pCreateInfo, pAllocator, pYcbcrConversion));
	return ret;
}
void vkGetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) {
	(((PFN_vkGetDeviceQueue2)vksProcAddresses.pvkGetDeviceQueue2)(device, pQueueInfo, pQueue));
}
void vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
	(((PFN_vkTrimCommandPool)vksProcAddresses.pvkTrimCommandPool)(device, commandPool, flags));
}
void vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
	(((PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)vksProcAddresses.pvkGetPhysicalDeviceSparseImageFormatProperties2)(physicalDevice, pFormatInfo, pPropertyCount, pProperties));
}
void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
	(((PFN_vkGetPhysicalDeviceMemoryProperties2)vksProcAddresses.pvkGetPhysicalDeviceMemoryProperties2)(physicalDevice, pMemoryProperties));
}
void vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
	(((PFN_vkGetPhysicalDeviceQueueFamilyProperties2)vksProcAddresses.pvkGetPhysicalDeviceQueueFamilyProperties2)(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties));
}
VkResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceImageFormatProperties2)vksProcAddresses.pvkGetPhysicalDeviceImageFormatProperties2)(physicalDevice, pImageFormatInfo, pImageFormatProperties));
	return ret;
}
void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
	(((PFN_vkGetPhysicalDeviceFormatProperties2)vksProcAddresses.pvkGetPhysicalDeviceFormatProperties2)(physicalDevice, format, pFormatProperties));
}
void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
	(((PFN_vkGetPhysicalDeviceProperties2)vksProcAddresses.pvkGetPhysicalDeviceProperties2)(physicalDevice, pProperties));
}
void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
	(((PFN_vkGetPhysicalDeviceFeatures2)vksProcAddresses.pvkGetPhysicalDeviceFeatures2)(physicalDevice, pFeatures));
}
void vkGetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
	(((PFN_vkGetImageSparseMemoryRequirements2)vksProcAddresses.pvkGetImageSparseMemoryRequirements2)(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements));
}
void vkGetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
	(((PFN_vkGetBufferMemoryRequirements2)vksProcAddresses.pvkGetBufferMemoryRequirements2)(device, pInfo, pMemoryRequirements));
}
void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
	(((PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)vksProcAddresses.pvkGetPhysicalDeviceExternalSemaphoreProperties)(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties));
}
void vkGetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
	(((PFN_vkGetImageMemoryRequirements2)vksProcAddresses.pvkGetImageMemoryRequirements2)(device, pInfo, pMemoryRequirements));
}
void vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorUpdateTemplate)vksProcAddresses.pvkDestroyDescriptorUpdateTemplate)(device, descriptorUpdateTemplate, pAllocator));
}
VkResult vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
	VkResult ret = (((PFN_vkEnumeratePhysicalDeviceGroups)vksProcAddresses.pvkEnumeratePhysicalDeviceGroups)(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties));
	return ret;
}
void vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	(((PFN_vkCmdDispatchBase)vksProcAddresses.pvkCmdDispatchBase)(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ));
}
void vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
	(((PFN_vkCmdSetDeviceMask)vksProcAddresses.pvkCmdSetDeviceMask)(commandBuffer, deviceMask));
}
void vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
	(((PFN_vkGetDeviceGroupPeerMemoryFeatures)vksProcAddresses.pvkGetDeviceGroupPeerMemoryFeatures)(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures));
}
void vkGetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
	(((PFN_vkGetDescriptorSetLayoutSupport)vksProcAddresses.pvkGetDescriptorSetLayoutSupport)(device, pCreateInfo, pSupport));
}
VkResult vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos) {
	VkResult ret = (((PFN_vkBindImageMemory2)vksProcAddresses.pvkBindImageMemory2)(device, bindInfoCount, pBindInfos));
	return ret;
}
VkResult vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos) {
	VkResult ret = (((PFN_vkBindBufferMemory2)vksProcAddresses.pvkBindBufferMemory2)(device, bindInfoCount, pBindInfos));
	return ret;
}
VkResult vkEnumerateInstanceVersion(uint32_t* pApiVersion) {
	VkResult ret = (((PFN_vkEnumerateInstanceVersion)vksProcAddresses.pvkEnumerateInstanceVersion)(pApiVersion));
	return ret;
}
void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
	(((PFN_vkGetPhysicalDeviceExternalBufferProperties)vksProcAddresses.pvkGetPhysicalDeviceExternalBufferProperties)(physicalDevice, pExternalBufferInfo, pExternalBufferProperties));
}
VkResult vkWaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
	VkResult ret = (((PFN_vkWaitSemaphores)vksProcAddresses.pvkWaitSemaphores)(device, pWaitInfo, timeout));
	return ret;
}
void vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	(((PFN_vkResetQueryPool)vksProcAddresses.pvkResetQueryPool)(device, queryPool, firstQuery, queryCount));
}
VkResult vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
	VkResult ret = (((PFN_vkGetSemaphoreCounterValue)vksProcAddresses.pvkGetSemaphoreCounterValue)(device, semaphore, pValue));
	return ret;
}
void vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo) {
	(((PFN_vkCmdBeginRenderPass2)vksProcAddresses.pvkCmdBeginRenderPass2)(commandBuffer, pRenderPassBegin, pSubpassBeginInfo));
}
void vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo) {
	(((PFN_vkCmdEndRenderPass2)vksProcAddresses.pvkCmdEndRenderPass2)(commandBuffer, pSubpassEndInfo));
}
VkDeviceAddress vkGetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
	VkDeviceAddress ret = (((PFN_vkGetBufferDeviceAddress)vksProcAddresses.pvkGetBufferDeviceAddress)(device, pInfo));
	return ret;
}
uint64_t vkGetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
	uint64_t ret = (((PFN_vkGetBufferOpaqueCaptureAddress)vksProcAddresses.pvkGetBufferOpaqueCaptureAddress)(device, pInfo));
	return ret;
}
uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
	uint64_t ret = (((PFN_vkGetDeviceMemoryOpaqueCaptureAddress)vksProcAddresses.pvkGetDeviceMemoryOpaqueCaptureAddress)(device, pInfo));
	return ret;
}
void vkCmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo) {
	(((PFN_vkCmdNextSubpass2)vksProcAddresses.pvkCmdNextSubpass2)(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo));
}
VkResult vkCreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
	VkResult ret = (((PFN_vkCreateRenderPass2)vksProcAddresses.pvkCreateRenderPass2)(device, pCreateInfo, pAllocator, pRenderPass));
	return ret;
}
void vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndexedIndirectCount)vksProcAddresses.pvkCmdDrawIndexedIndirectCount)(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride));
}
void vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndirectCount)vksProcAddresses.pvkCmdDrawIndirectCount)(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride));
}
VkResult vkSignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo) {
	VkResult ret = (((PFN_vkSignalSemaphore)vksProcAddresses.pvkSignalSemaphore)(device, pSignalInfo));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vksProcAddresses.pvkGetPhysicalDeviceSurfacePresentModesKHR)(physicalDevice, surface, pPresentModeCount, pPresentModes));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vksProcAddresses.pvkGetPhysicalDeviceSurfaceFormatsKHR)(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vksProcAddresses.pvkGetPhysicalDeviceSurfaceCapabilitiesKHR)(physicalDevice, surface, pSurfaceCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceSupportKHR)vksProcAddresses.pvkGetPhysicalDeviceSurfaceSupportKHR)(physicalDevice, queueFamilyIndex, surface, pSupported));
	return ret;
}
void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySurfaceKHR)vksProcAddresses.pvkDestroySurfaceKHR)(instance, surface, pAllocator));
}
VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)vksProcAddresses.pvkGetPhysicalDeviceSurfaceCapabilities2KHR)(physicalDevice, pSurfaceInfo, pSurfaceCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)vksProcAddresses.pvkGetPhysicalDeviceSurfaceFormats2KHR)(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats));
	return ret;
}
VkResult vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {
	VkResult ret = (((PFN_vkGetPhysicalDevicePresentRectanglesKHR)vksProcAddresses.pvkGetPhysicalDevicePresentRectanglesKHR)(physicalDevice, surface, pRectCount, pRects));
	return ret;
}
VkResult vkAcquireNextImage2KHR(VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) {
	VkResult ret = (((PFN_vkAcquireNextImage2KHR)vksProcAddresses.pvkAcquireNextImage2KHR)(device, pAcquireInfo, pImageIndex));
	return ret;
}
VkResult vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) {
	VkResult ret = (((PFN_vkGetDeviceGroupSurfacePresentModesKHR)vksProcAddresses.pvkGetDeviceGroupSurfacePresentModesKHR)(device, surface, pModes));
	return ret;
}
VkResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) {
	VkResult ret = (((PFN_vkGetDeviceGroupPresentCapabilitiesKHR)vksProcAddresses.pvkGetDeviceGroupPresentCapabilitiesKHR)(device, pDeviceGroupPresentCapabilities));
	return ret;
}
VkResult vkQueuePresentKHR(VkQueue queue, VkPresentInfoKHR* pPresentInfo) {
	VkResult ret = (((PFN_vkQueuePresentKHR)vksProcAddresses.pvkQueuePresentKHR)(queue, pPresentInfo));
	return ret;
}
VkResult vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
	VkResult ret = (((PFN_vkAcquireNextImageKHR)vksProcAddresses.pvkAcquireNextImageKHR)(device, swapchain, timeout, semaphore, fence, pImageIndex));
	return ret;
}
VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
	VkResult ret = (((PFN_vkGetSwapchainImagesKHR)vksProcAddresses.pvkGetSwapchainImagesKHR)(device, swapchain, pSwapchainImageCount, pSwapchainImages));
	return ret;
}
void vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySwapchainKHR)vksProcAddresses.pvkDestroySwapchainKHR)(device, swapchain, pAllocator));
}
VkResult vkCreateSwapchainKHR(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
	VkResult ret = (((PFN_vkCreateSwapchainKHR)vksProcAddresses.pvkCreateSwapchainKHR)(device, pCreateInfo, pAllocator, pSwapchain));
	return ret;
}
VkResult vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	VkResult ret = (((PFN_vkCreateDisplayPlaneSurfaceKHR)vksProcAddresses.pvkCreateDisplayPlaneSurfaceKHR)(instance, pCreateInfo, pAllocator, pSurface));
	return ret;
}
VkResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) {
	VkResult ret = (((PFN_vkGetDisplayPlaneCapabilitiesKHR)vksProcAddresses.pvkGetDisplayPlaneCapabilitiesKHR)(physicalDevice, mode, planeIndex, pCapabilities));
	return ret;
}
VkResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) {
	VkResult ret = (((PFN_vkCreateDisplayModeKHR)vksProcAddresses.pvkCreateDisplayModeKHR)(physicalDevice, display, pCreateInfo, pAllocator, pMode));
	return ret;
}
VkResult vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetDisplayModePropertiesKHR)vksProcAddresses.pvkGetDisplayModePropertiesKHR)(physicalDevice, display, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {
	VkResult ret = (((PFN_vkGetDisplayPlaneSupportedDisplaysKHR)vksProcAddresses.pvkGetDisplayPlaneSupportedDisplaysKHR)(physicalDevice, planeIndex, pDisplayCount, pDisplays));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)vksProcAddresses.pvkGetPhysicalDeviceDisplayPlanePropertiesKHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)vksProcAddresses.pvkGetPhysicalDeviceDisplayPropertiesKHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) {
	VkResult ret = (((PFN_vkGetDisplayPlaneCapabilities2KHR)vksProcAddresses.pvkGetDisplayPlaneCapabilities2KHR)(physicalDevice, pDisplayPlaneInfo, pCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayProperties2KHR)vksProcAddresses.pvkGetPhysicalDeviceDisplayProperties2KHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)vksProcAddresses.pvkGetPhysicalDeviceDisplayPlaneProperties2KHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetDisplayModeProperties2KHR)vksProcAddresses.pvkGetDisplayModeProperties2KHR)(physicalDevice, display, pPropertyCount, pProperties));
	return ret;
}
VkResult vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) {
	VkResult ret = (((PFN_vkCreateSharedSwapchainsKHR)vksProcAddresses.pvkCreateSharedSwapchainsKHR)(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains));
	return ret;
}
