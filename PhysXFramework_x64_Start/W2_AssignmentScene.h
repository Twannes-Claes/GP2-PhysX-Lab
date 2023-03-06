#pragma once
#include "GameScene.h"

class W2_AssignmentScene final : public GameScene
{

public:

	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:

	void Initialize() override;
	void Update() override;
	void Draw() const override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:

	GameObject* m_pLevel{};

	GameObject* m_pSphere{};
	GameObject* m_pSphere2{};
	GameObject* m_pSphere3{};

	XMFLOAT3 m_SpherePos{0.f, 10.f, 0.f};

	GameObject* m_pTriggerLeft{};
	GameObject* m_pTriggerRight{};

	GameObject* m_pBoxLeft{};
	GameObject* m_pBoxRight{};

	GameObject* m_pTrapLeft{};
	GameObject* m_pTrapRight{};

	XMFLOAT3 m_TrapPos{ 8.85f, 17.f, 0.f };

	FMOD::Channel* m_pChannel2D{};
	FMOD::Sound* m_pSound2D{};


	bool m_IsTriggeredLeft{};
	bool m_IsTriggeredRight{};

};