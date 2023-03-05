#include "D3D12RenderContext.h"

#include "Core/UtilitiesMacros.h"

#include "D3D12GraphicsPlugin/Commands/D3D12GraphicsCommandList.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineState.h"
#include "D3D12GraphicsPlugin/Pipeline/D3D12PipelineBindingLayout.h"
#include "D3D12GraphicsPlugin/Buffer/D3D12GPUBuffer.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptor.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptorHeap.h"
#include "D3D12GraphicsPlugin/Utilities/InputLayoutConverter.h"
#include "D3D12GraphicsPlugin/Utilities/ResourceStateConverter.h"
#include "D3D12GraphicsPlugin/Utilities/PrimitiveTopologyConverter.h"

bool VT_D3D12::D3D12RenderContext::init(VT::ICommandList* commandList)
{
	m_commandList = reinterpret_cast<D3D12GraphicsCommandList*>(commandList);
	VT_CHECK_INITIALIZATION(m_commandList);

	VT_CHECK_INITIALIZATION(m_barrierCache.init(32));

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
	m_barrierCache.flush(m_commandList->getD3D12CommandList());
	m_commandList->close();
}

void VT_D3D12::D3D12RenderContext::clearRenderTarget(const VT::IGraphicResourceDescriptor* renderTargetView,
	const float* clearValues)
{
	m_barrierCache.flush(m_commandList->getD3D12CommandList());

	const D3D12ResourceDescriptor* d3d12Descriptor = reinterpret_cast<const D3D12ResourceDescriptor*>(renderTargetView);
	m_commandList->getD3D12CommandList()->ClearRenderTargetView(d3d12Descriptor->getCPUHandle(), clearValues, 0, nullptr);
}

void VT_D3D12::D3D12RenderContext::clearDepthStencilTarget(const VT::IGraphicResourceDescriptor* depthStencilView,
	float depthClearValue, uint32_t stencilClearValue)
{
	m_barrierCache.flush(m_commandList->getD3D12CommandList());

	const D3D12ResourceDescriptor* d3d12Descriptor = reinterpret_cast<const D3D12ResourceDescriptor*>(depthStencilView);
	m_commandList->getD3D12CommandList()->ClearDepthStencilView(d3d12Descriptor->getCPUHandle(), D3D12_CLEAR_FLAG_DEPTH,
		depthClearValue, stencilClearValue, 0, nullptr);
}

void VT_D3D12::D3D12RenderContext::setRenderTargets(uint32_t count,
                                                    VT::IGraphicResourceDescriptor* const* renderTargetViews, const VT::IGraphicResourceDescriptor* depthStencilView)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3d12RTVHandles[MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < count; ++i)
	{
		const VT::IGraphicResourceDescriptor* rtvDescriptor = renderTargetViews[i];

		const D3D12ResourceDescriptor* d3d12RTDescriptor = reinterpret_cast<const D3D12ResourceDescriptor*>(rtvDescriptor);
		d3d12RTVHandles[i] = d3d12RTDescriptor->getCPUHandle();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE d3d12DSVHandle{0};
	if (depthStencilView)
	{
		const D3D12ResourceDescriptor* d3d12DSDescriptor = reinterpret_cast<const D3D12ResourceDescriptor*>(depthStencilView);
		d3d12DSVHandle = d3d12DSDescriptor->getCPUHandle();
	}

	m_commandList->getD3D12CommandList()->OMSetRenderTargets(static_cast<UINT>(count), d3d12RTVHandles,
		false, depthStencilView ? &d3d12DSVHandle : nullptr);
}

void VT_D3D12::D3D12RenderContext::setViewports(uint32_t count, const VT::Viewport* viewports)
{
	D3D12_VIEWPORT d3d12Viewports[MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < count; ++i)
	{
		const VT::Viewport& viewport = viewports[i];

		D3D12_VIEWPORT& d3d12Viewport = d3d12Viewports[i];
		d3d12Viewport.TopLeftX = static_cast<float>(viewport.m_x);
		d3d12Viewport.TopLeftY = static_cast<float>(viewport.m_y);
		d3d12Viewport.Width = static_cast<float>(viewport.m_width);
		d3d12Viewport.Height = static_cast<float>(viewport.m_height);
		d3d12Viewport.MaxDepth = D3D12_MAX_DEPTH;
		d3d12Viewport.MinDepth = D3D12_MIN_DEPTH;
	}

	m_commandList->getD3D12CommandList()->RSSetViewports(count, d3d12Viewports);
}

void VT_D3D12::D3D12RenderContext::setScissors(uint32_t count, const VT::Scissors* scissors)
{
	D3D12_RECT d3d12Scissors[MAX_RENDER_TARGETS_COUNT];

	for (uint32_t i = 0; i < count; ++i)
	{
		const VT::Scissors& targetScissors = scissors[i];

		D3D12_RECT& d3d12TargetScissors = d3d12Scissors[i];
		d3d12TargetScissors.left = static_cast<long>(targetScissors.m_left);
		d3d12TargetScissors.top = static_cast<long>(targetScissors.m_top);
		d3d12TargetScissors.right = static_cast<long>(targetScissors.m_right);
		d3d12TargetScissors.bottom = static_cast<long>(targetScissors.m_bottom);
	}

	m_commandList->getD3D12CommandList()->RSSetScissorRects(count, d3d12Scissors);
}

