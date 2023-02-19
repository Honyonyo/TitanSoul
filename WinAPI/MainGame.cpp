#include "stdafx.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "FrostMountains.h"
#include "YetiScene.h"
#include "Floor.h"
#include "ColossusScene.h"
#include "IntroScene.h"

HRESULT MainGame::Init(void)
{
	GameNode::Init(true);
	//SCENEMANAGER->addScene("IntroScene", new IntroScene);
	//SCENEMANAGER->addScene("FrostMountain", new FrostMountains);
	//SCENEMANAGER->addScene("Floor", new Floor);
	SCENEMANAGER->addScene("YetiScene", new YetiScene);
	
	//SCENEMANAGER->changeScene("Floor");
	SCENEMANAGER->changeScene("YetiScene");
	//SCENEMANAGER->changeScene("IntroScene");
	OBJECTMANAGER->PlayerInit();
	
	return S_OK;
}

void MainGame::Release(void)
{
	GameNode::Release();
}

void MainGame::Update(void)
{
	GameNode::Update();
	OBJECTMANAGER->Update();
	SCENEMANAGER->Update();
}

void MainGame::Render(void)
{
	IMAGEMANAGER->Begin();	
	SCENEMANAGER->Render();
	OBJECTMANAGER->Render();
	IMAGEMANAGER->Render();
	TIMEMANAGER->Render();
	IMAGEMANAGER->End();
}