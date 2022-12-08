#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	using ResourceConverterArgsType = HashTyped::Type;
	using ResourceConverterArgsHash = uint32_t;

	class IResourceConverterArgs : public HashTyped
	{
	public:
		IResourceConverterArgs() = default;

		virtual ResourceConverterArgsHash getHash() const = 0;
		virtual ResourceConverterArgsType getType() const = 0;
	};
}

#define VT_RESOURCE_CONVERTER_ARGS_TYPE(ARGS_TYPE) VT_HASH_TYPE(#ARGS_TYPE, ResourceConverterArgsType, ResourceConverterArgs)