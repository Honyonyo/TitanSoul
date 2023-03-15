#include "stdafx.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "YetiScene.h"
#include "Floor.h"
#include "IntroScene.h"

HRESULT MainGame::Init(void)
{
	GameNode::Init(true);
	//SCENEMANAGER->addScene("IntroScene", new IntroScene);
	SCENEMANAGER->addScene("Floor", new Floor);
	//SCENEMANAGER->addScene("YetiScene", new YetiScene);
	
	//SCENEMANAGER->changeScene("IntroScene");
	//SCENEMANAGER->changeScene("YetiScene");
	SCENEMANAGER->changeScene("Floor");
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