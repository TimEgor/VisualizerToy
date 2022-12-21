#pragma once

#include "ObjectPoolHandle.h"

#include "Core/UtilitiesMacros.h"
#include "Core/Memory.h"

#include <cassert>
#include <vector>
#include <deque>

namespace VT
{
	template <typename ValType, typename HandleType>
	class ObjectPool final
	{
		using KeyType = typename HandleType::KeyType;
		using IndexType = typename HandleType::IndexType;
		using VersionType = typename HandleType::VersionType;

		static constexpr VersionType InvalidKey = typename HandleType::InvalidKey;
		static constexpr VersionType InvalidVersion = typename HandleType::InvalidVersion;
		static constexpr HandleType InvalidHandle = HandleType(InvalidKey);

		struct Page final
		{
			void* m_mem = nullptr;
			ValType* m_valsMem = nullptr;
			VersionType* m_versionMem = nullptr;
			bool* m_aliveMem = nullptr;

			size_t m_capacity = 0;
			size_t m_size = 0;

			Page() = default;
			Page(const Page&) = delete;
			Page(Page&& page)
				: m_mem(nullptr),
				m_valsMem(nullptr),
				m_versionMem(nullptr),
				m_aliveMem(nullptr),
				m_capacity(page.m_capacity),
				m_size(page.m_size)
			{
				std::swap(m_mem, page.m_mem);
				std::swap(m_valsMem, page.m_valsMem);
				std::swap(m_versionMem, page.m_versionMem);
				std::swap(m_aliveMem, page.m_aliveMem);
				page.m_capacity = 0;
				page.m_size = 0;
			}

			~Page() { deallocate(); }

			Page& operator=(const Page&) = delete;
			Page& operator=(Page&&) = delete;

			void allocate(size_t capacity)
			{
				size_t valueMemSize = align(sizeof(ValType) * capacity, sizeof(void*));
				size_t versionMemSize = align(sizeof(VersionType) * capacity, sizeof(void*));
				size_t aliveMemSize = sizeof(bool) * capacity;

				size_t memSize = align(valueMemSize + versionMemSize + aliveMemSize, sizeof(void*));
				uint8_t* mem = new uint8_t[memSize];

				m_mem = mem;
				m_valsMem = reinterpret_cast<ValType*>(mem);
				m_versionMem = reinterpret_cast<VersionType*>(mem + valueMemSize);
				m_aliveMem = reinterpret_cast<bool*>(mem + valueMemSize + versionMemSize);

				m_capacity = capacity;
				m_size = 0;

				assert(m_mem);
				assert(capacity);

				memset(m_versionMem, InvalidVersion, versionMemSize);
				memset(m_aliveMem, 0, aliveMemSize);
			}

			void deallocate()
			{
				VT_SAFE_DESTROY_ARRAY(m_mem);

				m_valsMem = nullptr;
				m_versionMem = nullptr;
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
		using HandleElementType = HandleType;

		struct NewElementInfo final
		{
			HandleType m_elementHandle = InvalidHandle;
			ValType* m_elementPtr = nullptr;
		};

	private:
		std::vector<Page> m_pages;
		std::deque<IndexType> m_freeIndices;
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

		ValType* getElementInternal(HandleType handle)
		{
			assert(m_pageSize != 0);

			VersionType version = handle.getVersion();
			if (version == InvalidVersion)
			{
				return nullptr;
			}

			IndexType index = handle.getIndex();
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

			VersionType lastElementVersion = page.m_versionMem[elementLocation.elementIndex];

			if (lastElementVersion != version)
			{
				return nullptr;
			}

			return &page.m_valsMem[elementLocation.elementIndex];
		}

	public:
		ObjectPool() = default;
		ObjectPool(const ObjectPool&) = delete;
		ObjectPool(ObjectPool&& pool)
			: m_size(pool.m_size),
			m_emptyPages(pool.m_emptyPages),
			m_maxUsedIndex(pool.m_maxUsedIndex),
			m_pageSize(pool.m_pageSize),
			m_maxFreePageCount(pool.m_maxFreePageCount),
			m_minFreeIndexCount(pool.m_minFreeIndexCount)
		{
			std::swap(m_pages, pool.m_pages);
			std::swap(m_freeIndices, pool.m_freeIndices);
			pool.m_size = 0;
			pool.m_emptyPages = 0;
			pool.m_maxUsedIndex = 0;

			pool.m_pageSize = 0;
			pool.m_maxFreePageCount = 0;
			pool.m_minFreeIndexCount = 0;
		}
		~ObjectPool() { release(); }


		bool init(size_t pageSize = 4096, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64)
		{
			assert(m_pageSize == 0);

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

				for (size_t elementIndex = 0; elementIndex < page.m_size; ++elementIndex)
				{
					if (!page.m_aliveMem[elementIndex])
					{
						continue;
					}

					if constexpr (!std::is_trivially_destructible<ValType>::value)
					{
						ValType* val = &page.m_valsMem[elementIndex];
						val->~ValType();
					}
				}

				page.deallocate();
			}
		}

		bool isValid(HandleType handle) const
		{
			assert(m_pageSize != 0);

			VersionType version = handle.getVersion();
			if (version == InvalidVersion)
			{
				return false;
			}

			IndexType index = handle.getIndex();
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

			VersionType lastElementVersion = page.m_versionMem[elementLocation.elementIndex];

			return lastElementVersion == version;
		}

		const ValType* getElement(HandleType handle) const
		{
			return const_cast<ObjectPool*>(this)->getElementInternal(handle);
		}

		ValType* getElement(HandleType handle)
		{
			return getElementInternal(handle);
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
				page.allocate(m_pageSize);
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

			ValType* val = &page.m_valsMem[elementLocation.elementIndex];

			VersionType& version = page.m_versionMem[elementLocation.elementIndex];

			++version;
			if (version == InvalidVersion)
			{
				++version;
			}

			aliveState = true;

			info = NewElementInfo{ HandleType(index, version), val };
		}

		NewElementInfo addElementRaw()
		{
			NewElementInfo info;
			addElementRaw(info);

			return info;
		}

		template <typename... Args>
		void addElement(NewElementInfo& info, Args&&... args)
		{
			addElementRaw(info);
			new (info.m_elementPtr) ValType(args...);
		}

		template <typename... Args>
		NewElementInfo addElement(Args&&... args)
		{
			NewElementInfo info = addElementRaw();
			new (info.m_elementPtr) ValType(args...);

			return info;
		}

		void removeElement(HandleType handle)
		{
			assert(m_pageSize != 0);

			VersionType version = handle.getVersion();
			if (version == InvalidVersion)
			{
				return;
			}

			IndexType index = handle.getIndex();
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

			VersionType lastElementVersion = page.m_versionMem[elementLocation.elementIndex];
			if (lastElementVersion != version)
			{
				return;
			}

			if constexpr (!std::is_trivially_destructible<ValType>::value)
			{
				ValType* val = &page.m_valsMem[elementLocation.elementIndex];
				val->~ValType();
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