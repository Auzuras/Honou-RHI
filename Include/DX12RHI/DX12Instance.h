#ifndef HONOU_RENDER_DX12INSTANCE_H
#define HONOU_RENDER_DX12INSTANCE_H

#include <RHI/IInstance.h>

#include <wrl.h>
#include <dxgi1_6.h>

class DX12Instance : public IInstance
{
public:
	~DX12Instance() override final = default;

	void CreateInstance() override final;
	void DestroyInstance() override final;

	VKInstance* CastToVK() override final { return nullptr; }
	DX12Instance* CastToDX12() override final { return this; }
	OGLInstance* CastToOGL() override final { return nullptr; }

	Microsoft::WRL::ComPtr<IDXGIFactory6> GetRaw() const { return m_Factory; }

private:

	Microsoft::WRL::ComPtr<IDXGIFactory6> m_Factory = nullptr;
};

#endif //HONOU_RENDER_DX12INSTANCE_H