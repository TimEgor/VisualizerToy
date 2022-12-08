#pragma once

namespace VT
{
	enum class ShaderStageType
	{
		Vertex,
		Pixel
	};

	class IShader
	{
	public:
		IShader() = default;
		virtual ~IShader() {}
	};

	class IVertexShader : public IShader
	{
	public:
		IVertexShader() = default;
	};

	class IPixelShader : public IShader
	{
	public:
		IPixelShader() = default;
	};
}