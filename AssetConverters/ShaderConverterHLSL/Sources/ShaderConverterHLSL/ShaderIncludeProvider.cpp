#include "ShaderIncludeProvider.h"

#include "Core/String/StringConverter.h"
#include "ShaderConverterHLSL.h"

#include <filesystem>

VT::FileName VT_SHADER_RC::ShaderIncludeProvider::resolveAssetPath(const char* includePath)
{
	std::filesystem::path tmpPath((m_shaderPath + includePath).c_str());
	std::filesystem::path resultPath = std::filesystem::weakly_canonical(tmpPath);

	std::string result = resultPath.string();
	std::replace(result.begin(), result.end(), '\\', '/');

	return result;
}

HRESULT __stdcall VT_SHADER_RC::ShaderIncludeProvider::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
{
	std::string path = VT::convertWStringToString(pFilename);

	size_t dataSize = 0;
	void* data = m_fileSystem.readResourceBinaryData(resolveAssetPath(path.c_str()), dataSize);

	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	m_converter.getDxcUtils()->CreateBlob(data, dataSize, CP_UTF8, sourceBlob.GetAddressOf());

	*ppIncludeSource = sourceBlob.Detach();

	return S_OK;
}

HRESULT __stdcall VT_SHADER_RC::ShaderIncludeProvider::QueryInterface(REFIID riid, void** ppvObject)
{
	return m_converter.getDefaultShaderIncludeHandler()->QueryInterface(riid, ppvObject);
}