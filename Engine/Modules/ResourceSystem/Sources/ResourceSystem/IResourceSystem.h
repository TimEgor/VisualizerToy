#pragma once

#include <mutex>

namespace VT
{
	struct ResourceData final
	{
		void* m_data;
		size_t m_dataSize;
	};

	class IResourceSystem
	{
	public:
		IResourceSystem() = default;
		virtual ~IResourceSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual ResourceData getResource(const char*) = 0;
	};
}