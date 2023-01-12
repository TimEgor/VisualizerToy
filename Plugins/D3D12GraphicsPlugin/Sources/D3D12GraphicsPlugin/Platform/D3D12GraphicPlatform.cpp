#include "D3D12GraphicPlatform.h"

#include "Core/UtilitiesMacros.h"

#include "D3D12GraphicsPlugin/Device/D3D12GraphicDevice.h"
#include "D3D12GraphicsPlugin/Render/D3D12RenderContext.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

bool VT_D3D12::D3D12GraphicPlatform::init(bool isSwapChainEnabled)
{
	return true;
}

void VT_D3D12::D3D12GraphicPlatform::release()
{
	VT::IGraphicDevice* graphicDevie = VT::EngineInstance::getInstance()->getEnvironment()->m_graphicDevice;

	if (!graphicDevie)
	{
		assert(false && "D3D12GraphicPlatform::release() : Engine environment graphicDevie is null.");
		return;
	}

	if (graphicDevie->getType() != D3D12GraphicDevice::getGraphicDeviceType())
	{
		assert(false && "D3D12GraphicPlatform::release() : Engine environment graphicDevie isn't Vulkan.");
		return;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(graphicDevie);
}

VT::IGraphicDevice* VT_D3D12::D3D12GraphicPlatform::createGraphicDevice()
{
	return new D3D12GraphicDevice();
}

VT::IRenderContext* VT_D3D12::D3D12GraphicPlatform::createRenderContext()
{
	return new D3D12RenderContext();
}
