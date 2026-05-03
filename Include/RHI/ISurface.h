#ifndef HONOU_RENDER_ISURFACE_H
#define HONOU_RENDER_ISURFACE_H

class VKSurface;
class DX12Surface;
class OGLSurface;

class IInstance;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ISurface
{
public:

	struct SurfaceInformation
	{
		HINSTANCE instance = NULL;
		HWND hwnd = NULL;
	};

	virtual ~ISurface() = default;

	virtual void CreateSurface(IInstance* _instance, const SurfaceInformation& _surfaceInfo) = 0;
	virtual void DestroySurface(IInstance* _instance) = 0;

	virtual VKSurface* const CastToVK() { return nullptr; }
	virtual DX12Surface* CastToDX12() { return nullptr; }
	virtual OGLSurface* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ISURFACE_H
