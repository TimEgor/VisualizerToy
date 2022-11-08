#pragma once

#include <cstdint>

namespace VT
{
	template <typename ResourceType, typename ResourceHandle>
	class IGraphicResourceContainer
	{
	public:
		using GraphicResourceType = ResourceType;
		using GraphicResourceHandle = ResourceHandle;

		static constexpr GraphicResourceHandle InvalidGraphicResourceHandle = 0;

		struct NewResourceInfo final
		{
			ResourceHandle m_resourceHandle = InvalidGraphicResourceHandle;
			ResourceType* m_resourcePtr = nullptr;
		};

	public:
		IGraphicResourceContainer() = default;
		virtual ~IGraphicResourceContainer() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void addResource(NewResourceInfo& info) = 0;
		virtual void removeResource(ResourceHandle handle) = 0;

		virtual ResourceType* getResource(ResourceHandle handle) = 0;

		virtual bool isValidResourceHandle(ResourceHandle handle) const = 0;
	};
}