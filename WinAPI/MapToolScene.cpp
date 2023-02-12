#include "stdafx.h"
#include "MapToolScene.h"

HRESULT MapToolScene::Init(void)
{
	return S_OK;
}

void MapToolScene::Release(void)
{
}

void MapToolScene::Update(void)
{
}

void MapToolScene::Render(void)
{
	TextOut(getMemDC(), 100, 100, "·ê·ç", strlen("·ê·ç"));
}
