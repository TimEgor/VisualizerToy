#pragma once

#include "ResourceData.h"

#include <functional>

namespace VT
{
	class IPackagedResourceRequest
	{
	public:
		using ResourceRequestResultCollection = std::vector<ResourceDataReference>;

	public:
		IPackagedResourceRequest() = default;
		virtual ~IPackagedResourceRequest() {}

		virtual size_t getRequestedResourceCount() const = 0;

		virtual const ResourceRequestResultCollection& getResult() const = 0;
	};

	class PackagedResourceRequestBase : public CountableObjectBase, public IPackagedResourceRequest
	{
	public:
		PackagedResourceRequestBase() = default;
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(PackagedResourceRequestBase, PackagedResourceRequest);

	using PackageRequestCallback = std::function<void(PackagedResourceRequestReference)>;
}