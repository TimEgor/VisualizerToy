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

		IScene::DirtyState m_dirtyState = static_cast<IScene::DirtyState>(IScene::DirtyStateVariants::NONE);
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

		NodeID addNodeInfo(NodeInfo* parentNodeInfo, NodeID parentNodeID);
		void collectRemovingNodes(NodeID nodeID, RemovingNodeCollection& nodes) const;
		void setDirtyState(NodeID nodeID, NodeInfo& nodeInfo, DirtyStateVariants state);

		void updateNodeTransform(NodeID nodeId);
		void updateNodeTransform(NodeID nodeId, NodeInfo& nodeInfo);
		void updateNodeTransform(NodeID nodeId, NodeTransforms& transforms);

		void updateNodeTransformInternal(NodeTransforms& transforms, NodeInfo& nodeInfo);
		void updateRootNodeTransformInternal(DirtyState state, NodeTransforms& transforms);
		void updateChildNodeTransformInternal(DirtyState state, NodeTransforms& transforms, const Transform& parentWorldTransform);

		const NodeTransforms* getNodeTransforms(NodeID nodeId) const;
		NodeTransforms* getNodeTransforms(NodeID nodeId);

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

		virtual const Transform& getNodeWorldTransform(NodeID nodeId) const override;
		virtual Transform& getNodeWorldTransformRaw(NodeID nodeId, bool checkState) override;
		virtual void setNodeWorldTransform(NodeID nodeId, const Transform& transform) override;

		virtual const Transform& getNodeLocalTransform(NodeID nodeId) const override;
		virtual Transform& getNodeLocalTransformRaw(NodeID nodeId, bool checkState) override;
		virtual void setNodeLocalTransform(NodeID nodeId, const Transform& transform) override;

		virtual bool isNodeDirty(NodeID nodeId) const override;
		virtual DirtyState getNodeDirtyState(NodeID nodeId) const override;
		virtual void setDirtyState(NodeID nodeId, DirtyStateVariants checkState) override;

		virtual void recalculateTransforms() override;
		virtual void recalculateNodeTransform(NodeID nodeId) override;
	};
}
