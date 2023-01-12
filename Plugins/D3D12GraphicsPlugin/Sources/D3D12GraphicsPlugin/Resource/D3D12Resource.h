#pragma once

#include "D3D12GraphicsPlugin/D3D12ComPtrDefines.h"

namespace VT_D3D12
{
	class D3D12ResourceBase
	{
	private:
		D3D12ResourceComPtr m_d3d12Resource = nullptr;

	public:
		D3D12ResourceBase(D3D12ResourceComPtr d3d12Resource)
			: m_d3d12Resource(d3d12Resource) {}

		D3D12ResourceComPtr getD3D12Resource() const { return m_d3d12Resource; }
	};
}
