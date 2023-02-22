#pragma once
#include "GameScene.h"

class PhysXTestScene final : public GameScene
{
public:
	PhysXTestScene() :GameScene(L"PhysXTestScene") {}
	~PhysXTestScene() override = default;
	PhysXTestScene(const PhysXTestScene& other) = delete;
	PhysXTestScene(PhysXTestScene&& other) noexcept = delete;
	PhysXTestScene& operator=(const PhysXTestScene& other) = delete;
	PhysXTestScene& operator=(PhysXTestScene&& other) noexcept = delete;

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

	GameObject* m_pCube{ nullptr };

	const XMFLOAT3 m_Pos{ 0.f, 5.f, 0.f };

	const XMFLOAT3 m_Rot{ 0.f, 110.f, 45.f };

	void Reset() const;

};

