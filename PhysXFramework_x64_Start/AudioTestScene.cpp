#include "stdafx.h"
#include "AudioTestScene.h"

#include "Logger.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"

void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysx = PhysxManager::GetInstance()->GetPhysics();

	const auto pDefaultMaterial = pPhysx->createMaterial(0.f, .5f, 0.2f);

	//ground plane
	const auto pGroundActor = pPhysx->createRigidStatic(PxTransform({ PxPiDivTwo, {0,0,1} }));

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	//sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10.f, 10.f, XMFLOAT4(Colors::DarkRed));

	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysx->createRigidDynamic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);

	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	const auto input = m_SceneContext.GetInput();

	input->AddInputAction({ Play2D, InputTriggerState::pressed, '2'});
	input->AddInputAction({ Play3D, InputTriggerState::pressed, '3'});
	input->AddInputAction({ Increase, InputTriggerState::pressed, VK_UP });
	input->AddInputAction({ Decrease, InputTriggerState::pressed, VK_DOWN });


	//sound
	const auto pFMOD = SoundManager::GetInstance()->GetSystem();

	FMOD::Sound* pSound2D{};

	FMOD_RESULT result = pFMOD->createStream("Resources/Sounds/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL , nullptr,&pSound2D);

	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFMOD->playSound(pSound2D, nullptr, true, &m_pChannel2D);

	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel2D->setVolume(.5f);



	FMOD::Sound* pSound3D{};
	
	FMOD_RESULT result2 = pFMOD->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	
	SoundManager::GetInstance()->ErrorCheck(result2);
	
	result2 = pFMOD->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	
	SoundManager::GetInstance()->ErrorCheck(result2);
	
	m_pChannel3D->setVolume(1.f);

	m_pChannel3D->set3DMinMaxDistance(1.f, 200.f);

}

void AudioTestScene::Update()
{
	constexpr float force{ 5.f };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool isPaused{};
		m_pChannel2D->getPaused(&isPaused);
	
		m_pChannel2D->setPaused(!isPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool isPaused{};
		m_pChannel3D->getPaused(&isPaused);
	
		m_pChannel3D->setPaused(!isPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Increase))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
		volume += 0.1f;
		Clamp(volume, 10.f, 0.f);
		m_pChannel2D->setVolume(volume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Decrease))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
		volume -= 0.1f;
		Clamp(volume, 10.f, 0.f);
		m_pChannel2D->setVolume(volume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);
	}

	const auto cam = m_SceneContext.GetCamera();

	auto pos = TOFMOD(cam->GetPosition());
	auto forward = TOFMOD(cam->GetForward());
	auto up = TOFMOD(cam->GetUp());

	FMOD_VECTOR vel{};

	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();

	vel.x = (pos.x - m_PrevCamPos.x) / deltaT;
	vel.y = (pos.y - m_PrevCamPos.y) / deltaT;
	vel.z = (pos.z - m_PrevCamPos.z) / deltaT;

	m_PrevCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos ,&vel, &forward, & up );

	auto spherePos = TOFMOD(m_pSphere->GetPosition());
	auto sphereVel = TOFMOD(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());

	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

inline FMOD_VECTOR AudioTestScene::TOFMOD(const XMFLOAT3& v) const
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}


inline FMOD_VECTOR AudioTestScene::TOFMOD(const PxVec3& v) const
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Draw() const
{
}
