#include "Scene.h"

#include "Math/ComputeMatrix.h"
#include "Multithreading/LockGuard.h"

bool VT::Scene::init()
{
	UniqueLockGuard locker(m_lockMutex);

	VT_CHECK_INITIALIZATION(m_nodeInfos.init(512, 1, 256));
	VT_CHECK_INITIALIZATION(m_nodeTransforms.init(512, 1, 256));

	m_dirtyNodeLevels = DirtyLevelCollection(16);

	return true;
}

void VT::Scene::release()
{
	UniqueLockGuard locker(m_lockMutex);

	m_nodeInfos.release();
	m_nodeTransforms.release();

	m_dirtyNodeLevels = DirtyLevelCollection();
}

VT::NodeID VT::Scene::addNodeInfo(NodeInfo* parentNodeInfo, NodeID parentNodeID)
{
	NodeInfoPool::NewElementInfo newNodeElemInfo = m_nodeInfos.addElement();
	NodeID newNodeID = newNodeElemInfo.m_elementHandle.getKey();
	NodeInfo* newNodeInfo = newNodeElemInfo.m_elementPtr;

	if (parentNodeInfo)
	{
		if (parentNodeInfo->m_firstChildNodeID != InvalidNodeID)
		{
			parentNodeInfo->m_firstChildNodeID = newNodeID;
		}
		else
		{
			NodeInfo* firstChildNodeInfo = m_nodeInfos.getElement(parentNodeInfo->m_firstChildNodeID);
			if (!firstChildNodeInfo)
			{
				assert(false && "First parent child node is invalid.");
				m_nodeInfos.removeElement(newNodeID);
				return InvalidNodeID;
			}

			NodeInfo* lastChildNodeInfo = m_nodeInfos.getElement(firstChildNodeInfo->m_lastSiblingNodeID);
			if (!lastChildNodeInfo)
			{
				assert(false && "Last sibling node is invalid.");
				m_nodeInfos.removeElement(newNodeID);
				return InvalidNodeID;
			}

			firstChildNodeInfo->m_lastSiblingNodeID = newNodeID;
			lastChildNodeInfo->m_nextSiblingNodeID = newNodeID;
		}

		newNodeInfo->m_level = ++parentNodeInfo->m_level;
	}
	else
	{
		newNodeInfo->m_level = 0;
	}

	newNodeInfo->m_parentNodeID = parentNodeID;

	return newNodeID;
}

void VT::Scene::collectRemovingNodes(NodeID nodeID, RemovingNodeCollection& nodes) const
{
	const NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeID);
	if (!nodeInfo)
	{
		return;
	}

	nodes.push_back(nodeID);

	for (NodeID childNodeID = nodeInfo->m_firstChildNodeID; childNodeID != InvalidNodeID; childNodeID = nodeInfo->m_nextSiblingNodeID)
	{
		collectRemovingNodes(childNodeID, nodes);
	}
}

void VT::Scene::markDirtyNode(NodeID nodeID)
{
	NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeID);
	if (!nodeInfo)
	{
		return;
	}

	if (m_dirtyNodeLevels.size() < nodeInfo->m_level + 1)
	{
		m_dirtyNodeLevels.resize(nodeInfo->m_level + 1);
	}

	m_dirtyNodeLevels[nodeInfo->m_level].pushBack(nodeID);

	for (NodeID childNodeID = nodeInfo->m_firstChildNodeID; childNodeID != InvalidNodeID; childNodeID = nodeInfo->m_nextSiblingNodeID)
	{
		markDirtyNode(childNodeID);
	}
}

VT::NodeID VT::Scene::addNode(NodeID parentNodeID)
{
	UniqueLockGuard locker(m_lockMutex);

	NodeInfo* parentNodeInfo = nullptr;
	if (parentNodeID != InvalidNodeID)
	{
		parentNodeInfo = m_nodeInfos.getElement(parentNodeID);
		if (!parentNodeInfo)
		{
			return InvalidNodeID;
		}
	}

	NodeID newNodeID = addNodeInfo(parentNodeInfo, parentNodeID);
	if (newNodeID != InvalidNodeID)
	{
		NodeTransformPool::NewElementInfo newTransformElementInfo = m_nodeTransforms.addElement();
		NodeID newTransformID = newTransformElementInfo.m_elementHandle.getKey();
		NodeTransforms* newTransforms = newTransformElementInfo.m_elementPtr;

		if (newTransformID != newNodeID)
		{
			assert(false && "The new node info and transform IDs aren't same.");

			m_nodeInfos.removeElement(newNodeID);
			m_nodeTransforms.removeElement(newTransformID);
			return InvalidNodeID;
		}

		markDirtyNode(newNodeID);
	}

	return newNodeID;
}

