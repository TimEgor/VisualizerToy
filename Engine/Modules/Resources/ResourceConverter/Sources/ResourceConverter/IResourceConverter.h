#pragma once

namespace VT
{
	class IResourceConverter
	{
	public:
		IResourceConverter() = default;
		virtual ~IResourceConverter() {}

		virtual bool init() = 0;
		virtual void release() = 0;
	};
}