#pragma once

#pragma once

#include "ManagedObjectHandles/ManagedInputLayoutHandle.h"
#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"

#include <unordered_map>


namespace VT
{
	class InputLayoutCollection final
	{
		friend class GraphicResourceManager;

	public:
		using InputLayoutPoolHandle = ObjectPoolHandle16;

		struct InputLayoutAccessInfo final
		{
			InputLayoutReference m_layout = nullptr;
			bool m_isNew = false;
		};

	private:
		using InputLayoutPool = ObjectPool<ManagedInputLayoutHandle, InputLayoutPoolHandle>;
		using InputLayoutHandleInfo = InputLayoutPool::NewElementInfo;
		using HashCotainer = std::unordered_map<InputLayoutHash, InputLayoutReference>;

		InputLayoutPool m_layouts;
		HashCotainer m_layoutIDs;
		mutable SharedMutex m_lockMutex;

		InputLayoutReference addInputLayoutInternal(const InputLayoutDesc& desc);
		InputLayoutReference getInputLayoutInternal(InputLayoutHash hash);
		InputLayoutConstReference getInputLayoutInternal(InputLayoutHash hash) const;

		void removeInputLayoutHandle(InputLayoutHandleID handleID);

	public:
		InputLayoutCollection() = default;
		~InputLayoutCollection() { release(); }

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		InputLayoutReference getInputLayout(InputLayoutHash hash);
		InputLayoutConstReference getInputLayout(InputLayoutHash hash) const;

		InputLayoutReference addInputLayout(const InputLayoutDesc& desc);

		void removeInputLayout(InputLayoutHash hash);
	};
}
