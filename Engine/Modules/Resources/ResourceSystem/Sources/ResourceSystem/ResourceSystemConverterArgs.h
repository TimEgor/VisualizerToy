#pragma once

#include "Core/Paterns/Singleton.h"
#include "Multithreading/CountableObject/CountableReference.h"
#include "ResourceConverter/IResourceConverterArgs.h"

namespace VT
{
	class IResourceConverterArgs;

	class ResourceSystemConverterArgs : public CountableObjectBase
	{
	public:
		ResourceSystemConverterArgs() = default;

		virtual IResourceConverterArgs* getArgs() const = 0;

		template <typename ArgsType>
		ArgsType* getArgsCast() { return reinterpret_cast<ArgsType*>(getArgs()); }
		template <typename ArgsType>
		const ArgsType* getArgsCast() const { return reinterpret_cast<const ArgsType*>(getArgs()); }
	};

	COUNTABLE_REFERENCES_DECLARATION(ResourceSystemConverterArgs);

	class IResourceSystemConverterArgsDestructor
	{
	public:
		IResourceSystemConverterArgsDestructor() = default;
		virtual ~IResourceSystemConverterArgsDestructor() = default;

		virtual void destroy(IResourceConverterArgs* args) = 0;
	};

	template <typename ArgsType>
	class ResourceSystemConverterArgsHandlerUtil final
		: public Singleton<ResourceSystemConverterArgsHandlerUtil<ArgsType>>, public IResourceSystemConverterArgsDestructor
	{
	public:
		ResourceSystemConverterArgsHandlerUtil() = default;

		virtual void destroy(IResourceConverterArgs* args) override { destroyArgs(args); }

		template <typename... Args>
		static ArgsType* createArgs(IResourceConverterArgs* argsPtr, Args... args)
		{
			return new (argsPtr) ArgsType(args...);
		}

		static void destroyArgs(IResourceConverterArgs* args)
		{
			reinterpret_cast<ArgsType*>(args)->~ArgsType();
		}
	};
}