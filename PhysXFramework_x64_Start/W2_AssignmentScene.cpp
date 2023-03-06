#include "stdafx.h"
#include "W2_AssignmentScene.h"

#include "ContentManager.h"
#include "CubePosColorNorm.h"
#include "MeshObject.h"
#include "PhysxManager.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"

void W2_AssignmentScene::Initialize()
{

	EnablePhysxDebugRendering(true);

	const auto pPhysx = PhysxManager::GetInstance()->GetPhysics();

	const auto pDefaultMaterial = pPhysx->createMaterial(.5f, .5f, 0.2f);
	const auto pDefaultMaterialSphere = pPhysx->createMaterial(1.f, 1.f, 0.2f);
	const auto pDefaultMaterialBox = pPhysx->createMaterial(.1f, .1f, 0.2f);

	//TRIANGLE MESH
	m_pLevel = new MeshObject(L"Resources/Meshes/Level.ovm");

	AddGameObject(m_pLevel);

	const auto pLevelMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");

	const auto pLevelActor = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });

	pLevelActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pLevelActor, PxTriangleMeshGeometry{ pLevelMesh }, *pDefaultMaterial);

	m_pLevel->AttachRigidActor(pLevelActor);

	//sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10.f, 10.f, XMFLOAT4(Colors::Gray));
	m_pSphere2 = new SpherePosColorNorm(1.f, 10.f, 10.f, XMFLOAT4(Colors::Gray));
	m_pSphere3 = new SpherePosColorNorm(1.f, 10.f, 10.f, XMFLOAT4(Colors::Gray));

	AddGameObject(m_pSphere);
	AddGameObject(m_pSphere2);
	AddGameObject(m_pSphere3);

	const auto pSphereActor = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pSphereActor2 = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pSphereActor3 = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterialSphere);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, PxSphereGeometry{ 1.f }, *pDefaultMaterialSphere);
	PxRigidActorExt::createExclusiveShape(*pSphereActor3, PxSphereGeometry{ 1.f }, *pDefaultMaterialSphere);

	pSphereActor->setMass(1);

	m_pSphere->AttachRigidActor(pSphereActor);
	m_pSphere2->AttachRigidActor(pSphereActor2);
	m_pSphere3->AttachRigidActor(pSphereActor3);

	m_pSphere->Translate(m_SpherePos);
	m_pSphere2->Translate({5.f, 20.f, 0.f});
	m_pSphere3->Translate({ -5.f, 20.f, 0.f });


	auto lockJoint1 = PxD6JointCreate(*pPhysx, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });
	auto lockJoint2 = PxD6JointCreate(*pPhysx, nullptr, PxTransform{ PxIdentity }, pSphereActor2, PxTransform{ PxIdentity });
	auto lockJoint3 = PxD6JointCreate(*pPhysx, nullptr, PxTransform{ PxIdentity }, pSphereActor3, PxTransform{ PxIdentity });

	lockJoint1->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	lockJoint2->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	lockJoint3->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);

	lockJoint1->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	lockJoint2->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	lockJoint3->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);

	lockJoint1->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	lockJoint2->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	lockJoint3->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);

	lockJoint1->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	lockJoint2->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	lockJoint3->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);

	lockJoint1->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	lockJoint2->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	lockJoint3->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	//trigger LEFT

	m_pTriggerLeft = new CubePosColorNorm(2, 1, 4, XMFLOAT4{ Colors::Red });

	AddGameObject(m_pTriggerLeft);

	const auto pLeftActor = pPhysx->createRigidStatic(PxTransform{ {-7.25f, 2.f, 0.f} });
	const auto pShapeLeft = PxRigidActorExt::createExclusiveShape(*pLeftActor, PxBoxGeometry{ 1.f, 0.5f, 2.f }, *pDefaultMaterial);

	m_pTriggerLeft->AttachRigidActor(pLeftActor);
	m_pTriggerLeft->Translate({ -7.25f, 2.f, 0.f });

	pShapeLeft->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeLeft->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	//trigger RIGHT
	m_pTriggerRight = new CubePosColorNorm(2, 1, 4, XMFLOAT4{ Colors::Blue });

	AddGameObject(m_pTriggerRight);

	const auto pRightActor = pPhysx->createRigidStatic(PxTransform{ {6.5f, 2.f, 0.f} });
	const auto pShapeRight = PxRigidActorExt::createExclusiveShape(*pRightActor, PxBoxGeometry{ 1.f, 0.5f, 2.f }, *pDefaultMaterial);

	m_pTriggerRight->AttachRigidActor(pRightActor);
	m_pTriggerRight->Translate({ 6.5f, 2.f, 0.f });

	pShapeRight->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeRight->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	//box LEFT

	m_pBoxLeft = new CubePosColorNorm(1, 1, 1, XMFLOAT4{ Colors::Red });

	AddGameObject(m_pBoxLeft);

	const auto pLeftBoxActor = pPhysx->createRigidDynamic(PxTransform{ {PxIdentity} });
	PxRigidActorExt::createExclusiveShape(*pLeftBoxActor, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterialBox);

	pLeftBoxActor->setMass(.1f);

	m_pBoxLeft->AttachRigidActor(pLeftBoxActor);
	m_pBoxLeft->Translate({ -3.25f, 5.f, 0.f });

	//box RIGHT
	m_pBoxRight = new CubePosColorNorm(1, 1, 1, XMFLOAT4{ Colors::Blue });

	AddGameObject(m_pBoxRight);

	const auto pRightBoxActor = pPhysx->createRigidDynamic(PxTransform{ {PxIdentity} });
	PxRigidActorExt::createExclusiveShape(*pRightBoxActor, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterialBox);

	pRightBoxActor->setMass(.1f);

	m_pBoxRight->AttachRigidActor(pRightBoxActor);
	m_pBoxRight->Translate({ 2.5f, 5.f, 0.f });


	//trapdoor right
	m_pTrapRight = new CubePosColorNorm(1, 1, 1, XMFLOAT4{ Colors::Blue });

	AddGameObject(m_pTrapRight);

	const auto pRightTrapActor = pPhysx->createRigidDynamic(PxTransform{ {PxIdentity} });
	PxRigidActorExt::createExclusiveShape(*pRightTrapActor, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, * pDefaultMaterialBox);

	pRightTrapActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	XMFLOAT3 tempPos = m_TrapPos;

	m_pTrapRight->AttachRigidActor(pRightTrapActor);
	m_pTrapRight->Translate(tempPos);

	//trapdoor left
	m_pTrapLeft = new CubePosColorNorm(1, 1, 1, XMFLOAT4{ Colors::Red });

	AddGameObject(m_pTrapLeft);

	const auto pLeftTrapActor = pPhysx->createRigidDynamic(PxTransform{ {PxIdentity} });
	PxRigidActorExt::createExclusiveShape(*pLeftTrapActor, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, * pDefaultMaterialBox);

	pLeftTrapActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	tempPos.x *= -1;

	m_pTrapLeft->AttachRigidActor(pLeftTrapActor);
	m_pTrapLeft->Translate(tempPos);

	//sound
	const auto pFMOD = SoundManager::GetInstance()->GetSystem();

	FMOD_RESULT result = pFMOD->createStream("Resources/Sounds/bell.mp3", FMOD_2D | FMOD_LOOP_OFF, nullptr, &m_pSound2D);

	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFMOD->playSound(m_pSound2D, nullptr, true, &m_pChannel2D);

	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel2D->setVolume(.5f);
	
}

