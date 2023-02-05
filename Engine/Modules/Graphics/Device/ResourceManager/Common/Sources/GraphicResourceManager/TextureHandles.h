#pragma once

#include "ResourceHandleDecalaration.h"
#include "Textures/TextureCommon.h"

namespace VT
{
	using TextureHandleID = GraphicResourceHandleID;

	class TextureResourceHandle : public GraphicTypedResourceHandle<ITexture>
	{
	public:
		TextureResourceHandle() = default;
		TextureResourceHandle(ITexture* texture, GraphicResourceHandleID handleID)
			: GraphicTypedResourceHandle(texture, handleID) {}

		ITexture* getTexture() const { return getTypedResource(); }

		template <typename TextureType>
		TextureType* getTextureCast() const
		{
			static_assert(std::is_base_of_v<ITexture, TextureType>);

			return reinterpret_cast<TextureType*>(getTexture());
		}
	};

	template <typename TextureType>
	class TypedTextureResourceHandle : public TextureResourceHandle
	{
	public:
		using HandleObjectType = TextureType;

	public:
		TypedTextureResourceHandle() = default;
		TypedTextureResourceHandle(TextureType* texture, GraphicResourceHandleID handleID)
			: TextureResourceHandle(texture, handleID) {}

		TextureType* getTypeTexture() const { return getTextureCast<TextureType>(); }
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(TextureResourceHandle, Texture)

#define TYPED_TEXTURE_RESOURCE(NAME, TEXTURE_INTERFACE_TYPE)							\
	class TEXTURE_INTERFACE_TYPE;														\
	using NAME##ResourceHandle = TypedTextureResourceHandle<TEXTURE_INTERFACE_TYPE>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##ResourceHandle, NAME)
}