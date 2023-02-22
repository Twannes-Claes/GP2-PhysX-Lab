#pragma once
#include "GameScene.h"

class BoxForceScene final : public GameScene
{
public:
	BoxForceScene() :GameScene(L"BoxForceScene") {}
	~BoxForceScene() override = default;
	BoxForceScene(const BoxForceScene& other) = delete;
	BoxForceScene(BoxForceScene&& other) noexcept = delete;
	BoxForceScene& operator=(const BoxForceScene& other) = delete;
	BoxForceScene& operator=(BoxForceScene&& other) noexcept = delete;

	enum InputID : int
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	GameObject* m_pBox{ nullptr };

	const XMFLOAT3 m_Pos{ 0.f, 1.f, 0.f };

	void Reset() const;

};

