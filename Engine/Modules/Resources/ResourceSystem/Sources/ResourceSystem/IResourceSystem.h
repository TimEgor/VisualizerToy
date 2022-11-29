#pragma once

#include "ResourceData.h"
#include "PackagedResourceRequest.h"

#include "Core/HashFunctions/CRC32.h"
#include "Core/FileName/FileName.h"

#include <functional>
#include <cassert>

namespace VT
{
	using ResourceSystemType = uint32_t;

	class IResourceSystem
	{
	public:
		using LoadedResourceCallback = std::function<void(ResourceDataReference)>;

		struct DelayedResourceRequest final
		{
			FileName m_resourceName = nullptr;
			LoadedResourceCallback m_callback = nullptr;

			DelayedResourceRequest(const FileName& resourceName)
				: m_resourceName(resourceName)
			{
				assert(resourceName);
			}

			DelayedResourceRequest(const FileName& resourceName, const LoadedResourceCallback& callback)
				: m_resourceName(resourceName), m_callback(callback)
			{
				assert(resourceName);
				assert(callback);
			}
		};

		using PackageResourceRequestCollection = std::vector<FileName>;
		using PackageResourceRequestResultCollection = std::vector<ResourceDataReference>;
		using DelayedPackageResourceRequestCollection = std::vector<DelayedResourceRequest>;

	public:
		IResourceSystem() = default;
		virtual ~IResourceSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual ResourceDataReference getResource(const FileName& resName) = 0;
		virtual void getResourceAsync(const FileName& resName, const LoadedResourceCallback& callback = nullptr) = 0;

		virtual size_t getPackagedResource(const PackageResourceRequestCollection& request, PackageResourceRequestResultCollection& result) = 0;
		virtual void getPackagedResourceAsync(const DelayedPackageResourceRequestCollection& request, const PackageRequestCallback& callback = nullptr) = 0;

		virtual ResourceSystemType getType() const = 0;
	};
}

#define VT_RESOURCE_SYSTEM_TYPE(RES_SYSTEM_TYPE)																	\
	static inline constexpr VT::ResourceSystemType getResourceSystemType() { return VT::crc32(#RES_SYSTEM_TYPE); }	\
	virtual VT::ResourceSystemType getType() const override { return getResourceSystemType(); }