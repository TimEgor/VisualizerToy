#include "D3D12GraphicsPlugin.h"

#include "Platform/D3D12GraphicPlatform.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

VT_PLUGIN(VT_D3D12::D3D12GraphicPlugin)

void VT_D3D12::D3D12GraphicPlugin::onLoaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "D3D12GraphicPlugin::onLoaded() : Engine environment is invalid.");
		return;
	}

	if (environment->m_graphicPlatform)
	{
		assert(false && "D3D12GraphicPlugin::onLoaded() : Graphic platform has been already initialized.");
		return;
	}

	environment->m_graphicPlatform = new D3D12GraphicPlatform();
}

void VT_D3D12::D3D12GraphicPlugin::onUnloaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "D3D12GraphicPlugin::onUnloaded() : Engine environment is invalid.");
		return;
	}

	VT::IGraphicPlatform* graphicPlatform = environment->m_graphicPlatform;

	if (!graphicPlatform)
	{
		assert(false && "D3D12GraphicPlugin::onUnloaded() : Engine environment graphicPlatform is null.");
		return;
	}
	
	if(graphicPlatform->getType() != D3D12GraphicPlatform::getGraphicPlatformType())
	{
		assert(false && "D3D12GraphicPlugin::onUnloaded() : Engine environment graphicPlatform isn't D3D12.");
		return;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(graphicPlatform);
}
