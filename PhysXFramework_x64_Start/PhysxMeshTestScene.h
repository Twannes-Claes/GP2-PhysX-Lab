#pragma once
#include "GameScene.h"

class PhysxMeshTestScene final : public GameScene
{

public:

	PhysxMeshTestScene() :GameScene(L"PhysxMeshTestScene") {}
	~PhysxMeshTestScene() override = default;
	PhysxMeshTestScene(const PhysxMeshTestScene& other) = delete;
	PhysxMeshTestScene(PhysxMeshTestScene&& other) noexcept = delete;
	PhysxMeshTestScene& operator=(const PhysxMeshTestScene& other) = delete;
	PhysxMeshTestScene& operator=(PhysxMeshTestScene&& other) noexcept = delete;

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

private:

	GameObject* m_pChairConvex{};

	GameObject* m_pChairTriangle{};

	PxVec3 m_KinematicPos{};

};

