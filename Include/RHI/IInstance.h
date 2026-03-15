#ifndef HONOU_RENDER_IINSTANCE_H
#define HONOU_RENDER_IINSTANCE_H

class VKInstance;
class DX12Instance;
class OGLInstance;

class IInstance
{
public:
	virtual ~IInstance() { };

	virtual void CreateInstance() = 0;
	virtual void DestroyInstance() = 0;

	virtual VKInstance* CastToVK() { return nullptr; }
	virtual DX12Instance* CastToDX12() { return nullptr; }
	virtual OGLInstance* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IINSTANCE_H