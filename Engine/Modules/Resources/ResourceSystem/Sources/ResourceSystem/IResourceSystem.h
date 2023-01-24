#pragma once

#include "ResourceData.h"
#include "ResourceDependencyState.h"

#include "ResourceConverter/IResourceConverter.h"
#include "ResourceSystemConverterArgs.h"

#include "Core/HashFunctions/CRC32.h"
#include "Core/FileName/FileName.h"

#include <functional>
#include <type_traits>
#include <cassert>

namespace VT
{
	using ResourceSystemType = uint32_t;

	class IResourceSystem
	{
	public:
		using LoadingResourceCallback = std::function<void(ResourceDataReference)>;

	protected:
		virtual ResourceSystemConverterArgs* allocateResourceConverterArgs(ResourceConverterType type) = 0;
		virtual void addResourceConverterArgs(ResourceConverterType type,
			IResourceSystemConverterArgsDestructor* destructor, size_t argsSize) = 0;

	public:
		IResourceSystem() = default;
		virtual ~IResourceSystem() {}

		virtual bool init(const FileName& path) = 0;
		virtual void release() = 0;

		virtual void addResourceConverter(IFileResourceConverter* converter) = 0;
		virtual void removeResourceConverter(ResourceConverterType converterType) = 0;

		virtual ResourceDataReference getResource(const FileName& resName, ResourceSystemConverterArgsReference args = nullptr) = 0;
		virtual void getResourceAsync(const FileName& resName, const LoadingResourceCallback& callback = nullptr, ResourceSystemConverterArgsReference args = nullptr) = 0;

		virtual ResourceDependencyStateReference createResourceDependencyState(const ResourceDependencyState::Callback& callback) = 0;

		template <typename ConverterArgsType>
		void addResourceConverterArgsType()
		{
			static_assert(std::is_base_of<IResourceConverterArgs, ConverterArgsType>::value);

			addResourceConverterArgs(ConverterArgsType::getResourceConverterArgsType(),
				&ResourceSystemConverterArgsHandlerUtil<ConverterArgsType>::getInstance(), sizeof(ConverterArgsType));
		}

		template <typename ConverterArgsType, typename ...Args>
		ResourceSystemConverterArgsReference createResourceConverterArgs(Args... args)
		{
			static_assert(std::is_base_of<IResourceConverterArgs, ConverterArgsType>::value);

			ResourceSystemConverterArgs* argsHolderPtr
				= allocateResourceConverterArgs(ConverterArgsType::getResourceConverterArgsType());
			assert(argsHolderPtr);

			ConverterArgsType* converterArgs = argsHolderPtr->getArgsCast<ConverterArgsType>();
			ResourceSystemConverterArgsHandlerUtil<ConverterArgsType>::createArgs(converterArgs, args...);

			return argsHolderPtr;
		}

		virtual ResourceSystemType getType() const = 0;
	};
}

#define VT_RESOURCE_SYSTEM_TYPE(RES_SYSTEM_TYPE)																	\
	static inline constexpr VT::ResourceSystemType getResourceSystemType() { return VT::crc32(#RES_SYSTEM_TYPE); }	\
	virtual VT::ResourceSystemType getType() const override { return getResourceSystemType(); }