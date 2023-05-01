#include "Scene.h"

#include "Math/ComputeMatrix.h"
#include "Multithreading/LockGuard.h"

bool VT::Scene::init()
{
	VT_CHECK_INITIALIZATION(m_nodeInfos.init(512, 1, 256));
	VT_CHECK_INITIALIZATION(m_nodeTransforms.init(512, 1, 256));

	m_dirtyNodeLevels = DirtyLevelCollection(16);

	return true;
}

void VT::Scene::release()
{
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

void VT::Scene::setDirtyState(NodeID nodeID, NodeInfo& nodeInfo, DirtyStateVariants state)
{
	if (state == DirtyStateVariants::PARENT_TRANSFORM &&
		nodeInfo.m_dirtyState & static_cast<DirtyState>(DirtyStateVariants::WORLD_TRANSFORM))
	{
		updateNodeTransform(nodeID, nodeInfo);
	}

	nodeInfo.m_dirtyState |= static_cast<DirtyState>(state);

	if (m_dirtyNodeLevels.size() < nodeInfo.m_level + 1)
	{
		m_dirtyNodeLevels.resize(nodeInfo.m_level + 1);
	}

	m_dirtyNodeLevels[nodeInfo.m_level].pushBack(nodeID);

	for (NodeID childNodeID = nodeInfo.m_firstChildNodeID; childNodeID != InvalidNodeID; childNodeID = nodeInfo.m_nextSiblingNodeID)
	{
		setDirtyState(childNodeID, DirtyStateVariants::PARENT_TRANSFORM);
	}
}

void VT::Scene::updateNodeTransform(NodeID nodeId)
{
	NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
	assert(nodeInfo);

	if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::NONE))
	{
		return;
	}

	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
}

void VT::Scene::updateNodeTransform(NodeID nodeId, NodeInfo& nodeInfo)
{
	if (nodeInfo.m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::NONE))
	{
		return;
	}

	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	updateNodeTransformInternal(*nodeTransforms, nodeInfo);
}

void VT::Scene::updateNodeTransform(NodeID nodeId, NodeTransforms& transforms)
{
	NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
	assert(nodeInfo);

	if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::NONE))
	{
		return;
	}

	updateNodeTransformInternal(transforms, *nodeInfo);
}

void VT::Scene::updateNodeTransformInternal(NodeTransforms& transforms, NodeInfo& nodeInfo)
{
	if (nodeInfo.m_level == 0)
	{
		updateRootNodeTransformInternal(nodeInfo.m_dirtyState, transforms);
	}
	else
	{
		if (nodeInfo.m_dirtyState & static_cast<DirtyState>(DirtyStateVariants::PARENT_TRANSFORM))
		{
			updateNodeTransform(nodeInfo.m_parentNodeID);
			nodeInfo.m_dirtyState &= ~static_cast<DirtyState>(DirtyStateVariants::PARENT_TRANSFORM);
		}

		updateChildNodeTransformInternal(nodeInfo.m_dirtyState, transforms, getNodeTransforms(nodeInfo.m_parentNodeID)->m_worldTransform);
	}

	nodeInfo.m_dirtyState = static_cast<DirtyState>(DirtyStateVariants::NONE);
}

void VT::Scene::updateRootNodeTransformInternal(DirtyState state, NodeTransforms& transforms)
{
	if (state == static_cast<DirtyState>(DirtyStateVariants::WORLD_TRANSFORM))
	{
		transforms.m_localTransform = transforms.m_worldTransform;
	}
	else if (state == static_cast<DirtyState>(DirtyStateVariants::LOCAL_TRANSFORM))
	{
		transforms.m_worldTransform = transforms.m_localTransform;
	}
	else
	{
		assert(false && "Invalid scene node dirty state.");
	}
}

