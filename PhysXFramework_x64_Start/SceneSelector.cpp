#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

#define W01
//#define W02

#ifdef W01
#include "TestScene.h"
#include "PhysXTestScene.h"
#include "BouncingBallScene.h"
#include "BoxForceScene.h"
#include "W1_AssignmentScene.h"

#endif

#ifdef W02

#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		pSceneManager->AddGameScene(new TestScene());
		pSceneManager->AddGameScene(new PhysXTestScene());
		pSceneManager->AddGameScene(new BouncingBallScene());
		pSceneManager->AddGameScene(new BoxForceScene());
		pSceneManager->AddGameScene(new W1_AssignmentScene());
#endif

#ifdef W02

#endif

	}
}

