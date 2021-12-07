#ifndef __VKS_H__
#define __VKS_H__
#define VK_ENABLE_BETA_EXTENSIONS
#define VK_NO_PROTOTYPES
#include <stdlib.h>
#include <string.h>
#include "vulkan/vulkan.h"

typedef struct vksProcAddr { 
	void* pvkGetPhysicalDeviceImageFormatProperties;
	void* pvkDestroyDescriptorPool;
	void* pvkResetDescriptorPool;
	void* pvkAllocateDescriptorSets;
	void* pvkFreeDescriptorSets;
	void* pvkUpdateDescriptorSets;
	void* pvkCreateDescriptorPool;
	void* pvkDestroyDescriptorSetLayout;
	void* pvkCreateDescriptorSetLayout;
	void* pvkDestroySampler;
	void* pvkCreateSampler;
	void* pvkDestroyPipelineLayout;
	void* pvkCreatePipelineLayout;
	void* pvkDestroyPipeline;
	void* pvkCreateComputePipelines;
	void* pvkCreateGraphicsPipelines;
	void* pvkMergePipelineCaches;
	void* pvkGetPipelineCacheData;
	void* pvkDestroyPipelineCache;
	void* pvkCreatePipelineCache;
	void* pvkDestroyShaderModule;
	void* pvkCreateShaderModule;
	void* pvkDestroyImageView;
	void* pvkCreateImageView;
	void* pvkCreateFramebuffer;
	void* pvkDestroyFramebuffer;
	void* pvkCreateRenderPass;
	void* pvkDestroyRenderPass;
	void* pvkGetRenderAreaGranularity;
	void* pvkGetImageSubresourceLayout;
	void* pvkDestroyImage;
	void* pvkCreateImage;
	void* pvkDestroyBufferView;
	void* pvkCreateBufferView;
	void* pvkDestroyBuffer;
	void* pvkCreateBuffer;
	void* pvkGetQueryPoolResults;
	void* pvkDestroyQueryPool;
	void* pvkCreateQueryPool;
	void* pvkResetEvent;
	void* pvkSetEvent;
	void* pvkGetEventStatus;
	void* pvkDestroyEvent;
	void* pvkCreateEvent;
	void* pvkDestroySemaphore;
	void* pvkCreateSemaphore;
	void* pvkWaitForFences;
	void* pvkGetFenceStatus;
	void* pvkResetFences;
	void* pvkDestroyFence;
	void* pvkCreateFence;
	void* pvkQueueBindSparse;
	void* pvkCreateCommandPool;
	void* pvkDestroyCommandPool;
	void* pvkResetCommandPool;
	void* pvkGetPhysicalDeviceSparseImageFormatProperties;
	void* pvkGetImageSparseMemoryRequirements;
	void* pvkGetImageMemoryRequirements;
	void* pvkGetBufferMemoryRequirements;
	void* pvkBindImageMemory;
	void* pvkBindBufferMemory;
	void* pvkAllocateCommandBuffers;
	void* pvkFreeCommandBuffers;
	void* pvkBeginCommandBuffer;
	void* pvkEndCommandBuffer;
	void* pvkResetCommandBuffer;
	void* pvkGetDeviceMemoryCommitment;
	void* pvkInvalidateMappedMemoryRanges;
	void* pvkFlushMappedMemoryRanges;
	void* pvkUnmapMemory;
	void* pvkMapMemory;
	void* pvkFreeMemory;
	void* pvkAllocateMemory;
	void* pvkDeviceWaitIdle;
	void* pvkQueueWaitIdle;
	void* pvkQueueSubmit;
	void* pvkGetDeviceQueue;
	void* pvkCmdBindPipeline;
	void* pvkCmdSetViewport;
	void* pvkCmdSetScissor;
	void* pvkCmdSetLineWidth;
	void* pvkEnumerateDeviceLayerProperties;
	void* pvkCmdSetBlendConstants;
	void* pvkCmdSetDepthBounds;
	void* pvkCmdSetStencilCompareMask;
	void* pvkCmdSetStencilWriteMask;
	void* pvkCmdSetStencilReference;
	void* pvkCmdBindDescriptorSets;
	void* pvkCmdBindIndexBuffer;
	void* pvkCmdBindVertexBuffers;
	void* pvkCmdDraw;
	void* pvkCmdDrawIndexed;
	void* pvkCmdDrawIndirect;
	void* pvkCmdDrawIndexedIndirect;
	void* pvkCmdDispatch;
	void* pvkCmdDispatchIndirect;
	void* pvkCmdCopyBuffer;
	void* pvkCmdCopyImage;
	void* pvkCmdBlitImage;
	void* pvkCmdCopyBufferToImage;
	void* pvkCmdCopyImageToBuffer;
	void* pvkCmdUpdateBuffer;
	void* pvkCmdFillBuffer;
	void* pvkCmdClearColorImage;
	void* pvkCmdClearDepthStencilImage;
	void* pvkCmdClearAttachments;
	void* pvkCmdResolveImage;
	void* pvkCmdSetEvent;
	void* pvkCmdResetEvent;
	void* pvkCmdWaitEvents;
	void* pvkCmdPipelineBarrier;
	void* pvkCmdBeginQuery;
	void* pvkCmdEndQuery;
	void* pvkCmdResetQueryPool;
	void* pvkCmdWriteTimestamp;
	void* pvkCmdCopyQueryPoolResults;
	void* pvkCmdPushConstants;
	void* pvkCmdBeginRenderPass;
	void* pvkCmdNextSubpass;
	void* pvkCmdEndRenderPass;
	void* pvkCmdExecuteCommands;
	void* pvkEnumerateInstanceLayerProperties;
	void* pvkEnumerateDeviceExtensionProperties;
	void* pvkEnumerateInstanceExtensionProperties;
	void* pvkDestroyDevice;
	void* pvkCreateDevice;
	void* pvkGetDeviceProcAddr;
	void* pvkGetInstanceProcAddr;
	void* pvkGetPhysicalDeviceMemoryProperties;
	void* pvkGetPhysicalDeviceQueueFamilyProperties;
	void* pvkGetPhysicalDeviceProperties;
	void* pvkGetPhysicalDeviceFormatProperties;
	void* pvkGetPhysicalDeviceFeatures;
	void* pvkEnumeratePhysicalDevices;
	void* pvkDestroyInstance;
	void* pvkCreateInstance;
	void* pvkCmdSetDepthBias;
	void* pvkUpdateDescriptorSetWithTemplate;
	void* pvkDestroySamplerYcbcrConversion;
	void* pvkCreateDescriptorUpdateTemplate;
	void* pvkGetPhysicalDeviceExternalFenceProperties;
	void* pvkCreateSamplerYcbcrConversion;
	void* pvkGetDeviceQueue2;
	void* pvkTrimCommandPool;
	void* pvkGetPhysicalDeviceSparseImageFormatProperties2;
	void* pvkGetPhysicalDeviceMemoryProperties2;
	void* pvkGetPhysicalDeviceQueueFamilyProperties2;
	void* pvkGetPhysicalDeviceImageFormatProperties2;
	void* pvkGetPhysicalDeviceFormatProperties2;
	void* pvkGetPhysicalDeviceProperties2;
	void* pvkGetPhysicalDeviceFeatures2;
	void* pvkGetImageSparseMemoryRequirements2;
	void* pvkGetBufferMemoryRequirements2;
	void* pvkGetPhysicalDeviceExternalSemaphoreProperties;
	void* pvkGetImageMemoryRequirements2;
	void* pvkDestroyDescriptorUpdateTemplate;
	void* pvkEnumeratePhysicalDeviceGroups;
	void* pvkCmdDispatchBase;
	void* pvkCmdSetDeviceMask;
	void* pvkGetDeviceGroupPeerMemoryFeatures;
	void* pvkGetDescriptorSetLayoutSupport;
	void* pvkBindImageMemory2;
	void* pvkBindBufferMemory2;
	void* pvkEnumerateInstanceVersion;
	void* pvkGetPhysicalDeviceExternalBufferProperties;
	void* pvkWaitSemaphores;
	void* pvkResetQueryPool;
	void* pvkGetSemaphoreCounterValue;
	void* pvkCmdBeginRenderPass2;
	void* pvkCmdEndRenderPass2;
	void* pvkGetBufferDeviceAddress;
	void* pvkGetBufferOpaqueCaptureAddress;
	void* pvkGetDeviceMemoryOpaqueCaptureAddress;
	void* pvkCmdNextSubpass2;
	void* pvkCreateRenderPass2;
	void* pvkCmdDrawIndexedIndirectCount;
	void* pvkCmdDrawIndirectCount;
	void* pvkSignalSemaphore;
	void* pvkGetPhysicalDeviceSurfacePresentModesKHR;
	void* pvkGetPhysicalDeviceSurfaceFormatsKHR;
	void* pvkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	void* pvkGetPhysicalDeviceSurfaceSupportKHR;
	void* pvkDestroySurfaceKHR;
	void* pvkGetPhysicalDeviceSurfaceCapabilities2KHR;
	void* pvkGetPhysicalDeviceSurfaceFormats2KHR;
	void* pvkGetPhysicalDevicePresentRectanglesKHR;
	void* pvkAcquireNextImage2KHR;
	void* pvkGetDeviceGroupSurfacePresentModesKHR;
	void* pvkGetDeviceGroupPresentCapabilitiesKHR;
	void* pvkQueuePresentKHR;
	void* pvkAcquireNextImageKHR;
	void* pvkGetSwapchainImagesKHR;
	void* pvkDestroySwapchainKHR;
	void* pvkCreateSwapchainKHR;
	void* pvkCreateDisplayPlaneSurfaceKHR;
	void* pvkGetDisplayPlaneCapabilitiesKHR;
	void* pvkCreateDisplayModeKHR;
	void* pvkGetDisplayModePropertiesKHR;
	void* pvkGetDisplayPlaneSupportedDisplaysKHR;
	void* pvkGetPhysicalDeviceDisplayPlanePropertiesKHR;
	void* pvkGetPhysicalDeviceDisplayPropertiesKHR;
	void* pvkGetDisplayPlaneCapabilities2KHR;
	void* pvkGetPhysicalDeviceDisplayProperties2KHR;
	void* pvkGetPhysicalDeviceDisplayPlaneProperties2KHR;
	void* pvkGetDisplayModeProperties2KHR;
	void* pvkCreateSharedSwapchainsKHR;
} vksProcAddr;
extern vksProcAddr vksProcAddresses;
void vksLoadInstanceProcAddrs(VkInstance, vksProcAddr*);
void vksLoadDeviceProcAddrs(VkDevice, vksProcAddr*, vksProcAddr*);

