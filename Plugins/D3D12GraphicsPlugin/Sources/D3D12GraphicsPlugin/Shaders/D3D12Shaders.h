#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedShaderObjects.h"

namespace VT_D3D12
{
	class D3D12ShaderBase
	{
	public:
		using CodeBlobComPtr = D3D12BlobComPtr;

	private:
		CodeBlobComPtr m_dxcShaderCodeBlob = nullptr;

	public:
		D3D12ShaderBase(CodeBlobComPtr dxcCodeBlob)
			: m_dxcShaderCodeBlob(dxcCodeBlob) {}
		virtual ~D3D12ShaderBase() = default;

		CodeBlobComPtr getDXCShaderCodeBlob() const { return m_dxcShaderCodeBlob; }
		CodeBlobComPtr& getDXCShaderCodeBlobRef() { return m_dxcShaderCodeBlob; }
	};

	class D3D12VertexShader final : public VT::ManagedGraphicDevice::ManagedVertexShaderBase, public D3D12ShaderBase
	{
	public:
		D3D12VertexShader(CodeBlobComPtr codeBlob)
			: D3D12ShaderBase(codeBlob) {}

		virtual void* getNativeHandle() const override { return getDXCShaderCodeBlob().Get(); }
	};

	class D3D12PixelShader final : public VT::ManagedGraphicDevice::ManagedPixelShaderBase, public D3D12ShaderBase
	{
	public:
		D3D12PixelShader(CodeBlobComPtr codeBlob)
			: D3D12ShaderBase(codeBlob) {}

		virtual void* getNativeHandle() const override { return getDXCShaderCodeBlob().Get(); }
	};

	class D3D12ComputeShader final : public VT::ManagedGraphicDevice::ManagedComputeShaderBase, public D3D12ShaderBase
	{
	public:
		D3D12ComputeShader(CodeBlobComPtr codeBlob)
			: D3D12ShaderBase(codeBlob) {}

		virtual void* getNativeHandle() const override { return getDXCShaderCodeBlob().Get(); }
	};
}
