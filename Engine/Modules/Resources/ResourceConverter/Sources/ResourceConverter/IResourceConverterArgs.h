#pragma once

#include "Core/HashedType.h"

namespace VT
{
	using ResourceConverterArgsType = HashedType::Type;
	using ResourceConverterArgsHash = uint32_t;

	class IResourceConverterArgs : public HashedType
	{
	public:
		IResourceConverterArgs() = default;

		virtual ResourceConverterArgsHash getHash() const = 0;
		virtual ResourceConverterArgsType getType() const = 0;
	};
}

#define VT_RESOURCE_CONVERTER_ARGS_TYPE(ARGS_TYPE) VT_HASH_TYPE(#ARGS_TYPE, ResourceConverterArgsType, ResourceConverterArgs)