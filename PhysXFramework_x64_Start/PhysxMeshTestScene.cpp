#include "stdafx.h"
#include "PhysxMeshTestScene.h"

#include "ContentManager.h"
#include "MeshObject.h"
#include "PhysxManager.h"

void PhysxMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysx = PhysxManager::GetInstance()->GetPhysics();

	const auto pDefaultMaterial = pPhysx->createMaterial(.5f, .5f, 0.9f);

	//ground plane
	const auto pGroundActor = pPhysx->createRigidStatic(PxTransform({ PxPiDivTwo, {0,0,1} }));

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");

	AddGameObject(m_pChairConvex);

	//CONVEX MESH
	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");

	const auto pConvexChairActor = pPhysx->createRigidDynamic(PxTransform{PxIdentity});

	PxRigidActorExt::createExclusiveShape(*pConvexChairActor, PxConvexMeshGeometry{pConvexMesh}, *pDefaultMaterial);

	const PxVec3 massLocalPose{ 0, 2.5f, 0.5f };

	PxRigidBodyExt::updateMassAndInertia(*pConvexChairActor, 10.f, &massLocalPose);

	m_pChairConvex->AttachRigidActor(pConvexChairActor);

	m_pChairConvex->Translate(0.f, 20.f, 0.5f);



	//TRIANGLE MESH
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");

	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");

	const auto pTriangleChairActor = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });

	pTriangleChairActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pTriangleChairActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	const PxVec3 massLocalPose2{ 0, 2.5f, 0.5f };

	m_pChairTriangle->AttachRigidActor(pTriangleChairActor);

	m_pChairTriangle->Translate({ 10.f, 0.f, 0.f });

	m_KinematicPos = { 10.f, 0.f, 0.f };

}

void PhysxMeshTestScene::Update()
{
	if(m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(0.f, 20.f, 0.5f);
		m_pChairConvex->RotateDegrees(0, 0, 0);

		m_pChairTriangle->Translate({ 10.f, 0.f, 0.f });
		m_KinematicPos = { 10.f, 0.f, 0.f };
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPos.x -= 50.f * m_SceneContext.GetGameTime()->GetElapsed();

		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPos });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_KinematicPos.x += 50.f * m_SceneContext.GetGameTime()->GetElapsed();

		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPos });
	}
}

void PhysxMeshTestScene::Draw() const
{

}
