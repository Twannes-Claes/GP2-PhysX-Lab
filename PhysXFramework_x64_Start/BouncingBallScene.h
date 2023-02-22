#pragma once

#include "GameScene.h"

class BouncingBallScene final : public GameScene
{
public:
	BouncingBallScene() :GameScene(L"GameScene") {}
	~BouncingBallScene() override = default;
	BouncingBallScene(const BouncingBallScene& other) = delete;
	BouncingBallScene(BouncingBallScene&& other) noexcept = delete;
	BouncingBallScene& operator=(const BouncingBallScene& other) = delete;
	BouncingBallScene& operator=(BouncingBallScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	GameObject* m_pSphere1{ nullptr };
	GameObject* m_pSphere2{ nullptr };
	GameObject* m_pSphere3{ nullptr };

	const XMFLOAT3 m_Pos{ 0.f, 10.f, 0.f };

	const float m_PosOffset{ 5.f };

	void Reset() const;

};

