#pragma once

#include "ResourceConverter/IResourceConverter.h"
#include "ResourceSystem/ResourceData.h"
#include "ResourceSystem/ResourceSystemConverterArgs.h"

#include <unordered_map>

namespace VT
{
	class IFileSystem;
}

namespace VT_DUMMY_RS
{
	class DummyResourceSystem;

	class ResourceLoader final
	{
		using ConvertersCollection = std::unordered_map<VT::ResourceConverterType, VT::IFileResourceConverter*>;
	public:
		struct Request final
		{
			VT::ResourceDataReference m_data = nullptr;
			VT::ResourceSystemConverterArgsReference m_args = nullptr;
		};

	private:
		ConvertersCollection m_converters;

		DummyResourceSystem* m_resourceSystem = nullptr;
		VT::IFileSystem* m_fileSystem = nullptr;

		void readResourceData(const VT::FileName& name, VT::ResourceDataReference dataRef);
		VT::IFileResourceConverter* chooseConvert(const VT::FileName& name);

	public:
		ResourceLoader() = default;
		~ResourceLoader() { release(); }

		bool init(DummyResourceSystem* resourceSystem);
		void release();

		void loadResource(const VT::FileName& name, Request request);
		void loadResourceAsync(const VT::FileName& name, Request request);

		void addResourceConverter(VT::IFileResourceConverter* converter);
		void removeResourceConverter(VT::ResourceConverterType converterType);
	};
}