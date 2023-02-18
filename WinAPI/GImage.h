#pragma once
class GImage
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0, LOAD_FILE,	//���ҽ��� �ε�, ���Ϸ� �ε�
		LOAD_EMPTY, LOAD_END		//�� ��Ʈ�� �ε�
	};

	typedef struct tagImage
	{
		DWORD		resID;			//���ҽ� ID
		HBITMAP		hBit;			//��Ʈ��
		HBITMAP		hOBit;			//�õ� ��Ʈ��
		int			width;			//�̹��� ����ũ��
		int			height;			//�̹��� ����ũ��
		BYTE		loadType;		//�̹��� �ε� Ÿ��
		HDC			hMemDC;
		tagImage()
		{
			resID = 0;
			hMemDC = nullptr;
			hBit = nullptr;
			hOBit = nullptr;
			width = 0;
			height = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, * LPIMAGE_INFO;

private:
	char* _fileName;		//�̹��� �̸�
	bool			_isTrans;		//����
	COLORREF		_transColor;	//���� ���� RGB (RGB(255,0,255))

	CImage* _renderImg;

	//���� ����� hdc�� memDC�� ��� ������ �ϰ��ִµ�
	//�� ������� GImage�� hmemDC�� �����Ѵٸ�
	//GImage�� �׷��� ���� ������� ������?
	//
public:
	COLORREF** _pixel;

	LPIMAGE_INFO	_imageInfo;		//�̹��� ����
	HRESULT Init(float width, float height);
	HRESULT Init(string path, float width, float height);
	HDC GetMemDC() { return _imageInfo->hMemDC; }
	CImage* GetRenderImage() { return _renderImg; }
	float GetWidth() { return _imageInfo->width; }
	float GetHight() { return _imageInfo->height; }
	void TileRender(GImage* tileSheet, vector<vector<int>> tileVector, int tileStartNum);
	void Render(HDC hdc, int targetX=0, int targetY=0, int targetW=0, int targetH=0, int cropX=0, int cropY=0);
	void Release();

	GImage()
		: _imageInfo(nullptr),
		_fileName(nullptr),
		_isTrans(false),
		_pixel(nullptr),
		_transColor(RGB(255, 0, 255))
	{

	}
};