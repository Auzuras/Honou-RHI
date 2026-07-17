#ifndef HONOU_RENDER_IDESCRIPTOR_H
#define HONOU_RENDER_IDESCRIPTOR_H

class VKDescriptor;
class DX12Descriptor;
class OGLDescriptor;

class IDevice;
class IBuffer;
class IImage;
class ISampler;

class IDescriptor
{
public:
	virtual ~IDescriptor() = default;

	virtual void Update(IDevice* _device, IBuffer* _buffer) const = 0;
	virtual void Update(IDevice* _device, IImage* _image) const = 0;
	virtual void Update(IDevice* _device, ISampler* _sampler) const = 0;

	virtual VKDescriptor* CastToVK() { return nullptr; }
	virtual DX12Descriptor* CastToDX12() { return nullptr; }
	virtual OGLDescriptor* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IDESCRIPTOR_H
