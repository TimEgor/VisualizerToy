#pragma once

#include "DXCCommon.h"

#include "FileSystem/IFileSystem.h"
#include "Core/FileName/FileName.h"

namespace VT_SHADER_RC
{
	class ShaderConverterHLSL;

	class ShaderIncludeProvider final : public IDxcIncludeHandler
	{
	private:
		const VT::IFileSystem& m_fileSystem;
		const VT::FileName& m_shaderPath;
		const ShaderConverterHLSL& m_converter;

		VT::FileName resolveAssetPath(const char* includePath);

	public:
		ShaderIncludeProvider(const VT::IFileSystem& fileSystem, const VT::FileName& shaderPath, const ShaderConverterHLSL& converter)
			: m_fileSystem(fileSystem), m_shaderPath(shaderPath), m_converter(converter) {}

		virtual HRESULT STDMETHODCALLTYPE LoadSource(_In_z_ LPCWSTR pFilename, _COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource) override;

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
		virtual ULONG STDMETHODCALLTYPE AddRef(void) override { return 0; }
		virtual ULONG STDMETHODCALLTYPE Release(void) override { return 0; }
	};
}