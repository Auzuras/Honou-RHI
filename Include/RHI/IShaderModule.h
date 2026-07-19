#ifndef HONOU_RHI_ISHADERMODULE_H
#define HONOU_RHI_ISHADERMODULE_H

class VKShaderModule;
class DX12ShaderModule;
class OGLShaderModule;

class IDevice;

#include <vector>
#include <fstream>

class IShaderModule
{
public:
	virtual ~IShaderModule() = default;

	static std::vector<char> GetShaderData(const std::string& _path)
	{
		std::ifstream file(_path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			return std::vector<char>();

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	virtual void CreateShaderModule(IDevice* _device, const std::vector<char>& _data) = 0;
	virtual void DestroyShaderModule(IDevice* _device) = 0;

	virtual VKShaderModule* const CastToVK() { return nullptr; }
	virtual DX12ShaderModule* CastToDX12() { return nullptr; }
	virtual OGLShaderModule* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_ISHADERMODULE_H
