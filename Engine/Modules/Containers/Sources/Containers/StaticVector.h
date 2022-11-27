#pragma once

#include <vector>
#include <cassert>

namespace VT
{
	template <typename T, size_t InlineSize>
	class StaticVector final
	{
	public:
		using Iterator = T*;
		using ConstIterator = const T*;

		using ReverseIterator = std::reverse_iterator<Iterator>;
		using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

	private:
		std::vector<T> m_heapData;
		T m_inlineData[InlineSize];
		size_t m_dataSize = 0;


	public:
		StaticVector() = default;
		StaticVector(const StaticVector& vector);
		StaticVector(StaticVector&& vector);

		StaticVector& operator=(const StaticVector& vector);
		StaticVector& operator=(StaticVector&& vector);

		T& operator[](size_t index);
		const T& operator[](size_t index) const;

		void clear();

		void pushBack(const T& val);
		void pushBack(T&& val);

		void popBack();

		template <typename... Args>
		void emplaceBack(Args... args);

		size_t getSize() const;

		T* getData();
		const T* getData() const;

		Iterator begin();
		ConstIterator begin() const;

		Iterator end();
		ConstIterator end() const;

		ReverseIterator rbegin();
		ConstReverseIterator rbegin() const;

		ReverseIterator rend();
		ConstReverseIterator rend() const;
	};

	template<typename T, size_t InlineSize>
	inline StaticVector<T, InlineSize>::StaticVector(const StaticVector& vector)
		: m_heapData(vector.m_heapData), m_dataSize(vector.m_dataSize)
	{
		if (m_dataSize > 0)
		{
			memcpy(m_inlineData, vector.m_inlineData, sizeof(T) * m_dataSize);
		}
	}
	template<typename T, size_t InlineSize>
	inline StaticVector<T, InlineSize>::StaticVector(StaticVector && vector)
		: m_heapData(std::move(vector.m_heapData)), m_dataSize(vector.m_dataSize)
	{
		if (m_dataSize > 0)
		{
			memmove(m_inlineData, vector.m_inlineData, sizeof(T) * m_dataSize);
		}
	}

	template<typename T, size_t InlineSize>
	inline StaticVector<T, InlineSize>& StaticVector<T, InlineSize>::operator=(const StaticVector& vector)
	{
		m_heapData = vector.m_heapData;
		m_dataSize = vector.m_dataSize;
		
		if (m_dataSize > 0)
		{
			memcpy(m_inlineData, vector.m_inlineData, sizeof(T) * m_dataSize);
		}

		return *this;
	}
	template<typename T, size_t InlineSize>
	inline StaticVector<T, InlineSize>& StaticVector<T, InlineSize>::operator=(StaticVector&& vector)
	{
		m_heapData = std::move(vector.m_heapData);
		m_dataSize = vector.m_dataSize;

		if (m_dataSize > 0)
		{
			memmove(m_inlineData, vector.m_inlineData, sizeof(T) * m_dataSize);
		}

		return *this;
	}

	template<typename T, size_t InlineSize>
	inline T& StaticVector<T, InlineSize>::operator[](size_t index)
	{
		assert(index < m_dataSize || index < m_heapData.size());
		return *(begin() + index);
	}
	template<typename T, size_t InlineSize>
	inline const T& StaticVector<T, InlineSize>::operator[](size_t index) const
	{
		assert(index < m_dataSize || index < m_heapData.size());
		return *(begin() + index);
	}

	template<typename T, size_t InlineSize>
	inline void StaticVector<T, InlineSize>::clear()
	{
		m_heapData.clear();
		m_dataSize = 0;
	}

	template<typename T, size_t InlineSize>
	inline void StaticVector<T, InlineSize>::pushBack(const T & val)
	{
		if (m_dataSize < InlineSize)
		{
			new (m_inlineData + m_dataSize) T(val);
			++m_dataSize;

			return;
		}
		else if (m_dataSize == InlineSize)
		{
			m_heapData.resize(InlineSize);
			memmove(m_heapData.data(), m_inlineData, sizeof(T) * m_dataSize);
		}

		m_heapData.push_back(val);

		++m_dataSize;
	}
	template<typename T, size_t InlineSize>
	inline void StaticVector<T, InlineSize>::pushBack(T && val)
	{

	}
	template<typename T, size_t InlineSize>
	inline void StaticVector<T, InlineSize>::popBack()
	{
		if (m_dataSize <= InlineSize)
		{
			if constexpr (!std::is_trivially_destructible<ValType>::value)
			{
				T* val = m_inlineData + m_dataSize - 1;
				val->~ValType();
			}
		}
		else
		{
			m_heapData.pop_back();
		}

		--m_dataSize;
	}

	template<typename T, size_t InlineSize>
	inline size_t StaticVector<T, InlineSize>::getSize() const
	{
		return m_dataSize;
	}

	template<typename T, size_t InlineSize>
	inline T* StaticVector<T, InlineSize>::getData()
	{
		return begin();
	}

	template<typename T, size_t InlineSize>
	inline const T* StaticVector<T, InlineSize>::getData() const
	{
		return begin();
	}

	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::Iterator StaticVector<T, InlineSize>::begin()
	{
		return m_dataSize <= InlineSize ? m_inlineData : m_heapData.data();
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ConstIterator StaticVector<T, InlineSize>::begin() const
	{
		return begin();
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::Iterator StaticVector<T, InlineSize>::end()
	{
		return begin() + m_dataSize;
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ConstIterator StaticVector<T, InlineSize>::end() const
	{
		return end();
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ReverseIterator StaticVector<T, InlineSize>::rbegin()
	{
		return ReverseIterator(end());
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ConstReverseIterator StaticVector<T, InlineSize>::rbegin() const
	{
		return ConstReverseIterator(end());
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ReverseIterator StaticVector<T, InlineSize>::rend()
	{
		return ReverseIterator(begin());
	}
	template<typename T, size_t InlineSize>
	inline typename StaticVector<T, InlineSize>::ConstReverseIterator StaticVector<T, InlineSize>::rend() const
	{
		return ConstReverseIterator(begin());
	}
	template<typename T, size_t InlineSize>
	template<typename ...Args>
	inline void StaticVector<T, InlineSize>::emplaceBack(Args ...args)
	{
		if (m_dataSize < InlineSize)
		{
			new (m_inlineData + m_dataSize) T(args...);
			++m_dataSize;

			return;
		}
		else if (m_dataSize == InlineSize)
		{
			m_heapData.resize(InlineSize);
			memmove(m_heapData.data(), m_inlineData, sizeof(T) * m_dataSize);
		}

		m_heapData.emplace_back(args...);

		++m_dataSize;
	}
}