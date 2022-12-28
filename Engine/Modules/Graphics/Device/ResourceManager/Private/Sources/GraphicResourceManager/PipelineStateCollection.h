#pragma once

#include "ManagedResourceHandles.h"
#include "GraphicPipeline/IPipelineState.h"
#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"

#include <unordered_map>

namespace VT
{
	class PipelineStateCollection final
	{
	public:
		using PipelineStatePoolHandle = ObjectPoolHandle32;

		struct PipelineStateAccessInfo final
		{
			PipelineStateReference m_state = nullptr;
			bool m_isNew = false;
		};

	private:
		using PipelineStatePool = ObjectPool<ManagedPipelineStateResourceHandle, PipelineStatePoolHandle>;
		using PipelineStateHandleInfo = PipelineStatePool::NewElementInfo;
		using HashCotainer = std::unordered_map<PipelineStateHash, PipelineStateReference>;

		PipelineStatePool m_states;
		HashCotainer m_stateIDs;
		mutable SharedMutex m_lockMutex;

		PipelineStateReference addPipelineStateInternal(PipelineStateHash hash);
		PipelineStateReference getPipelineStateInternal(PipelineStateHash hash);
		PipelineStateConstReference getPipelineStateInternal(PipelineStateHash hash) const;

	public:
		PipelineStateCollection() = default;
		~PipelineStateCollection() { release(); }

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		bool isValid(PipelineStatePoolHandle handle) const;

		PipelineStateConstReference getPipelineState(PipelineStateHash hash) const;
		PipelineStateReference getPipelineState(PipelineStateHash hash);

		PipelineStateReference addPipelineState(PipelineStateHash hash);

		PipelineStateAccessInfo getOrAddPipelineState(PipelineStateHash hash);

		void removePipelineState(PipelineStateHash hash);
	};
}
