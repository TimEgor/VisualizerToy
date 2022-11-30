#pragma once

#include "Multithreading/CountableObject/CountableReference.h"
#include "ResourceSystem/ResourceDependencyState.h"

#include <functional>

namespace VT_DUMMY_RS
{
	using PackageRequestID = uint32_t;
	constexpr PackageRequestID InvalidPackageRequestID = -1;

	class ManagedResourceDependencyState final : public VT::ResourceDependencyState
	{
	private:
		PackageRequestID m_requestID = InvalidPackageRequestID;

		virtual void selfDestroy() override;

	public:
		ManagedResourceDependencyState(PackageRequestID requestID, const VT::ResourceDependencyState::Callback& callback);

		PackageRequestID getRequestID() const { return m_requestID; }
	};
}