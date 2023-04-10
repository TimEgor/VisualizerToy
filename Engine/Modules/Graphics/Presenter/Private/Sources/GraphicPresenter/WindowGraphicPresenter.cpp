#include "WindowGraphicPresenter.h"

#include "Core/Output.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "SwapChain/ISwapChain.h"

void VT::WindowGraphicPresenter::onWindowSizeEvent(const WindowSizeEvent& event)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	assert(environment && environment->m_graphicDevice);

	environment->m_graphicDevice->resizeSwapChain(m_swapChain, event.m_window->getClientAreaSize());
}

bool VT::WindowGraphicPresenter::init(const IWindow* window, const SwapChainDesc& swapChainDesc)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_FALSE_ASSERT(environment && environment->m_graphicDevice);

	m_swapChain = environment->m_graphicDevice->createSwapChain(swapChainDesc, window);
	VT_CHECK_RETURN_FALSE_ASSERT_MSG(m_swapChain, "Swap chain hasn't been created.");

	m_windowSizeEventDispatcher = environment->m_eventSystem->addInstanceEventDispatcher(window->getWindowSizeEventID(),
		[this](const IInstancedEvent& event) { onWindowSizeEvent(reinterpret_cast<const WindowSizeEvent&>(event)); }
	);
	VT_CHECK_INITIALIZATION(m_windowSizeEventDispatcher != InvalidEventDispatcherID);

	return true;
}

void VT::WindowGraphicPresenter::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_ASSERT(environment && environment->m_windowSystem && environment->m_graphicResourceManager);

	if (m_windowSizeEventDispatcher != InvalidInstancedEventDispatcherID)
	{
		environment->m_eventSystem->removeInstancedEventDispatcher(m_windowSizeEventDispatcher);
		m_windowSizeEventDispatcher = InvalidInstancedEventDispatcherID;
	}

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