void VT::Scene::removeNode(NodeID nodeID, const DeleteNodeCallback& callback)
{
	std::vector<NodeID> deletingNodes;
	deletingNodes.reserve(32);

	collectRemovingNodes(nodeID, deletingNodes); // TODO: probably need to replaced with cycle

	for (const NodeID id : deletingNodes)
	{
		m_nodeInfos.removeElement(id);
		m_nodeTransforms.removeElement(id);

		if (callback)
		{
			callback(id);
		}
	}
}

VT::NodeID VT::Scene::getParentNodeID(NodeID nodeID) const
{
	SharedLockGuard locker(m_lockMutex);

	const NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeID);
	if (!nodeInfo)
	{
		return InvalidNodeID;
	}

	return nodeInfo->m_parentNodeID;
}

VT::NodeID VT::Scene::getFirstChildNodeID(NodeID parentNodeID) const
{
	SharedLockGuard locker(m_lockMutex);

	const NodeInfo* parentNodeInfo = m_nodeInfos.getElement(parentNodeID);
	if (!parentNodeInfo)
	{
		return InvalidNodeID;
	}

	return parentNodeInfo->m_firstChildNodeID;
}

VT::NodeID VT::Scene::getNextSiblingNodeID(NodeID nodeID) const
{
	SharedLockGuard locker(m_lockMutex);

	const NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeID);
	if (!nodeInfo)
	{
		return InvalidNodeID;
	}

	return nodeInfo->m_nextSiblingNodeID;
}

VT::NodeTransforms* VT::Scene::getNodeTransforms(NodeID nodeID)
{
	SharedLockGuard locker(m_lockMutex);

	return m_nodeTransforms.getElement(nodeID);
}

const VT::NodeTransforms* VT::Scene::getNodeTransforms(NodeID nodeID) const
{
	SharedLockGuard locker(m_lockMutex);

	return m_nodeTransforms.getElement(nodeID);
}

void VT::Scene::recalculateTransforms()
{
	if (m_dirtyNodeLevels.size() > 0)
	{
		for (const NodeID id : m_dirtyNodeLevels[0])
		{
			NodeTransforms* transforms = getNodeTransforms(id);
			if (!transforms)
			{
				continue;
			}

			transforms->m_globalTransform = transforms->m_localTransform;
		}
	}

	NodeLevelType levelsCount = static_cast<NodeLevelType>(m_dirtyNodeLevels.size());
	for (NodeLevelType level = 1; level < levelsCount; ++level)
	{
		LevelDirtyNodeCollection& dirtyNodeCollection = m_dirtyNodeLevels[level];
		for (const NodeID id : dirtyNodeCollection)
		{
			NodeTransforms* transforms = getNodeTransforms(id);
			if (!transforms)
			{
				continue;
			}

			const NodeID parentID = getParentNodeID(id);
			if (parentID == InvalidNodeID)
			{
				assert(false && "Invalid parent node ID.");
				continue;
			}

			NodeTransforms* parentTransforms = getNodeTransforms(parentID);
			if (!parentTransforms)
			{
				assert(false && "Invalid parent transform.");
				continue;
			}

			
			COMPUTE_MATH::ComputeMatrix parentGlobalTransform = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(parentTransforms->m_globalTransform);
			COMPUTE_MATH::ComputeMatrix localTransform = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms->m_localTransform);
			COMPUTE_MATH::ComputeMatrix globalTransform = COMPUTE_MATH::matrixMultiply(parentGlobalTransform, localTransform);

			transforms->m_globalTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(globalTransform);
		}
	}
}

void VT::Scene::markDirty(NodeID nodeID)
{
	UniqueLockGuard locker(m_lockMutex);

	markDirtyNode(nodeID); // TODO: probably need to replaced with cycle
}
