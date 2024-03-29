#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Vector.h"

namespace VT
{
	constexpr uint32_t LIGHT_VOLUME_TILE_DIM_SIZE = 32;
	constexpr uint32_t LIGHT_VOLUME_SLICE_COUNT = 10;

	constexpr uint32_t MAX_POINT_LIGHT_NUM = 256;
	constexpr uint32_t POINT_LIGHT_MASK_SIZE = MAX_POINT_LIGHT_NUM / 8;

	constexpr uint32_t MAX_DIR_LIGHT_NUM = 32;

	class IGraphicDevice;
	class IGraphicResourceManager;
	class RenderPassEnvironment;

	class LightVolumeData final
	{
		struct LightVolumeInfo final
		{
			uint32_t m_tileCountX;
			uint32_t m_tileCountY;
			uint32_t m_sliceCount;
		};

		struct TileDepth final
		{
			float m_min;
			float m_max;
		};

	public:
		struct ZSlice final
		{
			uint32_t m_minLightIndex;
			uint32_t m_maxLightIndex;
		};

	private:
		GPUBufferReference m_pointLightBuffer;
		GPUBufferReference m_pointLightTileMasksBuffer;
		GPUBufferReference m_pointLightZSliceBuffer;

		GPUBufferReference m_directionalLightBuffer;

		GPUBufferReference m_tilesDepthBuffer;
		GPUBufferReference m_tilesBoundingBoxesBuffer;
		GPUBufferReference m_lightVolumeInfoBuffer;

		ShaderResourceViewReference m_pointLightBufferSRV;
		ShaderResourceViewReference m_pointLightTileMasksBufferSRV;
		ShaderResourceViewReference m_pointLightTileMasksBufferUAV;
		ShaderResourceViewReference m_pointLightZSliceBufferSRV;

		ShaderResourceViewReference m_directionalLightBufferSRV;

		ShaderResourceViewReference m_tilesDepthBufferSRV;
		ShaderResourceViewReference m_tilesDepthBufferUAV;
		ShaderResourceViewReference m_tilesBoundingBoxesBufferSRV;
		ShaderResourceViewReference m_tilesBoundingBoxesBufferUAV;
		ShaderResourceViewReference m_lightVolumeInfoBufferSRV;

		Vector3UInt16 m_tilesSlicesCount;

		bool initPointLightBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initPointLightTileMaskBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initPointLightZSliceBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);

		bool initDirectionalLightBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);

		bool initTilesDepthBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initTilesBoundingBoxesBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initVolumeInfoBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);

	public:
		LightVolumeData() = default;

		bool init(const Vector2UInt16& targetSize);
		void release();

		void fillEnvironment(RenderPassEnvironment& environment) const;

		const Vector2UInt16& getTilesCounts() const { return m_tilesSlicesCount.m_xy; }
		uint16_t getSlicesCount() const { return m_tilesSlicesCount.m_z; }
	};
}
