#include "VulkanSwapChain.h"

#include "VulkanGraphicsPlugin/Utilities/VulkanEnvironmentGraphicPlatform.h"
#include "VulkanGraphicsPlugin/Device/VulkanGraphicDevice.h"

uint32_t VT_VK::VulkanSwapChain::getTextureCount() const
{
	return m_textureCount;
}

const VT::ITexture2D* VT_VK::VulkanSwapChain::getTexture(uint32_t index) const
{
	assert(m_textures);
	return &m_textures[index];
}

const VT::ITexture2DView* VT_VK::VulkanSwapChain::getTargetView(uint32_t index) const
{
	assert(m_targetViews);
	return &m_targetViews[index];
}
