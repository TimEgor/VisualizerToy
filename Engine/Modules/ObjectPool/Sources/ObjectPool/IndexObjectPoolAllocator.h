#pragma once

#include "Core/UtilitiesMacros.h"
#include "Core/Memory.h"

#include <type_traits>
#include <cassert>
#include <vector>
#include <deque>

namespace VT
{
	template <typename IndexType>
	class IndexObjectPoolAllocator final
	{
		static_assert(std::is_integral<IndexType>::value);

		static constexpr IndexType InvalidIndex = 0;

		struct Page final
		{
			void* m_mem = nullptr;
			uint8_t* m_valsMem = nullptr;
			bool* m_aliveMem = nullptr;

			size_t m_capacity = 0;
			size_t m_size = 0;

			Page() = default;
			Page(const Page&) = delete;
			Page(Page&& page)
				: m_capacity(page.m_capacity),
				m_size(page.m_size)
			{
				std::swap(m_mem, page.m_mem);
				std::swap(m_valsMem, page.m_valsMem);
				std::swap(m_aliveMem, page.m_aliveMem);
				page.m_capacity = 0;
				page.m_size = 0;
			}

			~Page() { deallocate(); }

			Page& operator=(const Page&) = delete;
			Page& operator=(Page&&) = delete;

			void allocate(size_t elementSize, size_t capacity)
			{
				size_t valueMemSize = align(elementSize * capacity, sizeof(void*));
				size_t aliveMemSize = sizeof(bool) * capacity;

				size_t memSize = align(valueMemSize + aliveMemSize, sizeof(void*));
				uint8_t* mem = new uint8_t[memSize];

				m_mem = mem;
				m_valsMem = mem;
				m_aliveMem = reinterpret_cast<bool*>(mem + valueMemSize);

				m_capacity = capacity;
				m_size = 0;

				assert(m_mem);
				assert(capacity);

				memset(m_aliveMem, 0, aliveMemSize);
			}

			void deallocate()
			{
				VT_SAFE_DESTROY_ARRAY(m_mem);

				m_valsMem = nullptr;
				m_aliveMem = nullptr;
				m_capacity = 0;
				m_size = 0;
			}

			bool isAllocated() const
			{
				return m_mem;
			}
		};

	public:
		struct NewElementInfo final
		{
			IndexType m_elementIndex = InvalidIndex;
			void* m_elementPtr = nullptr;
		};

	private:
		std::vector<Page> m_pages;
		std::deque<IndexType> m_freeIndices;
		size_t m_elementSize = 0;
		size_t m_size = 0;
		size_t m_emptyPages = 0;
		IndexType m_maxUsedIndex = 0;

		size_t m_pageSize = 0;
		size_t m_maxFreePageCount = 0;
		size_t m_minFreeIndexCount = 0;

		struct ElementLocation final
		{
			size_t pageIndex = -1;
			size_t elementIndex = -1;
		};

		inline bool checkElementLocation(const ElementLocation& location) const
		{
			return location.elementIndex < m_pageSize;
		}

		inline bool checkElementLocationWithPage(const ElementLocation& location) const
		{
			return checkElementLocation(location) && location.pageIndex < m_pages.size();
		}

		ElementLocation getElementLocation(IndexType index) const
		{
			ElementLocation location;
			location.pageIndex = index / m_pageSize;
			location.elementIndex = index % m_pageSize;

			return location;
		}

		void* getElementInternal(IndexType index)
		{
			assert(m_pageSize != 0);

			ElementLocation elementLocation = getElementLocation(index);

			if (!checkElementLocationWithPage(elementLocation))
			{
				return nullptr;
			}

			const Page& page = m_pages[elementLocation.pageIndex];
			if (!page.isAllocated() || !page.m_aliveMem[elementLocation.elementIndex])
			{
				return nullptr;
			}

			return &page.m_valsMem[elementLocation.elementIndex * m_elementSize];
		}

