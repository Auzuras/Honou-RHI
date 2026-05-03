#ifndef HONOU_RENDER_VKSURFACE_H
#define HONOU_RENDER_VKSURFACE_H

#include <RHI/ISurface.h>

#include <vulkan/vulkan.h>

class VKSurface : public ISurface
{
public:
	~VKSurface() override final = default;

	void CreateSurface(IInstance* _instance, const SurfaceInformation& _surfaceInfo) override final;
	void DestroySurface(IInstance* _instance) override final;

	VKSurface* const CastToVK() override final { return this; }
	DX12Surface* CastToDX12() override final { return nullptr; }
	OGLSurface* CastToOGL() override final { return nullptr; }

	VkSurfaceKHR GetRaw() const { return m_Surface; }

private:

	VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKSURFACE_H
