#ifndef HONOU_RHI_DX12DEVICE_H
#define HONOU_RHI_DX12DEVICE_H

#include <RHI/IDevice.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Device : public IDevice
{
public:
	~DX12Device() override final = default;

	void CreateDevice(IInstance* _instance) override final;
	void DestroyDevice() override final;

	void WaitIdle() override final;

	VKDevice* const CastToVK() override final { return nullptr; }
	DX12Device* CastToDX12() override final { return this; }
	OGLDevice* CastToOGL() override final { return nullptr; }

	ID3D12Device* const GetRaw() const { return m_Device.Get(); }

private:

	Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr;
};

#endif //HONOU_RHI_DX12DEVICE_H