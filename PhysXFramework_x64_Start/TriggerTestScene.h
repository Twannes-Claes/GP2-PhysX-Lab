#pragma once
#include "GameScene.h"

class TriggerTestScene final : public GameScene
{

public:

	TriggerTestScene() :GameScene(L"TriggerTestScene") {}
	~TriggerTestScene() override = default;
	TriggerTestScene(const TriggerTestScene& other) = delete;
	TriggerTestScene(TriggerTestScene&& other) noexcept = delete;
	TriggerTestScene& operator=(const TriggerTestScene& other) = delete;
	TriggerTestScene& operator=(TriggerTestScene&& other) noexcept = delete;

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

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:

	GameObject* m_pSphere{ nullptr };

	PxRigidStatic* m_pTriggerLeft{};

	PxRigidStatic* m_pTriggerRight{};

	bool m_IsTriggeredLeft{};
	bool m_IsTriggeredRight{};

};
