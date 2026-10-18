#ifndef HONOU_RHI_DX12FENCE_H
#define HONOU_RHI_DX12FENCE_H

#include <RHI/IFence.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Fence : public IFence
{
public:
	~DX12Fence() override final = default;

	void CreateFence(IDevice* _device, bool _signaled = true) override final;
	void DestroyFence(IDevice* _device) override final;

	void Wait(IDevice* _device) override final;
	void Reset(IDevice* _device) override final;

	VKFence* CastToVK() override final { return nullptr; }
	DX12Fence* CastToDX12() override final { return this; }
	OGLFence* CastToOGL() override final { return nullptr; }

private:
	
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence = nullptr;
	HANDLE m_Event = nullptr;

	UINT64 m_NextValue = 1;
	UINT64 m_WaitValue = 0;
};

#endif //HONOU_RHI_DX12FENCE_H