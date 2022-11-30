#pragma once

#include "ResourceData.h"
#include "ResourceDependencyState.h"

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
		using LoadingResourceCallback = std::function<void(ResourceDataReference)>;

	public:
		IResourceSystem() = default;
		virtual ~IResourceSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual ResourceDataReference getResource(const FileName& resName) = 0;
		virtual void getResourceAsync(const FileName& resName, const LoadingResourceCallback& callback = nullptr) = 0;

		virtual ResourceDependencyStateReference createResourceDependencyState(const ResourceDependencyState::Callback& callback) = 0;

		virtual ResourceSystemType getType() const = 0;
	};
}

#define VT_RESOURCE_SYSTEM_TYPE(RES_SYSTEM_TYPE)																	\
	static inline constexpr VT::ResourceSystemType getResourceSystemType() { return VT::crc32(#RES_SYSTEM_TYPE); }	\
	virtual VT::ResourceSystemType getType() const override { return getResourceSystemType(); }