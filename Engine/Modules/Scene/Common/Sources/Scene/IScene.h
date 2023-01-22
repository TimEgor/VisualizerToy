#pragma once

#include <functional>

#include "Node.h"

namespace VT
{
	class IScene
	{
	public:
		using DeleteNodeCallback = std::function<void(NodeID)>;

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

		virtual NodeTransforms* getNodeTransforms(NodeID nodeID) = 0;
		virtual const NodeTransforms* getNodeTransforms(NodeID nodeID) const = 0;

		virtual void recalculateTransforms() = 0;
		virtual void markDirty(NodeID nodeId) = 0;
	};
}
