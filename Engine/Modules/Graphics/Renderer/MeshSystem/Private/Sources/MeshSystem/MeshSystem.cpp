#include "MeshSystem.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "ResourceSystem/IResourceSystem.h"

#include <cassert>

#include "MeshLoader.h"

namespace VT
{
	inline IResourceSystem* getResourceSystem()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_resourceSystem;
	}
}

void VT::MeshSystem::deleteMesh(ManagedMesh* mesh)
{
	m_meshes.removeElement(mesh->getID());
}

void VT::MeshSystem::deleteMeshHandle(MeshHandleID id)
{
	assert(MeshHandlePool::Handle(id).m_handle.getElementType() == NAMELESS_ELEMENT_TYPE);
	m_handles.removeElement(id);
}

void VT::MeshSystem::deleteNamedMeshHandle(FileNameID nameID)
{
	m_namedHandles.removeElement(nameID);
}

bool VT::MeshSystem::init()
{
	VT_CHECK_INITIALIZATION(m_meshes.init(256, 1, 128));
	VT_CHECK_INITIALIZATION(m_handles.init(32, 0, 16));
	VT_CHECK_INITIALIZATION(m_namedHandles.init(256, 1, 128));

	return true;
}

void VT::MeshSystem::release()
{
	m_handles.release();
	m_namedHandles.release();

	m_meshes.release();
}

VT::MeshReference VT::MeshSystem::createMesh(const MeshVertexData& vertData)
{
	MeshPool::NewElementInfo meshInfo = m_meshes.addElementRaw();
	ManagedMesh* mesh = new (meshInfo.m_elementPtr) ManagedMesh(vertData, meshInfo.m_elementHandle.getKey());

	MeshHandlePool::NewElementInfo handleInfo = m_handles.addElementRaw();
	ManagedMeshHandle* meshHandle = new (handleInfo.m_elementPtr) ManagedMeshHandle(mesh, handleInfo.m_elementHandle.getKey());

	return meshHandle;
}

VT::MeshReference VT::MeshSystem::createMesh(const MeshVertexData& vertData, const MeshIndexData& indexData)
{
	MeshPool::NewElementInfo meshInfo = m_meshes.addElementRaw();
	ManagedMesh* mesh = new (meshInfo.m_elementPtr) ManagedMesh(vertData, indexData, meshInfo.m_elementHandle.getKey());

	MeshHandlePool::NewElementInfo handleInfo = m_handles.addElementRaw();
	ManagedMeshHandle* meshHandle = new (handleInfo.m_elementPtr) ManagedMeshHandle(mesh, handleInfo.m_elementHandle.getKey());

	return meshHandle;
}

VT::MeshReference VT::MeshSystem::getMesh(MeshHandleID id)
{
	NamedMeshHandlePool::ElementPoolHandleType poolHandle = id;
	if (poolHandle.m_handle.getElementType() == NAMELESS_ELEMENT_TYPE)
	{
		return m_handles.getElementCast<MeshReference>(id);
	}

	return m_namedHandles.getElement(poolHandle);
}

VT::MeshReference VT::MeshSystem::getNamedMesh(const MeshName& name)
{
	return m_namedHandles.getElement(name);
}

VT::MeshReference VT::MeshSystem::getNamedMesh(MeshNameID id)
{
	return m_namedHandles.getElement(id);
}

VT::MeshReference VT::MeshSystem::loadMesh(const MeshName& meshPath)
{
	MeshNameID nameID = meshPath.hash();
	NamedMeshHandlePool::ElementAccessInfo meshAccessor = m_namedHandles.getOrAddElement(nameID);
	if (meshAccessor.m_isNew)
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		ResourceDataReference resourceData = resourceSystem->getResource(meshPath);

		if (resourceData && resourceData->getState() != ResourceState::INVALID)
		{
			const void* data = resourceData->getData();
			size_t dataSize = resourceData->getDataSize();

			if (data && dataSize > 0)
			{
				MeshVertexData vertexData;
				MeshIndexData indexData;

				MeshLoader::loadMesh(data, dataSize, vertexData, indexData);

				MeshPool::NewElementInfo meshInfo = m_meshes.addElementRaw();
				ManagedMesh* mesh = new (meshInfo.m_elementPtr) ManagedMesh(vertexData, indexData, meshInfo.m_elementHandle.getKey());

				meshAccessor.m_element.getObjectCast<ManagedMeshHandle>()->setMesh(mesh);
			}
		}
	}

	return meshAccessor.m_element;
}

VT::MeshReference VT::MeshSystem::loadMeshAsync(const MeshName& name, OnLoadedMeshCallback callback)
{
	NamedMeshHandlePool::ElementAccessInfo meshAccessor = m_namedHandles.getOrAddElement(name);
	if (!meshAccessor.m_isNew)
	{
		if (callback)
		{
			callback(meshAccessor.m_element);
		}
	}
	else
	{
		IResourceSystem* resourceSystem = getResourceSystem();
		assert(resourceSystem);

		resourceSystem->getResourceAsync(name,
			[meshHandleReference = meshAccessor.m_element, onLoadedCallback = callback, &meshPool = m_meshes](const ResourceDataReference& resourceData)
			{
				if (resourceData && resourceData->getState() != ResourceState::INVALID)
				{
					const void* data = resourceData->getData();
					size_t dataSize = resourceData->getDataSize();

					if (data && dataSize > 0)
					{
						MeshVertexData vertexData;
						MeshIndexData indexData;

						MeshLoader::loadMesh(data, dataSize, vertexData, indexData);

						MeshPool::NewElementInfo meshInfo = meshPool.addElementRaw();
						ManagedMesh* mesh = new (meshInfo.m_elementPtr) ManagedMesh(vertexData, indexData, meshInfo.m_elementHandle.getKey());

						meshHandleReference.getObjectCast<ManagedMeshHandle>()->setMesh(mesh);
					}
				}

				if (onLoadedCallback)
				{
					onLoadedCallback(meshHandleReference);
				}
			}
		);
	}

	return meshAccessor.m_element;
}
