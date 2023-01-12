#include "D3D12Fence.h"

void VT_D3D12::D3D12Fence::wait(VT::FenceValueType value)
{
    if (m_d3d12Fence->GetCompletedValue() < value)
    {
        m_d3d12Fence->SetEventOnCompletion(value, m_winEvent);
        WaitForSingleObject(m_winEvent, INFINITE);
    }
}
