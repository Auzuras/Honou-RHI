#ifndef HONOU_RHI_IRENDER_HARDWARE_H
#define HONOU_RHI_IRENDER_HARDWARE_H

#include <RHI/IInstance.h>
#include <RHI/IDevice.h>

#include <RHI/ISwapChain.h>
#include <RHI/IPipeline.h>
#include <RHI/IShaderModule.h>

#include <RHI/ICommandAllocator.h>
#include <RHI/IDescriptorAllocator.h>

#include <RHI/IImage.h>
#include <RHI/IBuffer.h>
#include <RHI/ISampler.h>

#include <cstdint>

/**
 * @brief IRenderHardware interface
 */
class IRenderHardware
{
public:

	/**
	 * @brief RenderHardware enumeration allowing to select the API backend used
	 */
	enum class RenderHardware : uint8_t
	{
		HOUNOU_RENDER_HARDWARE_VULKAN		=	0,	/*	  Vulkan 1.4	*/
		HOUNOU_RENDER_HARDWARE_DIRECTX		=	1,	/*	  DirectX 12	*/
		HOUNOU_RENDER_HARDWARE_OPENGL		=	2	/*	   OpenGL 4		*/
	};

public:
	/**
	 * @brief 
	 */
	virtual ~IRenderHardware() = default;

	/*
		Initialization and termination
	*/

	/**
	 * @brief Initialize the RHI according to the selected backend
	 * @param _renderHardware Selected backend
	 */
	virtual void Initialize(RenderHardware _renderHardware = RenderHardware::HOUNOU_RENDER_HARDWARE_VULKAN) = 0;
	
	/**
	 * @brief Terminate the RHI
	 */
	virtual void Terminate() = 0;

	/*
		Instance and device
	*/

	/**
	 * @brief Create an API instance
	 * @return A pointer to the created API instance
	 */
	virtual IInstance* CreateInstance() = 0;
	
	/**
	 * @brief Destroy the specified API instance
	 * @param _instance Instance to destroy
	 */
	virtual void DestroyInstance(IInstance* _instance) = 0;

	/**
	 * @brief Create an API device
	 * @param _instance Instance of the API
	 * @return A pointer to the created API device
	 */
	virtual IDevice* CreateDevice(IInstance* const _instance) = 0;
	
	/**
	 * @brief Destroy the specified API device
	 * @param _device Device to destroy
	 */
	virtual void DestroyDevice(IDevice* _device) = 0;

	/*
		Render objects
	*/

	/**
	 * @brief Create a Swap Chain render object
	 * @param _instance Instance of the API
	 * @param _device Current device
	 * @param _description Information about the Swap Chain
	 * @return A pointer to the created Swap Chain
	 */
	virtual ISwapChain* CreateSwapChain(IInstance* const _instance, IDevice* const _device, const ISwapChain::Description& _description) = 0;

	/**
	 * @brief Destroy a specified Swap Chain
	 * @param _instance Instance of the API
	 * @param _device Current device
	 * @param _swapChain Swap Chain to destroy
	 */
	virtual void DestroySwapChain(IInstance* const _instance, IDevice* const _device, ISwapChain* _swapChain) = 0;

	/**
	 * @brief Create a Graphics Pipeline render object
	 * @param _device Device of the application
	 * @param _description Information about the Graphics Pipeline
	 * @return A pointer to the created Graphics Pipeline
	 */
	virtual IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::GraphicsDescription& _description) = 0;

	/**
	 * @brief Create a Compute Pipeline render object
	 * @param _device Device of the application
	 * @param _description Information about the Compute Pipeline
	 * @return A pointer to the created Compute Pipeline
	 */
	virtual IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::ComputeDescription& _description) = 0;

	/**
	 * @brief Destroy a specified Pipeline
	 * @param _device Device of the application
	 * @param _pipeline Pipeline to destroy
	 */
	virtual void DestroyPipeline(IDevice* const _device, IPipeline* _pipeline) = 0;

	/**
	 * @brief Create a Shader Module render object
	 * @param _device Device of the application
	 * @param _data Compiled data of the shader
	 * @return A pointer to the created Shader Module
	 */
	virtual IShaderModule* CreateShaderModule(IDevice* const _device, const std::vector<char>& _data) = 0;

	/**
	 * @brief Destroy a specified Shader Module
	 * @param _device Device of the application
	 * @param _shaderModule Shader Module to destroy
	 */
	virtual void DestroyShaderModule(IDevice* const _device, IShaderModule* _shaderModule) = 0;

	/*
		Allocators
	*/

	/**
	 * @brief Create a Command Allocator object
	 * @param _device Device of the application
	 * @return A pointer to the created Command Allocator
	 */
	virtual ICommandAllocator* CreateCommandAllocator(IDevice* const _device) = 0;

	/**
	 * @brief Destroy a specified Command Allocator
	 * @param _device Device of the application
	 * @param _commandAllocator Command Allocator to destroy
	 */
	virtual void DestroyCommandAllocator(IDevice* const _device, ICommandAllocator* _commandAllocator) = 0;

	/**
	 * @brief Create a Descriptor Allocator object
	 * @param _device Device of the application
	 * @return A pointer to the created Descriptor Allocator
	 */
	virtual IDescriptorAllocator* CreateDescriptorAllocator(IDevice* const _device) = 0;

	/**
	 * @brief Destroy a specified Descriptor Allocator
	 * @param _device Device of the application
	 * @param _descriptorAllocator Descriptor Allocator to destroy
	 */
	virtual void DestroyDescriptorAllocator(IDevice* const _device, IDescriptorAllocator* _descriptorAllocator) = 0;

	/*
		Resources
	*/

	/**
	 * @brief Create an Image resource object
	 * @param _device Device of the application
	 * @param _description Information about the Image
	 * @return A pointer to the created Image
	 */
	virtual IImage* CreateImage(IDevice* const _device, const IImage::Description& _description) = 0;

	/**
	 * @brief Destroy a specified Image resource object
	 * @param _device Device of the application
	 * @param _image Image to destroy
	 */
	virtual void DestroyImage(IDevice* const _device, IImage* _image) = 0;

	/**
	 * @brief Create a Buffer resource object
	 * @param _device Device of the application
	 * @param _description Information about the buffer
	 * @return A pointer to the created Buffer
	 */
	virtual IBuffer* CreateBuffer(IDevice* const _device, const IBuffer::Description& _description) = 0;

	/**
	 * @brief Destroy a specified Buffer resource object
	 * @param _device Device of the application
	 * @param _buffer Buffer to destroy
	 */
	virtual void DestroyBuffer(IDevice* const _device, IBuffer* _buffer) = 0;

	/**
	 * @brief Create a Sampler resource object
	 * @param _device Device of the application
	 * @param _description Information about the Sampler
	 * @return A pointer to the created Sampler
	 */
	virtual ISampler* CreateSampler(IDevice* const _device, const ISampler::Description& _description) = 0;

	/**
	 * @brief Destroy a specified Sampler resource object
	 * @param _device Device of the application
	 * @param _sampler Sampler to destroy
	 */
	virtual void DestroySampler(IDevice* const _device, ISampler* _sampler) = 0;
};

#endif //HONOU_RHI_IRENDER_HARDWARE_H
