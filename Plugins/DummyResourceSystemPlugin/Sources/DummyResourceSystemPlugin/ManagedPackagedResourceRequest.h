#pragma once

#include "Multithreading/CountableObject/CountableReference.h"
#include "ResourceSystem/PackagedResourceRequest.h"

#include <functional>

namespace VT_DUMMY_RS
{
	using PackageRequestID = uint32_t;
	constexpr PackageRequestID InvalidPackageRequestID = -1;

	class ManagedPackagedResourceRequest final : public VT::PackagedResourceRequestBase
	{
	private:
		ResourceRequestResultCollection m_result;

		VT::PackageRequestCallback m_callback = nullptr;

		const size_t m_requestedResourceCount = 0;
		VT::Atomic<size_t> m_loadedResourceCount = 0;

		PackageRequestID m_requestID = InvalidPackageRequestID;

		virtual void selfDestroy() override;

	public:
		ManagedPackagedResourceRequest(PackageRequestID requestID, size_t requestedResourceCount, const VT::PackageRequestCallback& callback);

		virtual size_t getRequestedResourceCount() const override { return m_requestedResourceCount; }

		void setResourceResult(size_t index, VT::ResourceDataReference resource);
		virtual const ResourceRequestResultCollection& getResult() const override { return m_result; }

		PackageRequestID getRequestID() const { return m_requestID; }

	};
}