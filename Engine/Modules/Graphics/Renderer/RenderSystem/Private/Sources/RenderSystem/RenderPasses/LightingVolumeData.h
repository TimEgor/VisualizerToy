#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Vector.h"

namespace VT
{
	constexpr uint32_t MAX_POINT_LIGHT_NUM = 1024;
	constexpr uint32_t MAX_POINT_LIGHT_NUM_PER_CLUSTER = 16;

	class IGraphicResourceManager;
	struct AABB;

	struct LightVolumeInfo final
	{
		uint32_t m_tileCountX;
		uint32_t m_tileCountY;
		uint32_t m_sliceCount;
	};

	class LightVolumeData final
	{
		using LightIndex = uint8_t;

	private:
		GPUBufferReference m_pointLightBuffer;
		GPUBufferReference m_pointLightClusterBuffer;

		GPUBufferReference m_lightVolumeInfoBuffer;
		GPUBufferReference m_lightVolumeBoundingBoxBuffer;

		ShaderResourceViewReference m_pointLightBufferSRV;
		ShaderResourceViewReference m_pointLightClusterBufferSRV;

		ShaderResourceViewReference m_lightVolumeInfoBufferSRV;
		ShaderResourceViewReference m_lightVolumeBoundingBoxBufferSRV;

		Vector3UInt16 m_tilesSlicesCount;
		Vector2UInt16 m_targetSize;
		Vector2UInt16 m_tileSize;

		float m_nearProjPlane;
		float m_farProjPlane;

		bool initPointLightBuffer(IGraphicResourceManager* resManager);
		bool initPointLightClusterBuffer(IGraphicResourceManager* resManager);

		bool initVolumeInfoBuffer(IGraphicResourceManager* resManager);
		bool initVolumeInfoBoundingBoxBuffer(IGraphicResourceManager* resManager);

		void calcClusterBoudingBoxes(std::vector<AABB>& boxes) const;
		Vector3 convertScreenPosToView(const Vector3& screenPos) const;

	public:
		LightVolumeData() = default;

		bool init(const Vector2UInt16& targetSize, float nearProjPlane, float farProjPlane,
			const Vector2UInt16& tileSize, uint16_t slicesCount);
		void release();
	};
}
