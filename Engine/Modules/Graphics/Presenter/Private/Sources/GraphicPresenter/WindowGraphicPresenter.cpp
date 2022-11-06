#include "WindowGraphicPresenter.h"

#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"

#include "WindowSystem/IWindowSystem.h"

bool VT::WindowGraphicPresenter::init(const char* windowTitle, const WindowSize& windowSize, const SwapChainDesc& swapChainDesc)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_FALSE_ASSERT(environment && environment->m_windowSystem && environment->m_graphicDevice);

	IWindowContainer::NewWindowInfo windowInfo = environment->m_windowSystem->createWindow(windowTitle, windowSize);
	VT_CHECK_RETURN_FALSE_ASSERT(windowInfo.m_windowHandle != InvalidWindowHandle && windowInfo.m_windowPtr);
	m_windowHandle = windowInfo.m_windowHandle;

	SwapChainContainer::NewResourceInfo newSwapCahinInfo = environment->m_graphicResourceManager->createSwapChain(swapChainDesc, windowInfo.m_windowPtr);
	m_swapChainHandle = newSwapCahinInfo.m_resourceHandle;

	VT_CHECK_RETURN_FALSE_ASSERT_MSG(newSwapCahinInfo.m_resourcePtr, "Swap chain hasn't been created.");

	return true;
}

void VT::WindowGraphicPresenter::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	VT_CHECK_RETURN_ASSERT(environment && environment->m_windowSystem && environment->m_graphicResourceManager);

	environment->m_windowSystem->destroyWindow(m_windowHandle);
	environment->m_graphicResourceManager->deleteSwapChain(m_swapChainHandle);

	m_windowHandle = 0;
	m_swapChainHandle = 0;
}

uint32_t VT::WindowGraphicPresenter::getFrameCount() const
{
	return uint32_t();
}

VT::ITexture2D* VT::WindowGraphicPresenter::getTargetTexture(uint32_t frameIndex)
{
	return nullptr;
}
