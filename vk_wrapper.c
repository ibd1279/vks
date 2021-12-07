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

	vksProcAddresses.pvkEnumerateInstanceVersion = vkGetInstanceProcAddr(&vksProcAddresses, NULL, "vkEnumerateInstanceVersion");
	vksProcAddresses.pvkEnumerateInstanceExtensionProperties = vkGetInstanceProcAddr(&vksProcAddresses, NULL, "vkEnumerateInstanceExtensionProperties");
	vksProcAddresses.pvkEnumerateInstanceLayerProperties = vkGetInstanceProcAddr(&vksProcAddresses, NULL, "vkEnumerateInstanceLayerProperties");
	vksProcAddresses.pvkCreateInstance = vkGetInstanceProcAddr(&vksProcAddresses, NULL, "vkCreateInstance");
	return VK_SUCCESS;
}

void vksDynamicUnload() {
	dlclose(vulkanHandle);
}

// call getProcAddress for all the commands.
void vksLoadInstanceProcAddrs(VkInstance hndl, vksProcAddr* addrs) { 
	addrs->pvkGetPhysicalDeviceImageFormatProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceImageFormatProperties");
	addrs->pvkDestroyDescriptorPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyDescriptorPool");
	addrs->pvkResetDescriptorPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetDescriptorPool");
	addrs->pvkAllocateDescriptorSets = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkAllocateDescriptorSets");
	addrs->pvkFreeDescriptorSets = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkFreeDescriptorSets");
	addrs->pvkUpdateDescriptorSets = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkUpdateDescriptorSets");
	addrs->pvkCreateDescriptorPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDescriptorPool");
	addrs->pvkDestroyDescriptorSetLayout = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyDescriptorSetLayout");
	addrs->pvkCreateDescriptorSetLayout = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDescriptorSetLayout");
	addrs->pvkDestroySampler = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroySampler");
	addrs->pvkCreateSampler = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateSampler");
	addrs->pvkDestroyPipelineLayout = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyPipelineLayout");
	addrs->pvkCreatePipelineLayout = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreatePipelineLayout");
	addrs->pvkDestroyPipeline = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyPipeline");
	addrs->pvkCreateComputePipelines = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateComputePipelines");
	addrs->pvkCreateGraphicsPipelines = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateGraphicsPipelines");
	addrs->pvkMergePipelineCaches = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkMergePipelineCaches");
	addrs->pvkGetPipelineCacheData = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPipelineCacheData");
	addrs->pvkDestroyPipelineCache = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyPipelineCache");
	addrs->pvkCreatePipelineCache = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreatePipelineCache");
	addrs->pvkDestroyShaderModule = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyShaderModule");
	addrs->pvkCreateShaderModule = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateShaderModule");
	addrs->pvkDestroyImageView = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyImageView");
	addrs->pvkCreateImageView = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateImageView");
	addrs->pvkCreateFramebuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateFramebuffer");
	addrs->pvkDestroyFramebuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyFramebuffer");
	addrs->pvkCreateRenderPass = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateRenderPass");
	addrs->pvkDestroyRenderPass = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyRenderPass");
	addrs->pvkGetRenderAreaGranularity = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetRenderAreaGranularity");
	addrs->pvkGetImageSubresourceLayout = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetImageSubresourceLayout");
	addrs->pvkDestroyImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyImage");
	addrs->pvkCreateImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateImage");
	addrs->pvkDestroyBufferView = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyBufferView");
	addrs->pvkCreateBufferView = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateBufferView");
	addrs->pvkDestroyBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyBuffer");
	addrs->pvkCreateBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateBuffer");
	addrs->pvkGetQueryPoolResults = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetQueryPoolResults");
	addrs->pvkDestroyQueryPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyQueryPool");
	addrs->pvkCreateQueryPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateQueryPool");
	addrs->pvkResetEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetEvent");
	addrs->pvkSetEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkSetEvent");
	addrs->pvkGetEventStatus = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetEventStatus");
	addrs->pvkDestroyEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyEvent");
	addrs->pvkCreateEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateEvent");
	addrs->pvkDestroySemaphore = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroySemaphore");
	addrs->pvkCreateSemaphore = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateSemaphore");
	addrs->pvkWaitForFences = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkWaitForFences");
	addrs->pvkGetFenceStatus = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetFenceStatus");
	addrs->pvkResetFences = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetFences");
	addrs->pvkDestroyFence = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyFence");
	addrs->pvkCreateFence = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateFence");
	addrs->pvkQueueBindSparse = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkQueueBindSparse");
	addrs->pvkCreateCommandPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateCommandPool");
	addrs->pvkDestroyCommandPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyCommandPool");
	addrs->pvkResetCommandPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetCommandPool");
	addrs->pvkGetPhysicalDeviceSparseImageFormatProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSparseImageFormatProperties");
	addrs->pvkGetImageSparseMemoryRequirements = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetImageSparseMemoryRequirements");
	addrs->pvkGetImageMemoryRequirements = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetImageMemoryRequirements");
	addrs->pvkGetBufferMemoryRequirements = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetBufferMemoryRequirements");
	addrs->pvkBindImageMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkBindImageMemory");
	addrs->pvkBindBufferMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkBindBufferMemory");
	addrs->pvkAllocateCommandBuffers = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkAllocateCommandBuffers");
	addrs->pvkFreeCommandBuffers = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkFreeCommandBuffers");
	addrs->pvkBeginCommandBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkBeginCommandBuffer");
	addrs->pvkEndCommandBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEndCommandBuffer");
	addrs->pvkResetCommandBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetCommandBuffer");
	addrs->pvkGetDeviceMemoryCommitment = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceMemoryCommitment");
	addrs->pvkInvalidateMappedMemoryRanges = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkInvalidateMappedMemoryRanges");
	addrs->pvkFlushMappedMemoryRanges = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkFlushMappedMemoryRanges");
	addrs->pvkUnmapMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkUnmapMemory");
	addrs->pvkMapMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkMapMemory");
	addrs->pvkFreeMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkFreeMemory");
	addrs->pvkAllocateMemory = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkAllocateMemory");
	addrs->pvkDeviceWaitIdle = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDeviceWaitIdle");
	addrs->pvkQueueWaitIdle = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkQueueWaitIdle");
	addrs->pvkQueueSubmit = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkQueueSubmit");
	addrs->pvkGetDeviceQueue = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceQueue");
	addrs->pvkCmdBindPipeline = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBindPipeline");
	addrs->pvkCmdSetViewport = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetViewport");
	addrs->pvkCmdSetScissor = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetScissor");
	addrs->pvkCmdSetLineWidth = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetLineWidth");
	addrs->pvkEnumerateDeviceLayerProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumerateDeviceLayerProperties");
	addrs->pvkCmdSetBlendConstants = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetBlendConstants");
	addrs->pvkCmdSetDepthBounds = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetDepthBounds");
	addrs->pvkCmdSetStencilCompareMask = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetStencilCompareMask");
	addrs->pvkCmdSetStencilWriteMask = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetStencilWriteMask");
	addrs->pvkCmdSetStencilReference = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetStencilReference");
	addrs->pvkCmdBindDescriptorSets = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBindDescriptorSets");
	addrs->pvkCmdBindIndexBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBindIndexBuffer");
	addrs->pvkCmdBindVertexBuffers = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBindVertexBuffers");
	addrs->pvkCmdDraw = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDraw");
	addrs->pvkCmdDrawIndexed = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDrawIndexed");
	addrs->pvkCmdDrawIndirect = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDrawIndirect");
	addrs->pvkCmdDrawIndexedIndirect = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDrawIndexedIndirect");
	addrs->pvkCmdDispatch = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDispatch");
	addrs->pvkCmdDispatchIndirect = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDispatchIndirect");
	addrs->pvkCmdCopyBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdCopyBuffer");
	addrs->pvkCmdCopyImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdCopyImage");
	addrs->pvkCmdBlitImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBlitImage");
	addrs->pvkCmdCopyBufferToImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdCopyBufferToImage");
	addrs->pvkCmdCopyImageToBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdCopyImageToBuffer");
	addrs->pvkCmdUpdateBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdUpdateBuffer");
	addrs->pvkCmdFillBuffer = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdFillBuffer");
	addrs->pvkCmdClearColorImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdClearColorImage");
	addrs->pvkCmdClearDepthStencilImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdClearDepthStencilImage");
	addrs->pvkCmdClearAttachments = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdClearAttachments");
	addrs->pvkCmdResolveImage = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdResolveImage");
	addrs->pvkCmdSetEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetEvent");
	addrs->pvkCmdResetEvent = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdResetEvent");
	addrs->pvkCmdWaitEvents = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdWaitEvents");
	addrs->pvkCmdPipelineBarrier = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdPipelineBarrier");
	addrs->pvkCmdBeginQuery = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBeginQuery");
	addrs->pvkCmdEndQuery = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdEndQuery");
	addrs->pvkCmdResetQueryPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdResetQueryPool");
	addrs->pvkCmdWriteTimestamp = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdWriteTimestamp");
	addrs->pvkCmdCopyQueryPoolResults = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdCopyQueryPoolResults");
	addrs->pvkCmdPushConstants = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdPushConstants");
	addrs->pvkCmdBeginRenderPass = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBeginRenderPass");
	addrs->pvkCmdNextSubpass = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdNextSubpass");
	addrs->pvkCmdEndRenderPass = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdEndRenderPass");
	addrs->pvkCmdExecuteCommands = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdExecuteCommands");
	addrs->pvkEnumerateInstanceLayerProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumerateInstanceLayerProperties");
	addrs->pvkEnumerateDeviceExtensionProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumerateDeviceExtensionProperties");
	addrs->pvkEnumerateInstanceExtensionProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumerateInstanceExtensionProperties");
	addrs->pvkDestroyDevice = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyDevice");
	addrs->pvkCreateDevice = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDevice");
	addrs->pvkGetDeviceProcAddr = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceProcAddr");
	addrs->pvkGetInstanceProcAddr = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetInstanceProcAddr");
	addrs->pvkGetPhysicalDeviceMemoryProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceMemoryProperties");
	addrs->pvkGetPhysicalDeviceQueueFamilyProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceQueueFamilyProperties");
	addrs->pvkGetPhysicalDeviceProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceProperties");
	addrs->pvkGetPhysicalDeviceFormatProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceFormatProperties");
	addrs->pvkGetPhysicalDeviceFeatures = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceFeatures");
	addrs->pvkEnumeratePhysicalDevices = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumeratePhysicalDevices");
	addrs->pvkDestroyInstance = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyInstance");
	addrs->pvkCreateInstance = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateInstance");
	addrs->pvkCmdSetDepthBias = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetDepthBias");
	addrs->pvkUpdateDescriptorSetWithTemplate = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkUpdateDescriptorSetWithTemplate");
	addrs->pvkDestroySamplerYcbcrConversion = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroySamplerYcbcrConversion");
	addrs->pvkCreateDescriptorUpdateTemplate = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDescriptorUpdateTemplate");
	addrs->pvkGetPhysicalDeviceExternalFenceProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceExternalFenceProperties");
	addrs->pvkCreateSamplerYcbcrConversion = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateSamplerYcbcrConversion");
	addrs->pvkGetDeviceQueue2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceQueue2");
	addrs->pvkTrimCommandPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkTrimCommandPool");
	addrs->pvkGetPhysicalDeviceSparseImageFormatProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSparseImageFormatProperties2");
	addrs->pvkGetPhysicalDeviceMemoryProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceMemoryProperties2");
	addrs->pvkGetPhysicalDeviceQueueFamilyProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceQueueFamilyProperties2");
	addrs->pvkGetPhysicalDeviceImageFormatProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceImageFormatProperties2");
	addrs->pvkGetPhysicalDeviceFormatProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceFormatProperties2");
	addrs->pvkGetPhysicalDeviceProperties2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceProperties2");
	addrs->pvkGetPhysicalDeviceFeatures2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceFeatures2");
	addrs->pvkGetImageSparseMemoryRequirements2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetImageSparseMemoryRequirements2");
	addrs->pvkGetBufferMemoryRequirements2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetBufferMemoryRequirements2");
	addrs->pvkGetPhysicalDeviceExternalSemaphoreProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceExternalSemaphoreProperties");
	addrs->pvkGetImageMemoryRequirements2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetImageMemoryRequirements2");
	addrs->pvkDestroyDescriptorUpdateTemplate = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroyDescriptorUpdateTemplate");
	addrs->pvkEnumeratePhysicalDeviceGroups = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumeratePhysicalDeviceGroups");
	addrs->pvkCmdDispatchBase = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDispatchBase");
	addrs->pvkCmdSetDeviceMask = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdSetDeviceMask");
	addrs->pvkGetDeviceGroupPeerMemoryFeatures = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceGroupPeerMemoryFeatures");
	addrs->pvkGetDescriptorSetLayoutSupport = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDescriptorSetLayoutSupport");
	addrs->pvkBindImageMemory2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkBindImageMemory2");
	addrs->pvkBindBufferMemory2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkBindBufferMemory2");
	addrs->pvkEnumerateInstanceVersion = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkEnumerateInstanceVersion");
	addrs->pvkGetPhysicalDeviceExternalBufferProperties = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceExternalBufferProperties");
	addrs->pvkWaitSemaphores = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkWaitSemaphores");
	addrs->pvkResetQueryPool = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkResetQueryPool");
	addrs->pvkGetSemaphoreCounterValue = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetSemaphoreCounterValue");
	addrs->pvkCmdBeginRenderPass2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdBeginRenderPass2");
	addrs->pvkCmdEndRenderPass2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdEndRenderPass2");
	addrs->pvkGetBufferDeviceAddress = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetBufferDeviceAddress");
	addrs->pvkGetBufferOpaqueCaptureAddress = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetBufferOpaqueCaptureAddress");
	addrs->pvkGetDeviceMemoryOpaqueCaptureAddress = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceMemoryOpaqueCaptureAddress");
	addrs->pvkCmdNextSubpass2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdNextSubpass2");
	addrs->pvkCreateRenderPass2 = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateRenderPass2");
	addrs->pvkCmdDrawIndexedIndirectCount = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDrawIndexedIndirectCount");
	addrs->pvkCmdDrawIndirectCount = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCmdDrawIndirectCount");
	addrs->pvkSignalSemaphore = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkSignalSemaphore");
	addrs->pvkGetPhysicalDeviceSurfacePresentModesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	addrs->pvkGetPhysicalDeviceSurfaceFormatsKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	addrs->pvkGetPhysicalDeviceSurfaceCapabilitiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	addrs->pvkGetPhysicalDeviceSurfaceSupportKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfaceSupportKHR");
	addrs->pvkDestroySurfaceKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroySurfaceKHR");
	addrs->pvkGetPhysicalDeviceSurfaceCapabilities2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
	addrs->pvkGetPhysicalDeviceSurfaceFormats2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceSurfaceFormats2KHR");
	addrs->pvkGetPhysicalDevicePresentRectanglesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDevicePresentRectanglesKHR");
	addrs->pvkAcquireNextImage2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkAcquireNextImage2KHR");
	addrs->pvkGetDeviceGroupSurfacePresentModesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceGroupSurfacePresentModesKHR");
	addrs->pvkGetDeviceGroupPresentCapabilitiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDeviceGroupPresentCapabilitiesKHR");
	addrs->pvkQueuePresentKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkQueuePresentKHR");
	addrs->pvkAcquireNextImageKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkAcquireNextImageKHR");
	addrs->pvkGetSwapchainImagesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetSwapchainImagesKHR");
	addrs->pvkDestroySwapchainKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkDestroySwapchainKHR");
	addrs->pvkCreateSwapchainKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateSwapchainKHR");
	addrs->pvkCreateDisplayPlaneSurfaceKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDisplayPlaneSurfaceKHR");
	addrs->pvkGetDisplayPlaneCapabilitiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDisplayPlaneCapabilitiesKHR");
	addrs->pvkCreateDisplayModeKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateDisplayModeKHR");
	addrs->pvkGetDisplayModePropertiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDisplayModePropertiesKHR");
	addrs->pvkGetDisplayPlaneSupportedDisplaysKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDisplayPlaneSupportedDisplaysKHR");
	addrs->pvkGetPhysicalDeviceDisplayPlanePropertiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
	addrs->pvkGetPhysicalDeviceDisplayPropertiesKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceDisplayPropertiesKHR");
	addrs->pvkGetDisplayPlaneCapabilities2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDisplayPlaneCapabilities2KHR");
	addrs->pvkGetPhysicalDeviceDisplayProperties2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceDisplayProperties2KHR");
	addrs->pvkGetPhysicalDeviceDisplayPlaneProperties2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
	addrs->pvkGetDisplayModeProperties2KHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkGetDisplayModeProperties2KHR");
	addrs->pvkCreateSharedSwapchainsKHR = vkGetInstanceProcAddr(&vksProcAddresses, hndl, "vkCreateSharedSwapchainsKHR");
}

