#include "D3D12RenderContext.h"

#include "Core/UtilitiesMacros.h"

#include "D3D12GraphicsPlugin/Commands/D3D12GraphicsCommandList.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineState.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineBindingLayout.h"
#include "D3D12GraphicsPlugin/Textures/D3D12Texture2D.h"
#include "D3D12GraphicsPlugin/Buffer/D3D12GPUBuffer.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptor.h"

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

void VT_D3D12::D3D12RenderContext::setPipelineState(const VT::IPipelineState* pipelineState, const VT::IPipelineBindingLayout* bindingLayout)
{
	assert(pipelineState && bindingLayout);
	assert(pipelineState->getBindingLayoutHash() == bindingLayout->getHash());

	const D3D12PipelineState* d3d12PipelineState = reinterpret_cast<const D3D12PipelineState*>(pipelineState);
	const D3D12PipelineBindingLayout* d3d12PipelineBindingLayout = reinterpret_cast<const D3D12PipelineBindingLayout*>(bindingLayout);

	D3D12GraphicsCommandListComPtr d3d12CommandList = m_commandList->getD3D12CommandList();
	d3d12CommandList->SetPipelineState(d3d12PipelineState->getD3D12Pipeline().Get());
	d3d12CommandList->SetGraphicsRootSignature(d3d12PipelineBindingLayout->getD3D12RootSignature().Get());
}

void VT_D3D12::D3D12RenderContext::setVertexBuffer(VT::IGPUBuffer* buffer)
{
}

void VT_D3D12::D3D12RenderContext::setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer** buffers)
{
}

void VT_D3D12::D3D12RenderContext::setIndexBuffer(VT::IGPUBuffer* buffer, VT::InputLayoutElementType indexType)
{
}

void VT_D3D12::D3D12RenderContext::draw(uint32_t vertCount)
{
	m_commandList->getD3D12CommandList()->DrawInstanced(vertCount, 1, 0, 0);
}

void VT_D3D12::D3D12RenderContext::drawIndexed(uint32_t indexCount)
{
}
