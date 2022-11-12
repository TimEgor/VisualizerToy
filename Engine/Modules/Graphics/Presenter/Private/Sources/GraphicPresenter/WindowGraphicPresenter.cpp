#include "WindowGraphicPresenter.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "WindowSystem/IWindowSystem.h"
#include "WindowSystem/IWindow.h"
#include "SwapChain/ISwapChain.h"

bool VT::WindowGraphicPresenter::init(const char* windowTitle, const WindowSize& windowSize, const SwapChainDesc& swapChainDesc)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_FALSE_ASSERT(environment && environment->m_windowSystem && environment->m_graphicDevice);

	m_window = environment->m_windowSystem->createWindow(windowTitle, windowSize);
	VT_CHECK_RETURN_FALSE_ASSERT(m_window);

	m_swapChain = environment->m_graphicDevice->createSwapChain(swapChainDesc, m_window);
	VT_CHECK_RETURN_FALSE_ASSERT_MSG(m_swapChain, "Swap chain hasn't been created.");

	return true;
}

void VT::WindowGraphicPresenter::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_ASSERT(environment && environment->m_windowSystem && environment->m_graphicResourceManager);

	environment->m_windowSystem->destroyWindow(m_window);
	environment->m_graphicDevice->destroySwapChain(m_swapChain);

	m_window = nullptr;
	m_swapChain = nullptr;
}

uint32_t VT::WindowGraphicPresenter::getFrameCount() const
{
	assert(m_swapChain);
	return m_swapChain->getTextureCount();
}

const VT::ITexture2D* VT::WindowGraphicPresenter::getTargetTexture(uint32_t frameIndex) const
{
	assert(m_swapChain);
	return m_swapChain->getTexture(frameIndex);
}
