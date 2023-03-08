#include "stdafx.h"
#include "GImage.h"

HRESULT GImage::Init(float width, float height)
{
	//재초기화 방지용 / 이미지 정보에 값이 들어 있다면 해제부터 하겠다.
	if (_imageInfo != nullptr) this->Release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = nullptr;
	_isTrans = true;
	_transColor = RGB(255, 0, 255);

	//리소스 얻어오는 데 실패
	if (_imageInfo->hBit == 0)
	{
		Release();
		return E_FAIL;
	}

	//DC 해제
	ReleaseDC(_hWnd, hdc);

	_pixel = new COLORREF * [height];
	for (int i = 0; i < height; i++)
	{
		_pixel[i] = new COLORREF[width];
	}

	return S_OK;
}

HRESULT GImage::Init(string path, float width, float height)
{
	//재초기화 방지용 / 이미지 정보에 값이 들어 있다면 해제부터 하겠다.
	if (_imageInfo != nullptr) this->Release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hinstance, path.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = nullptr;
	_isTrans = true;
	_transColor = RGB(255, 0, 255);

	//리소스 얻어오는 데 실패
	if (_imageInfo->hBit == 0)
	{
		Release();
		return E_FAIL;
	}

	//DC 해제
	ReleaseDC(_hWnd, hdc);

	_pixel = new COLORREF * [height];
	for (int i = 0; i < height; i++)
	{
		_pixel[i] = new COLORREF[width];
	}

	return S_OK;
}

void GImage::TileRender(GImage* tileSheet, vector<vector<int>> tileVector, int tileStartNum)
{
	int tileLineMaxNumber = tileSheet->GetWidth() / TILE_SIZE;
	for (int low = 0; low < tileVector.size(); low++)
	{
		for (int column = 0; column < tileVector[low].size(); column++)
		{
			int tileNumber = (tileVector[low][column] - tileStartNum) < 0 ?
				0 : (tileVector[low][column] - tileStartNum);
			int	tileLow = tileNumber / tileLineMaxNumber;
			int tileColumn = tileNumber - (tileLow * tileLineMaxNumber);
			int a = tileVector[low][column];
			if (_isTrans)
			{
				//GdiTransparentBlt(): 비트맵을 불러올 때 특정 색상을 제외하고 복사
				GdiTransparentBlt
				(
					_imageInfo->hMemDC,	//복사할 장소의 DC(화면에 보여지는 DC)
					column * TILE_SIZE,	//복사될 좌표 시작(x)
					low * TILE_SIZE,	//복사될 좌표 시작(y)
					TILE_SIZE,	//복사될 이미지 가로크기
					TILE_SIZE,	//복사될 이미지 세로크기
					tileSheet->_imageInfo->hMemDC,				//복사될 대상 메모리 DC
					tileColumn * TILE_SIZE, tileLow * TILE_SIZE, //복사 시작 시점
					TILE_SIZE,	//복사 영역 가로크기
					TILE_SIZE,	//복사 영역 세로크기
					_transColor			//복사할 때 제외할 색상
				);
			}
			else
			{
				//BitBlt(): DC 간의 영역끼리 서로 고속복사를 해준다.
				//SRCCOPY: 소스 영역을 대상 영역에 복사한다.
				BitBlt(_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, tileSheet->_imageInfo->hMemDC, 0, 0, SRCCOPY);
			}
		}
	}
	for (int i = 0; i < _imageInfo->height; i++)
	{
		for (int j = 0; j < _imageInfo->width; j++)
		{
			_pixel[i][j] = GetPixel(_imageInfo->hMemDC, j, i);
		}
	}

	float colorR = GetRValue(_pixel[00][00]);
	float colorG = GetGValue(_pixel[00][00]);
	float colorB = GetBValue(_pixel[00][00]);
	cout << "\n충돌 타일 색상 1,1\nR:" << colorR << "\tG:" << colorG << "\tB:" << colorB << endl;
	_renderImg = new CImage(IMAGEMANAGER->CreateBitmapFromHBit(_imageInfo->hOBit), _imageInfo->width, _imageInfo->height);
}

void GImage::Render(HDC hdc, int targetX, int targetY, int targetW, int targetH, int cropX, int cropY)
{
	int width = (targetW == 0) ? _imageInfo->width : targetW;
	int height = (targetH == 0) ? _imageInfo->height : targetH;
	if (_isTrans)
	{
		//GdiTransparentBlt(): 비트맵을 불러올 때 특정 색상을 제외하고 복사
		GdiTransparentBlt
		(
			hdc,				//복사할 장소의 DC(화면에 보여지는 DC)
			targetX,					//복사될 좌표 시작(x)
			targetY,					//복사될 좌표 시작(y)
			width,	//복사될 이미지 가로크기
			height,	//복사될 이미지 세로크기
			_imageInfo->hMemDC, //복사될 대상 메모리 DC
			cropX, cropY,				//복사 시작 시점
			width,	//복사 영역 가로크기
			height,	//복사 영역 세로크기
			_transColor			//복사할 때 제외할 색상
		);
	}
	else
	{
		//BitBlt(): DC 간의 영역끼리 서로 고속복사를 해준다.
		//SRCCOPY: 소스 영역을 대상 영역에 복사한다.
		BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}

}

void GImage::Release()	//release에 문제생김
{
	//if(_pixel != nullptr)
	//	for (int i = 0; i < _imageInfo->height; i++)
	//	{
	//		SAFE_DELETE_ARRAY(_pixel);
	//	}

	//SAFE_DELETE(_imageInfo);
}

