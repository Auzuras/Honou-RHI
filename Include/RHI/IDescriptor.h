#ifndef HONOU_RENDER_IDESCRIPTOR_H
#define HONOU_RENDER_IDESCRIPTOR_H

class VKDescriptor;
class DX12Descriptor;
class OGLDescriptor;

class IDescriptor
{
public:
	virtual ~IDescriptor() = default;

	virtual VKDescriptor* CastToVK() { return nullptr; }
	virtual DX12Descriptor* CastToDX12() { return nullptr; }
	virtual OGLDescriptor* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IDESCRIPTOR_H
