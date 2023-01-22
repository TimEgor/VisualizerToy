#pragma once

#include "Scene/Node.h"

namespace VT
{
	class SceneNodeIDComponent final
	{
	private:
		NodeID m_nodeID;

	public:
		SceneNodeIDComponent(NodeID nodeID)
			: m_nodeID(nodeID) {}

		NodeID getNodeID() const { return m_nodeID; }
	};
}