void VT::Scene::updateChildNodeTransformInternal(DirtyState state, NodeTransforms& transforms,
	const Transform& parentWorldTransform)
{
	if (state == static_cast<DirtyState>(DirtyStateVariants::WORLD_TRANSFORM))
	{
		COMPUTE_MATH::ComputeMatrix invParentWorldTransformMatrix = COMPUTE_MATH::matrixInverse(
			COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(parentWorldTransform.m_matrix));
		COMPUTE_MATH::ComputeMatrix worldTransformMatrix = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms.m_worldTransform.m_matrix);
		COMPUTE_MATH::ComputeMatrix localTransformMatrix = COMPUTE_MATH::matrixMultiply(invParentWorldTransformMatrix, worldTransformMatrix);

		transforms.m_localTransform.m_matrix = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(localTransformMatrix);
	}
	else if (state == static_cast<DirtyState>(DirtyStateVariants::LOCAL_TRANSFORM))
	{
		COMPUTE_MATH::ComputeMatrix parentWorldTransformMatrix = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(parentWorldTransform.m_matrix);
		COMPUTE_MATH::ComputeMatrix localTransformMatrix = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms.m_localTransform.m_matrix);
		COMPUTE_MATH::ComputeMatrix worldTransformMatrix = COMPUTE_MATH::matrixMultiply(parentWorldTransformMatrix, localTransformMatrix);

		transforms.m_worldTransform.m_matrix = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(worldTransformMatrix);
	}
	else
	{
		assert(false && "Invalid scene node dirty state.");
	}
}

const VT::NodeTransforms* VT::Scene::getNodeTransforms(NodeID nodeId) const
{
	return m_nodeTransforms.getElement(nodeId);
}

VT::NodeTransforms* VT::Scene::getNodeTransforms(NodeID nodeId)
{
	return m_nodeTransforms.getElement(nodeId);
}

VT::NodeID VT::Scene::addNode(NodeID parentNodeID)
{
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

		if (newTransformID != newNodeID)
		{
			assert(false && "The new node info and transform IDs aren't same.");

			m_nodeInfos.removeElement(newNodeID);
			m_nodeTransforms.removeElement(newTransformID);
			return InvalidNodeID;
		}

		setDirtyState(newNodeID, DirtyStateVariants::WORLD_TRANSFORM);
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
	const NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeID);
	if (!nodeInfo)
	{
		return InvalidNodeID;
	}

	return nodeInfo->m_parentNodeID;
}

VT::NodeID VT::Scene::getFirstChildNodeID(NodeID parentNodeID) const
{
	const NodeInfo* parentNodeInfo = m_nodeInfos.getElement(parentNodeID);
	if (!parentNodeInfo)
	{
		return InvalidNodeID;
	}

	return parentNodeInfo->m_firstChildNodeID;
}

VT::NodeID VT::Scene::getNextSiblingNodeID(NodeID nodeId) const
{
	const NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
	if (!nodeInfo)
	{
		return InvalidNodeID;
	}

	return nodeInfo->m_nextSiblingNodeID;
}

void VT::Scene::setDirtyState(NodeID nodeId, DirtyStateVariants checkState)
{
	assert(checkState != DirtyStateVariants::NONE);

	NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
	assert(nodeInfo);

	setDirtyState(nodeId, *nodeInfo, checkState);
}

void VT::Scene::recalculateTransforms()
{
	if (m_dirtyNodeLevels.size() > 0)
	{
		for (const NodeID id : m_dirtyNodeLevels[0])
		{
			NodeInfo* nodeInfo = m_nodeInfos.getElement(id);
			if (!nodeInfo)
			{
				continue;
			}

			if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::NONE))
			{
				continue;
			}

			NodeTransforms* transforms = getNodeTransforms(id);
			if (!transforms)
			{
				continue;
			}

			updateRootNodeTransformInternal(nodeInfo->m_dirtyState, *transforms);

			nodeInfo->m_dirtyState = static_cast<DirtyState>(DirtyStateVariants::NONE);
		}

		m_dirtyNodeLevels[0].clear();
	}

	const NodeLevelType levelsCount = static_cast<NodeLevelType>(m_dirtyNodeLevels.size());
	for (NodeLevelType level = 1; level < levelsCount; ++level)
	{
		LevelDirtyNodeCollection& dirtyNodeCollection = m_dirtyNodeLevels[level];
		for (const NodeID id : dirtyNodeCollection)
		{
			NodeInfo* nodeInfo = m_nodeInfos.getElement(id);
			if (!nodeInfo)
			{
				continue;
			}

			if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::NONE))
			{
				continue;
			}

			NodeTransforms* transforms = getNodeTransforms(id);
			if (!transforms)
			{
				continue;
			}

			const NodeID parentID = nodeInfo->m_parentNodeID;
			if (parentID == InvalidNodeID)
			{
				assert(false && "Invalid parent node ID.");
				continue;
			}

			const NodeTransforms* parentTransforms = getNodeTransforms(parentID);
			if (!parentTransforms)
			{
				assert(false && "Invalid parent transform.");
				continue;
			}

			updateChildNodeTransformInternal(nodeInfo->m_dirtyState, *transforms, parentTransforms->m_worldTransform);

			nodeInfo->m_dirtyState = static_cast<DirtyState>(DirtyStateVariants::NONE);
		}

		dirtyNodeCollection.clear();
	}
}

