#include "GBuffer.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "Textures/ITexture2D.h"

#include "GraphRender/RenderPassEnvironment.h"

bool VT::GBuffer::init(const Vector2UInt16& bufferResolution)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	IGraphicDevice* device = environment->m_graphicDevice;

	Texture2DDesc bufferTextureDesc{};
	bufferTextureDesc.m_format = Format::R32G32B32A32_SFLOAT;
	bufferTextureDesc.m_width = bufferResolution.m_x;
	bufferTextureDesc.m_height = bufferResolution.m_y;
	bufferTextureDesc.m_usage = TEXTURE_USAGE_RENDER_TARGET | GRAPHIC_USAGE_SHADER_RESOURCE;

	m_color = resManager->createTexture2D(bufferTextureDesc, TEXTURE_STATE_RENDER_TARGET);
	m_normal = resManager->createTexture2D(bufferTextureDesc, TEXTURE_STATE_RENDER_TARGET);
	m_position = resManager->createTexture2D(bufferTextureDesc, TEXTURE_STATE_RENDER_TARGET);

	VT_CHECK_RETURN_FALSE(m_color && m_normal && m_position);

	bufferTextureDesc.m_format = Format::D24_UNORM_S8_UINT;
	bufferTextureDesc.m_usage = TEXTURE_USAGE_DEPTH_STENCIL | GRAPHIC_USAGE_SHADER_RESOURCE;
	m_depth = resManager->createTexture2D(bufferTextureDesc, TEXTURE_STATE_DEPTH_STENCIL);

	VT_CHECK_RETURN_FALSE(m_depth);

	m_colorRTV = resManager->createRenderTargetDescriptor(m_color.getObject());
	m_normalRTV = resManager->createRenderTargetDescriptor(m_normal.getObject());
	m_positionRTV = resManager->createRenderTargetDescriptor(m_position.getObject());
	m_depthDSV = resManager->createDepthStencilDescriptor(m_depth.getObject());

	VT_CHECK_RETURN_FALSE(m_colorRTV && m_normalRTV && m_positionRTV && m_depthDSV);

	m_colorSRV = resManager->createShaderResourceDescriptor(m_color.getObject());
	m_normalSRV = resManager->createShaderResourceDescriptor(m_normal.getObject());
	m_positionSRV = resManager->createShaderResourceDescriptor(m_position.getObject());
	m_depthSRV = resManager->createShaderResourceDescriptor(m_depth.getObject());

	VT_CHECK_RETURN_FALSE(m_colorSRV && m_normalSRV && m_positionSRV && m_depthSRV);

	device->setResourceName(m_color->getResource(), "gb_color");
	device->setResourceName(m_normal->getResource(), "gb_normal");
	device->setResourceName(m_position->getResource(), "gb_position");
	device->setResourceName(m_depth->getResource(), "gb_depth");

	return true;
}

void VT::GBuffer::release()
{
	m_color = nullptr;
	m_normal = nullptr;
	m_position = nullptr;
	m_depth = nullptr;

	m_colorRTV = nullptr;
	m_normalRTV = nullptr;
	m_positionRTV = nullptr;
	m_depthDSV = nullptr;

	m_colorSRV = nullptr;
	m_normalSRV = nullptr;
	m_positionSRV = nullptr;
	m_depthSRV = nullptr;
}

void VT::GBuffer::fillEnvironment(RenderPassEnvironment& environment) const
{
	environment.addTexture("gb_color_texture", m_color.getObject());
	environment.addTexture("gb_normal_texture", m_normal.getObject());
	environment.addTexture("gb_position_texture", m_position.getObject());
	environment.addTexture("gb_depth_texture", m_depth.getObject());

	environment.addRenderTargetView("gb_color_rtv", m_colorRTV);
	environment.addRenderTargetView("gb_normal_rtv", m_normalRTV);
	environment.addRenderTargetView("gb_position_rtv", m_positionRTV);
	environment.addDepthStencilView("gb_depth_dsv", m_depthDSV);

	environment.addShaderResourceView("gb_color_srv", m_colorSRV);
	environment.addShaderResourceView("gb_normal_srv", m_normalSRV);
	environment.addShaderResourceView("gb_position_srv", m_positionSRV);
	environment.addShaderResourceView("gb_depth_srv", m_depthSRV);
}
