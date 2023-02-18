#pragma once
class GImage
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0, LOAD_FILE,	//리소스로 로딩, 파일로 로딩
		LOAD_EMPTY, LOAD_END		//빈 비트맵 로딩
	};

	typedef struct tagImage
	{
		DWORD		resID;			//리소스 ID
		HBITMAP		hBit;			//비트맵
		HBITMAP		hOBit;			//올드 비트맵
		int			width;			//이미지 가로크기
		int			height;			//이미지 세로크기
		BYTE		loadType;		//이미지 로드 타입
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
	char* _fileName;		//이미지 이름
	bool			_isTrans;		//배경색
	COLORREF		_transColor;	//배경색 없앨 RGB (RGB(255,0,255))

	CImage* _renderImg;

	//렌더 대상을 hdc나 memDC로 잡고 렌더를 하고있는데
	//그 대상으로 GImage의 hmemDC를 선택한다면
	//GImage에 그려진 값이 저장되지 않을까?
	//
public:
	COLORREF** _pixel;

	LPIMAGE_INFO	_imageInfo;		//이미지 정보
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