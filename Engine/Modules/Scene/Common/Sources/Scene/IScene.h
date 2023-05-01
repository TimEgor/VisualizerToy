#pragma once

#include <functional>

#include "Node.h"

namespace VT
{
	class IScene
	{
	public:
		using DeleteNodeCallback = std::function<void(NodeID)>;

		using DirtyState = uint32_t;
		enum class DirtyStateVariants
		{
			NONE = 0,

			WORLD_TRANSFORM = 1,
			LOCAL_TRANSFORM = 2,
			PARENT_TRANSFORM = 4
		};

	public:
		IScene() = default;
		virtual ~IScene() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual NodeID addNode(NodeID parentNodeID = InvalidNodeID) = 0;
		virtual void removeNode(NodeID nodeID, const DeleteNodeCallback& callback = nullptr) = 0;

		virtual NodeID getParentNodeID(NodeID nodeID) const = 0;
		virtual NodeID getFirstChildNodeID(NodeID parentNodeID) const = 0;
		virtual NodeID getNextSiblingNodeID(NodeID nodeID) const = 0;

		virtual const Transform& getNodeWorldTransform(NodeID nodeId) const = 0;
		virtual Transform& getNodeWorldTransformRaw(NodeID nodeId, bool checkState = true) = 0;
		virtual void setNodeWorldTransform(NodeID nodeId, const Transform& transform) = 0;

		virtual const Transform& getNodeLocalTransform(NodeID nodeId) const = 0;
		virtual Transform& getNodeLocalTransformRaw(NodeID nodeId, bool checkState = true) = 0;
		virtual void setNodeLocalTransform(NodeID nodeId, const Transform& transform) = 0;

		virtual DirtyState getNodeDirtyState(NodeID nodeId) const = 0;
		virtual bool isNodeDirty(NodeID nodeId) const = 0;
		virtual void setDirtyState(NodeID nodeId, DirtyStateVariants state) = 0;

		virtual void recalculateTransforms() = 0;
		virtual void recalculateNodeTransform(NodeID nodeId) = 0;
	};
}
