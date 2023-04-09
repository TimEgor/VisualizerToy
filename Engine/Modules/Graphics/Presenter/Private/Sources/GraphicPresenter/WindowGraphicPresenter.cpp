#include "WindowGraphicPresenter.h"

#include "Core/Output.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "SwapChain/ISwapChain.h"

bool VT::WindowGraphicPresenter::init(const IWindow* window, const SwapChainDesc& swapChainDesc)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_FALSE_ASSERT(environment && environment->m_graphicDevice);

	m_swapChain = environment->m_graphicDevice->createSwapChain(swapChainDesc, window);
	VT_CHECK_RETURN_FALSE_ASSERT_MSG(m_swapChain, "Swap chain hasn't been created.");

	return true;
}

void VT::WindowGraphicPresenter::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_ASSERT(environment && environment->m_windowSystem && environment->m_graphicResourceManager);

	if (m_swapChain)
	{
		environment->m_graphicDevice->destroySwapChain(m_swapChain);
		m_swapChain = nullptr;
	}
}

void VT::WindowGraphicPresenter::updateNextTargetTextureIndex()
{
	assert(m_swapChain);
	m_swapChain->updateCurrentTextureIndex();
}

uint32_t VT::WindowGraphicPresenter::getCurrentTargetTextureIndex() const
{
	assert(m_swapChain);
	return m_swapChain->getCurrentTextureIndex();
}

uint32_t VT::WindowGraphicPresenter::getFrameCount() const
{
	assert(m_swapChain);
	return m_swapChain->getTextureCount();
}

VT::ITexture2D* VT::WindowGraphicPresenter::getTargetTexture(uint32_t frameIndex)
{
	assert(m_swapChain);
	return m_swapChain->getTargetTexture(frameIndex);
}

VT::IGraphicResourceDescriptor* VT::WindowGraphicPresenter::getTargetTextureView(uint32_t frameIndex)
{
	assert(m_swapChain);
	return m_swapChain->getTargetTextureView(frameIndex);
}

void VT::WindowGraphicPresenter::present()
{
	assert(m_swapChain);
	m_swapChain->present();
}
