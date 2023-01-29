#pragma once

#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	enum class ShaderStageType
	{
		Vertex,
		Pixel
	};

	class IShader : public IGraphicObject
	{
	public:
		IShader() = default;
		virtual ~IShader() {}

		virtual ShaderStageType getStageType() const = 0;
	};

	class IVertexShader : public IShader
	{
	public:
		IVertexShader() = default;

		virtual ShaderStageType getStageType() const override { return ShaderStageType::Vertex; }
	};

	class IPixelShader : public IShader
	{
	public:
		IPixelShader() = default;

		virtual ShaderStageType getStageType() const override { return ShaderStageType::Pixel; }
	};
}