#include "D3D12RenderContext.h"

#include "Core/UtilitiesMacros.h"

#include "D3D12GraphicsPlugin/Commands/D3D12GraphicsCommandList.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineState.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineBindingLayout.h"
#include "D3D12GraphicsPlugin/Textures/D3D12Texture2D.h"
#include "D3D12GraphicsPlugin/Buffer/D3D12GPUBuffer.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptor.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptorHeap.h"
#include "D3D12GraphicsPlugin/Utilities/InputLayoutConverter.h"

bool VT_D3D12::D3D12RenderContext::init(VT::ICommandList* commandList)
{
	m_commandList = reinterpret_cast<D3D12GraphicsCommandList*>(commandList);
	VT_CHECK_INITIALIZATION(m_commandList);

	return true;
}

void VT_D3D12::D3D12RenderContext::release()
{
}

VT::ICommandList* VT_D3D12::D3D12RenderContext::getCommandList()
{
	return m_commandList;
}

void VT_D3D12::D3D12RenderContext::begin()
{
	m_commandList->reset();
}

void VT_D3D12::D3D12RenderContext::end()
{
	m_commandList->close();
}

void VT_D3D12::D3D12RenderContext::beginRendering(const VT::RenderContextBeginInfo& info)
{
	D3D12GraphicsCommandListComPtr d3d12CommandList = m_commandList->getD3D12CommandList();

	const size_t targetsCount = info.m_targets.size();

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> targetHandles;
	std::vector<D3D12_VIEWPORT> viewports;
	std::vector<D3D12_RECT> scissors;

	targetHandles.reserve(targetsCount);
	viewports.reserve(targetsCount);
	scissors.reserve(targetsCount);

	for (const auto& target : info.m_targets)
	{
		D3D12_VIEWPORT d3d12Viewport{};
		d3d12Viewport.TopLeftX = static_cast<float>(target.m_viewport.m_x);
		d3d12Viewport.TopLeftY = static_cast<float>(target.m_viewport.m_y);
		d3d12Viewport.Width = static_cast<float>(target.m_viewport.m_width);
		d3d12Viewport.Height = static_cast<float>(target.m_viewport.m_height);
		d3d12Viewport.MaxDepth = 1.0f;
		d3d12Viewport.MinDepth = 0.0f;

		D3D12_RECT d3d12Scissors{};
		d3d12Scissors.left = target.m_scissors.m_left;
		d3d12Scissors.top = target.m_scissors.m_top;
		d3d12Scissors.right = target.m_scissors.m_right;
		d3d12Scissors.bottom = target.m_scissors.m_bottom;

		D3D12ResourceDescriptor* d3d12Descriptor = reinterpret_cast<D3D12ResourceDescriptor*>(target.m_view);
		targetHandles.push_back(d3d12Descriptor->getCPUHandle());
		viewports.push_back(d3d12Viewport);
		scissors.push_back(d3d12Scissors);
	}

	d3d12CommandList->OMSetRenderTargets(targetHandles.size(), targetHandles.data(), false, nullptr);

	float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	for (const auto& handle : targetHandles)
	{
		d3d12CommandList->ClearRenderTargetView(handle, clearColor, 0, nullptr);
	}

	//TMP
	d3d12CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3d12CommandList->RSSetViewports(targetsCount, viewports.data());
	d3d12CommandList->RSSetScissorRects(targetsCount, scissors.data());
}

void VT_D3D12::D3D12RenderContext::endRendering()
{
}

void VT_D3D12::D3D12RenderContext::prepareTextureForRendering(VT::ITexture2D* texture)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = reinterpret_cast<D3D12Texture2D*>(texture)->getD3D12Resource().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_commandList->getD3D12CommandList()->ResourceBarrier(1, &barrier);
}

void VT_D3D12::D3D12RenderContext::prepareTextureForPresenting(VT::ITexture2D* texture)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = reinterpret_cast<D3D12Texture2D*>(texture)->getD3D12Resource().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_commandList->getD3D12CommandList()->ResourceBarrier(1, &barrier);
}

