#include "ShaderConverterHLSLPlugin.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicPlatform/IGraphicPlatform.h"

VT_PLUGIN(VT_SHADER_RC::ShaderConverterHLSLPlugin)

#define VT_GRAPHIC_PLARFORM_VULKAN_TYPE "VT_GRAPHIC_PLARFORM_VULKAN_TYPE"
#define VT_GRAPHIC_PLARFORM_D3D12_TYPE "VT_GRAPHIC_PLARFORM_D3D12_TYPE"

VT::IFileResourceConverter* VT_SHADER_RC::ShaderConverterHLSLPlugin::createConverter()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "Engine environment is invalid.");
		return nullptr;
	}

	VT::IGraphicPlatform* graphicPlatform = environment->m_graphicPlatform;

	if (!graphicPlatform)
	{
		assert(false && "Engine environment graphicPlatform is null.");
		return nullptr;
	}

	ShaderConverterHLSL::OutputILType ilType;

	VT::GraphicPlatformType platformType = graphicPlatform->getType();
	if (platformType == VT::IGraphicPlatform::calcType(VT_GRAPHIC_PLARFORM_VULKAN_TYPE))
	{
		ilType = ShaderConverterHLSL::OutputILType::SPIR_V;
	}
	else if (platformType == VT::IGraphicPlatform::calcType(VT_GRAPHIC_PLARFORM_D3D12_TYPE))
	{
		ilType = ShaderConverterHLSL::OutputILType::DXIL;
	}
	else
	{
		assert(false && "Graphic platform type is unknown.");
		return nullptr;
	}


	return createShaderConverterHLSL(ilType);
}

VT_SHADER_RC::ShaderConverterHLSL* VT_SHADER_RC::ShaderConverterHLSLPlugin::createShaderConverterHLSL(ShaderConverterHLSL::OutputILType ilType)
{
	return new ShaderConverterHLSL(ilType);
}
