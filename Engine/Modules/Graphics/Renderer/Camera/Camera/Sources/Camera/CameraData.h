#pragma once

namespace VT
{
	struct CameraData
	{
		float m_nearClipPlane = 0.1f;
		float m_farClipPlane = 1000.0f;

		virtual ~CameraData() {}
	};

	struct PerspectiveCameraData final : public CameraData
	{
		float m_FoV = 60.0f;
	};
}
