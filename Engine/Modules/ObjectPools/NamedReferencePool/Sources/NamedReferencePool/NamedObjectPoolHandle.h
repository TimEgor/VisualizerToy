#pragma once

#include <cstdint>

#include "ObjectPool/ObjectPoolHandle.h"

namespace VT
{
	enum NamedHandleResourceType
	{
		NAMELESS_RESOURCE_TYPE = 0,
		NAMED_RESOURCE_TYPE = 1
	};

	struct NamedObjectPoolHandleStorage32 final
	{
		using KeyType = uint32_t;
		using IndexType = uint16_t;
		using VersionType = uint16_t;

		using UserResourceType = uint16_t;

		static constexpr KeyType InvalidKey = 0;
		static constexpr VersionType InvalidVersion = 0;

		static constexpr VersionType VersionMask = 0x7FFF;
		static constexpr size_t VersionOffset = 16;

		static constexpr UserResourceType ResourceTypeMask = 0x1;
		static constexpr size_t ResourceTypeOffset = 31;

		union
		{
			KeyType m_key = InvalidKey;

			struct
			{
				IndexType m_index;
				VersionType m_version : 15;
				UserResourceType m_resourceType : 1;
			};
		};

		constexpr NamedObjectPoolHandleStorage32() = default;
		constexpr NamedObjectPoolHandleStorage32(KeyType key) : m_key(key) {}
		constexpr NamedObjectPoolHandleStorage32(IndexType index, VersionType version)
			: m_index(index), m_resourceType(0)
		{
			setVersion(version);
		}
		NamedObjectPoolHandleStorage32(IndexType index, VersionType version, UserResourceType resourceType)
			: m_index(index)
		{
			setVersion(version);
			setResourceType(resourceType);
		}

		constexpr NamedObjectPoolHandleStorage32(const NamedObjectPoolHandleStorage32&) = default;

		constexpr IndexType getIndex() const { return m_index; }
		constexpr VersionType getVersion() const { return m_version; }
		constexpr UserResourceType getResourceType() const { return m_resourceType; }

		constexpr void setVersion(VersionType version)
		{
			m_version = version & VersionMask;
		}

		constexpr void setResourceType(UserResourceType type)
		{
			m_resourceType = type & ResourceTypeMask;
		}
	};

	struct NamedObjectPoolHandleStorage64 final
	{
		using KeyType = uint64_t;
		using IndexType = uint32_t;
		using VersionType = uint32_t;

		using UserResourceType = uint32_t;

		static constexpr KeyType InvalidKey = 0;
		static constexpr VersionType InvalidVersion = 0;

		static constexpr VersionType VersionMask = 0x7FFF'FFFF;
		static constexpr size_t VersionOffset = 32;

		static constexpr UserResourceType ResourceTypeMask = 0x1;
		static constexpr size_t ResourceTypeOffset = 63;

		union
		{
			KeyType m_key = InvalidKey;

			struct
			{
				IndexType m_index;
				VersionType m_version : 31;
				UserResourceType m_resourceType : 1;
			};
		};

		constexpr NamedObjectPoolHandleStorage64() = default;
		constexpr NamedObjectPoolHandleStorage64(KeyType key) : m_key(key) {}
		constexpr NamedObjectPoolHandleStorage64(IndexType index, VersionType version)
			: m_index(index), m_resourceType(0)
		{
			setVersion(version);
		}
		constexpr NamedObjectPoolHandleStorage64(IndexType index, VersionType version, UserResourceType resourceType)
			: m_index(index)
		{
			setVersion(version);
			setResourceType(resourceType);
		}
		constexpr NamedObjectPoolHandleStorage64(const NamedObjectPoolHandleStorage64&) = default;

		constexpr IndexType getIndex() const { return m_index; }
		constexpr VersionType getVersion() const { return m_version; }
		constexpr UserResourceType getResourceType() const { return m_resourceType; }

		constexpr void setVersion(VersionType version)
		{
			m_version = version & VersionMask;
		}

		constexpr void setResourceType(UserResourceType type)
		{
			m_resourceType = type & ResourceTypeMask;
		}
	};

	using NamedObjectPoolHandle32 = ObjectPoolHandle<NamedObjectPoolHandleStorage32>;
	using NamedObjectPoolHandle64 = ObjectPoolHandle<NamedObjectPoolHandleStorage64>;
}