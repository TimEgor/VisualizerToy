#include "D3D12RenderContext.h"

#include "Core/UtilitiesMacros.h"

#include "D3D12GraphicsPlugin/Commands/D3D12GraphicsCommandList.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineState.h"
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

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> targetHandles;
	targetHandles.reserve(info.m_targets.size());

	for (const auto& target : info.m_targets)
	{
		D3D12ResourceDescriptor* d3d12Descriptor = reinterpret_cast<D3D12ResourceDescriptor*>(target.m_view);
		targetHandles.push_back(d3d12Descriptor->getCPUHandle());
	}

	d3d12CommandList->OMSetRenderTargets(targetHandles.size(), targetHandles.data(), false, nullptr);

	float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	for (const auto& handle : targetHandles)
	{
		d3d12CommandList->ClearRenderTargetView(handle, clearColor, 0, nullptr);
	}
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

void VT_D3D12::D3D12RenderContext::setPipelineState(VT::IPipelineState* pipelineState)
{
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
}

void VT_D3D12::D3D12RenderContext::drawIndexed(uint32_t indexCount)
{
}
