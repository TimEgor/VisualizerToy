#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Vector.h"

namespace VT
{
	constexpr uint32_t MAX_POINT_LIGHT_NUM = 256;
	constexpr uint32_t POINT_LIGHT_MASK_SIZE = MAX_POINT_LIGHT_NUM / 8;

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

		GPUBufferReference m_lightVolumeInfoBuffer;

		ShaderResourceViewReference m_pointLightBufferSRV;
		ShaderResourceViewReference m_pointLightTileMasksBufferSRV;
		ShaderResourceViewReference m_pointLightTileMasksBufferUAV;
		ShaderResourceViewReference m_pointLightZSliceBufferSRV;

		ShaderResourceViewReference m_lightVolumeInfoBufferSRV;

		Vector3UInt16 m_tilesSlicesCount;
		Vector2UInt16 m_targetSize;
		Vector2UInt16 m_tileSize;

		bool initPointLightBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initPointLightTileMaskBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);
		bool initPointLightZSliceBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);

		bool initVolumeInfoBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager);

	public:
		LightVolumeData() = default;

		bool init(const Vector2UInt16& targetSize, const Vector2UInt16& tileSize, uint16_t slicesCount);
		void release();

		void fillEnvironment(RenderPassEnvironment& environment) const;

		const Vector2UInt16& getTilesCounts() const { return m_tilesSlicesCount.m_xy; }
		uint16_t getSlicesCount() const { return m_tilesSlicesCount.m_z; }
		const Vector2UInt16& getTargetSize() const { return m_targetSize; }
		const Vector2UInt16& getTileSizes() const { return m_tileSize; }
	};
}
