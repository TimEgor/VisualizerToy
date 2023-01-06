#pragma once

#include "Multithreading/CountableObject/CountableReference.h"

namespace VT
{
	class IMesh;

	using MeshHandleID = uint32_t;

	class MeshHandle : public CountableObjectBase
	{
	public:
		MeshHandle() = default;

		virtual IMesh* getMesh() const = 0;
		virtual MeshHandleID getID() const = 0;
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(MeshHandle, Mesh);
}
