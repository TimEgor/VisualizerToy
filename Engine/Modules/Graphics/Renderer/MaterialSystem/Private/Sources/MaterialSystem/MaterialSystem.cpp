#include "MaterialSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

VT::IMaterial* VT::MaterialSystem::createMaterialInternal(const PipelineStateDesc& desc)
{
	IGraphicResourceManager* resources = EngineInstance::getInstance()->getEnvironment()->m_graphicResourceManager;
	assert(resources);



	auto materialInfo = m_materials.addElementRaw();

	return nullptr;
}

void VT::MaterialSystem::deleteMaterial(MaterialID id)
{
}

void VT::MaterialSystem::deleteMaterialHandle(MaterialHandleID id)
{
}

void VT::MaterialSystem::deleteNamedMaterialHandle(MaterialNameID id)
{
}

bool VT::MaterialSystem::init()
{
	VT_CHECK_INITIALIZATION(m_materials.init(256, 1, 64));
	VT_CHECK_INITIALIZATION(m_handles.init(16, 0, 8));
	VT_CHECK_INITIALIZATION(m_namedHandles.init(256, 1, 64));

	return true;
}

void VT::MaterialSystem::release()
{
	m_handles.release();
	m_namedHandles.release();

	m_materials.release();
}

VT::MaterialReference VT::MaterialSystem::createMaterial(const PipelineStateDesc& desc)
{
	return {};
}

VT::MaterialReference VT::MaterialSystem::getMaterial(MaterialID id)
{
	return {};
}

VT::MaterialReference VT::MaterialSystem::getNamedMaterial(const MaterialName& name)
{
	return {};
}

VT::MaterialReference VT::MaterialSystem::getNamedMaterial(MaterialNameID id)
{
	return {};
}

VT::MaterialReference VT::MaterialSystem::loadMaterial(const FileName& name)
{
	return {};
}

VT::MaterialReference VT::MaterialSystem::loadMaterialAsync(const FileName& name)
{
	return {};
}
