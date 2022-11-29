#pragma once

#include "ResourceSystem/IResourceSystem.h"

namespace VT_DUMMY_RS
{
	using ManagedResourceDataID = VT::FileNameID;
	constexpr ManagedResourceDataID InvalidManagedResourceDataID = VT::InvalidFileNameID;

	class ManagedResourceData final : public VT::ResourceData
	{
	private:
		ManagedResourceDataID m_id = InvalidManagedResourceDataID;

		virtual void selfDestroy() override;

	public:
		ManagedResourceData(ManagedResourceDataID id)
			: m_id(id) {}

		void* getData() { return m_data; }
		void setData(void* data, size_t dataSize);

		ManagedResourceDataID getID() const { return m_id; }
		void setState(VT::ResourceState state);
	};
}