// call getProcAddress for all the commands.
void vksLoadDeviceProcAddrs(VkDevice hndl, vksProcAddr* addrs, vksProcAddr* parent) { 
	addrs->pvkGetPhysicalDeviceImageFormatProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceImageFormatProperties");
	addrs->pvkDestroyDescriptorPool = vkGetDeviceProcAddr(parent, hndl, "vkDestroyDescriptorPool");
	addrs->pvkResetDescriptorPool = vkGetDeviceProcAddr(parent, hndl, "vkResetDescriptorPool");
	addrs->pvkAllocateDescriptorSets = vkGetDeviceProcAddr(parent, hndl, "vkAllocateDescriptorSets");
	addrs->pvkFreeDescriptorSets = vkGetDeviceProcAddr(parent, hndl, "vkFreeDescriptorSets");
	addrs->pvkUpdateDescriptorSets = vkGetDeviceProcAddr(parent, hndl, "vkUpdateDescriptorSets");
	addrs->pvkCreateDescriptorPool = vkGetDeviceProcAddr(parent, hndl, "vkCreateDescriptorPool");
	addrs->pvkDestroyDescriptorSetLayout = vkGetDeviceProcAddr(parent, hndl, "vkDestroyDescriptorSetLayout");
	addrs->pvkCreateDescriptorSetLayout = vkGetDeviceProcAddr(parent, hndl, "vkCreateDescriptorSetLayout");
	addrs->pvkDestroySampler = vkGetDeviceProcAddr(parent, hndl, "vkDestroySampler");
	addrs->pvkCreateSampler = vkGetDeviceProcAddr(parent, hndl, "vkCreateSampler");
	addrs->pvkDestroyPipelineLayout = vkGetDeviceProcAddr(parent, hndl, "vkDestroyPipelineLayout");
	addrs->pvkCreatePipelineLayout = vkGetDeviceProcAddr(parent, hndl, "vkCreatePipelineLayout");
	addrs->pvkDestroyPipeline = vkGetDeviceProcAddr(parent, hndl, "vkDestroyPipeline");
	addrs->pvkCreateComputePipelines = vkGetDeviceProcAddr(parent, hndl, "vkCreateComputePipelines");
	addrs->pvkCreateGraphicsPipelines = vkGetDeviceProcAddr(parent, hndl, "vkCreateGraphicsPipelines");
	addrs->pvkMergePipelineCaches = vkGetDeviceProcAddr(parent, hndl, "vkMergePipelineCaches");
	addrs->pvkGetPipelineCacheData = vkGetDeviceProcAddr(parent, hndl, "vkGetPipelineCacheData");
	addrs->pvkDestroyPipelineCache = vkGetDeviceProcAddr(parent, hndl, "vkDestroyPipelineCache");
	addrs->pvkCreatePipelineCache = vkGetDeviceProcAddr(parent, hndl, "vkCreatePipelineCache");
	addrs->pvkDestroyShaderModule = vkGetDeviceProcAddr(parent, hndl, "vkDestroyShaderModule");
	addrs->pvkCreateShaderModule = vkGetDeviceProcAddr(parent, hndl, "vkCreateShaderModule");
	addrs->pvkDestroyImageView = vkGetDeviceProcAddr(parent, hndl, "vkDestroyImageView");
	addrs->pvkCreateImageView = vkGetDeviceProcAddr(parent, hndl, "vkCreateImageView");
	addrs->pvkCreateFramebuffer = vkGetDeviceProcAddr(parent, hndl, "vkCreateFramebuffer");
	addrs->pvkDestroyFramebuffer = vkGetDeviceProcAddr(parent, hndl, "vkDestroyFramebuffer");
	addrs->pvkCreateRenderPass = vkGetDeviceProcAddr(parent, hndl, "vkCreateRenderPass");
	addrs->pvkDestroyRenderPass = vkGetDeviceProcAddr(parent, hndl, "vkDestroyRenderPass");
	addrs->pvkGetRenderAreaGranularity = vkGetDeviceProcAddr(parent, hndl, "vkGetRenderAreaGranularity");
	addrs->pvkGetImageSubresourceLayout = vkGetDeviceProcAddr(parent, hndl, "vkGetImageSubresourceLayout");
	addrs->pvkDestroyImage = vkGetDeviceProcAddr(parent, hndl, "vkDestroyImage");
	addrs->pvkCreateImage = vkGetDeviceProcAddr(parent, hndl, "vkCreateImage");
	addrs->pvkDestroyBufferView = vkGetDeviceProcAddr(parent, hndl, "vkDestroyBufferView");
	addrs->pvkCreateBufferView = vkGetDeviceProcAddr(parent, hndl, "vkCreateBufferView");
	addrs->pvkDestroyBuffer = vkGetDeviceProcAddr(parent, hndl, "vkDestroyBuffer");
	addrs->pvkCreateBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCreateBuffer");
	addrs->pvkGetQueryPoolResults = vkGetDeviceProcAddr(parent, hndl, "vkGetQueryPoolResults");
	addrs->pvkDestroyQueryPool = vkGetDeviceProcAddr(parent, hndl, "vkDestroyQueryPool");
	addrs->pvkCreateQueryPool = vkGetDeviceProcAddr(parent, hndl, "vkCreateQueryPool");
	addrs->pvkResetEvent = vkGetDeviceProcAddr(parent, hndl, "vkResetEvent");
	addrs->pvkSetEvent = vkGetDeviceProcAddr(parent, hndl, "vkSetEvent");
	addrs->pvkGetEventStatus = vkGetDeviceProcAddr(parent, hndl, "vkGetEventStatus");
	addrs->pvkDestroyEvent = vkGetDeviceProcAddr(parent, hndl, "vkDestroyEvent");
	addrs->pvkCreateEvent = vkGetDeviceProcAddr(parent, hndl, "vkCreateEvent");
	addrs->pvkDestroySemaphore = vkGetDeviceProcAddr(parent, hndl, "vkDestroySemaphore");
	addrs->pvkCreateSemaphore = vkGetDeviceProcAddr(parent, hndl, "vkCreateSemaphore");
	addrs->pvkWaitForFences = vkGetDeviceProcAddr(parent, hndl, "vkWaitForFences");
	addrs->pvkGetFenceStatus = vkGetDeviceProcAddr(parent, hndl, "vkGetFenceStatus");
	addrs->pvkResetFences = vkGetDeviceProcAddr(parent, hndl, "vkResetFences");
	addrs->pvkDestroyFence = vkGetDeviceProcAddr(parent, hndl, "vkDestroyFence");
	addrs->pvkCreateFence = vkGetDeviceProcAddr(parent, hndl, "vkCreateFence");
	addrs->pvkQueueBindSparse = vkGetDeviceProcAddr(parent, hndl, "vkQueueBindSparse");
	addrs->pvkCreateCommandPool = vkGetDeviceProcAddr(parent, hndl, "vkCreateCommandPool");
	addrs->pvkDestroyCommandPool = vkGetDeviceProcAddr(parent, hndl, "vkDestroyCommandPool");
	addrs->pvkResetCommandPool = vkGetDeviceProcAddr(parent, hndl, "vkResetCommandPool");
	addrs->pvkGetPhysicalDeviceSparseImageFormatProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSparseImageFormatProperties");
	addrs->pvkGetImageSparseMemoryRequirements = vkGetDeviceProcAddr(parent, hndl, "vkGetImageSparseMemoryRequirements");
	addrs->pvkGetImageMemoryRequirements = vkGetDeviceProcAddr(parent, hndl, "vkGetImageMemoryRequirements");
	addrs->pvkGetBufferMemoryRequirements = vkGetDeviceProcAddr(parent, hndl, "vkGetBufferMemoryRequirements");
	addrs->pvkBindImageMemory = vkGetDeviceProcAddr(parent, hndl, "vkBindImageMemory");
	addrs->pvkBindBufferMemory = vkGetDeviceProcAddr(parent, hndl, "vkBindBufferMemory");
	addrs->pvkAllocateCommandBuffers = vkGetDeviceProcAddr(parent, hndl, "vkAllocateCommandBuffers");
	addrs->pvkFreeCommandBuffers = vkGetDeviceProcAddr(parent, hndl, "vkFreeCommandBuffers");
	addrs->pvkBeginCommandBuffer = vkGetDeviceProcAddr(parent, hndl, "vkBeginCommandBuffer");
	addrs->pvkEndCommandBuffer = vkGetDeviceProcAddr(parent, hndl, "vkEndCommandBuffer");
	addrs->pvkResetCommandBuffer = vkGetDeviceProcAddr(parent, hndl, "vkResetCommandBuffer");
	addrs->pvkGetDeviceMemoryCommitment = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceMemoryCommitment");
	addrs->pvkInvalidateMappedMemoryRanges = vkGetDeviceProcAddr(parent, hndl, "vkInvalidateMappedMemoryRanges");
	addrs->pvkFlushMappedMemoryRanges = vkGetDeviceProcAddr(parent, hndl, "vkFlushMappedMemoryRanges");
	addrs->pvkUnmapMemory = vkGetDeviceProcAddr(parent, hndl, "vkUnmapMemory");
	addrs->pvkMapMemory = vkGetDeviceProcAddr(parent, hndl, "vkMapMemory");
	addrs->pvkFreeMemory = vkGetDeviceProcAddr(parent, hndl, "vkFreeMemory");
	addrs->pvkAllocateMemory = vkGetDeviceProcAddr(parent, hndl, "vkAllocateMemory");
	addrs->pvkDeviceWaitIdle = vkGetDeviceProcAddr(parent, hndl, "vkDeviceWaitIdle");
	addrs->pvkQueueWaitIdle = vkGetDeviceProcAddr(parent, hndl, "vkQueueWaitIdle");
	addrs->pvkQueueSubmit = vkGetDeviceProcAddr(parent, hndl, "vkQueueSubmit");
	addrs->pvkGetDeviceQueue = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceQueue");
	addrs->pvkCmdBindPipeline = vkGetDeviceProcAddr(parent, hndl, "vkCmdBindPipeline");
	addrs->pvkCmdSetViewport = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetViewport");
	addrs->pvkCmdSetScissor = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetScissor");
	addrs->pvkCmdSetLineWidth = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetLineWidth");
	addrs->pvkEnumerateDeviceLayerProperties = vkGetDeviceProcAddr(parent, hndl, "vkEnumerateDeviceLayerProperties");
	addrs->pvkCmdSetBlendConstants = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetBlendConstants");
	addrs->pvkCmdSetDepthBounds = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetDepthBounds");
	addrs->pvkCmdSetStencilCompareMask = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetStencilCompareMask");
	addrs->pvkCmdSetStencilWriteMask = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetStencilWriteMask");
	addrs->pvkCmdSetStencilReference = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetStencilReference");
	addrs->pvkCmdBindDescriptorSets = vkGetDeviceProcAddr(parent, hndl, "vkCmdBindDescriptorSets");
	addrs->pvkCmdBindIndexBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCmdBindIndexBuffer");
	addrs->pvkCmdBindVertexBuffers = vkGetDeviceProcAddr(parent, hndl, "vkCmdBindVertexBuffers");
	addrs->pvkCmdDraw = vkGetDeviceProcAddr(parent, hndl, "vkCmdDraw");
	addrs->pvkCmdDrawIndexed = vkGetDeviceProcAddr(parent, hndl, "vkCmdDrawIndexed");
	addrs->pvkCmdDrawIndirect = vkGetDeviceProcAddr(parent, hndl, "vkCmdDrawIndirect");
	addrs->pvkCmdDrawIndexedIndirect = vkGetDeviceProcAddr(parent, hndl, "vkCmdDrawIndexedIndirect");
	addrs->pvkCmdDispatch = vkGetDeviceProcAddr(parent, hndl, "vkCmdDispatch");
	addrs->pvkCmdDispatchIndirect = vkGetDeviceProcAddr(parent, hndl, "vkCmdDispatchIndirect");
	addrs->pvkCmdCopyBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCmdCopyBuffer");
	addrs->pvkCmdCopyImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdCopyImage");
	addrs->pvkCmdBlitImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdBlitImage");
	addrs->pvkCmdCopyBufferToImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdCopyBufferToImage");
	addrs->pvkCmdCopyImageToBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCmdCopyImageToBuffer");
	addrs->pvkCmdUpdateBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCmdUpdateBuffer");
	addrs->pvkCmdFillBuffer = vkGetDeviceProcAddr(parent, hndl, "vkCmdFillBuffer");
	addrs->pvkCmdClearColorImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdClearColorImage");
	addrs->pvkCmdClearDepthStencilImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdClearDepthStencilImage");
	addrs->pvkCmdClearAttachments = vkGetDeviceProcAddr(parent, hndl, "vkCmdClearAttachments");
	addrs->pvkCmdResolveImage = vkGetDeviceProcAddr(parent, hndl, "vkCmdResolveImage");
	addrs->pvkCmdSetEvent = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetEvent");
	addrs->pvkCmdResetEvent = vkGetDeviceProcAddr(parent, hndl, "vkCmdResetEvent");
	addrs->pvkCmdWaitEvents = vkGetDeviceProcAddr(parent, hndl, "vkCmdWaitEvents");
	addrs->pvkCmdPipelineBarrier = vkGetDeviceProcAddr(parent, hndl, "vkCmdPipelineBarrier");
	addrs->pvkCmdBeginQuery = vkGetDeviceProcAddr(parent, hndl, "vkCmdBeginQuery");
	addrs->pvkCmdEndQuery = vkGetDeviceProcAddr(parent, hndl, "vkCmdEndQuery");
	addrs->pvkCmdResetQueryPool = vkGetDeviceProcAddr(parent, hndl, "vkCmdResetQueryPool");
	addrs->pvkCmdWriteTimestamp = vkGetDeviceProcAddr(parent, hndl, "vkCmdWriteTimestamp");
	addrs->pvkCmdCopyQueryPoolResults = vkGetDeviceProcAddr(parent, hndl, "vkCmdCopyQueryPoolResults");
	addrs->pvkCmdPushConstants = vkGetDeviceProcAddr(parent, hndl, "vkCmdPushConstants");
	addrs->pvkCmdBeginRenderPass = vkGetDeviceProcAddr(parent, hndl, "vkCmdBeginRenderPass");
	addrs->pvkCmdNextSubpass = vkGetDeviceProcAddr(parent, hndl, "vkCmdNextSubpass");
	addrs->pvkCmdEndRenderPass = vkGetDeviceProcAddr(parent, hndl, "vkCmdEndRenderPass");
	addrs->pvkCmdExecuteCommands = vkGetDeviceProcAddr(parent, hndl, "vkCmdExecuteCommands");
	addrs->pvkEnumerateInstanceLayerProperties = vkGetDeviceProcAddr(parent, hndl, "vkEnumerateInstanceLayerProperties");
	addrs->pvkEnumerateDeviceExtensionProperties = vkGetDeviceProcAddr(parent, hndl, "vkEnumerateDeviceExtensionProperties");
	addrs->pvkEnumerateInstanceExtensionProperties = vkGetDeviceProcAddr(parent, hndl, "vkEnumerateInstanceExtensionProperties");
	addrs->pvkDestroyDevice = vkGetDeviceProcAddr(parent, hndl, "vkDestroyDevice");
	addrs->pvkCreateDevice = vkGetDeviceProcAddr(parent, hndl, "vkCreateDevice");
	addrs->pvkGetDeviceProcAddr = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceProcAddr");
	addrs->pvkGetInstanceProcAddr = vkGetDeviceProcAddr(parent, hndl, "vkGetInstanceProcAddr");
	addrs->pvkGetPhysicalDeviceMemoryProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceMemoryProperties");
	addrs->pvkGetPhysicalDeviceQueueFamilyProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceQueueFamilyProperties");
	addrs->pvkGetPhysicalDeviceProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceProperties");
	addrs->pvkGetPhysicalDeviceFormatProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceFormatProperties");
	addrs->pvkGetPhysicalDeviceFeatures = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceFeatures");
	addrs->pvkEnumeratePhysicalDevices = vkGetDeviceProcAddr(parent, hndl, "vkEnumeratePhysicalDevices");
	addrs->pvkDestroyInstance = vkGetDeviceProcAddr(parent, hndl, "vkDestroyInstance");
	addrs->pvkCreateInstance = vkGetDeviceProcAddr(parent, hndl, "vkCreateInstance");
	addrs->pvkCmdSetDepthBias = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetDepthBias");
	addrs->pvkUpdateDescriptorSetWithTemplate = vkGetDeviceProcAddr(parent, hndl, "vkUpdateDescriptorSetWithTemplate");
	addrs->pvkDestroySamplerYcbcrConversion = vkGetDeviceProcAddr(parent, hndl, "vkDestroySamplerYcbcrConversion");
	addrs->pvkCreateDescriptorUpdateTemplate = vkGetDeviceProcAddr(parent, hndl, "vkCreateDescriptorUpdateTemplate");
	addrs->pvkGetPhysicalDeviceExternalFenceProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceExternalFenceProperties");
	addrs->pvkCreateSamplerYcbcrConversion = vkGetDeviceProcAddr(parent, hndl, "vkCreateSamplerYcbcrConversion");
	addrs->pvkGetDeviceQueue2 = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceQueue2");
	addrs->pvkTrimCommandPool = vkGetDeviceProcAddr(parent, hndl, "vkTrimCommandPool");
	addrs->pvkGetPhysicalDeviceSparseImageFormatProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSparseImageFormatProperties2");
	addrs->pvkGetPhysicalDeviceMemoryProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceMemoryProperties2");
	addrs->pvkGetPhysicalDeviceQueueFamilyProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceQueueFamilyProperties2");
	addrs->pvkGetPhysicalDeviceImageFormatProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceImageFormatProperties2");
	addrs->pvkGetPhysicalDeviceFormatProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceFormatProperties2");
	addrs->pvkGetPhysicalDeviceProperties2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceProperties2");
	addrs->pvkGetPhysicalDeviceFeatures2 = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceFeatures2");
	addrs->pvkGetImageSparseMemoryRequirements2 = vkGetDeviceProcAddr(parent, hndl, "vkGetImageSparseMemoryRequirements2");
	addrs->pvkGetBufferMemoryRequirements2 = vkGetDeviceProcAddr(parent, hndl, "vkGetBufferMemoryRequirements2");
	addrs->pvkGetPhysicalDeviceExternalSemaphoreProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceExternalSemaphoreProperties");
	addrs->pvkGetImageMemoryRequirements2 = vkGetDeviceProcAddr(parent, hndl, "vkGetImageMemoryRequirements2");
	addrs->pvkDestroyDescriptorUpdateTemplate = vkGetDeviceProcAddr(parent, hndl, "vkDestroyDescriptorUpdateTemplate");
	addrs->pvkEnumeratePhysicalDeviceGroups = vkGetDeviceProcAddr(parent, hndl, "vkEnumeratePhysicalDeviceGroups");
	addrs->pvkCmdDispatchBase = vkGetDeviceProcAddr(parent, hndl, "vkCmdDispatchBase");
	addrs->pvkCmdSetDeviceMask = vkGetDeviceProcAddr(parent, hndl, "vkCmdSetDeviceMask");
	addrs->pvkGetDeviceGroupPeerMemoryFeatures = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceGroupPeerMemoryFeatures");
	addrs->pvkGetDescriptorSetLayoutSupport = vkGetDeviceProcAddr(parent, hndl, "vkGetDescriptorSetLayoutSupport");
	addrs->pvkBindImageMemory2 = vkGetDeviceProcAddr(parent, hndl, "vkBindImageMemory2");
	addrs->pvkBindBufferMemory2 = vkGetDeviceProcAddr(parent, hndl, "vkBindBufferMemory2");
	addrs->pvkEnumerateInstanceVersion = vkGetDeviceProcAddr(parent, hndl, "vkEnumerateInstanceVersion");
	addrs->pvkGetPhysicalDeviceExternalBufferProperties = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceExternalBufferProperties");
	addrs->pvkWaitSemaphores = vkGetDeviceProcAddr(parent, hndl, "vkWaitSemaphores");
	addrs->pvkResetQueryPool = vkGetDeviceProcAddr(parent, hndl, "vkResetQueryPool");
	addrs->pvkGetSemaphoreCounterValue = vkGetDeviceProcAddr(parent, hndl, "vkGetSemaphoreCounterValue");
	addrs->pvkCmdBeginRenderPass2 = vkGetDeviceProcAddr(parent, hndl, "vkCmdBeginRenderPass2");
	addrs->pvkCmdEndRenderPass2 = vkGetDeviceProcAddr(parent, hndl, "vkCmdEndRenderPass2");
	addrs->pvkGetBufferDeviceAddress = vkGetDeviceProcAddr(parent, hndl, "vkGetBufferDeviceAddress");
	addrs->pvkGetBufferOpaqueCaptureAddress = vkGetDeviceProcAddr(parent, hndl, "vkGetBufferOpaqueCaptureAddress");
	addrs->pvkGetDeviceMemoryOpaqueCaptureAddress = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceMemoryOpaqueCaptureAddress");
	addrs->pvkCmdNextSubpass2 = vkGetDeviceProcAddr(parent, hndl, "vkCmdNextSubpass2");
	addrs->pvkCreateRenderPass2 = vkGetDeviceProcAddr(parent, hndl, "vkCreateRenderPass2");
	addrs->pvkCmdDrawIndexedIndirectCount = vkGetDeviceProcAddr(parent, hndl, "vkCmdDrawIndexedIndirectCount");
	addrs->pvkCmdDrawIndirectCount = vkGetDeviceProcAddr(parent, hndl, "vkCmdDrawIndirectCount");
	addrs->pvkSignalSemaphore = vkGetDeviceProcAddr(parent, hndl, "vkSignalSemaphore");
	addrs->pvkGetPhysicalDeviceSurfacePresentModesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	addrs->pvkGetPhysicalDeviceSurfaceFormatsKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	addrs->pvkGetPhysicalDeviceSurfaceCapabilitiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	addrs->pvkGetPhysicalDeviceSurfaceSupportKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfaceSupportKHR");
	addrs->pvkDestroySurfaceKHR = vkGetDeviceProcAddr(parent, hndl, "vkDestroySurfaceKHR");
	addrs->pvkGetPhysicalDeviceSurfaceCapabilities2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
	addrs->pvkGetPhysicalDeviceSurfaceFormats2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceSurfaceFormats2KHR");
	addrs->pvkGetPhysicalDevicePresentRectanglesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDevicePresentRectanglesKHR");
	addrs->pvkAcquireNextImage2KHR = vkGetDeviceProcAddr(parent, hndl, "vkAcquireNextImage2KHR");
	addrs->pvkGetDeviceGroupSurfacePresentModesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceGroupSurfacePresentModesKHR");
	addrs->pvkGetDeviceGroupPresentCapabilitiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDeviceGroupPresentCapabilitiesKHR");
	addrs->pvkQueuePresentKHR = vkGetDeviceProcAddr(parent, hndl, "vkQueuePresentKHR");
	addrs->pvkAcquireNextImageKHR = vkGetDeviceProcAddr(parent, hndl, "vkAcquireNextImageKHR");
	addrs->pvkGetSwapchainImagesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetSwapchainImagesKHR");
	addrs->pvkDestroySwapchainKHR = vkGetDeviceProcAddr(parent, hndl, "vkDestroySwapchainKHR");
	addrs->pvkCreateSwapchainKHR = vkGetDeviceProcAddr(parent, hndl, "vkCreateSwapchainKHR");
	addrs->pvkCreateDisplayPlaneSurfaceKHR = vkGetDeviceProcAddr(parent, hndl, "vkCreateDisplayPlaneSurfaceKHR");
	addrs->pvkGetDisplayPlaneCapabilitiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDisplayPlaneCapabilitiesKHR");
	addrs->pvkCreateDisplayModeKHR = vkGetDeviceProcAddr(parent, hndl, "vkCreateDisplayModeKHR");
	addrs->pvkGetDisplayModePropertiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDisplayModePropertiesKHR");
	addrs->pvkGetDisplayPlaneSupportedDisplaysKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDisplayPlaneSupportedDisplaysKHR");
	addrs->pvkGetPhysicalDeviceDisplayPlanePropertiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
	addrs->pvkGetPhysicalDeviceDisplayPropertiesKHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceDisplayPropertiesKHR");
	addrs->pvkGetDisplayPlaneCapabilities2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDisplayPlaneCapabilities2KHR");
	addrs->pvkGetPhysicalDeviceDisplayProperties2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceDisplayProperties2KHR");
	addrs->pvkGetPhysicalDeviceDisplayPlaneProperties2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
	addrs->pvkGetDisplayModeProperties2KHR = vkGetDeviceProcAddr(parent, hndl, "vkGetDisplayModeProperties2KHR");
	addrs->pvkCreateSharedSwapchainsKHR = vkGetDeviceProcAddr(parent, hndl, "vkCreateSharedSwapchainsKHR");
}

VkResult vkGetPhysicalDeviceImageFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceImageFormatProperties)addrs->pvkGetPhysicalDeviceImageFormatProperties)(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties));
	return ret;
}
void vkDestroyDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorPool)addrs->pvkDestroyDescriptorPool)(device, descriptorPool, pAllocator));
}
VkResult vkResetDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
	VkResult ret = (((PFN_vkResetDescriptorPool)addrs->pvkResetDescriptorPool)(device, descriptorPool, flags));
	return ret;
}
VkResult vkAllocateDescriptorSets(vksProcAddr* addrs, VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {
	VkResult ret = (((PFN_vkAllocateDescriptorSets)addrs->pvkAllocateDescriptorSets)(device, pAllocateInfo, pDescriptorSets));
	return ret;
}
VkResult vkFreeDescriptorSets(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets) {
	VkResult ret = (((PFN_vkFreeDescriptorSets)addrs->pvkFreeDescriptorSets)(device, descriptorPool, descriptorSetCount, pDescriptorSets));
	return ret;
}
void vkUpdateDescriptorSets(vksProcAddr* addrs, VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies) {
	(((PFN_vkUpdateDescriptorSets)addrs->pvkUpdateDescriptorSets)(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies));
}
VkResult vkCreateDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {
	VkResult ret = (((PFN_vkCreateDescriptorPool)addrs->pvkCreateDescriptorPool)(device, pCreateInfo, pAllocator, pDescriptorPool));
	return ret;
}
void vkDestroyDescriptorSetLayout(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorSetLayout)addrs->pvkDestroyDescriptorSetLayout)(device, descriptorSetLayout, pAllocator));
}
VkResult vkCreateDescriptorSetLayout(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) {
	VkResult ret = (((PFN_vkCreateDescriptorSetLayout)addrs->pvkCreateDescriptorSetLayout)(device, pCreateInfo, pAllocator, pSetLayout));
	return ret;
}
void vkDestroySampler(vksProcAddr* addrs, VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySampler)addrs->pvkDestroySampler)(device, sampler, pAllocator));
}
VkResult vkCreateSampler(vksProcAddr* addrs, VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {
	VkResult ret = (((PFN_vkCreateSampler)addrs->pvkCreateSampler)(device, pCreateInfo, pAllocator, pSampler));
	return ret;
}
void vkDestroyPipelineLayout(vksProcAddr* addrs, VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipelineLayout)addrs->pvkDestroyPipelineLayout)(device, pipelineLayout, pAllocator));
}
VkResult vkCreatePipelineLayout(vksProcAddr* addrs, VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {
	VkResult ret = (((PFN_vkCreatePipelineLayout)addrs->pvkCreatePipelineLayout)(device, pCreateInfo, pAllocator, pPipelineLayout));
	return ret;
}
void vkDestroyPipeline(vksProcAddr* addrs, VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipeline)addrs->pvkDestroyPipeline)(device, pipeline, pAllocator));
}
VkResult vkCreateComputePipelines(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	VkResult ret = (((PFN_vkCreateComputePipelines)addrs->pvkCreateComputePipelines)(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines));
	return ret;
}
VkResult vkCreateGraphicsPipelines(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	VkResult ret = (((PFN_vkCreateGraphicsPipelines)addrs->pvkCreateGraphicsPipelines)(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines));
	return ret;
}
VkResult vkMergePipelineCaches(vksProcAddr* addrs, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches) {
	VkResult ret = (((PFN_vkMergePipelineCaches)addrs->pvkMergePipelineCaches)(device, dstCache, srcCacheCount, pSrcCaches));
	return ret;
}
VkResult vkGetPipelineCacheData(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) {
	VkResult ret = (((PFN_vkGetPipelineCacheData)addrs->pvkGetPipelineCacheData)(device, pipelineCache, pDataSize, pData));
	return ret;
}
void vkDestroyPipelineCache(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyPipelineCache)addrs->pvkDestroyPipelineCache)(device, pipelineCache, pAllocator));
}
VkResult vkCreatePipelineCache(vksProcAddr* addrs, VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {
	VkResult ret = (((PFN_vkCreatePipelineCache)addrs->pvkCreatePipelineCache)(device, pCreateInfo, pAllocator, pPipelineCache));
	return ret;
}
void vkDestroyShaderModule(vksProcAddr* addrs, VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyShaderModule)addrs->pvkDestroyShaderModule)(device, shaderModule, pAllocator));
}
VkResult vkCreateShaderModule(vksProcAddr* addrs, VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
	VkResult ret = (((PFN_vkCreateShaderModule)addrs->pvkCreateShaderModule)(device, pCreateInfo, pAllocator, pShaderModule));
	return ret;
}
void vkDestroyImageView(vksProcAddr* addrs, VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyImageView)addrs->pvkDestroyImageView)(device, imageView, pAllocator));
}
VkResult vkCreateImageView(vksProcAddr* addrs, VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView) {
	VkResult ret = (((PFN_vkCreateImageView)addrs->pvkCreateImageView)(device, pCreateInfo, pAllocator, pView));
	return ret;
}
VkResult vkCreateFramebuffer(vksProcAddr* addrs, VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {
	VkResult ret = (((PFN_vkCreateFramebuffer)addrs->pvkCreateFramebuffer)(device, pCreateInfo, pAllocator, pFramebuffer));
	return ret;
}
void vkDestroyFramebuffer(vksProcAddr* addrs, VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyFramebuffer)addrs->pvkDestroyFramebuffer)(device, framebuffer, pAllocator));
}
VkResult vkCreateRenderPass(vksProcAddr* addrs, VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
	VkResult ret = (((PFN_vkCreateRenderPass)addrs->pvkCreateRenderPass)(device, pCreateInfo, pAllocator, pRenderPass));
	return ret;
}
void vkDestroyRenderPass(vksProcAddr* addrs, VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyRenderPass)addrs->pvkDestroyRenderPass)(device, renderPass, pAllocator));
}
void vkGetRenderAreaGranularity(vksProcAddr* addrs, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) {
	(((PFN_vkGetRenderAreaGranularity)addrs->pvkGetRenderAreaGranularity)(device, renderPass, pGranularity));
}
void vkGetImageSubresourceLayout(vksProcAddr* addrs, VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {
	(((PFN_vkGetImageSubresourceLayout)addrs->pvkGetImageSubresourceLayout)(device, image, pSubresource, pLayout));
}
void vkDestroyImage(vksProcAddr* addrs, VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyImage)addrs->pvkDestroyImage)(device, image, pAllocator));
}
VkResult vkCreateImage(vksProcAddr* addrs, VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage) {
	VkResult ret = (((PFN_vkCreateImage)addrs->pvkCreateImage)(device, pCreateInfo, pAllocator, pImage));
	return ret;
}
void vkDestroyBufferView(vksProcAddr* addrs, VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyBufferView)addrs->pvkDestroyBufferView)(device, bufferView, pAllocator));
}
VkResult vkCreateBufferView(vksProcAddr* addrs, VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView) {
	VkResult ret = (((PFN_vkCreateBufferView)addrs->pvkCreateBufferView)(device, pCreateInfo, pAllocator, pView));
	return ret;
}
void vkDestroyBuffer(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyBuffer)addrs->pvkDestroyBuffer)(device, buffer, pAllocator));
}
VkResult vkCreateBuffer(vksProcAddr* addrs, VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {
	VkResult ret = (((PFN_vkCreateBuffer)addrs->pvkCreateBuffer)(device, pCreateInfo, pAllocator, pBuffer));
	return ret;
}
VkResult vkGetQueryPoolResults(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
	VkResult ret = (((PFN_vkGetQueryPoolResults)addrs->pvkGetQueryPoolResults)(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags));
	return ret;
}
void vkDestroyQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyQueryPool)addrs->pvkDestroyQueryPool)(device, queryPool, pAllocator));
}
VkResult vkCreateQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {
	VkResult ret = (((PFN_vkCreateQueryPool)addrs->pvkCreateQueryPool)(device, pCreateInfo, pAllocator, pQueryPool));
	return ret;
}
VkResult vkResetEvent(vksProcAddr* addrs, VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkResetEvent)addrs->pvkResetEvent)(device, event));
	return ret;
}
VkResult vkSetEvent(vksProcAddr* addrs, VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkSetEvent)addrs->pvkSetEvent)(device, event));
	return ret;
}
VkResult vkGetEventStatus(vksProcAddr* addrs, VkDevice device, VkEvent event) {
	VkResult ret = (((PFN_vkGetEventStatus)addrs->pvkGetEventStatus)(device, event));
	return ret;
}
void vkDestroyEvent(vksProcAddr* addrs, VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyEvent)addrs->pvkDestroyEvent)(device, event, pAllocator));
}
VkResult vkCreateEvent(vksProcAddr* addrs, VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {
	VkResult ret = (((PFN_vkCreateEvent)addrs->pvkCreateEvent)(device, pCreateInfo, pAllocator, pEvent));
	return ret;
}
void vkDestroySemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySemaphore)addrs->pvkDestroySemaphore)(device, semaphore, pAllocator));
}
VkResult vkCreateSemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {
	VkResult ret = (((PFN_vkCreateSemaphore)addrs->pvkCreateSemaphore)(device, pCreateInfo, pAllocator, pSemaphore));
	return ret;
}
VkResult vkWaitForFences(vksProcAddr* addrs, VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
	VkResult ret = (((PFN_vkWaitForFences)addrs->pvkWaitForFences)(device, fenceCount, pFences, waitAll, timeout));
	return ret;
}
VkResult vkGetFenceStatus(vksProcAddr* addrs, VkDevice device, VkFence fence) {
	VkResult ret = (((PFN_vkGetFenceStatus)addrs->pvkGetFenceStatus)(device, fence));
	return ret;
}
VkResult vkResetFences(vksProcAddr* addrs, VkDevice device, uint32_t fenceCount, VkFence* pFences) {
	VkResult ret = (((PFN_vkResetFences)addrs->pvkResetFences)(device, fenceCount, pFences));
	return ret;
}
void vkDestroyFence(vksProcAddr* addrs, VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyFence)addrs->pvkDestroyFence)(device, fence, pAllocator));
}
VkResult vkCreateFence(vksProcAddr* addrs, VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence) {
	VkResult ret = (((PFN_vkCreateFence)addrs->pvkCreateFence)(device, pCreateInfo, pAllocator, pFence));
	return ret;
}
VkResult vkQueueBindSparse(vksProcAddr* addrs, VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence) {
	VkResult ret = (((PFN_vkQueueBindSparse)addrs->pvkQueueBindSparse)(queue, bindInfoCount, pBindInfo, fence));
	return ret;
}
VkResult vkCreateCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {
	VkResult ret = (((PFN_vkCreateCommandPool)addrs->pvkCreateCommandPool)(device, pCreateInfo, pAllocator, pCommandPool));
	return ret;
}
void vkDestroyCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyCommandPool)addrs->pvkDestroyCommandPool)(device, commandPool, pAllocator));
}
VkResult vkResetCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
	VkResult ret = (((PFN_vkResetCommandPool)addrs->pvkResetCommandPool)(device, commandPool, flags));
	return ret;
}
void vkGetPhysicalDeviceSparseImageFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) {
	(((PFN_vkGetPhysicalDeviceSparseImageFormatProperties)addrs->pvkGetPhysicalDeviceSparseImageFormatProperties)(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties));
}
void vkGetImageSparseMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {
	(((PFN_vkGetImageSparseMemoryRequirements)addrs->pvkGetImageSparseMemoryRequirements)(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements));
}
void vkGetImageMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) {
	(((PFN_vkGetImageMemoryRequirements)addrs->pvkGetImageMemoryRequirements)(device, image, pMemoryRequirements));
}
void vkGetBufferMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) {
	(((PFN_vkGetBufferMemoryRequirements)addrs->pvkGetBufferMemoryRequirements)(device, buffer, pMemoryRequirements));
}
VkResult vkBindImageMemory(vksProcAddr* addrs, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	VkResult ret = (((PFN_vkBindImageMemory)addrs->pvkBindImageMemory)(device, image, memory, memoryOffset));
	return ret;
}
VkResult vkBindBufferMemory(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	VkResult ret = (((PFN_vkBindBufferMemory)addrs->pvkBindBufferMemory)(device, buffer, memory, memoryOffset));
	return ret;
}
VkResult vkAllocateCommandBuffers(vksProcAddr* addrs, VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
	VkResult ret = (((PFN_vkAllocateCommandBuffers)addrs->pvkAllocateCommandBuffers)(device, pAllocateInfo, pCommandBuffers));
	return ret;
}
void vkFreeCommandBuffers(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
	(((PFN_vkFreeCommandBuffers)addrs->pvkFreeCommandBuffers)(device, commandPool, commandBufferCount, pCommandBuffers));
}
VkResult vkBeginCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo) {
	VkResult ret = (((PFN_vkBeginCommandBuffer)addrs->pvkBeginCommandBuffer)(commandBuffer, pBeginInfo));
	return ret;
}
VkResult vkEndCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer) {
	VkResult ret = (((PFN_vkEndCommandBuffer)addrs->pvkEndCommandBuffer)(commandBuffer));
	return ret;
}
VkResult vkResetCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
	VkResult ret = (((PFN_vkResetCommandBuffer)addrs->pvkResetCommandBuffer)(commandBuffer, flags));
	return ret;
}
void vkGetDeviceMemoryCommitment(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) {
	(((PFN_vkGetDeviceMemoryCommitment)addrs->pvkGetDeviceMemoryCommitment)(device, memory, pCommittedMemoryInBytes));
}
VkResult vkInvalidateMappedMemoryRanges(vksProcAddr* addrs, VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
	VkResult ret = (((PFN_vkInvalidateMappedMemoryRanges)addrs->pvkInvalidateMappedMemoryRanges)(device, memoryRangeCount, pMemoryRanges));
	return ret;
}
VkResult vkFlushMappedMemoryRanges(vksProcAddr* addrs, VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
	VkResult ret = (((PFN_vkFlushMappedMemoryRanges)addrs->pvkFlushMappedMemoryRanges)(device, memoryRangeCount, pMemoryRanges));
	return ret;
}
void vkUnmapMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory) {
	(((PFN_vkUnmapMemory)addrs->pvkUnmapMemory)(device, memory));
}
VkResult vkMapMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
	VkResult ret = (((PFN_vkMapMemory)addrs->pvkMapMemory)(device, memory, offset, size, flags, ppData));
	return ret;
}
void vkFreeMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkFreeMemory)addrs->pvkFreeMemory)(device, memory, pAllocator));
}
VkResult vkAllocateMemory(vksProcAddr* addrs, VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
	VkResult ret = (((PFN_vkAllocateMemory)addrs->pvkAllocateMemory)(device, pAllocateInfo, pAllocator, pMemory));
	return ret;
}
VkResult vkDeviceWaitIdle(vksProcAddr* addrs, VkDevice device) {
	VkResult ret = (((PFN_vkDeviceWaitIdle)addrs->pvkDeviceWaitIdle)(device));
	return ret;
}
VkResult vkQueueWaitIdle(vksProcAddr* addrs, VkQueue queue) {
	VkResult ret = (((PFN_vkQueueWaitIdle)addrs->pvkQueueWaitIdle)(queue));
	return ret;
}
VkResult vkQueueSubmit(vksProcAddr* addrs, VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence) {
	VkResult ret = (((PFN_vkQueueSubmit)addrs->pvkQueueSubmit)(queue, submitCount, pSubmits, fence));
	return ret;
}
void vkGetDeviceQueue(vksProcAddr* addrs, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
	(((PFN_vkGetDeviceQueue)addrs->pvkGetDeviceQueue)(device, queueFamilyIndex, queueIndex, pQueue));
}
void vkCmdBindPipeline(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
	(((PFN_vkCmdBindPipeline)addrs->pvkCmdBindPipeline)(commandBuffer, pipelineBindPoint, pipeline));
}
void vkCmdSetViewport(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports) {
	(((PFN_vkCmdSetViewport)addrs->pvkCmdSetViewport)(commandBuffer, firstViewport, viewportCount, pViewports));
}
void vkCmdSetScissor(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors) {
	(((PFN_vkCmdSetScissor)addrs->pvkCmdSetScissor)(commandBuffer, firstScissor, scissorCount, pScissors));
}
void vkCmdSetLineWidth(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float lineWidth) {
	(((PFN_vkCmdSetLineWidth)addrs->pvkCmdSetLineWidth)(commandBuffer, lineWidth));
}
VkResult vkEnumerateDeviceLayerProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateDeviceLayerProperties)addrs->pvkEnumerateDeviceLayerProperties)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
void vkCmdSetBlendConstants(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float blendConstants[4]) {
	(((PFN_vkCmdSetBlendConstants)addrs->pvkCmdSetBlendConstants)(commandBuffer, blendConstants));
}
void vkCmdSetDepthBounds(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
	(((PFN_vkCmdSetDepthBounds)addrs->pvkCmdSetDepthBounds)(commandBuffer, minDepthBounds, maxDepthBounds));
}
void vkCmdSetStencilCompareMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
	(((PFN_vkCmdSetStencilCompareMask)addrs->pvkCmdSetStencilCompareMask)(commandBuffer, faceMask, compareMask));
}
void vkCmdSetStencilWriteMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
	(((PFN_vkCmdSetStencilWriteMask)addrs->pvkCmdSetStencilWriteMask)(commandBuffer, faceMask, writeMask));
}
void vkCmdSetStencilReference(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
	(((PFN_vkCmdSetStencilReference)addrs->pvkCmdSetStencilReference)(commandBuffer, faceMask, reference));
}
void vkCmdBindDescriptorSets(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets) {
	(((PFN_vkCmdBindDescriptorSets)addrs->pvkCmdBindDescriptorSets)(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets));
}
void vkCmdBindIndexBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
	(((PFN_vkCmdBindIndexBuffer)addrs->pvkCmdBindIndexBuffer)(commandBuffer, buffer, offset, indexType));
}
void vkCmdBindVertexBuffers(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets) {
	(((PFN_vkCmdBindVertexBuffers)addrs->pvkCmdBindVertexBuffers)(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets));
}
void vkCmdDraw(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
	(((PFN_vkCmdDraw)addrs->pvkCmdDraw)(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance));
}
void vkCmdDrawIndexed(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
	(((PFN_vkCmdDrawIndexed)addrs->pvkCmdDrawIndexed)(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance));
}
void vkCmdDrawIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndirect)addrs->pvkCmdDrawIndirect)(commandBuffer, buffer, offset, drawCount, stride));
}
void vkCmdDrawIndexedIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndexedIndirect)addrs->pvkCmdDrawIndexedIndirect)(commandBuffer, buffer, offset, drawCount, stride));
}
void vkCmdDispatch(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	(((PFN_vkCmdDispatch)addrs->pvkCmdDispatch)(commandBuffer, groupCountX, groupCountY, groupCountZ));
}
void vkCmdDispatchIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
	(((PFN_vkCmdDispatchIndirect)addrs->pvkCmdDispatchIndirect)(commandBuffer, buffer, offset));
}
void vkCmdCopyBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions) {
	(((PFN_vkCmdCopyBuffer)addrs->pvkCmdCopyBuffer)(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions));
}
void vkCmdCopyImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions) {
	(((PFN_vkCmdCopyImage)addrs->pvkCmdCopyImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdBlitImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter) {
	(((PFN_vkCmdBlitImage)addrs->pvkCmdBlitImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter));
}
void vkCmdCopyBufferToImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions) {
	(((PFN_vkCmdCopyBufferToImage)addrs->pvkCmdCopyBufferToImage)(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdCopyImageToBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions) {
	(((PFN_vkCmdCopyImageToBuffer)addrs->pvkCmdCopyImageToBuffer)(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions));
}
void vkCmdUpdateBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData) {
	(((PFN_vkCmdUpdateBuffer)addrs->pvkCmdUpdateBuffer)(commandBuffer, dstBuffer, dstOffset, dataSize, pData));
}
void vkCmdFillBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
	(((PFN_vkCmdFillBuffer)addrs->pvkCmdFillBuffer)(commandBuffer, dstBuffer, dstOffset, size, data));
}
void vkCmdClearColorImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
	(((PFN_vkCmdClearColorImage)addrs->pvkCmdClearColorImage)(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges));
}
void vkCmdClearDepthStencilImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
	(((PFN_vkCmdClearDepthStencilImage)addrs->pvkCmdClearDepthStencilImage)(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges));
}
void vkCmdClearAttachments(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects) {
	(((PFN_vkCmdClearAttachments)addrs->pvkCmdClearAttachments)(commandBuffer, attachmentCount, pAttachments, rectCount, pRects));
}
void vkCmdResolveImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions) {
	(((PFN_vkCmdResolveImage)addrs->pvkCmdResolveImage)(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions));
}
void vkCmdSetEvent(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	(((PFN_vkCmdSetEvent)addrs->pvkCmdSetEvent)(commandBuffer, event, stageMask));
}
void vkCmdResetEvent(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	(((PFN_vkCmdResetEvent)addrs->pvkCmdResetEvent)(commandBuffer, event, stageMask));
}
void vkCmdWaitEvents(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
	(((PFN_vkCmdWaitEvents)addrs->pvkCmdWaitEvents)(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers));
}
void vkCmdPipelineBarrier(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
	(((PFN_vkCmdPipelineBarrier)addrs->pvkCmdPipelineBarrier)(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers));
}
void vkCmdBeginQuery(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
	(((PFN_vkCmdBeginQuery)addrs->pvkCmdBeginQuery)(commandBuffer, queryPool, query, flags));
}
void vkCmdEndQuery(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
	(((PFN_vkCmdEndQuery)addrs->pvkCmdEndQuery)(commandBuffer, queryPool, query));
}
void vkCmdResetQueryPool(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	(((PFN_vkCmdResetQueryPool)addrs->pvkCmdResetQueryPool)(commandBuffer, queryPool, firstQuery, queryCount));
}
void vkCmdWriteTimestamp(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
	(((PFN_vkCmdWriteTimestamp)addrs->pvkCmdWriteTimestamp)(commandBuffer, pipelineStage, queryPool, query));
}
void vkCmdCopyQueryPoolResults(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
	(((PFN_vkCmdCopyQueryPoolResults)addrs->pvkCmdCopyQueryPoolResults)(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags));
}
void vkCmdPushConstants(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues) {
	(((PFN_vkCmdPushConstants)addrs->pvkCmdPushConstants)(commandBuffer, layout, stageFlags, offset, size, pValues));
}
void vkCmdBeginRenderPass(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
	(((PFN_vkCmdBeginRenderPass)addrs->pvkCmdBeginRenderPass)(commandBuffer, pRenderPassBegin, contents));
}
void vkCmdNextSubpass(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassContents contents) {
	(((PFN_vkCmdNextSubpass)addrs->pvkCmdNextSubpass)(commandBuffer, contents));
}
void vkCmdEndRenderPass(vksProcAddr* addrs, VkCommandBuffer commandBuffer) {
	(((PFN_vkCmdEndRenderPass)addrs->pvkCmdEndRenderPass)(commandBuffer));
}
void vkCmdExecuteCommands(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
	(((PFN_vkCmdExecuteCommands)addrs->pvkCmdExecuteCommands)(commandBuffer, commandBufferCount, pCommandBuffers));
}
VkResult vkEnumerateInstanceLayerProperties(vksProcAddr* addrs, uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateInstanceLayerProperties)addrs->pvkEnumerateInstanceLayerProperties)(pPropertyCount, pProperties));
	return ret;
}
VkResult vkEnumerateDeviceExtensionProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateDeviceExtensionProperties)addrs->pvkEnumerateDeviceExtensionProperties)(physicalDevice, pLayerName, pPropertyCount, pProperties));
	return ret;
}
VkResult vkEnumerateInstanceExtensionProperties(vksProcAddr* addrs, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	VkResult ret = (((PFN_vkEnumerateInstanceExtensionProperties)addrs->pvkEnumerateInstanceExtensionProperties)(pLayerName, pPropertyCount, pProperties));
	return ret;
}
void vkDestroyDevice(vksProcAddr* addrs, VkDevice device, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDevice)addrs->pvkDestroyDevice)(device, pAllocator));
}
VkResult vkCreateDevice(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
	VkResult ret = (((PFN_vkCreateDevice)addrs->pvkCreateDevice)(physicalDevice, pCreateInfo, pAllocator, pDevice));
	return ret;
}
PFN_vkVoidFunction vkGetDeviceProcAddr(vksProcAddr* addrs, VkDevice device, char* pName) {
	PFN_vkVoidFunction ret = (((PFN_vkGetDeviceProcAddr)addrs->pvkGetDeviceProcAddr)(device, pName));
	return ret;
}
PFN_vkVoidFunction vkGetInstanceProcAddr(vksProcAddr* addrs, VkInstance instance, char* pName) {
	PFN_vkVoidFunction ret = (((PFN_vkGetInstanceProcAddr)addrs->pvkGetInstanceProcAddr)(instance, pName));
	return ret;
}
void vkGetPhysicalDeviceMemoryProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
	(((PFN_vkGetPhysicalDeviceMemoryProperties)addrs->pvkGetPhysicalDeviceMemoryProperties)(physicalDevice, pMemoryProperties));
}
void vkGetPhysicalDeviceQueueFamilyProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) {
	(((PFN_vkGetPhysicalDeviceQueueFamilyProperties)addrs->pvkGetPhysicalDeviceQueueFamilyProperties)(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties));
}
void vkGetPhysicalDeviceProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) {
	(((PFN_vkGetPhysicalDeviceProperties)addrs->pvkGetPhysicalDeviceProperties)(physicalDevice, pProperties));
}
void vkGetPhysicalDeviceFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) {
	(((PFN_vkGetPhysicalDeviceFormatProperties)addrs->pvkGetPhysicalDeviceFormatProperties)(physicalDevice, format, pFormatProperties));
}
void vkGetPhysicalDeviceFeatures(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
	(((PFN_vkGetPhysicalDeviceFeatures)addrs->pvkGetPhysicalDeviceFeatures)(physicalDevice, pFeatures));
}
VkResult vkEnumeratePhysicalDevices(vksProcAddr* addrs, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) {
	VkResult ret = (((PFN_vkEnumeratePhysicalDevices)addrs->pvkEnumeratePhysicalDevices)(instance, pPhysicalDeviceCount, pPhysicalDevices));
	return ret;
}
void vkDestroyInstance(vksProcAddr* addrs, VkInstance instance, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyInstance)addrs->pvkDestroyInstance)(instance, pAllocator));
}
VkResult vkCreateInstance(vksProcAddr* addrs, VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
	VkResult ret = (((PFN_vkCreateInstance)addrs->pvkCreateInstance)(pCreateInfo, pAllocator, pInstance));
	return ret;
}
void vkCmdSetDepthBias(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
	(((PFN_vkCmdSetDepthBias)addrs->pvkCmdSetDepthBias)(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor));
}
void vkUpdateDescriptorSetWithTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData) {
	(((PFN_vkUpdateDescriptorSetWithTemplate)addrs->pvkUpdateDescriptorSetWithTemplate)(device, descriptorSet, descriptorUpdateTemplate, pData));
}
void vkDestroySamplerYcbcrConversion(vksProcAddr* addrs, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySamplerYcbcrConversion)addrs->pvkDestroySamplerYcbcrConversion)(device, ycbcrConversion, pAllocator));
}
VkResult vkCreateDescriptorUpdateTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
	VkResult ret = (((PFN_vkCreateDescriptorUpdateTemplate)addrs->pvkCreateDescriptorUpdateTemplate)(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate));
	return ret;
}
void vkGetPhysicalDeviceExternalFenceProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
	(((PFN_vkGetPhysicalDeviceExternalFenceProperties)addrs->pvkGetPhysicalDeviceExternalFenceProperties)(physicalDevice, pExternalFenceInfo, pExternalFenceProperties));
}
VkResult vkCreateSamplerYcbcrConversion(vksProcAddr* addrs, VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
	VkResult ret = (((PFN_vkCreateSamplerYcbcrConversion)addrs->pvkCreateSamplerYcbcrConversion)(device, pCreateInfo, pAllocator, pYcbcrConversion));
	return ret;
}
void vkGetDeviceQueue2(vksProcAddr* addrs, VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) {
	(((PFN_vkGetDeviceQueue2)addrs->pvkGetDeviceQueue2)(device, pQueueInfo, pQueue));
}
void vkTrimCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
	(((PFN_vkTrimCommandPool)addrs->pvkTrimCommandPool)(device, commandPool, flags));
}
void vkGetPhysicalDeviceSparseImageFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
	(((PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)addrs->pvkGetPhysicalDeviceSparseImageFormatProperties2)(physicalDevice, pFormatInfo, pPropertyCount, pProperties));
}
void vkGetPhysicalDeviceMemoryProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
	(((PFN_vkGetPhysicalDeviceMemoryProperties2)addrs->pvkGetPhysicalDeviceMemoryProperties2)(physicalDevice, pMemoryProperties));
}
void vkGetPhysicalDeviceQueueFamilyProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
	(((PFN_vkGetPhysicalDeviceQueueFamilyProperties2)addrs->pvkGetPhysicalDeviceQueueFamilyProperties2)(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties));
}
VkResult vkGetPhysicalDeviceImageFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceImageFormatProperties2)addrs->pvkGetPhysicalDeviceImageFormatProperties2)(physicalDevice, pImageFormatInfo, pImageFormatProperties));
	return ret;
}
void vkGetPhysicalDeviceFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
	(((PFN_vkGetPhysicalDeviceFormatProperties2)addrs->pvkGetPhysicalDeviceFormatProperties2)(physicalDevice, format, pFormatProperties));
}
void vkGetPhysicalDeviceProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
	(((PFN_vkGetPhysicalDeviceProperties2)addrs->pvkGetPhysicalDeviceProperties2)(physicalDevice, pProperties));
}
void vkGetPhysicalDeviceFeatures2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
	(((PFN_vkGetPhysicalDeviceFeatures2)addrs->pvkGetPhysicalDeviceFeatures2)(physicalDevice, pFeatures));
}
void vkGetImageSparseMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
	(((PFN_vkGetImageSparseMemoryRequirements2)addrs->pvkGetImageSparseMemoryRequirements2)(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements));
}
void vkGetBufferMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
	(((PFN_vkGetBufferMemoryRequirements2)addrs->pvkGetBufferMemoryRequirements2)(device, pInfo, pMemoryRequirements));
}
void vkGetPhysicalDeviceExternalSemaphoreProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
	(((PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)addrs->pvkGetPhysicalDeviceExternalSemaphoreProperties)(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties));
}
void vkGetImageMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
	(((PFN_vkGetImageMemoryRequirements2)addrs->pvkGetImageMemoryRequirements2)(device, pInfo, pMemoryRequirements));
}
void vkDestroyDescriptorUpdateTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroyDescriptorUpdateTemplate)addrs->pvkDestroyDescriptorUpdateTemplate)(device, descriptorUpdateTemplate, pAllocator));
}
VkResult vkEnumeratePhysicalDeviceGroups(vksProcAddr* addrs, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
	VkResult ret = (((PFN_vkEnumeratePhysicalDeviceGroups)addrs->pvkEnumeratePhysicalDeviceGroups)(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties));
	return ret;
}
void vkCmdDispatchBase(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	(((PFN_vkCmdDispatchBase)addrs->pvkCmdDispatchBase)(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ));
}
void vkCmdSetDeviceMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t deviceMask) {
	(((PFN_vkCmdSetDeviceMask)addrs->pvkCmdSetDeviceMask)(commandBuffer, deviceMask));
}
void vkGetDeviceGroupPeerMemoryFeatures(vksProcAddr* addrs, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
	(((PFN_vkGetDeviceGroupPeerMemoryFeatures)addrs->pvkGetDeviceGroupPeerMemoryFeatures)(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures));
}
void vkGetDescriptorSetLayoutSupport(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
	(((PFN_vkGetDescriptorSetLayoutSupport)addrs->pvkGetDescriptorSetLayoutSupport)(device, pCreateInfo, pSupport));
}
VkResult vkBindImageMemory2(vksProcAddr* addrs, VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos) {
	VkResult ret = (((PFN_vkBindImageMemory2)addrs->pvkBindImageMemory2)(device, bindInfoCount, pBindInfos));
	return ret;
}
VkResult vkBindBufferMemory2(vksProcAddr* addrs, VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos) {
	VkResult ret = (((PFN_vkBindBufferMemory2)addrs->pvkBindBufferMemory2)(device, bindInfoCount, pBindInfos));
	return ret;
}
VkResult vkEnumerateInstanceVersion(vksProcAddr* addrs, uint32_t* pApiVersion) {
	VkResult ret = (((PFN_vkEnumerateInstanceVersion)addrs->pvkEnumerateInstanceVersion)(pApiVersion));
	return ret;
}
void vkGetPhysicalDeviceExternalBufferProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
	(((PFN_vkGetPhysicalDeviceExternalBufferProperties)addrs->pvkGetPhysicalDeviceExternalBufferProperties)(physicalDevice, pExternalBufferInfo, pExternalBufferProperties));
}
VkResult vkWaitSemaphores(vksProcAddr* addrs, VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
	VkResult ret = (((PFN_vkWaitSemaphores)addrs->pvkWaitSemaphores)(device, pWaitInfo, timeout));
	return ret;
}
void vkResetQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	(((PFN_vkResetQueryPool)addrs->pvkResetQueryPool)(device, queryPool, firstQuery, queryCount));
}
VkResult vkGetSemaphoreCounterValue(vksProcAddr* addrs, VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
	VkResult ret = (((PFN_vkGetSemaphoreCounterValue)addrs->pvkGetSemaphoreCounterValue)(device, semaphore, pValue));
	return ret;
}
void vkCmdBeginRenderPass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo) {
	(((PFN_vkCmdBeginRenderPass2)addrs->pvkCmdBeginRenderPass2)(commandBuffer, pRenderPassBegin, pSubpassBeginInfo));
}
void vkCmdEndRenderPass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo) {
	(((PFN_vkCmdEndRenderPass2)addrs->pvkCmdEndRenderPass2)(commandBuffer, pSubpassEndInfo));
}
VkDeviceAddress vkGetBufferDeviceAddress(vksProcAddr* addrs, VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
	VkDeviceAddress ret = (((PFN_vkGetBufferDeviceAddress)addrs->pvkGetBufferDeviceAddress)(device, pInfo));
	return ret;
}
uint64_t vkGetBufferOpaqueCaptureAddress(vksProcAddr* addrs, VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
	uint64_t ret = (((PFN_vkGetBufferOpaqueCaptureAddress)addrs->pvkGetBufferOpaqueCaptureAddress)(device, pInfo));
	return ret;
}
uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(vksProcAddr* addrs, VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
	uint64_t ret = (((PFN_vkGetDeviceMemoryOpaqueCaptureAddress)addrs->pvkGetDeviceMemoryOpaqueCaptureAddress)(device, pInfo));
	return ret;
}
void vkCmdNextSubpass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo) {
	(((PFN_vkCmdNextSubpass2)addrs->pvkCmdNextSubpass2)(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo));
}
VkResult vkCreateRenderPass2(vksProcAddr* addrs, VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
	VkResult ret = (((PFN_vkCreateRenderPass2)addrs->pvkCreateRenderPass2)(device, pCreateInfo, pAllocator, pRenderPass));
	return ret;
}
void vkCmdDrawIndexedIndirectCount(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndexedIndirectCount)addrs->pvkCmdDrawIndexedIndirectCount)(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride));
}
void vkCmdDrawIndirectCount(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	(((PFN_vkCmdDrawIndirectCount)addrs->pvkCmdDrawIndirectCount)(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride));
}
VkResult vkSignalSemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphoreSignalInfo* pSignalInfo) {
	VkResult ret = (((PFN_vkSignalSemaphore)addrs->pvkSignalSemaphore)(device, pSignalInfo));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)addrs->pvkGetPhysicalDeviceSurfacePresentModesKHR)(physicalDevice, surface, pPresentModeCount, pPresentModes));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)addrs->pvkGetPhysicalDeviceSurfaceFormatsKHR)(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)addrs->pvkGetPhysicalDeviceSurfaceCapabilitiesKHR)(physicalDevice, surface, pSurfaceCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceSupportKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceSupportKHR)addrs->pvkGetPhysicalDeviceSurfaceSupportKHR)(physicalDevice, queueFamilyIndex, surface, pSupported));
	return ret;
}
void vkDestroySurfaceKHR(vksProcAddr* addrs, VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySurfaceKHR)addrs->pvkDestroySurfaceKHR)(instance, surface, pAllocator));
}
VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)addrs->pvkGetPhysicalDeviceSurfaceCapabilities2KHR)(physicalDevice, pSurfaceInfo, pSurfaceCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceSurfaceFormats2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)addrs->pvkGetPhysicalDeviceSurfaceFormats2KHR)(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats));
	return ret;
}
VkResult vkGetPhysicalDevicePresentRectanglesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {
	VkResult ret = (((PFN_vkGetPhysicalDevicePresentRectanglesKHR)addrs->pvkGetPhysicalDevicePresentRectanglesKHR)(physicalDevice, surface, pRectCount, pRects));
	return ret;
}
VkResult vkAcquireNextImage2KHR(vksProcAddr* addrs, VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) {
	VkResult ret = (((PFN_vkAcquireNextImage2KHR)addrs->pvkAcquireNextImage2KHR)(device, pAcquireInfo, pImageIndex));
	return ret;
}
VkResult vkGetDeviceGroupSurfacePresentModesKHR(vksProcAddr* addrs, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) {
	VkResult ret = (((PFN_vkGetDeviceGroupSurfacePresentModesKHR)addrs->pvkGetDeviceGroupSurfacePresentModesKHR)(device, surface, pModes));
	return ret;
}
VkResult vkGetDeviceGroupPresentCapabilitiesKHR(vksProcAddr* addrs, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) {
	VkResult ret = (((PFN_vkGetDeviceGroupPresentCapabilitiesKHR)addrs->pvkGetDeviceGroupPresentCapabilitiesKHR)(device, pDeviceGroupPresentCapabilities));
	return ret;
}
VkResult vkQueuePresentKHR(vksProcAddr* addrs, VkQueue queue, VkPresentInfoKHR* pPresentInfo) {
	VkResult ret = (((PFN_vkQueuePresentKHR)addrs->pvkQueuePresentKHR)(queue, pPresentInfo));
	return ret;
}
VkResult vkAcquireNextImageKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
	VkResult ret = (((PFN_vkAcquireNextImageKHR)addrs->pvkAcquireNextImageKHR)(device, swapchain, timeout, semaphore, fence, pImageIndex));
	return ret;
}
VkResult vkGetSwapchainImagesKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
	VkResult ret = (((PFN_vkGetSwapchainImagesKHR)addrs->pvkGetSwapchainImagesKHR)(device, swapchain, pSwapchainImageCount, pSwapchainImages));
	return ret;
}
void vkDestroySwapchainKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator) {
	(((PFN_vkDestroySwapchainKHR)addrs->pvkDestroySwapchainKHR)(device, swapchain, pAllocator));
}
VkResult vkCreateSwapchainKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
	VkResult ret = (((PFN_vkCreateSwapchainKHR)addrs->pvkCreateSwapchainKHR)(device, pCreateInfo, pAllocator, pSwapchain));
	return ret;
}
VkResult vkCreateDisplayPlaneSurfaceKHR(vksProcAddr* addrs, VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	VkResult ret = (((PFN_vkCreateDisplayPlaneSurfaceKHR)addrs->pvkCreateDisplayPlaneSurfaceKHR)(instance, pCreateInfo, pAllocator, pSurface));
	return ret;
}
VkResult vkGetDisplayPlaneCapabilitiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) {
	VkResult ret = (((PFN_vkGetDisplayPlaneCapabilitiesKHR)addrs->pvkGetDisplayPlaneCapabilitiesKHR)(physicalDevice, mode, planeIndex, pCapabilities));
	return ret;
}
VkResult vkCreateDisplayModeKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) {
	VkResult ret = (((PFN_vkCreateDisplayModeKHR)addrs->pvkCreateDisplayModeKHR)(physicalDevice, display, pCreateInfo, pAllocator, pMode));
	return ret;
}
VkResult vkGetDisplayModePropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetDisplayModePropertiesKHR)addrs->pvkGetDisplayModePropertiesKHR)(physicalDevice, display, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayPlaneSupportedDisplaysKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {
	VkResult ret = (((PFN_vkGetDisplayPlaneSupportedDisplaysKHR)addrs->pvkGetDisplayPlaneSupportedDisplaysKHR)(physicalDevice, planeIndex, pDisplayCount, pDisplays));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)addrs->pvkGetPhysicalDeviceDisplayPlanePropertiesKHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)addrs->pvkGetPhysicalDeviceDisplayPropertiesKHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayPlaneCapabilities2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) {
	VkResult ret = (((PFN_vkGetDisplayPlaneCapabilities2KHR)addrs->pvkGetDisplayPlaneCapabilities2KHR)(physicalDevice, pDisplayPlaneInfo, pCapabilities));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayProperties2KHR)addrs->pvkGetPhysicalDeviceDisplayProperties2KHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)addrs->pvkGetPhysicalDeviceDisplayPlaneProperties2KHR)(physicalDevice, pPropertyCount, pProperties));
	return ret;
}
VkResult vkGetDisplayModeProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) {
	VkResult ret = (((PFN_vkGetDisplayModeProperties2KHR)addrs->pvkGetDisplayModeProperties2KHR)(physicalDevice, display, pPropertyCount, pProperties));
	return ret;
}
VkResult vkCreateSharedSwapchainsKHR(vksProcAddr* addrs, VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) {
	VkResult ret = (((PFN_vkCreateSharedSwapchainsKHR)addrs->pvkCreateSharedSwapchainsKHR)(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains));
	return ret;
}
