#pragma once

#include "Multithreading/CountableObject/CountableReference.h"

namespace VT
{
	enum class ResourceState
	{
		INVALID,
		LOADING,
		LOADED
	};

	class ResourceData : public CountableObjectBase
	{
	protected:
		void* m_data = nullptr;
		size_t m_dataSize = 0;

		Atomic<ResourceState> m_state = ResourceState::INVALID;

	public:
		ResourceData() = default;
		ResourceData(const ResourceData&) = delete;
		ResourceData(ResourceData&& data)
			: m_data(data.m_data), m_dataSize(data.m_dataSize)
		{
			storeAtomic(&m_state, data.m_state);

			data.m_data = nullptr;
			data.m_dataSize = 0;
			data.m_state = ResourceState::INVALID;
		}

		ResourceData& operator=(const ResourceData&) = delete;
		ResourceData& operator=(ResourceData&& data)
		{
			m_data = data.m_data;
			m_dataSize = data.m_dataSize;
			storeAtomic(&m_state, data.m_state);

			data.m_data = nullptr;
			data.m_dataSize = 0;
			data.m_state = ResourceState::INVALID;

			return *this;
		}

		const void* getData() const { return m_data; }
		size_t getDataSize() const { return m_dataSize; }

		ResourceState getState() const { return m_state; }
	};

	COUNTABLE_REFERENCES_DECLARATION(ResourceData)
}