void VT_D3D12::D3D12RenderContext::setDescriptorHeap(VT::IGraphicResourceDescriptorHeap* heap)
{
	D3D12ResourceDescriptorHeap* d3d12Heap = reinterpret_cast<D3D12ResourceDescriptorHeap*>(heap);
	m_commandList->getD3D12CommandList()->SetDescriptorHeaps(1, d3d12Heap->getD3D12DescriptorHeap().GetAddressOf());
}

void VT_D3D12::D3D12RenderContext::setBindingParameterValue(uint32_t index, uint32_t offset, uint32_t value)
{
	m_commandList->getD3D12CommandList()->SetGraphicsRoot32BitConstant(index, value, offset);
}

void VT_D3D12::D3D12RenderContext::setBindingParameterValues(uint32_t index, uint32_t offset, uint32_t valuesCount, uint32_t* values)
{
	m_commandList->getD3D12CommandList()->SetGraphicsRoot32BitConstants(index, valuesCount, values, offset);
}

void VT_D3D12::D3D12RenderContext::setBindingLayout(const VT::IPipelineBindingLayout* bindingLayout)
{
	assert(bindingLayout);

	const D3D12PipelineBindingLayout* d3d12PipelineBindingLayout = reinterpret_cast<const D3D12PipelineBindingLayout*>(bindingLayout);
	m_commandList->getD3D12CommandList()->SetGraphicsRootSignature(d3d12PipelineBindingLayout->getD3D12RootSignature().Get());
}

void VT_D3D12::D3D12RenderContext::setPipelineState(const VT::IPipelineState* pipelineState)
{
	assert(pipelineState);

	const D3D12PipelineState* d3d12PipelineState = reinterpret_cast<const D3D12PipelineState*>(pipelineState);
	m_commandList->getD3D12CommandList()->SetPipelineState(d3d12PipelineState->getD3D12Pipeline().Get());
}

void VT_D3D12::D3D12RenderContext::setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer** buffers,
	const VT::InputLayoutDesc& inputLayoutDesc)
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> d3d12VertexViews(buffersCount);

	for (uint32_t bufferIndex = 0; bufferIndex < buffersCount; ++bufferIndex)
	{
		D3D12GPUBuffer* d3d12GPUBuffer = reinterpret_cast<D3D12GPUBuffer*>(buffers[bufferIndex]);

		const uint32_t bindingSlot = inputLayoutDesc.m_elements[bufferIndex].m_slot;
		const VT::InputLayoutBindingDesc& bindingDesc = inputLayoutDesc.m_bindings[bindingSlot];

		D3D12_VERTEX_BUFFER_VIEW& d3d12VertexView = d3d12VertexViews[bufferIndex];
		d3d12VertexView.BufferLocation = d3d12GPUBuffer->getD3D12Resource()->GetGPUVirtualAddress();
		d3d12VertexView.SizeInBytes = d3d12GPUBuffer->getDesc().m_byteSize;
		d3d12VertexView.StrideInBytes = bindingDesc.m_stride;
	}

	m_commandList->getD3D12CommandList()->IASetVertexBuffers(0, buffersCount, d3d12VertexViews.data());
}

void VT_D3D12::D3D12RenderContext::setIndexBuffer(VT::IGPUBuffer* buffer, VT::InputLayoutElementType indexType)
{
	D3D12GPUBuffer* d3d12GPUBuffer = reinterpret_cast<D3D12GPUBuffer*>(buffer);

	D3D12_INDEX_BUFFER_VIEW d3d12IndexView{};
	d3d12IndexView.Format = convertInputLayoutFormatVTtoDXGI(indexType, 1);
	d3d12IndexView.BufferLocation = d3d12GPUBuffer->getD3D12Resource()->GetGPUVirtualAddress();
	d3d12IndexView.SizeInBytes = d3d12GPUBuffer->getDesc().m_byteSize;

	m_commandList->getD3D12CommandList()->IASetIndexBuffer(&d3d12IndexView);
}

void VT_D3D12::D3D12RenderContext::draw(uint32_t vertCount)
{
	m_commandList->getD3D12CommandList()->DrawInstanced(vertCount, 1, 0, 0);
}

void VT_D3D12::D3D12RenderContext::drawIndexed(uint32_t indexCount)
{
	m_commandList->getD3D12CommandList()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}
