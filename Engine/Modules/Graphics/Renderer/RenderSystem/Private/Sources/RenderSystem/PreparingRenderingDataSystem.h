#pragma once

namespace VT
{
	class ILevel;
	class RenderingData;

	class PreparingRenderingDataSystem final
	{
	public:
		PreparingRenderingDataSystem() = default;

		static void prepareData(const ILevel& level, RenderingData& renderingData);
	};
}
