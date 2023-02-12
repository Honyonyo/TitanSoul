#include "stdafx.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "FrostMountains.h"

HRESULT MainGame::Init(void)
{
	GameNode::Init(true);
	SCENEMANAGER->addScene("FrostMountain", new FrostMountains);
	SCENEMANAGER->changeScene("FrostMountain");
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