void VT::Scene::recalculateNodeTransform(NodeID nodeId)
{
	updateNodeTransform(nodeId);
}

const VT::Transform& VT::Scene::getNodeWorldTransform(NodeID nodeId) const
{
	NodeTransforms* nodeTransforms = const_cast<NodeTransforms*>(getNodeTransforms(nodeId));
	assert(nodeTransforms);

	NodeInfo* nodeInfo = const_cast<NodeInfo*>(m_nodeInfos.getElement(nodeId));
	assert(nodeInfo);

	if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::LOCAL_TRANSFORM)
		|| nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::PARENT_TRANSFORM))
	{
		const_cast<Scene*>(this)->updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
	}

	return nodeTransforms->m_worldTransform;
}

VT::Transform& VT::Scene::getNodeWorldTransformRaw(NodeID nodeId, bool checkState)
{
	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	if (checkState)
	{
		NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
		assert(nodeInfo);

		if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::LOCAL_TRANSFORM)
			|| nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::PARENT_TRANSFORM))
		{
			updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
		}
	}

	return nodeTransforms->m_worldTransform;
}

void VT::Scene::setNodeWorldTransform(NodeID nodeId, const Transform& transform)
{
	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
	assert(nodeInfo);

	if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::LOCAL_TRANSFORM))
	{
		updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
	}

	nodeTransforms->m_worldTransform = transform;

	setDirtyState(nodeId, DirtyStateVariants::WORLD_TRANSFORM);
}

const VT::Transform& VT::Scene::getNodeLocalTransform(NodeID nodeId) const
{
	NodeTransforms* nodeTransforms = const_cast<NodeTransforms*>(getNodeTransforms(nodeId));
	assert(nodeTransforms);

	NodeInfo* nodeInfo = const_cast<NodeInfo*>(m_nodeInfos.getElement(nodeId));
	assert(nodeInfo);

	if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::WORLD_TRANSFORM))
	{
		const_cast<Scene*>(this)->updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
	}

	return nodeTransforms->m_localTransform;
}

VT::Transform& VT::Scene::getNodeLocalTransformRaw(NodeID nodeId, bool checkState)
{
	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	if (checkState) {
		NodeInfo* nodeInfo = m_nodeInfos.getElement(nodeId);
		assert(nodeInfo);

		if (nodeInfo->m_dirtyState == static_cast<DirtyState>(DirtyStateVariants::WORLD_TRANSFORM))
		{
			updateNodeTransformInternal(*nodeTransforms, *nodeInfo);
		}
	}

	return nodeTransforms->m_localTransform;
}

void VT::Scene::setNodeLocalTransform(NodeID nodeId, const Transform& transform)
{
	NodeTransforms* nodeTransforms = getNodeTransforms(nodeId);
	assert(nodeTransforms);

	nodeTransforms->m_localTransform = transform;

	setDirtyState(nodeId, DirtyStateVariants::LOCAL_TRANSFORM);
}

bool VT::Scene::isNodeDirty(NodeID nodeId) const
{
	return m_nodeInfos.getElement(nodeId)->m_dirtyState != static_cast<DirtyState>(DirtyStateVariants::NONE);
}

VT::IScene::DirtyState VT::Scene::getNodeDirtyState(NodeID nodeId) const
{
	NodeInfo* nodeInfo = const_cast<NodeInfo*>(m_nodeInfos.getElement(nodeId));
	assert(nodeInfo);

	return nodeInfo->m_dirtyState;
}