VkResult vksDynamicLoad();
void vksDynamicUnload();
VkResult vkGetPhysicalDeviceImageFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void vkDestroyDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator);
VkResult vkResetDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
VkResult vkAllocateDescriptorSets(vksProcAddr* addrs, VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
VkResult vkFreeDescriptorSets(vksProcAddr* addrs, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets);
void vkUpdateDescriptorSets(vksProcAddr* addrs, VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies);
VkResult vkCreateDescriptorPool(vksProcAddr* addrs, VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void vkDestroyDescriptorSetLayout(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator);
VkResult vkCreateDescriptorSetLayout(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void vkDestroySampler(vksProcAddr* addrs, VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator);
VkResult vkCreateSampler(vksProcAddr* addrs, VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void vkDestroyPipelineLayout(vksProcAddr* addrs, VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator);
VkResult vkCreatePipelineLayout(vksProcAddr* addrs, VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void vkDestroyPipeline(vksProcAddr* addrs, VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator);
VkResult vkCreateComputePipelines(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
VkResult vkCreateGraphicsPipelines(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
VkResult vkMergePipelineCaches(vksProcAddr* addrs, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches);
VkResult vkGetPipelineCacheData(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void vkDestroyPipelineCache(vksProcAddr* addrs, VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator);
VkResult vkCreatePipelineCache(vksProcAddr* addrs, VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void vkDestroyShaderModule(vksProcAddr* addrs, VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator);
VkResult vkCreateShaderModule(vksProcAddr* addrs, VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void vkDestroyImageView(vksProcAddr* addrs, VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator);
VkResult vkCreateImageView(vksProcAddr* addrs, VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView);
VkResult vkCreateFramebuffer(vksProcAddr* addrs, VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void vkDestroyFramebuffer(vksProcAddr* addrs, VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator);
VkResult vkCreateRenderPass(vksProcAddr* addrs, VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void vkDestroyRenderPass(vksProcAddr* addrs, VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator);
void vkGetRenderAreaGranularity(vksProcAddr* addrs, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void vkGetImageSubresourceLayout(vksProcAddr* addrs, VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void vkDestroyImage(vksProcAddr* addrs, VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);
VkResult vkCreateImage(vksProcAddr* addrs, VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);
void vkDestroyBufferView(vksProcAddr* addrs, VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator);
VkResult vkCreateBufferView(vksProcAddr* addrs, VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void vkDestroyBuffer(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);
VkResult vkCreateBuffer(vksProcAddr* addrs, VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
VkResult vkGetQueryPoolResults(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void vkDestroyQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator);
VkResult vkCreateQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
VkResult vkResetEvent(vksProcAddr* addrs, VkDevice device, VkEvent event);
VkResult vkSetEvent(vksProcAddr* addrs, VkDevice device, VkEvent event);
VkResult vkGetEventStatus(vksProcAddr* addrs, VkDevice device, VkEvent event);
void vkDestroyEvent(vksProcAddr* addrs, VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator);
VkResult vkCreateEvent(vksProcAddr* addrs, VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void vkDestroySemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator);
VkResult vkCreateSemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
VkResult vkWaitForFences(vksProcAddr* addrs, VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
VkResult vkGetFenceStatus(vksProcAddr* addrs, VkDevice device, VkFence fence);
VkResult vkResetFences(vksProcAddr* addrs, VkDevice device, uint32_t fenceCount, VkFence* pFences);
void vkDestroyFence(vksProcAddr* addrs, VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator);
VkResult vkCreateFence(vksProcAddr* addrs, VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence);
VkResult vkQueueBindSparse(vksProcAddr* addrs, VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence);
VkResult vkCreateCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void vkDestroyCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator);
VkResult vkResetCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void vkGetPhysicalDeviceSparseImageFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void vkGetImageSparseMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void vkGetImageMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void vkGetBufferMemoryRequirements(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
VkResult vkBindImageMemory(vksProcAddr* addrs, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
VkResult vkBindBufferMemory(vksProcAddr* addrs, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
VkResult vkAllocateCommandBuffers(vksProcAddr* addrs, VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void vkFreeCommandBuffers(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);
VkResult vkBeginCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo);
VkResult vkEndCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer);
VkResult vkResetCommandBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void vkGetDeviceMemoryCommitment(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
VkResult vkInvalidateMappedMemoryRanges(vksProcAddr* addrs, VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges);
VkResult vkFlushMappedMemoryRanges(vksProcAddr* addrs, VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges);
void vkUnmapMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory);
VkResult vkMapMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void vkFreeMemory(vksProcAddr* addrs, VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator);
VkResult vkAllocateMemory(vksProcAddr* addrs, VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
VkResult vkDeviceWaitIdle(vksProcAddr* addrs, VkDevice device);
VkResult vkQueueWaitIdle(vksProcAddr* addrs, VkQueue queue);
VkResult vkQueueSubmit(vksProcAddr* addrs, VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);
void vkGetDeviceQueue(vksProcAddr* addrs, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void vkCmdBindPipeline(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void vkCmdSetViewport(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports);
void vkCmdSetScissor(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors);
void vkCmdSetLineWidth(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float lineWidth);
VkResult vkEnumerateDeviceLayerProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void vkCmdSetBlendConstants(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float blendConstants[4]);
void vkCmdSetDepthBounds(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void vkCmdSetStencilCompareMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void vkCmdSetStencilWriteMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void vkCmdSetStencilReference(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void vkCmdBindDescriptorSets(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets);
void vkCmdBindIndexBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void vkCmdBindVertexBuffers(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets);
void vkCmdDraw(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void vkCmdDrawIndexed(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void vkCmdDrawIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void vkCmdDrawIndexedIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void vkCmdDispatch(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void vkCmdDispatchIndirect(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void vkCmdCopyBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions);
void vkCmdCopyImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions);
void vkCmdBlitImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter);
void vkCmdCopyBufferToImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions);
void vkCmdCopyImageToBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions);
void vkCmdUpdateBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData);
void vkCmdFillBuffer(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void vkCmdClearColorImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges);
void vkCmdClearDepthStencilImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges);
void vkCmdClearAttachments(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects);
void vkCmdResolveImage(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions);
void vkCmdSetEvent(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void vkCmdResetEvent(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void vkCmdWaitEvents(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers);
void vkCmdPipelineBarrier(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers);
void vkCmdBeginQuery(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void vkCmdEndQuery(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void vkCmdResetQueryPool(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void vkCmdWriteTimestamp(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void vkCmdCopyQueryPoolResults(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void vkCmdPushConstants(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues);
void vkCmdBeginRenderPass(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void vkCmdNextSubpass(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void vkCmdEndRenderPass(vksProcAddr* addrs, VkCommandBuffer commandBuffer);
void vkCmdExecuteCommands(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);
VkResult vkEnumerateInstanceLayerProperties(vksProcAddr* addrs, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
VkResult vkEnumerateDeviceExtensionProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
VkResult vkEnumerateInstanceExtensionProperties(vksProcAddr* addrs, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void vkDestroyDevice(vksProcAddr* addrs, VkDevice device, VkAllocationCallbacks* pAllocator);
VkResult vkCreateDevice(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
PFN_vkVoidFunction vkGetDeviceProcAddr(vksProcAddr* addrs, VkDevice device, char* pName);
PFN_vkVoidFunction vkGetInstanceProcAddr(vksProcAddr* addrs, VkInstance instance, char* pName);
void vkGetPhysicalDeviceMemoryProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void vkGetPhysicalDeviceQueueFamilyProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void vkGetPhysicalDeviceProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void vkGetPhysicalDeviceFormatProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void vkGetPhysicalDeviceFeatures(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
VkResult vkEnumeratePhysicalDevices(vksProcAddr* addrs, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void vkDestroyInstance(vksProcAddr* addrs, VkInstance instance, VkAllocationCallbacks* pAllocator);
VkResult vkCreateInstance(vksProcAddr* addrs, VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void vkCmdSetDepthBias(vksProcAddr* addrs, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void vkUpdateDescriptorSetWithTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData);
void vkDestroySamplerYcbcrConversion(vksProcAddr* addrs, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator);
VkResult vkCreateDescriptorUpdateTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void vkGetPhysicalDeviceExternalFenceProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
VkResult vkCreateSamplerYcbcrConversion(vksProcAddr* addrs, VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void vkGetDeviceQueue2(vksProcAddr* addrs, VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void vkTrimCommandPool(vksProcAddr* addrs, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void vkGetPhysicalDeviceSparseImageFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void vkGetPhysicalDeviceMemoryProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void vkGetPhysicalDeviceQueueFamilyProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
VkResult vkGetPhysicalDeviceImageFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void vkGetPhysicalDeviceFormatProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void vkGetPhysicalDeviceProperties2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void vkGetPhysicalDeviceFeatures2(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void vkGetImageSparseMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void vkGetBufferMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void vkGetPhysicalDeviceExternalSemaphoreProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void vkGetImageMemoryRequirements2(vksProcAddr* addrs, VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void vkDestroyDescriptorUpdateTemplate(vksProcAddr* addrs, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator);
VkResult vkEnumeratePhysicalDeviceGroups(vksProcAddr* addrs, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void vkCmdDispatchBase(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void vkCmdSetDeviceMask(vksProcAddr* addrs, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void vkGetDeviceGroupPeerMemoryFeatures(vksProcAddr* addrs, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void vkGetDescriptorSetLayoutSupport(vksProcAddr* addrs, VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
VkResult vkBindImageMemory2(vksProcAddr* addrs, VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos);
VkResult vkBindBufferMemory2(vksProcAddr* addrs, VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos);
VkResult vkEnumerateInstanceVersion(vksProcAddr* addrs, uint32_t* pApiVersion);
void vkGetPhysicalDeviceExternalBufferProperties(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
VkResult vkWaitSemaphores(vksProcAddr* addrs, VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void vkResetQueryPool(vksProcAddr* addrs, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
VkResult vkGetSemaphoreCounterValue(vksProcAddr* addrs, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void vkCmdBeginRenderPass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo);
void vkCmdEndRenderPass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo);
VkDeviceAddress vkGetBufferDeviceAddress(vksProcAddr* addrs, VkDevice device, VkBufferDeviceAddressInfo* pInfo);
uint64_t vkGetBufferOpaqueCaptureAddress(vksProcAddr* addrs, VkDevice device, VkBufferDeviceAddressInfo* pInfo);
uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(vksProcAddr* addrs, VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void vkCmdNextSubpass2(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo);
VkResult vkCreateRenderPass2(vksProcAddr* addrs, VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void vkCmdDrawIndexedIndirectCount(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void vkCmdDrawIndirectCount(vksProcAddr* addrs, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
VkResult vkSignalSemaphore(vksProcAddr* addrs, VkDevice device, VkSemaphoreSignalInfo* pSignalInfo);
VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
VkResult vkGetPhysicalDeviceSurfaceSupportKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void vkDestroySurfaceKHR(vksProcAddr* addrs, VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator);
VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
VkResult vkGetPhysicalDeviceSurfaceFormats2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
VkResult vkGetPhysicalDevicePresentRectanglesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
VkResult vkAcquireNextImage2KHR(vksProcAddr* addrs, VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
VkResult vkGetDeviceGroupSurfacePresentModesKHR(vksProcAddr* addrs, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
VkResult vkGetDeviceGroupPresentCapabilitiesKHR(vksProcAddr* addrs, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
VkResult vkQueuePresentKHR(vksProcAddr* addrs, VkQueue queue, VkPresentInfoKHR* pPresentInfo);
VkResult vkAcquireNextImageKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
VkResult vkGetSwapchainImagesKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void vkDestroySwapchainKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator);
VkResult vkCreateSwapchainKHR(vksProcAddr* addrs, VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
VkResult vkCreateDisplayPlaneSurfaceKHR(vksProcAddr* addrs, VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
VkResult vkGetDisplayPlaneCapabilitiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
VkResult vkCreateDisplayModeKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
VkResult vkGetDisplayModePropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
VkResult vkGetDisplayPlaneSupportedDisplaysKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
VkResult vkGetPhysicalDeviceDisplayPropertiesKHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
VkResult vkGetDisplayPlaneCapabilities2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
VkResult vkGetPhysicalDeviceDisplayProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
VkResult vkGetDisplayModeProperties2KHR(vksProcAddr* addrs, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
VkResult vkCreateSharedSwapchainsKHR(vksProcAddr* addrs, VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);

#endif