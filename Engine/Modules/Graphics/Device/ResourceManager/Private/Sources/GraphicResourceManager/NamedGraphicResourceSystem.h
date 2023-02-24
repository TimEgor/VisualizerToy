#pragma once

#include "GraphicResourceManager.h"
#include "NamedReferencePool/NamedObjectPool.h"

namespace VT
{
	class NamedGraphicResourceSystem final : public GraphicResourceManager
	{
		friend NamedManagedVertexShaderGraphicObjectHandle;
		friend NamedManagedPixelShaderGraphicObjectHandle;
		friend NamedManagedComputeShaderGraphicObjectHandle;

		using NamedVertexShaderPool = NamedObjectPool<NamedManagedVertexShaderGraphicObjectHandle,
			VertexShaderReference, NamedObjectPoolHandle32>;
		using NamedPixelShaderPool = NamedObjectPool<NamedManagedPixelShaderGraphicObjectHandle,
			PixelShaderReference, NamedObjectPoolHandle32>;
		using NamedComputeShaderPool = NamedObjectPool<NamedManagedComputeShaderGraphicObjectHandle,
			ComputeShaderReference, NamedObjectPoolHandle32>;

	private:
		NamedVertexShaderPool m_namedVertexShaderPool;
		NamedPixelShaderPool m_namedPixelShaderPool;
		NamedComputeShaderPool m_namedComputeShaderPool;

		void deleteVertexShaderReference(GraphicResourceNameID nameID);
		void deletePixelShaderReference(GraphicResourceNameID nameID);
		void deleteComputeShaderReference(GraphicResourceNameID nameID);

	public:
		NamedGraphicResourceSystem() = default;

		virtual bool init() override;
		virtual void release() override;

		//Vertex Shader
		virtual VertexShaderReference getVertexShader(VertexShaderHandleID handle) override;
		virtual bool isValidVertexShader(VertexShaderHandleID handle) const override;

		virtual VertexShaderReference getNamedVertexShader(const GraphicResourceName& name) override;
		virtual VertexShaderReference getNamedVertexShader(GraphicResourceNameID handle) override;
		virtual bool isValidNamedVertexShader(GraphicResourceNameID handle) const override;

		virtual VertexShaderReference loadVertexShader(const GraphicResourceName& shaderPath) override;
		virtual VertexShaderReference loadVertexShaderAsync(const GraphicResourceName& shaderPath,
			OnLoadedVertexShaderCallback callback) override;

		//Pixel Shader
		virtual PixelShaderReference getPixelShader(PixelShaderHandleID handle) override;
		virtual bool isValidPixelShader(PixelShaderHandleID handle) const override;

		virtual PixelShaderReference getNamedPixelShader(const GraphicResourceName& name) override;
		virtual PixelShaderReference getNamedPixelShader(GraphicResourceNameID handle) override;
		virtual bool isValidNamedPixelShader(GraphicResourceNameID handle) const override;

		virtual PixelShaderReference loadPixelShader(const GraphicResourceName& shaderPath) override;
		virtual PixelShaderReference loadPixelShaderAsync(const GraphicResourceName& shaderPath,
			OnLoadedPixelShaderCallback callback) override;

		//Compute Shader
		virtual ComputeShaderReference getComputeShader(ComputeShaderHandleID handle) override;
		virtual bool isValidComputeShader(ComputeShaderHandleID handle) const override;

		virtual ComputeShaderReference getNamedComputeShader(const GraphicResourceName& name) override;
		virtual ComputeShaderReference getNamedComputeShader(GraphicResourceNameID handle) override;
		virtual bool isValidNamedComputeShader(GraphicResourceNameID handle) const override;

		virtual ComputeShaderReference loadComputeShader(const GraphicResourceName& shaderPath) override;
		virtual ComputeShaderReference loadComputeShaderAsync(const GraphicResourceName& shaderPath,
			OnLoadedComputeShaderCallback callback) override;
	};
}