	public:
		IndexObjectPoolAllocator() = default;
		IndexObjectPoolAllocator(const IndexObjectPoolAllocator&) = delete;
		IndexObjectPoolAllocator(IndexObjectPoolAllocator&& pool)
			: m_elementSize(pool.m_elementSize),
			m_size(pool.m_size),
			m_emptyPages(pool.m_emptyPages),
			m_maxUsedIndex(pool.m_maxUsedIndex),
			m_pageSize(pool.m_pageSize),
			m_maxFreePageCount(pool.m_maxFreePageCount),
			m_minFreeIndexCount(pool.m_minFreeIndexCount)
		{
			std::swap(m_pages, pool.m_pages);
			std::swap(m_freeIndices, pool.m_freeIndices);

			pool.m_elementSize = 0;
			pool.m_size = 0;
			pool.m_emptyPages = 0;
			pool.m_maxUsedIndex = 0;

			pool.m_pageSize = 0;
			pool.m_maxFreePageCount = 0;
			pool.m_minFreeIndexCount = 0;
		}
		~IndexObjectPoolAllocator() { release(); }


		bool init(size_t elementSize, size_t pageSize = 4096, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64)
		{
			assert(m_pageSize == 0);

			m_elementSize = elementSize;
			m_pageSize = pageSize;
			m_maxFreePageCount = maxFreePageCount;
			m_minFreeIndexCount = minFreeIndexCount;

			m_pages.reserve(1);

			return true;
		}

		void release()
		{
			clear();
		}

		void clear()
		{
			for (auto& page : m_pages)
			{
				if (!page.isAllocated())
				{
					continue;
				}

				page.deallocate();
			}
		}

		bool isValid(IndexType index) const
		{
			assert(m_pageSize != 0);

			ElementLocation elementLocation = getElementLocation(index);

			if (!checkElementLocationWithPage(elementLocation))
			{
				return false;
			}

			const Page& page = m_pages[elementLocation.pageIndex];
			if (!page.isAllocated() || !page.m_aliveMem[elementLocation.elementIndex])
			{
				return false;
			}

			return true;
		}

		const void* getElement(IndexType index) const
		{
			return getElementInternal(index);
		}

		void* getElement(IndexType index)
		{
			return getElementInternal(index);
		}

		void addElementRaw(NewElementInfo& info)
		{
			assert(m_pageSize != 0);

			IndexType index;

			if (m_freeIndices.size() > m_minFreeIndexCount)
			{
				index = m_freeIndices.front();
				m_freeIndices.pop_front();
			}
			else
			{
				index = m_maxUsedIndex;
				++m_maxUsedIndex;
			}

			ElementLocation elementLocation = getElementLocation(index);
			if (!checkElementLocation(elementLocation))
			{
				assert(false && "ObjectPool::addElement() : Element indicing invalidation.");
				info = NewElementInfo();
				return;
			}

			Page* pagePtr = nullptr;

			size_t pagesCount = m_pages.size();
			if (pagesCount <= elementLocation.pageIndex)
			{
				if (elementLocation.pageIndex - pagesCount < 2)
				{
					pagePtr = &m_pages.emplace_back();
				}
			}
			else
			{
				pagePtr = &m_pages[elementLocation.pageIndex];
			}

			if (!pagePtr)
			{
				assert(false && "ObjectPool::addElement() : Page indicing invalidation.");
				info = NewElementInfo();
				return;
			}

			Page& page = *pagePtr;
			if (!page.isAllocated())
			{
				page.allocate(m_elementSize, m_pageSize);
			}
			assert(page.m_size < page.m_capacity);

			bool& aliveState = page.m_aliveMem[elementLocation.elementIndex];
			if (aliveState)
			{
				assert(false && "ObjectPool::addElement() : Reusing alive element.");
				info = NewElementInfo();
				return;
			}

			++page.m_size;

			void* val = &page.m_valsMem[elementLocation.elementIndex * m_elementSize];

			aliveState = true;

			info = NewElementInfo{ index, val };
		}

		NewElementInfo addElementRaw()
		{
			NewElementInfo info;
			addElementRaw(info);

			return info;
		}

		void removeElement(IndexType index)
		{
			assert(m_pageSize != 0);

			ElementLocation elementLocation = getElementLocation(index);

			if (!checkElementLocationWithPage(elementLocation))
			{
				return;
			}

			Page& page = m_pages[elementLocation.pageIndex];
			if (!page.isAllocated())
			{
				return;
			}

			bool& aliveState = page.m_aliveMem[elementLocation.elementIndex];
			if (!aliveState)
			{
				return;
			}

			m_freeIndices.push_back(index);
			aliveState = false;

			assert(page.m_size > 0);

			--page.m_size;
			if (page.m_size == 0)
			{
				++m_emptyPages;
				if (m_emptyPages > m_maxFreePageCount)
				{
					page.deallocate();
				}
			}
		}
	};
}