#include "stdafx.h"
#include "PhysXTestScene.h"

#include "CubePosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"
#include "TorusPosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.1f);

	constexpr float scale = 1.f;

	constexpr XMFLOAT3 actorDimension{ scale * 1.5f, scale, scale };

	m_pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);

	Reset();

	AddGameObject(m_pCube);

	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{PxIdentity});

	const PxBoxGeometry pBoxGeo = PxBoxGeometry{ actorDimension.x / 2, actorDimension.y / 2, actorDimension.z / 2 };

	//PxShape* pCubeShape = pPhysX->createShape(pBoxGeo, *pDefaultMaterial, true);

	//pCubeActor->attachShape(*pCubeShape);
	//
	//m_pPhysxScene->addActor(*pCubeActor);

	PxRigidActorExt::createExclusiveShape(*pCubeActor, pBoxGeo, *pDefaultMaterial);

	m_pCube->AttachRigidActor(pCubeActor);

	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo, {0,0,1} }});

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	m_SceneContext.GetCamera()->SetPositionAndLookAt({ 0,5,-20 },{});
	
}

void PhysXTestScene::Update()
{
	if(m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
}

void PhysXTestScene::Draw() const
{

}

void PhysXTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void PhysXTestScene::OnSceneDeactivated()
{
}

void PhysXTestScene::Reset() const
{
	m_pCube->Translate(m_Pos);
	m_pCube->RotateDegrees(m_Rot.x, m_Rot.y, m_Rot.z);
}
