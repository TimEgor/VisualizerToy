#pragma once

#include "ITexture2D.h"

namespace VT
{
	class Texture2DBase : public ITexture2D
	{
	protected:
		const Texture2DDesc m_desc;

	public:
		Texture2DBase(const Texture2DDesc& desc)
			: m_desc(desc) {}

		virtual const Texture2DDesc& getDesc() const override { return m_desc; }
	};
}