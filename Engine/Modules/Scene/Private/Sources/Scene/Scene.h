#pragma once

#include "Scene/IScene.h"
#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Containers/StaticVector.h"

namespace VT
{
	using NodeLevelType = uint32_t;
	constexpr NodeLevelType InvalidNodeLevel = -1;

	struct NodeInfo final
	{
		NodeID m_parentNodeID = InvalidNodeID;
		NodeID m_firstChildNodeID = InvalidNodeID;
		NodeID m_nextSiblingNodeID = InvalidNodeID;
		NodeID m_lastSiblingNodeID = InvalidNodeID; // It is available only for first child node.

		NodeLevelType m_level = InvalidNodeLevel;
	};

	class Scene final : public IScene
	{
		using NodePoolHandle = ObjectPoolHandle32;
		static_assert(sizeof(NodePoolHandle::KeyType) == sizeof(NodeID));

		using NodeInfoPool = ObjectPool<NodeInfo, NodePoolHandle>;
		using NodeTransformPool = ObjectPool<NodeTransforms, NodePoolHandle>;
		using LevelDirtyNodeCollection = StaticVector<NodeID, 16>;
		using DirtyLevelCollection = std::vector<LevelDirtyNodeCollection>;

		using RemovingNodeCollection = std::vector<NodeID>;

	private:
		NodeInfoPool m_nodeInfos;
		NodeTransformPool m_nodeTransforms;
		DirtyLevelCollection m_dirtyNodeLevels;

		mutable SharedMutex m_lockMutex;

		NodeID addNodeInfo(NodeInfo* parentNodeInfo, NodeID parentNodeID);
		void collectRemovingNodes(NodeID nodeID, RemovingNodeCollection& nodes) const;
		void markDirtyNode(NodeID nodeID);

	public:
		Scene() = default;
		~Scene() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual NodeID addNode(NodeID parentNodeID = InvalidNodeID) override;
		virtual void removeNode(NodeID nodeID, const DeleteNodeCallback& callback = nullptr) override;

		virtual NodeID getParentNodeID(NodeID nodeID) const override;
		virtual NodeID getFirstChildNodeID(NodeID parentNodeID) const override;
		virtual NodeID getNextSiblingNodeID(NodeID nodeID) const override;

		virtual NodeTransforms* getNodeTransforms(NodeID nodeID) override;
		virtual const NodeTransforms* getNodeTransforms(NodeID nodeID) const override;

		virtual void recalculateTransforms() override;
		virtual void markDirty(NodeID nodeID) override;
	};
}
