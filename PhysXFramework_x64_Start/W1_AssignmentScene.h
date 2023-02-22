#pragma once
#include "GameScene.h"
#include <vector>

class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

	enum InputID : int
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE
	};

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	const int m_AmountBlocks{10};

	std::vector<GameObject*> m_pBoxes{ nullptr };

	GameObject* m_pFloor{ nullptr };

	GameObject* m_pSphere{ nullptr };

	void Reset() const;

	const XMFLOAT3 m_BoxDimension{ 1.f , 1.f, 1.f };

	const float m_FloorScale{ 50.f };

};

