#pragma once

#include "ResourceConverter/IResourceConverter.h"
#include "Shaders/IShaders.h"

#include "ShaderIncludeProvider.h"

#include "DXCCommon.h"

#define VT_RESOURCE_CONVERTER_HLSL

namespace VT_SHADER_RC
{
	class ShaderConverterHLSL final : public VT::IFileResourceConverter
	{
	public:
		enum class OutputILType
		{
			DXIL,
			SPIR_V
		};

	private:
		ComPtr<IDxcCompiler3> m_dxcShaderCompiler = nullptr;
		ComPtr<IDxcUtils> m_dxcUtils = nullptr;
		ComPtr<IDxcIncludeHandler> m_defaultShaderIncludeHandler = nullptr;

		OutputILType m_ilType;

		LPWSTR getEntryPoint(VT::ShaderStageType stage) const;
		LPWSTR getTarget(VT::ShaderStageType stage) const;

		const char* getTargertResourceFileExtension() const;

		virtual void compileShader(void* inData, size_t inDataSize,
			void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args, ShaderIncludeProvider* includeProvider);

	public:
		ShaderConverterHLSL(OutputILType outputType)
			: m_ilType(outputType) {}

		virtual bool init() override;
		virtual void release() override;

		virtual void convert(void* inData, size_t inDataSize,
			void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args) override;
		virtual bool convert(void** inData, size_t& inDataSize,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;
		virtual void convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args) override;
		virtual bool convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;
		virtual void convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			void** outData, size_t& outDataSize,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;

		virtual const char* getFileExtensionPattern() const override { return "*.hlsl"; }
		virtual const char* getTargetFileExtension() const { return getTargertResourceFileExtension(); }

		ComPtr<IDxcUtils> getDxcUtils() const { return m_dxcUtils; }
		ComPtr<IDxcIncludeHandler> getDefaultShaderIncludeHandler() const { return m_defaultShaderIncludeHandler; }

		virtual VT::ResourceConverterVersion getVersion() const;

		VT_RESOURCE_CONVERTER_TYPE(VT_RESOURCE_CONVERTER_HLSL)
	};
}