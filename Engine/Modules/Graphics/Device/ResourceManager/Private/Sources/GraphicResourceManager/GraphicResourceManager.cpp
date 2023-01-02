#include "GraphicResourceManager.h"

bool VT::GraphicResourceManager::init()
{
	VT_CHECK_INITIALIZATION(m_buffers.init(256, 1, 128));

	VT_CHECK_INITIALIZATION(m_textures2D.init(256, 1, 64))
	VT_CHECK_INITIALIZATION(m_texture2DViews.init(256, 1, 64))

	VT_CHECK_INITIALIZATION(m_pixelShaders.init(16, 0, 8))
	VT_CHECK_INITIALIZATION(m_vertexShaders.init(16, 0, 8))

	VT_CHECK_INITIALIZATION(m_pipelineStateCollection.init(128, 1, 64))

	VT_CHECK_INITIALIZATION(m_inputLayoutCollection.init(16, 0, 8))

	return true;
}

void VT::GraphicResourceManager::release()
{
	m_buffers.release();

	m_textures2D.release();
	m_texture2DViews.release();

	m_vertexShaders.release();
	m_pixelShaders.release();

	m_pipelineStateCollection.release();
	m_inputLayoutCollection.release();
}