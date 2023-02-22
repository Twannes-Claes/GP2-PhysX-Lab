#include "stdafx.h"
#include "W1_AssignmentScene.h"

#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"


#include "Logger.h"

void W1_AssignmentScene::Initialize()
{
	std::srand(std::time(nullptr));

	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	const PxMaterial* pDefaultMaterialBox = pPhysX->createMaterial(0.1f, 0.1f, 0.5f);

	const PxMaterial* pDefaultMaterialPlane = pPhysX->createMaterial(0.f, 0.f, 0.2f);

	const PxMaterial* pDefaultMaterialSphere = pPhysX->createMaterial(0.4f, 0.4f, 0.2f);

	//boxes
	m_pBoxes.resize(m_AmountBlocks * m_AmountBlocks);

	const PxBoxGeometry pBoxGeo = PxBoxGeometry(m_BoxDimension.x / 2, m_BoxDimension.y / 2, m_BoxDimension.z / 2);

	for(int i{}; i < m_AmountBlocks * m_AmountBlocks; i++)
	{
		m_pBoxes[i] = new CubePosColorNorm(m_BoxDimension.x, m_BoxDimension.y, m_BoxDimension.z, XMFLOAT4{0.f, 1.f, 0.f, 1.f});
		AddGameObject(m_pBoxes[i]);

		PxRigidDynamic* pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
		PxRigidActorExt::createExclusiveShape(*pBoxActor, pBoxGeo, *pDefaultMaterialBox);

		m_pBoxes[i]->AttachRigidActor(pBoxActor);
	}


	//floor
	m_pFloor = new CubePosColorNorm(m_BoxDimension.x * m_FloorScale, m_BoxDimension.y * 0.1, m_BoxDimension.z * m_FloorScale, XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

	AddGameObject(m_pFloor);

	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });

	const PxBoxGeometry pFloorGeo = PxBoxGeometry(m_BoxDimension.x * (m_FloorScale/2), m_BoxDimension.y * 0.05f, m_BoxDimension.z * (m_FloorScale / 2));

	PxRigidActorExt::createExclusiveShape(*pGroundActor, pFloorGeo, *pDefaultMaterialPlane);

	m_pFloor->Translate(0, -0.05f, 0);

	m_pFloor->AttachRigidActor(pGroundActor);

	//sphere
	m_pSphere = new SpherePosColorNorm(1.f, 15, 15, XMFLOAT4{ 0.f, 1.f, 0.f, 1.f });

	AddGameObject(m_pSphere);

	PxRigidDynamic* pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	const PxSphereGeometry pSphereGeo = PxSphereGeometry(1.f);

	PxRigidActorExt::createExclusiveShape(*pSphereActor, pSphereGeo, *pDefaultMaterialSphere);

	PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, 25);

	m_pSphere->AttachRigidActor(pSphereActor);

	//set all the gameobject on their right pos
	Reset();

	//input
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

		m_SceneContext.GetCamera()->SetPositionAndLookAt({ 0,25,-50 }, { 0, 5.f , m_FloorScale / 2.5f });
		//m_SceneContext.GetCamera()->SetPositionAndLookAt({ 20,15,-5 }, { 0, 5.f , m_FloorScale / 2.5f });

}

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		
		if (PxRigidActor* rigidActor = m_pSphere->GetRigidActor(); rigidActor->is<physx::PxRigidDynamic>())
		{
			PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(rigidActor);

			rigidDynamic->addForce({0,150,0}, PxForceMode::eIMPULSE);
		}
		
	}


	PxVec3 force(0.f, 0.f, 0.f);
	bool canForce = false;

	constexpr float forceScale = 100.f;

	if (m_SceneContext.GetInput()->IsActionTriggered(LEFT))
	{
		canForce = true;
		const PxVec3 cameraLookAt = PxVec3(m_SceneContext.GetCamera()->GetForward().x, 0.0f, m_SceneContext.GetCamera()->GetForward().z).getNormalized();
		force += forceScale * cameraLookAt;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(RIGHT))
	{
		canForce = true;
		const PxVec3 cameraLookAt = PxVec3(m_SceneContext.GetCamera()->GetForward().x, 0.0f, m_SceneContext.GetCamera()->GetForward().z).getNormalized();
		force -= forceScale * cameraLookAt;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(UP))
	{
		canForce = true;
		const PxVec3 cameraRight = PxVec3(m_SceneContext.GetCamera()->GetRight().x, 0.0f, m_SceneContext.GetCamera()->GetRight().z).getNormalized();
		force += forceScale * cameraRight;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(DOWN))
	{
		canForce = true;
		const PxVec3 cameraRight = PxVec3(m_SceneContext.GetCamera()->GetRight().x, 0.0f, m_SceneContext.GetCamera()->GetRight().z).getNormalized();
		force -= forceScale * cameraRight;
	}

	if (canForce)
	{
		if (PxRigidActor* rigidActor = m_pSphere->GetRigidActor(); rigidActor->is<physx::PxRigidDynamic>())
		{
			PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(rigidActor);

			rigidDynamic->addTorque(force, PxForceMode::eFORCE);
		}
	}

}

void W1_AssignmentScene::Draw() const
{

}

void W1_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::Reset() const
{

	constexpr float boxOffset{ 0.1f };

	float startX{ (m_BoxDimension.x * (m_AmountBlocks/ 2)) + (boxOffset * 2)};
	const float startY{ m_BoxDimension.y / 2 };
	const float startZ{ m_FloorScale /2.5f};

	const int numBlocksPerSide{ (m_AmountBlocks - 1) / 2 };

	if(m_AmountBlocks % 2 == 0)
	{
		startX += (m_BoxDimension.x / 2) + (boxOffset / 2);
	}

	for(int i{}; i < m_AmountBlocks; ++i)
	{
		for(int j{}; j < m_AmountBlocks; ++j)
		{
			const int index{ (i * m_AmountBlocks) + j };
			
			XMFLOAT3 trans { -startX + (j * (m_BoxDimension.x + boxOffset)), startY + (i * (m_BoxDimension.y + 0.01f)), startZ };
	
			m_pBoxes[index]->Translate( trans );
	
			const int randomY = rand() % 21 - 10;
	
			m_pBoxes[index]->RotateDegrees( 0, randomY ,0 );
	
		}
	}

	m_pSphere->Translate(0, 1, -10);

	if (PxRigidActor* rigidActor = m_pSphere->GetRigidActor(); rigidActor->is<physx::PxRigidDynamic>())
	{
		PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(rigidActor);

		//rigidDynamic->setForceAndTorque({0,0,0},{0,0,0});
		//rigidDynamic->clearTorque();
	}

}