void W2_AssignmentScene::Update()
{

	constexpr float force{ 8.f };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, 'R'))
	{
		m_pSphere->Translate(m_SpherePos);
		m_pSphere->RotateDegrees(0,0,0);

		m_pBoxLeft->Translate({ -3.25f, 5.f, 0.f });
		m_pBoxRight->Translate({ 2.5f, 5.f, 0.f });
		m_pBoxLeft->RotateDegrees(0, 0, 0);
		m_pBoxRight->RotateDegrees(0, 0, 0);

		const auto right = m_pTrapRight->GetRigidActor()->is<PxRigidDynamic>();

		const auto pos = m_pTrapRight->GetPosition();

		right->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

		const auto left = m_pTrapLeft->GetRigidActor()->is<PxRigidDynamic>();

		const auto pos2 = m_pTrapLeft->GetPosition();

		left->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

		m_pSphere2->Translate({ 5.f, 20.f, 0.f });
		m_pSphere3->Translate({ -5.f, 20.f, 0.f });

		m_pSphere2->Rotate(0,0,0 );
		m_pSphere3->Rotate( 0,0,0);


	}

	if (m_IsTriggeredLeft)
	{
		m_IsTriggeredLeft = false;
		const auto left = m_pTrapLeft->GetRigidActor()->is<PxRigidDynamic>();

		const auto pos = m_pTrapLeft->GetPosition();

		left->setKinematicTarget(PxTransform{ pos.x + 5.f, pos.y + 0.75f, pos.z });
		left->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

		SoundManager::GetInstance()->GetSystem()->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
		
	}
	if (m_IsTriggeredRight)
	{
		m_IsTriggeredRight = false;
		const auto right = m_pTrapRight->GetRigidActor()->is<PxRigidDynamic>();

		const auto pos = m_pTrapRight->GetPosition();

		right->setKinematicTarget(PxTransform{pos.x - 5.f, pos.y + 0.75f, pos.z});
		right->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		bool isPaused{};

		m_pChannel2D->setPaused(false);

		SoundManager::GetInstance()->GetSystem()->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
	}

}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (int i{}; i < count; ++i)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER)) continue;

		const PxTriggerPair& pair = pairs[i];

		if (pair.triggerActor == m_pTriggerLeft->GetRigidActor()->is<PxRigidStatic>() && pair.otherActor == m_pBoxLeft->GetRigidActor()->is<PxRigidDynamic>())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Trigger FOUND trigger box RED");
				//m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f, 10.f, 0.f }, PxForceMode::eIMPULSE);
				m_IsTriggeredLeft = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//Logger::GetInstance()->LogInfo(L"Sphere LOSTD trigger box LEFT");
			}
		}
		else
		if (pair.triggerActor == m_pTriggerRight->GetRigidActor()->is<PxRigidStatic>() && pair.otherActor == m_pBoxRight->GetRigidActor()->is<PxRigidDynamic>())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Trigger FOUND trigger box BLUE");
				m_IsTriggeredRight = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//Logger::GetInstance()->LogInfo(L"Sphere LOSTD trigger box RIGHTT");
			}
		}
	}
}
