#include "stdafx.h"
#include "BoxForceScene.h"

#include "CubePosColorNorm.h"


#include "Logger.h"

void BoxForceScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	const PxMaterial* pDefaultMaterialBox = pPhysX->createMaterial(0.1f, 0.1f, 0.2f);

	const PxMaterial* pDefaultMaterialPlane = pPhysX->createMaterial(0.2f, 0.2f, 0.2f);
	
	constexpr float scale = 1.f;

	constexpr XMFLOAT3 actorDimension{ scale , scale, scale };

	m_pBox = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z, XMFLOAT4{ 0.f, 1.f, 0.f, 1.f });

	Reset();

	AddGameObject(m_pBox);

	PxRigidDynamic* pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	const PxBoxGeometry pBoxGeo1 = PxBoxGeometry(actorDimension.x/2, actorDimension.y/2, actorDimension.z/2);

	PxRigidActorExt::createExclusiveShape(*pBoxActor, pBoxGeo1, *pDefaultMaterialBox);

	m_pBox->AttachRigidActor(pBoxActor);

	//plane collider
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, {0,0,1} } });

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterialPlane);

	m_pPhysxScene->addActor(*pGroundActor);


	m_SceneContext.GetCamera()->SetPositionAndLookAt({ 0,20,-5 }, {});

	Reset();

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			InputID::DOWN, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			InputID::UP, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			InputID::LEFT, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			InputID::RIGHT, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT
		});

}

void BoxForceScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}

	
	PxVec3 force(0.f, 0.f, 0.f);
	bool canForce = false;

	const float forceScale = 2.5f;

	if (m_SceneContext.GetInput()->IsActionTriggered(LEFT))
	{
		canForce = true;
		force.z += forceScale;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(RIGHT))
	{
		canForce = true;
		force.z += -forceScale;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(UP))
	{
		canForce = true;
		force.x += forceScale;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(DOWN))
	{
		canForce = true;
		force.x += -forceScale;
	}

	if(canForce)
	{
		if (PxRigidActor* rigidActor = m_pBox->GetRigidActor(); rigidActor->is<physx::PxRigidDynamic>())
		{
			PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(rigidActor);

			//rigidDynamic->addForce(force, PxForceMode::eFORCE);
			rigidDynamic->addTorque(force, PxForceMode::eFORCE);
		}
	}

}

void BoxForceScene::Draw() const
{

}

void BoxForceScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void BoxForceScene::OnSceneDeactivated()
{
}

void BoxForceScene::Reset() const
{
	m_pBox->Translate(m_Pos.x, m_Pos.y, m_Pos.z);

	constexpr XMFLOAT3 rot{};

	m_pBox->RotateDegrees(rot.x, rot.y, rot.z);

}
