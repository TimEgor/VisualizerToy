#include "ShaderConverterHLSL.h"

#include "ShaderConverter/ShaderConverterArgs.h"
#include "Core/FileName/FileNameUtils.h"
#include "Core/Output.h"

#include <vector>

LPWSTR VT_SHADER_RC::ShaderConverterHLSL::getEntryPoint(VT::ShaderStageType stage) const
{
	switch (stage)
	{
	case VT::ShaderStageType::Vertex:
		return L"VS";
	case VT::ShaderStageType::Pixel:
		return L"PS";
	default:
		return L"";
	}
}

LPWSTR VT_SHADER_RC::ShaderConverterHLSL::getTarget(VT::ShaderStageType stage) const
{
	switch (stage)
	{
	case VT::ShaderStageType::Vertex:
		return L"vs_6_6";
	case VT::ShaderStageType::Pixel:
		return L"ps_6_6";
	default:
		return L"";
	}
}

const char* VT_SHADER_RC::ShaderConverterHLSL::getTargertResourceFileExtension() const
{
	switch (m_ilType)
	{
	case VT_SHADER_RC::ShaderConverterHLSL::OutputILType::DXIL:
		return ".dxil";
	case VT_SHADER_RC::ShaderConverterHLSL::OutputILType::SPIR_V:
		return ".spir_v";
	default:
		return "";
	}
}

void VT_SHADER_RC::ShaderConverterHLSL::compileShader(void* inData, size_t inDataSize,
	void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args, ShaderIncludeProvider* includeProvider)
{
	if (!args || args->getType() != VT::ShaderResourceConverterArgs::getResourceConverterArgsType())
	{
		return;
	}

	const VT::ShaderResourceConverterArgs* shaderArgs = reinterpret_cast<const VT::ShaderResourceConverterArgs*>(args);

	ComPtr<IDxcBlob> shaderBlob = nullptr;

	HRESULT compilationResult;
	ComPtr<IDxcResult> compilationDxcResult;
	DxcBuffer source{ inData, inDataSize, CP_UTF8 };

	std::vector<LPWSTR> dxcArguments;

	dxcArguments.push_back(L"-E");
	dxcArguments.push_back(getEntryPoint(shaderArgs->getStageType()));

	dxcArguments.push_back(L"-T");
	dxcArguments.push_back(getTarget(shaderArgs->getStageType()));

	if (m_ilType == OutputILType::SPIR_V)
	{
		dxcArguments.push_back(L"-spirv");
		dxcArguments.push_back(L"-D"); dxcArguments.push_back(L"SPIRV");
	}

	compilationResult = m_dxcShaderCompiler->Compile(&source, const_cast<LPCWSTR*>(dxcArguments.data()), static_cast<uint32_t>(dxcArguments.size()),
		includeProvider, IID_COM(compilationDxcResult), PPV_COM(compilationDxcResult));

	ComPtr<IDxcBlobEncoding> errorBlob = nullptr;
	compilationDxcResult->GetOutput(DXC_OUT_ERRORS, IID_COM(errorBlob), PPV_COM(errorBlob), nullptr);

	if (FAILED(compilationResult) || (errorBlob && errorBlob->GetBufferSize() > 0))
	{
		if (errorBlob)
		{
			VT_ENV_CONSOLE_OUTPUT(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}

		*outData = nullptr;
		outDataSize = 0;

		return;
	}

	compilationDxcResult->GetResult(shaderBlob.GetAddressOf());

	// TODO: need to remove this mem coping
	outDataSize = shaderBlob->GetBufferSize();
	*outData = new uint8_t[outDataSize];
	memcpy(*outData, shaderBlob->GetBufferPointer(), outDataSize);
}

bool VT_SHADER_RC::ShaderConverterHLSL::init()
{
	DxcCreateInstance(CLSID_DxcCompiler, IID_COM(m_dxcShaderCompiler), PPV_COM(m_dxcShaderCompiler));
	DxcCreateInstance(CLSID_DxcUtils, IID_COM(m_dxcUtils), PPV_COM(m_dxcUtils));

	return true;
}

void VT_SHADER_RC::ShaderConverterHLSL::release()
{}

void VT_SHADER_RC::ShaderConverterHLSL::convert(void* inData, size_t inDataSize,
	void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args)
{
	compileShader(inData, inDataSize, outData, outDataSize, args, nullptr);
}

bool VT_SHADER_RC::ShaderConverterHLSL::convert(void** inData, size_t& inDataSize, VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args)
{
	void* outData = nullptr;
	size_t outDataSize = 0;
	compileShader(inData, inDataSize, &outData, outDataSize, args, nullptr);

	if (outData && outDataSize > 0)
	{
		return outFileSystem.writeResource(outFilePath, outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
	}

	return false;
}

void VT_SHADER_RC::ShaderConverterHLSL::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args)
{
	size_t inDataSize = 0;
	void* inData = inFileSystem.readResourceBinaryData(inFilePath, inDataSize);

	if (inData && inDataSize > 0)
	{
		compileShader(inData, inDataSize, outData, outDataSize, args, nullptr);
	}
	else
	{
		*outData = nullptr;
		outDataSize = 0;
	}
}

bool VT_SHADER_RC::ShaderConverterHLSL::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args)
{
	size_t inDataSize = 0;
	void* inData = inFileSystem.readResourceBinaryData(inFilePath, inDataSize);

	void* outData = nullptr;
	size_t outDataSize = 0;
	if (inData && inDataSize > 0)
	{
		compileShader(inData, inDataSize, &outData, outDataSize, args, nullptr);

		if (outData && outDataSize > 0)
		{
			return outFileSystem.writeResource(outFilePath, outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
		}
	}

	return false;
}

void VT_SHADER_RC::ShaderConverterHLSL::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath, void** outData, size_t& outDataSize, VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args)
{
	size_t inDataSize = 0;
	void* inData = inFileSystem.readResourceBinaryData(inFilePath, inDataSize);

	if (inData && inDataSize > 0)
	{
		compileShader(inData, inDataSize, outData, outDataSize, args, nullptr);

		if (outData && *outData && outDataSize > 0)
		{
			outFileSystem.writeResource(outFilePath, *outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
		}
	}
}

VT::ResourceConverterVersion VT_SHADER_RC::ShaderConverterHLSL::getVersion() const
{
	return 2022'12'09;
}
