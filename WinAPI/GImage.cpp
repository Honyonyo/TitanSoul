#include "stdafx.h"
#include "GImage.h"

HRESULT GImage::Init(float width, float height)
{
	//���ʱ�ȭ ������ / �̹��� ������ ���� ��� �ִٸ� �������� �ϰڴ�.
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

	//���ҽ� ������ �� ����
	if (_imageInfo->hBit == 0)
	{
		Release();
		return E_FAIL;
	}

	//DC ����
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
	//���ʱ�ȭ ������ / �̹��� ������ ���� ��� �ִٸ� �������� �ϰڴ�.
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

	//���ҽ� ������ �� ����
	if (_imageInfo->hBit == 0)
	{
		Release();
		return E_FAIL;
	}

	//DC ����
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
				//GdiTransparentBlt(): ��Ʈ���� �ҷ��� �� Ư�� ������ �����ϰ� ����
				GdiTransparentBlt
				(
					_imageInfo->hMemDC,	//������ ����� DC(ȭ�鿡 �������� DC)
					column * TILE_SIZE,	//����� ��ǥ ����(x)
					low * TILE_SIZE,	//����� ��ǥ ����(y)
					TILE_SIZE,	//����� �̹��� ����ũ��
					TILE_SIZE,	//����� �̹��� ����ũ��
					tileSheet->_imageInfo->hMemDC,				//����� ��� �޸� DC
					tileColumn * TILE_SIZE, tileLow * TILE_SIZE, //���� ���� ����
					TILE_SIZE,	//���� ���� ����ũ��
					TILE_SIZE,	//���� ���� ����ũ��
					_transColor			//������ �� ������ ����
				);
			}
			else
			{
				//BitBlt(): DC ���� �������� ���� ��Ӻ��縦 ���ش�.
				//SRCCOPY: �ҽ� ������ ��� ������ �����Ѵ�.
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
	cout << "\n�浹 Ÿ�� ���� 1,1\nR:" << colorR << "\tG:" << colorG << "\tB:" << colorB << endl;
	_renderImg = new CImage(IMAGEMANAGER->CreateBitmapFromHBit(_imageInfo->hOBit), _imageInfo->width, _imageInfo->height);
}

void GImage::Render(HDC hdc, int targetX, int targetY, int targetW, int targetH, int cropX, int cropY)
{
	int width = (targetW == 0) ? _imageInfo->width : targetW;
	int height = (targetH == 0) ? _imageInfo->height : targetH;
	if (_isTrans)
	{
		//GdiTransparentBlt(): ��Ʈ���� �ҷ��� �� Ư�� ������ �����ϰ� ����
		GdiTransparentBlt
		(
			hdc,				//������ ����� DC(ȭ�鿡 �������� DC)
			targetX,					//����� ��ǥ ����(x)
			targetY,					//����� ��ǥ ����(y)
			width,	//����� �̹��� ����ũ��
			height,	//����� �̹��� ����ũ��
			_imageInfo->hMemDC, //����� ��� �޸� DC
			cropX, cropY,				//���� ���� ����
			width,	//���� ���� ����ũ��
			height,	//���� ���� ����ũ��
			_transColor			//������ �� ������ ����
		);
	}
	else
	{
		//BitBlt(): DC ���� �������� ���� ��Ӻ��縦 ���ش�.
		//SRCCOPY: �ҽ� ������ ��� ������ �����Ѵ�.
		BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}

}

void GImage::Release()	//release�� ��������
{
	//if(_pixel != nullptr)
	//	for (int i = 0; i < _imageInfo->height; i++)
	//	{
	//		SAFE_DELETE_ARRAY(_pixel);
	//	}

	//SAFE_DELETE(_imageInfo);
}

