#pragma once

#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	enum class ShaderType
	{
		Vertex,
		Pixel,

		Compute
	};

	class IShader : public IGraphicObject
	{
	public:
		IShader() = default;
		virtual ~IShader() {}

		virtual ShaderType getShaderType() const = 0;
	};

	class IVertexShader : public IShader
	{
	public:
		IVertexShader() = default;

		virtual ShaderType getShaderType() const override { return ShaderType::Vertex; }
	};

	class IPixelShader : public IShader
	{
	public:
		IPixelShader() = default;

		virtual ShaderType getShaderType() const override { return ShaderType::Pixel; }
	};

	class IComputeShader : public IShader
	{
	public:
		IComputeShader() = default;

		virtual ShaderType getShaderType() const override { return ShaderType::Compute; }
	};
}