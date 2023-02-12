#pragma once
#include <wrl.h>

#include "SingletonBase.h"
#include "CImage.h"
#include "GImage.h"

using Microsoft::WRL::ComPtr;

static const enum eTileSheetKey
{
	tile_overworld = 0,
	tile_overworld2,
	tile_overworld3,
	tile_underworld,
	tile_jam,
	tile_spectiles,
	tileCount
};
static map<string, eTileSheetKey> tileKeyTable;

static const enum eTileLayerType
{
	eBG = 0,	//바닥
	eFG,	//벽
	eCOL,	//픽충
	eMAT,
	eTileLayerTypeCount
};

static const enum eLayer
{
	eLayerBg = 0,
	eLayerShadow,
	eLayerUnderPlayer,
	eLayerPlayer,
	eLayerUpperPlayer,
	eLayerWall,
	eLayerTop,

	eLayerNumCount
};
static map<string, eTileLayerType> layerTypeTable;

class Animation;
class ImageManager : public SingletonBase<ImageManager>
{
private:
	std::map <string, CImage*> m_imageList;
	CImage* m_tileImageList[eTileSheetKey::tileCount];
	ID2D1Bitmap1* m_layer[eLayerNumCount];

	GImage* m_spectiles;
	bool isShake;
	RECT m_screenRect;

	bool m_isViewCollision = false;

	D2D1_MATRIX_3X2_F m_trsDefault;

	//D2D Brush
	ID2D1SolidColorBrush* m_brush;
	ID2D1SolidColorBrush* m_blackBrush;

private:
	ID2D1Factory1* _pD2Dfactory;
	ID2D1Bitmap* m_mapRenderBitmap;
	D2D1_BITMAP_PROPERTIES m_pros = D2D1::BitmapProperties();

	//IWICImagingFactory2* ImageFactory;
	ComPtr<IWICImagingFactory> cImageFactory = nullptr;
	D2D1_FACTORY_OPTIONS options;

	// Direct3D device
	ComPtr<ID3D11Device1> Direct3DDevice;
	// Direct3D device context
	ComPtr<ID3D11DeviceContext1> Direct3DContext;

	// Direct2D device
	ID2D1Device* m_d2dDevice = NULL;
	// Direct2D device context
	// The Render target device context
	ID2D1DeviceContext* m_d2dContext = NULL;

	// DXGI swap chain
	IDXGISwapChain1* DXGISwapChain;
	// Direct2D target Rendering bitmap
	// (linked to DXGI back buffer which is linked to Direct3D pipeline)
	ID2D1Bitmap1* Direct2DBackBuffer;
	// The Render target device context
	//ID2D1DeviceContext* Screen;

	//D2DCropEffect
	ComPtr<ID2D1Effect> m_cropEffect;
	//Render to hdc (member_pointer DC Render Target)
	ID2D1DCRenderTarget* m_pDCRT;

public:
	ImageManager();
	~ImageManager();
public:
	void Init();
	void Render();

	void BitmapRenderUseMatrix(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix, float alpha);
	void BitmapRenderToDCUseMatrix(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix);
	void BindD2DtoDC(HDC memDC);

	void Render(CImage* img, float x, float y, float sizeX = 1, float sizeY = 1, float rot = 0, float alpha = 1);
	void CropRender(CImage* img, float x, float y, RECT cropRc, bool isReversed = false, float sizeX = 1, float sizeY = 1, float rot = 0, float alpha = 1);
	void SampleRender(CImage* img, float x, float y, float sizeX = 1, float sizeY = 1, float rot = 0, float alpha = 1);
	void CenterRender(CImage* img, float x, float y, float sizeX = 1, float sizeY = 1, float rot = 0, bool isReverse = false, float alpha = 1);
	void CenterAniRender(CImage* img, float renderTargetX, float renderTargetY, Animation* ani, eLayer layer = eLayerPlayer, bool isReversed = false,  float rot = 0, float alpha = 1);
	void FrameRender(CImage* img, float x, float y, int frameX, int frameY, float sizeX = 1, float sizeY = 1, float rot = 0, bool isReverse = false, float alpha = 1);
	void CenterFrameRender(CImage* img, float x, float y, int frameX, int frameY, eLayer layer = eLayerPlayer,float scale = 1, float rot = 0, float alpha = 1);

	void TileRender(eTileSheetKey key, int tileNumber, int low, int column, int tileLineMaxNumber = 64);

	void ImageLoad();
	void LayerTypeTableGenerator();
	void TileKeyGenerator();
	eTileLayerType LayerTypeStringToEnum(string type);
	eTileSheetKey TileKeyStringToEnum(string tileSheet);

	GImage* AddBackBuffer(string key, float width, float height);

	ID2D1Bitmap1* AddBitmap(std::wstring path, UINT* Width, UINT* Height);
	CImage* AddImage(const std::string key, std::wstring path);
	CImage* AddImage(const std::string key, std::wstring path, int maxFrameX = 1, int maxFrameY = 1);
	CImage* AddTileSheet(eTileSheetKey key, std::wstring path);
	//맵 통짜 이미지 만들기용 함수. CImage* 반환
	//(사용하는 타일시트, 타일 번호 담은 벡터)
	CImage* AddTileLayerImage(map<int, eTileSheetKey> tileFirstgrid, vector<vector<int>> tileVector);

	CImage* FindImage(const std::string key);
	CImage* FindTileSheet(eTileSheetKey key);
	GImage* FindSpecTile() { return m_spectiles; }

	void DrawCircle(float x, float y, float width);
	void DrawCircleCamera(float x, float y, float width);
	void DrawRect(RECT rt);
	void DrawRectCenter(RECT rt, CImage* img);
	void DrawColorRender(CImage* img, float x, float y, float sizeX, float sizeY, float rot, bool isReverse, D2D1_COLOR_F colr);

	void ResetTRS()
	{
		D2D1_MATRIX_3X2_F matT;
		matT = D2D1::Matrix3x2F::Translation(100, 100);

		m_trsDefault.dx = 0;
		m_trsDefault.dy = 0;
		m_d2dContext->SetTransform(m_trsDefault);
	}
	void Begin() { m_d2dContext->BeginDraw(); }
	void End()
	{
		m_d2dContext->EndDraw();
		DXGI_PRESENT_PARAMETERS parameters = { 0 };
		//parameters.DirtyRectsCount = 0;
		//parameters.pDirtyRects = nullptr;
		//parameters.pScrollRect = nullptr;
		//parameters.pScrollOffset = nullptr;
		/*hr =*/DXGISwapChain->Present1(1, 0, &parameters);
	}

	void SetViewCollision(bool isView) { m_isViewCollision = isView; }
	ID2D1SolidColorBrush* SetBrushColor(COLORREF rgbColor, float alpha = 1)
	{
		m_brush->SetColor({ 1.0f / 255 * GetRValue(rgbColor),1.0f / 255 * GetGValue(rgbColor),1.0f / 255 * GetBValue(rgbColor),alpha });
		return m_brush;
	}
	ID2D1SolidColorBrush* SetBrushColor(D2D1_COLOR_F d2dColorF) { m_brush->SetColor(d2dColorF); return m_brush; }
	ID2D1DeviceContext** GetDeviceContext(void) { return &m_d2dContext; }
	ID2D1SolidColorBrush* GetBlackBrush(void) { return m_blackBrush; }
	ID2D1SolidColorBrush* GetBrush(void) { return m_brush; }
	bool GetViewCollision() { return m_isViewCollision; }

};


