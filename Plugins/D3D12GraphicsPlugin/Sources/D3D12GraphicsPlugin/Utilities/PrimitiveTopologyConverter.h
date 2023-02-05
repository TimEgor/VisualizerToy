#pragma once

#include "GraphicResourceCommon/PrimitiveTopology.h"
#include "D3D12GraphicsPlugin/D3D12Core.h"

namespace VT_D3D12
{
	inline D3D_PRIMITIVE_TOPOLOGY convertPrimitiveTopologyVTtoD3D12(const VT::PrimitiveTopology topology)
	{
		switch (topology)
		{
		case VT::PrimitiveTopology::POINTLIST:
			return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case VT::PrimitiveTopology::LINELIST:
			return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case VT::PrimitiveTopology::LINESTRIP:
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case VT::PrimitiveTopology::TRIANGLELIST:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case VT::PrimitiveTopology::TRIANGLESTRIP:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case VT::PrimitiveTopology::LINELIST_ADJ:
			return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		case VT::PrimitiveTopology::LINESTRIP_ADJ:
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
		case VT::PrimitiveTopology::TRIANGLELIST_ADJ:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
		case VT::PrimitiveTopology::TRIANGLESTRIP_ADJ:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

		case VT::PrimitiveTopology::UNDEFINED:
		default:
			return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}
}
