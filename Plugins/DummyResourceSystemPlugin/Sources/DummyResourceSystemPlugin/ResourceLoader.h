#pragma once

#include "ResourceSystem/IResourceSystem.h"

namespace VT_DUMMY_RS
{
	class ManagedResourceData;

	class ResourceLoader final
	{
	private:


	public:
		ResourceLoader() = default;
		~ResourceLoader() { release(); }

		bool init();
		void release();

		void loadResource(const VT::FileName& name, ManagedResourceData& data);
	};
}