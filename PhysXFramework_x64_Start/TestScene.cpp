#include "stdafx.h"
#include "TestScene.h"
#include "CubePosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"
#include "TorusPosColorNorm.h"

void TestScene::Initialize()
{
	m_pBox = new CubePosColorNorm(5.f, 2.f, 1.f);
	AddGameObject(m_pBox);
	m_pBox->Translate(0.f, 1.f, 0.f);

	m_pTorus = new TorusPosColorNorm(8.f, 50, 1.f, 10, XMFLOAT4{DirectX::Colors::DarkRed});
	AddGameObject(m_pTorus);

	m_pCylinder = new CylinderPosColorNorm(1.5f, 10, 4.f);
	m_pCylinder->Translate(0.f, 8.f, 0.f);
	m_pTorus->AddChild(m_pCylinder);

	m_SceneContext.GetCamera()->SetPosition({ 0, 10, -25 });
	m_SceneContext.GetCamera()->SetForward({ 0, -0.3f, 1 });

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

void TestScene::Update()
{
	const float totalTime = m_SceneContext.GetGameTime()->GetTotal();
	m_pBox->RotateDegrees(0.f, m_RotSpeed * totalTime, 0.f);

	XMFLOAT3 translation{};

	const float movSpeed{ m_MovSpeed * m_SceneContext.GetGameTime()->GetElapsed() };

	if(m_SceneContext.GetInput()->IsActionTriggered(LEFT))
	{
		translation.x -= movSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(RIGHT))
	{
		translation.x += movSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(UP))
	{
		translation.y += movSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(DOWN))
	{
		translation.y -= movSpeed;
	}

	XMStoreFloat3(&translation, XMVectorAdd(XMLoadFloat3(&translation), XMLoadFloat3(&m_pBox->GetPosition())));
	m_pBox->Translate(translation);
}

void TestScene::Draw() const
{

}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
