#pragma once

#include <cstdint>

namespace VT
{
	template <typename ResourceType, typename GraphicResourceHandle>
	class IGraphicResourceContainer
	{
	public:
		static constexpr GraphicResourceHandle InvalidGraphicResourceHandle = 0;

		struct NewResourceInfo final
		{
			GraphicResourceHandle m_resourceHandle = InvalidGraphicResourceHandle;
			ResourceType* m_resourcePtr = nullptr;
		};

	public:
		IGraphicResourceContainer() = default;
		virtual ~IGraphicResourceContainer() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void addResource(NewResourceInfo& info) = 0;
		virtual void removeResource(GraphicResourceHandle handle) = 0;

		virtual ResourceType* getResource(GraphicResourceHandle handle) = 0;

		virtual bool isValidResourceHandle(GraphicResourceHandle handle) const = 0;
	};
}