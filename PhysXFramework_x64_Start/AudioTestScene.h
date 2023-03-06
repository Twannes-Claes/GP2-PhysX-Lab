#pragma once
#include "GameScene.h"

class AudioTestScene final : public GameScene
{

public:

	AudioTestScene() :GameScene(L"AudioTestScene") {}
	~AudioTestScene() override = default;
	AudioTestScene(const AudioTestScene& other) = delete;
	AudioTestScene(AudioTestScene&& other) noexcept = delete;
	AudioTestScene& operator=(const AudioTestScene& other) = delete;
	AudioTestScene& operator=(AudioTestScene&& other) noexcept = delete;

	enum InputID : int
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

protected:

	void Initialize() override;
	FMOD_VECTOR TOFMOD(const XMFLOAT3& v) const;
	FMOD_VECTOR TOFMOD(const PxVec3& v) const;
	void Update() override;
	void Draw() const override;

private:

	GameObject* m_pSphere{ nullptr };

	FMOD::Channel* m_pChannel2D{};
	FMOD::Channel* m_pChannel3D{};

	FMOD_VECTOR m_PrevCamPos{};

	enum InputIDS : int
	{
		Play2D,
		Play3D,
		Increase,
		Decrease
	};

};
