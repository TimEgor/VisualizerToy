#include "FreePerspectiveCameraController.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "LevelSystem/ILevelSystem.h"
#include "Scene/SceneNodeIDComponent.h"

#include "Camera/PerspectiveCameraComponent.h"
#include "InputSystem/IInputSystem.h"
#include "Scene/IScene.h"

#include "Math/ComputeMath.h"
#include "Math/ComputeVector.h"
#include "Math/ComputeMatrix.h"
#include "Math/Consts.h"

static constexpr float DefaultCameraTranslationSpeed = 3.0f;
static constexpr float DefaultCameraRotationSpeed = 20.0f;

void VT_DEMO_COMMON::FreePerspectiveCameraController::updateCamera(VT::VT_Entity cameraEntity, float deltaTime)
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();

	VT::IInputSystem* inputSystem = environment->m_inputSystem;

	VT::ILevel* currentLevel = environment->m_levelSystem->getCurrentLevel();
	if (!currentLevel)
	{
		return;
	}

	VT::IScene* currentScene = currentLevel->getScene();
	VT::EntityComponentSystem* levelESC = currentLevel->getEntityComponentSystem();

	if (!levelESC->hasAllOfComponent<VT::PerspectiveCameraComponent, VT::SceneNodeIDComponent>(cameraEntity))
	{
		return;
	}

	VT::SceneNodeIDComponent& sceneNodeIDComponent = levelESC->getComponent<VT::SceneNodeIDComponent>(cameraEntity);
	//VT::PerspectiveCameraComponent& cameraComponent = levelESC->getComponent<VT::PerspectiveCameraComponent>(cameraEntity);
	VT::NodeID cameraNodeID = sceneNodeIDComponent.getNodeID();

	VT::Transform& cameraEntityWorldTransform = currentScene->getNodeWorldTransformRaw(cameraNodeID);

	VT::Vector3& cameraPosition = cameraEntityWorldTransform.getOrigin();
	VT::COMPUTE_MATH::ComputeVector cameraEntityPosition = VT::COMPUTE_MATH::loadComputeVectorFromVector3(cameraPosition);
	VT::COMPUTE_MATH::ComputeVector rightDir = VT::COMPUTE_MATH::loadComputeVectorFromVector3(cameraEntityWorldTransform.getAxisX());
	VT::COMPUTE_MATH::ComputeVector upDir = VT::COMPUTE_MATH::loadComputeVectorFromVector3(cameraEntityWorldTransform.getAxisY());
	VT::COMPUTE_MATH::ComputeVector forwardDir = VT::COMPUTE_MATH::loadComputeVectorFromVector3(cameraEntityWorldTransform.getAxisZ());

	VT::COMPUTE_MATH::ComputeMatrix cameraTransform = VT::COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(VT::Matrix44Identity);

	if (inputSystem->isKeyDown(VT::Key::MOUSE_MIDDLE))
	{
		const VT::Vector2Int16& mouseDelta = inputSystem->getMouseMovementOffset();

		cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
			VT::COMPUTE_MATH::vectorScale(rightDir, -mouseDelta.m_x * DefaultCameraTranslationSpeed * deltaTime),
			cameraEntityPosition
		);

		cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
			VT::COMPUTE_MATH::vectorScale(upDir, mouseDelta.m_y * DefaultCameraTranslationSpeed * deltaTime),
			cameraEntityPosition
		);
	}
	else {
		if (inputSystem->isKeyDown(VT::Key::W))
		{
			cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
				VT::COMPUTE_MATH::vectorScale(forwardDir, DefaultCameraTranslationSpeed * deltaTime),
				cameraEntityPosition
			);
		}
		if (inputSystem->isKeyDown(VT::Key::S))
		{
			cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
				VT::COMPUTE_MATH::vectorScale(forwardDir, -DefaultCameraTranslationSpeed * deltaTime),
				cameraEntityPosition
			);
		}
		if (inputSystem->isKeyDown(VT::Key::D))
		{
			cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
				VT::COMPUTE_MATH::vectorScale(rightDir, DefaultCameraTranslationSpeed * deltaTime),
				cameraEntityPosition
			);
		}
		if (inputSystem->isKeyDown(VT::Key::A))
		{
			cameraEntityPosition = VT::COMPUTE_MATH::vectorAdd(
				VT::COMPUTE_MATH::vectorScale(rightDir, -DefaultCameraTranslationSpeed * deltaTime),
				cameraEntityPosition
			);
		}


		float pitchAngle = asin(-cameraEntityWorldTransform.m_matrix.m_32);
		float yawAngle = 0.0f;
		float rollAngle = 0.0f;
		if (cosf(pitchAngle) > VT::VT_EPSILON)
		{
			yawAngle = atan2f(cameraEntityWorldTransform.m_matrix.m_31, cameraEntityWorldTransform.m_matrix.m_33);
			rollAngle = atan2f(cameraEntityWorldTransform.m_matrix.m_12, cameraEntityWorldTransform.m_matrix.m_22);
		}
		else
		{
			yawAngle = 0.0f;
			rollAngle = atan2f(-cameraEntityWorldTransform.m_matrix.m_21, cameraEntityWorldTransform.m_matrix.m_11);
		}


		if (inputSystem->isKeyDown(VT::Key::MOUSE_RIGHT))
		{
			const VT::Vector2Int16& mouseDelta = inputSystem->getMouseMovementOffset();

			pitchAngle += -mouseDelta.m_y * DefaultCameraRotationSpeed * deltaTime * VT::VT_DEG_TO_RAD;

			pitchAngle = std::min(pitchAngle, VT::VT_HALF_PI);
			pitchAngle = std::max(pitchAngle, -VT::VT_HALF_PI);

			yawAngle += -mouseDelta.m_x * DefaultCameraRotationSpeed * deltaTime * VT::VT_DEG_TO_RAD;

			if (yawAngle >= VT::VT_2_PI)
			{
				yawAngle -= VT::VT_2_PI;
			}
			else if (yawAngle <= -VT::VT_2_PI)
			{
				yawAngle += VT::VT_2_PI;
			}
		}

		cameraTransform = VT::COMPUTE_MATH::matrixMultiply(cameraTransform, VT::COMPUTE_MATH::matrixRotationRollPithYaw(pitchAngle, yawAngle, 0.0f));
	}

	cameraTransform = VT::COMPUTE_MATH::matrixMultiply(cameraTransform, VT::COMPUTE_MATH::matrixTranslation(cameraEntityPosition));
	cameraEntityWorldTransform.m_matrix = VT::COMPUTE_MATH::saveComputeMatrixToMatrix4x4(cameraTransform);

	currentScene->setDirtyState(cameraNodeID, VT::IScene::DirtyStateVariants::WORLD_TRANSFORM);
}
