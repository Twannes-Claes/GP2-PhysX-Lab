#include "stdafx.h"
#include "BouncingBallScene.h"

#include "SpherePosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	const PxMaterial* pDefaultMaterial1 = pPhysX->createMaterial(0.5f, 0.5f, 1.f);
	const PxMaterial* pDefaultMaterial2 = pPhysX->createMaterial(0.5f, 0.5f, 0.5f);
	const PxMaterial* pDefaultMaterial3 = pPhysX->createMaterial(0.5f, 0.5f, 0.f);

	const PxMaterial* pDefaultMaterialPlane = pPhysX->createMaterial(0.5f, 0.5f, 0.5f);


	m_pSphere1 = new SpherePosColorNorm(1.f, 15, 15, XMFLOAT4{0.f, 1.f, 0.f, 1.f});
	m_pSphere2 = new SpherePosColorNorm(1.f, 15, 15, XMFLOAT4{0.f, 1.f, 0.f, 1.f});
	m_pSphere3 = new SpherePosColorNorm(1.f, 15, 15, XMFLOAT4{0.f, 1.f, 0.f, 1.f});

	Reset();

	AddGameObject(m_pSphere1);
	AddGameObject(m_pSphere2);
	AddGameObject(m_pSphere3);

	PxRigidDynamic* pSphereActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	const PxSphereGeometry pSphereGeo1 = PxSphereGeometry(1.f);
	const PxSphereGeometry pSphereGeo2 = PxSphereGeometry(1.f);
	const PxSphereGeometry pSphereGeo3 = PxSphereGeometry(1.f);

	PxRigidActorExt::createExclusiveShape(*pSphereActor1, pSphereGeo1, *pDefaultMaterial1);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, pSphereGeo2, *pDefaultMaterial2);
	PxRigidActorExt::createExclusiveShape(*pSphereActor3, pSphereGeo3, *pDefaultMaterial3);

	m_pSphere1->AttachRigidActor(pSphereActor1);
	m_pSphere2->AttachRigidActor(pSphereActor2);
	m_pSphere3->AttachRigidActor(pSphereActor3);

	//plane collider
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, {0,0,1} } });

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterialPlane);

	m_pPhysxScene->addActor(*pGroundActor);


	m_SceneContext.GetCamera()->SetPositionAndLookAt({ 0,5,-40 }, {});

	Reset();

}

void BouncingBallScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
}

void BouncingBallScene::Draw() const
{

}

void BouncingBallScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void BouncingBallScene::OnSceneDeactivated()
{
}

void BouncingBallScene::Reset() const
{
	m_pSphere1->Translate(m_Pos.x - m_PosOffset, m_Pos.y, m_Pos.z);
	m_pSphere2->Translate(m_Pos.x	  , m_Pos.y, m_Pos.z);
	m_pSphere3->Translate(m_Pos.x + m_PosOffset, m_Pos.y, m_Pos.z);

	constexpr XMFLOAT3 rot{};

	m_pSphere1->RotateDegrees(rot.x, rot.y, rot.z);
	m_pSphere2->RotateDegrees(rot.x, rot.y, rot.z);
	m_pSphere3->RotateDegrees(rot.x, rot.y, rot.z);
}

