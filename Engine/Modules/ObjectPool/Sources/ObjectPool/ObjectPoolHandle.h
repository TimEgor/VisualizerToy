#pragma once

#include <cstdint>

namespace VT
{
	struct ObjectPoolHandleStorage32 final
	{
		using KeyType = uint32_t;
		using IndexType = uint16_t;
		using VersionType = uint16_t;

		static constexpr KeyType InvalidKey = 0;
		static constexpr VersionType InvalidVersion = 0;

		union
		{
			KeyType m_key = InvalidKey;

			struct
			{
				IndexType m_index;
				VersionType m_version;
			};
		};

		constexpr ObjectPoolHandleStorage32() = default;
		constexpr ObjectPoolHandleStorage32(KeyType key) : m_key(key) {}
		constexpr ObjectPoolHandleStorage32(IndexType index, VersionType version)
			: m_index(index), m_version(version) {}
		constexpr ObjectPoolHandleStorage32(const ObjectPoolHandleStorage32&) = default;
	};

	struct ObjectPoolHandleStorage64 final
	{
		using KeyType = uint64_t;
		using IndexType = uint32_t;
		using VersionType = uint32_t;

		static constexpr KeyType InvalidKey = 0;
		static constexpr VersionType InvalidVersion = 0;

		union
		{
			KeyType m_key = InvalidKey;

			struct
			{
				IndexType m_index;
				VersionType m_version;
			};
		};

		constexpr ObjectPoolHandleStorage64() = default;
		constexpr ObjectPoolHandleStorage64(KeyType key) : m_key(key) {}
		constexpr ObjectPoolHandleStorage64(IndexType index, VersionType version)
			: m_index(index), m_version(version) {}
		constexpr ObjectPoolHandleStorage64(const ObjectPoolHandleStorage64&) = default;
	};

	template <typename ObjectPoolHandleStorageType>
	struct ObjectPoolHandle final
	{
		using HandleStorageType = ObjectPoolHandleStorageType;

		using KeyType = typename ObjectPoolHandleStorageType::KeyType;
		using IndexType = typename ObjectPoolHandleStorageType::IndexType;
		using VersionType = typename ObjectPoolHandleStorageType::VersionType;

		static constexpr KeyType InvalidKey = typename ObjectPoolHandleStorageType::InvalidKey;
		static constexpr VersionType InvalidVersion = typename ObjectPoolHandleStorageType::InvalidVersion;

		HandleStorageType m_handle;

		constexpr ObjectPoolHandle() = default;
		constexpr ObjectPoolHandle(KeyType key) : m_handle(key) {}
		constexpr ObjectPoolHandle(IndexType index, VersionType version)
			: m_handle(index, version) {}
		constexpr ObjectPoolHandle(HandleStorageType handle) : m_handle(handle) {}
		constexpr ObjectPoolHandle(const ObjectPoolHandle&) = default;

		inline KeyType getKey() const { return m_handle.m_key; }
		inline IndexType getIndex() const { return m_handle.m_index; }
		inline VersionType getVersion() const { return m_handle.m_version; }

		static KeyType makeKey(IndexType index, VersionType version)
		{
			return HandleStorageType(index, version).m_key;
		}

		static IndexType getIndex(KeyType key)
		{
			return HandleStorageType(key).m_index;
		}

		static VersionType getVersion(KeyType key)
		{
			return HandleStorageType(key).m_version;
		}
	};

	using ObjectPoolHandle32 = ObjectPoolHandle<ObjectPoolHandleStorage32>;
	using ObjectPoolHandle64 = ObjectPoolHandle<ObjectPoolHandleStorage64>;
}