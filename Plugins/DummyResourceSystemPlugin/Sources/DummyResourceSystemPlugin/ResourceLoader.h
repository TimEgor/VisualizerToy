#pragma once

#include "ResourceSystem/IResourceSystem.h"

namespace VT_DUMMY_RS
{
	class ManagedResourceData;
	class DummyResourceSystem;

	class ResourceLoader final
	{
	private:
		DummyResourceSystem* m_resourceSystem = nullptr;

	public:
		ResourceLoader() = default;
		~ResourceLoader() { release(); }

		bool init(DummyResourceSystem* resourceSystem);
		void release();

		void loadResource(const VT::FileName& name, VT::ResourceDataReference dataRef);
		void loadResourceAsync(const VT::FileName& name, VT::ResourceDataReference dataRef);
	};
}