#pragma once

#include "ITextureViews.h"

namespace VT
{
	class Texture2DViewBase : public ITexture2DView
	{
	private:
		TextureViewDesc m_desc;

	public:
		Texture2DViewBase(const TextureViewDesc& desc)
			: m_desc(desc) {}

		virtual const TextureViewDesc& getDesc() const override { return m_desc; }
	};
}