void VT_D3D12::D3D12RenderContext::changeResourceState(VT::IGraphicResource* resource,
	VT::GraphicResourceStateValueType currentState, VT::GraphicResourceStateValueType targetState)
{
	if (currentState == targetState)
	{
		return;
	}

	D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_COMMON;
	D3D12_RESOURCE_STATES afterState = D3D12_RESOURCE_STATE_COMMON;

	VT::GraphicResourceType resourceType = resource->getType();
	if (resourceType == VT::IGPUBuffer::getGraphicResourceType())
	{
		beforeState = convertBufferStateVTtoD3D12(currentState);
		afterState = convertBufferStateVTtoD3D12(targetState);
	}
	else if (resourceType == VT::ITexture::getGraphicResourceType())
	{
		beforeState = convertTextureStateVTtoD3D12(currentState);
		afterState = convertTextureStateVTtoD3D12(targetState);
	}

	D3D12_RESOURCE_TRANSITION_BARRIER barrier{};
	barrier.pResource = resource->getNativeHandleCast<ID3D12Resource>();
	barrier.StateBefore = beforeState;
	barrier.StateAfter = afterState;

	m_barrierCache.addBarrier(m_commandList->getD3D12CommandList(), barrier);
}

void VT_D3D12::D3D12RenderContext::setDescriptorHeap(VT::IGraphicResourceDescriptorHeap* heap)
{
	D3D12ResourceDescriptorHeap* d3d12Heap = reinterpret_cast<D3D12ResourceDescriptorHeap*>(heap);
	m_commandList->getD3D12CommandList()->SetDescriptorHeaps(1, d3d12Heap->getD3D12DescriptorHeap().GetAddressOf());
}

void VT_D3D12::D3D12RenderContext::setGraphicBindingParameterValue(uint32_t index, uint32_t offset, uint32_t value)
{
	m_commandList->getD3D12CommandList()->SetGraphicsRoot32BitConstant(index, value, offset);
}

void VT_D3D12::D3D12RenderContext::setGraphicBindingParameterValues(uint32_t index, uint32_t offset, uint32_t valuesCount, uint32_t* values)
{
	m_commandList->getD3D12CommandList()->SetGraphicsRoot32BitConstants(index, valuesCount, values, offset);
}

void VT_D3D12::D3D12RenderContext::setComputeBindingParameterValue(uint32_t index, uint32_t offset, uint32_t value)
{
	m_commandList->getD3D12CommandList()->SetComputeRoot32BitConstant(index, value, offset);
}

void VT_D3D12::D3D12RenderContext::setComputeBindingParameterValues(uint32_t index, uint32_t offset, uint32_t valuesCount, uint32_t* values)
{
	m_commandList->getD3D12CommandList()->SetComputeRoot32BitConstants(index, valuesCount, values, offset);
}

void VT_D3D12::D3D12RenderContext::setGraphicBindingLayout(const VT::IPipelineBindingLayout* bindingLayout)
{
	assert(bindingLayout);

	const D3D12PipelineBindingLayout* d3d12PipelineBindingLayout = reinterpret_cast<const D3D12PipelineBindingLayout*>(bindingLayout);
	m_commandList->getD3D12CommandList()->SetGraphicsRootSignature(d3d12PipelineBindingLayout->getD3D12RootSignature().Get());
}

void VT_D3D12::D3D12RenderContext::setComputeBindingLayout(const VT::IPipelineBindingLayout* bindingLayout)
{
	assert(bindingLayout);

	const D3D12PipelineBindingLayout* d3d12PipelineBindingLayout = reinterpret_cast<const D3D12PipelineBindingLayout*>(bindingLayout);
	m_commandList->getD3D12CommandList()->SetComputeRootSignature(d3d12PipelineBindingLayout->getD3D12RootSignature().Get());
}

void VT_D3D12::D3D12RenderContext::setPipelineState(const VT::IPipelineState* pipelineState)
{
	assert(pipelineState);

	const D3D12GraphicPipelineState* d3d12PipelineState = reinterpret_cast<const D3D12GraphicPipelineState*>(pipelineState);
	m_commandList->getD3D12CommandList()->SetPipelineState(d3d12PipelineState->getD3D12Pipeline().Get());
}

void VT_D3D12::D3D12RenderContext::setVertexBuffers(uint32_t buffersCount, VT::IGPUBuffer** buffers,
	const VT::InputLayoutDesc& inputLayoutDesc)
{
	D3D12_VERTEX_BUFFER_VIEW d3d12VertexViews[MAX_VERTEX_BUFFERS_COUNT];

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

	m_commandList->getD3D12CommandList()->IASetVertexBuffers(0, buffersCount, d3d12VertexViews);
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

void VT_D3D12::D3D12RenderContext::setPrimitiveTopology(VT::PrimitiveTopology topology)
{
	m_commandList->getD3D12CommandList()->IASetPrimitiveTopology(convertPrimitiveTopologyVTtoD3D12(topology));
}

void VT_D3D12::D3D12RenderContext::draw(uint32_t vertCount)
{
	m_barrierCache.flush(m_commandList->getD3D12CommandList());
	m_commandList->getD3D12CommandList()->DrawInstanced(vertCount, 1, 0, 0);
}

void VT_D3D12::D3D12RenderContext::drawIndexed(uint32_t indexCount)
{
	m_barrierCache.flush(m_commandList->getD3D12CommandList());
	m_commandList->getD3D12CommandList()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}

void VT_D3D12::D3D12RenderContext::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	m_commandList->getD3D12CommandList()->Dispatch(threadGroupX, threadGroupY, threadGroupZ);
}
