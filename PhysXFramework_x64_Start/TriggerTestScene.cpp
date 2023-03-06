#include "stdafx.h"
#include "TriggerTestScene.h"

#include "Logger.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysx = PhysxManager::GetInstance()->GetPhysics();

	const auto pDefaultMaterial = pPhysx->createMaterial(.5f, .5f, 0.2f);

	//ground plane
	const auto pGroundActor = pPhysx->createRigidStatic(PxTransform({ PxPiDivTwo, {0,0,1} }));

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	//sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10.f, 10.f, XMFLOAT4(Colors::DarkRed));

	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{1.f}, *pDefaultMaterial);

	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	//trigger LEFT
	m_pTriggerLeft = pPhysx->createRigidStatic(PxTransform{ {-8.f, 0.5f, 0.f} });
	const auto pShapeLeft = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);

	m_pPhysxScene->addActor(*m_pTriggerLeft);

	pShapeLeft->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeLeft->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	//trigger RIGHT
	m_pTriggerRight = pPhysx->createRigidStatic(PxTransform{ {8.f, 0.5f, 0.f} });
	const auto pShapeRight = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);

	m_pPhysxScene->addActor(*m_pTriggerRight);

	pShapeRight->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeRight->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

}

void TriggerTestScene::Update()
{
	constexpr float force{ 5.f };

	if(m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}

	const float force2{8.5f};

	if(m_IsTriggeredLeft)
	{
		m_IsTriggeredLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({});
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ force2, force2, 0.f }, PxForceMode::eIMPULSE);
	}
	if(m_IsTriggeredRight)
	{
		m_IsTriggeredRight = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({});
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ -force2, force2, 0.f }, PxForceMode::eIMPULSE);
	}
}

void TriggerTestScene::Draw() const
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for(int i{}; i < count; ++i)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER)) continue;

		const PxTriggerPair& pair = pairs[i];

		if(pair.triggerActor == m_pTriggerLeft)
		{
			if(pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box LEFT");
				//m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f, 10.f, 0.f }, PxForceMode::eIMPULSE);
				m_IsTriggeredLeft = true;
			}
			else if(pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOSTD trigger box LEFT");
			}
		}
		else 
		if(pair.triggerActor == m_pTriggerRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box RIGHT");
				m_IsTriggeredRight = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOSTD trigger box RIGHTT");
			}
		}
	}
}
