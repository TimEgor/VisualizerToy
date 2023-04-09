#include "D3D12ImGuiRenderBackend.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "RenderContext/IRenderContext.h"

#include "D3D12GraphicsPlugin/Device/D3D12GraphicDevice.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptorHeap.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptor.h"

#include "backends/imgui_impl_dx12.h"

bool VT_D3D12::D3D12ImGuiRenderBackend::init(ImGuiContext* imguiContext)
{
	assert(!m_inited);

	VT_CHECK_INITIALIZATION(imguiContext);

	VT::IGraphicDevice* device = VT::EngineInstance::getInstance()->getEnvironment()->m_graphicDevice;
	VT_CHECK_INITIALIZATION(device && device->getType() == D3D12GraphicDevice::getGraphicDeviceType());

	D3D12GraphicDevice* d3d12Device = reinterpret_cast<D3D12GraphicDevice*>(device);

	D3D12ResourceDescriptorHeap* descriptorHeap = reinterpret_cast<D3D12ResourceDescriptorHeap*>(d3d12Device->getBindlessResourceDescriptionHeap());

	VT::DescriptorBindingHeapOffsetType fontTextureOffset = descriptorHeap->allocateDescriptor();
	D3D12_CPU_DESCRIPTOR_HANDLE fontTextureCpuHandle = descriptorHeap->getCPUHandle(fontTextureOffset);
	D3D12_GPU_DESCRIPTOR_HANDLE fontTextureGpuHandle = descriptorHeap->getGPUHandle(fontTextureOffset);

	m_fontTextureResourceDescriptor = new D3D12ResourceDescriptor(
		VT::GraphicResourceDescriptorType::UNDEFINED, fontTextureOffset, fontTextureCpuHandle, fontTextureGpuHandle);

	ImGui::SetCurrentContext(imguiContext);

	VT_CHECK_INITIALIZATION(ImGui_ImplDX12_Init(d3d12Device->getNativeD3D12Device().Get(), 2, DXGI_FORMAT_R8G8B8A8_UNORM,
		descriptorHeap->getD3D12DescriptorHeap().Get(), fontTextureCpuHandle, fontTextureGpuHandle));

	m_inited = true;

	return true;
}

void VT_D3D12::D3D12ImGuiRenderBackend::release()
{
	if (m_fontTextureResourceDescriptor)
	{
		VT::IGraphicDevice* device = VT::EngineInstance::getInstance()->getEnvironment()->m_graphicDevice;
		assert(device && device->getType() == D3D12GraphicDevice::getGraphicDeviceType());

		D3D12GraphicDevice* d3d12Device = reinterpret_cast<D3D12GraphicDevice*>(device);

		D3D12ResourceDescriptorHeap* descriptorHeap = reinterpret_cast<D3D12ResourceDescriptorHeap*>(d3d12Device->getBindlessResourceDescriptionHeap());
		descriptorHeap->deallocateDescriptor(m_fontTextureResourceDescriptor->getBindingHeapOffset());

		VT_SAFE_DESTROY(m_fontTextureResourceDescriptor);
	}

	if (m_inited)
	{
		ImGui_ImplDX12_Shutdown();

		m_inited = false;
	}
}

void VT_D3D12::D3D12ImGuiRenderBackend::newFrame()
{
	assert(m_inited);

	ImGui_ImplDX12_NewFrame();
}

void VT_D3D12::D3D12ImGuiRenderBackend::present(VT::IRenderContext* context)
{
	assert(m_inited);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), context->getCommandList()->getNativeHandleCast<ID3D12GraphicsCommandList>());
}