#include "stdafx.h"
#include "ImageManager.h"
#include "Camera.h"
#include "Animation.h"

ImageManager::ImageManager()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2Dfactory);
}

ImageManager::~ImageManager()
{
	for (auto iter : m_imageList)
	{
		SAFE_DELETE(iter.second);
	}
	m_imageList.clear();

	SAFE_RELEASE(Direct2DBackBuffer);
	SAFE_RELEASE(DXGISwapChain);
	SAFE_RELEASE(m_d2dContext);           // Direct2D 1.0 also (this line only)
	SAFE_RELEASE(m_d2dDevice);
	SAFE_RELEASE(Direct3DContext);
	SAFE_RELEASE(_pD2Dfactory);
}

void ImageManager::Init()
{
	TileKeyGenerator();
	LayerTypeTableGenerator();

	isShake = false;
	RECT rc;
	GetClientRect(_hWnd, &rc);

	//ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
//#if defined(_DEBUG)
//	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
//#endif
//	const GUID guid = __uuidof(ID2D1Factory1);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL returnedFeatureLevel;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	D3D11CreateDevice
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&device, &returnedFeatureLevel,
		&context
	);
	device.As(&Direct3DDevice);
	context.As(&Direct3DContext);
	ComPtr<IDXGIDevice> dxgiDevice;

	Direct3DDevice.As(&dxgiDevice);
	_pD2Dfactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);
	m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);

	//↓↓스왑체인↓↓
	// Get the GPU we are using
	IDXGIAdapter* dxgiAdapter;	//그래픽카드
	dxgiDevice->GetAdapter(&dxgiAdapter);

	// Get the DXGI factory instance
	IDXGIFactory2* dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	// Describe Windows 7-compatible Windowed swap chain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;		//SampleDesc : 멀티패킷 파라미터(안티에일리어싱)
	swapChainDesc.SampleDesc.Quality = 0;	//SampleDesc : 멀티패킷 파라미터(안티에일리어싱)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//이 스왑체인이 렌더링 타깃이 되도록
	swapChainDesc.BufferCount = 2;			//화면과 오프스크린 총 2개
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Create DXGI swap chain targeting a window handle (the only Windows 7-compatible option)
	dxgiFactory->CreateSwapChainForHwnd
	(
		Direct3DDevice.Get(),
		_hWnd, &swapChainDesc,
		nullptr,
		nullptr,
		&DXGISwapChain
	);

	// Get the back buffer as an IDXGISurface (Direct2D doesn't accept an ID3D11Texture2D directly as a Render target)
	IDXGISurface* dxgiBackBuffer;
	DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
	//↑↑스왑체인↑↑

	//백버퍼에 링크하기
	// Get screen DPI
	FLOAT dpiX, dpiY;
	_pD2Dfactory->GetDesktopDpi(&dpiX, &dpiY);

	// Create a Direct2D surface (bitmap) linked to the Direct3D texture back buffer via the DXGI back buffer
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1
		(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
			dpiX,
			dpiY
		);

	m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &Direct2DBackBuffer);
	// Set surface as Render target in Direct2D device context
	m_d2dContext->SetTarget(Direct2DBackBuffer);

	//Release
	SAFE_RELEASE(dxgiBackBuffer);
	SAFE_RELEASE(dxgiFactory);
	SAFE_RELEASE(dxgiAdapter);
	//end Release

	CoInitialize(0);
	CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&cImageFactory));
	D2D1_COLOR_F color;
	color.a = 255;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	m_d2dContext->CreateSolidColorBrush(color, &m_blackBrush);
	m_d2dContext->CreateSolidColorBrush(color, &m_brush);
	ImageLoad();

	m_d2dContext->CreateEffect(CLSID_D2D1Crop, &m_cropEffect);
	m_pros.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	//m_pros.
	D2D1_SIZE_U size = {};

	//Render to hdc
	//create DCRenderTarget
	//props = option to create DC Render Target
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(0, 0);
	matR = D2D1::Matrix3x2F::Rotation(NULL);
	matS = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1, 1));

	m_trsDefault = matS * matR * matT;

	for (int i = 0; i < eLayerNumCount; i++)
	{
		m_d2dContext->CreateBitmap(D2D1::SizeU(WINSIZE_X, WINSIZE_Y),NULL, NULL, &bitmapProperties, &m_layer[i]);
	}


	ImageLoad();
}

void ImageManager::Render()
{
	m_d2dContext->SetTarget(Direct2DBackBuffer);

	for (int i = 0; i < eLayerNumCount; i++)
	{
		m_d2dContext->DrawBitmap(m_layer[i]);
	}
}

void ImageManager::BindD2DtoDC(HDC memDC)
{
	//get dimensions from HWND to RECT (client drawing area)
	GetClientRect(_hWnd, &m_screenRect);

	m_pDCRT->BindDC(memDC, &m_screenRect);
}


ID2D1Bitmap1* ImageManager::AddBitmap(std::wstring path, UINT* Width, UINT* Height)
{
	HRESULT hr = S_OK;

	ID2D1Bitmap1* bitmap;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frameDecode = nullptr;

	hr = cImageFactory->CreateDecoderFromFilename
	(
		path.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder
	);
	if (SUCCEEDED(hr))
	{
		decoder->GetFrame(0, &frameDecode);
	}
	else
	{
		cout << "디코더 생성 안됨.  \n\tfile path : " << path.c_str() << endl;
		return nullptr;
	}

	IWICFormatConverter* converter = nullptr;
	hr = cImageFactory->CreateFormatConverter(&converter);

	if (SUCCEEDED(hr))
	{
		hr = converter->Initialize
		(
			frameDecode,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			0,
			0.0,
			WICBitmapPaletteTypeCustom
		);
	}
	else
	{
		cout << "컨버터 생성 안됨. \n\tfile path : " << path.c_str() << endl;
		decoder->Release();
		frameDecode->Release();
		return nullptr;
	}

	if (SUCCEEDED(hr))
	{
		hr = m_d2dContext->CreateBitmapFromWicBitmap(converter, &bitmap);
	}
	else
	{
		cout << "컨버터 이니셜라이즈 안됨.  \n\tfile path : " << path.c_str() << endl;
		decoder->Release();
		frameDecode->Release();
		converter->Release();

		return nullptr;
	}
	if (SUCCEEDED(hr))
	{
		converter->GetSize(Width, Height);
	}
	else
	{
		cout << "비트맵 변환 실패.  \n\tfile path : " << path.c_str() << endl;
	}
	decoder->Release();
	frameDecode->Release();
	converter->Release();

	return bitmap;
}

CImage* ImageManager::AddImage(const std::string key, std::wstring path)
{
	UINT _width = 0, _height = 0;
	CImage* img = new CImage(AddBitmap(path, &_width, &_height), _width, _height);
	m_imageList.insert(std::make_pair(key, img));
	return img;
}

CImage* ImageManager::AddImage(const std::string key, std::wstring path, int maxFrameX, int maxFrameY)
{
	CImage* img = FindImage(key);
	if (img)
	{
		cout << "CImage key값 겹침.\nkey : " << key << endl;
		return img;
	}

	img = new CImage;
	if (FAILED(img->Init(path, maxFrameX, maxFrameY)))
	{
		SAFE_DELETE(img);
		return nullptr;
	};

	m_imageList.insert(make_pair(key, img));

	return img;
}

CImage* ImageManager::AddTileLayerImage(map<int, eTileSheetKey> tileFirstgrid, vector<vector<int>> tileVector)
{
	ID2D1Bitmap1* (bitmap);
	int imageWidth = tileVector[0].size() * TILE_SIZE;
	int imageHeight = tileVector.size() * TILE_SIZE;
	D2D1_SIZE_U size_u = { imageWidth, imageHeight };
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1
		(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_NONE,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	m_d2dContext->CreateBitmap(size_u, 0, 0, bitmapProperties, &bitmap);

	map<int, eTileSheetKey>::reverse_iterator iterTile = tileFirstgrid.rbegin();
	map<int, eTileSheetKey>::reverse_iterator iterTilePrev = tileFirstgrid.rbegin();
	int tileLineMaxNumber = m_tileImageList[iterTile->second]->GetWidth() / TILE_SIZE;

	for (int low = 0; low < tileVector.size(); low++)
	{
		for (int column = 0; column < tileVector[low].size(); column++)
		{
			//타일 시트 여러개가 있을것으로 가정, 현재 인덱스의 타일번호가 몇번째 시트에 있는지 알아보기 위한 과정
			//타일시트 번호 시작점은 tileSheet의 키값으로 알 수 있음
			iterTile = tileFirstgrid.rbegin();			//현재 반복의 tileVetor안의 값
			int tileNumber = tileVector[low][column];
			//타일 시트에서의 가로세로 칸값
			int tileLow, tileColumn = NULL;

			//0값인 경우 빈칸
			if (tileNumber == 0)
			{
				continue;
			}
			/*
			map은 키값으로 자동정렬을 해주기 때문에 뒤로갈수록 key값이 더 커진다
			tileNumber가 이터레이터의 key보다 큰 경우 해당 시트에 있다고 간주
			(tileNumber - 한 시트에 들어간 총 타일 수가 0보다 커야지 진짜로 그 시트 안에 있는것)
			이 경우 반복문을 멈추고 해당 시트좌표값을 가진 이터레이터를 바로 뽑아낸다
			해당 시트에서 tile을 출력한다
			*/
			for (; iterTile != tileFirstgrid.rend();)
			{
				if (tileNumber >= iterTile->first)	//tileNumber가 이터레이터의 key보다 큰 경우 해당 시트에 있다고 간주
				{
					break;	//이터레이터 뽑아내기
				}
				else
				{
					iterTile++;
				}	//아닌경우 반복 더 돌리기
			}//end for

			if (iterTile == tileFirstgrid.rend())
			{
				//cout
				printf("타일 번호 잘못됨 (타일 시트 목록에 없는 번호)\n\t%d,%d : %d\n\n", low, column, tileNumber);
				continue;
			}
			if (iterTile != iterTilePrev)
			{
				iterTilePrev = iterTile;
				tileLineMaxNumber = m_tileImageList[iterTile->second]->GetWidth() / TILE_SIZE;
			}
			tileLow = (tileNumber - iterTile->first) / tileLineMaxNumber;
			tileColumn = (tileNumber - iterTile->first) - (tileLow * tileLineMaxNumber);

			D2D1_POINT_2U point = { (column - 1) * TILE_SIZE, (low - 1) * TILE_SIZE };
			bitmap->CopyFromBitmap(&point, m_tileImageList[iterTile->second]->GetBitMap(), &D2D1::RectU(tileColumn * TILE_SIZE, tileLow * TILE_SIZE, (tileColumn + 1) * TILE_SIZE, (tileLow + 1) * TILE_SIZE));
			//bitmap->CopyFromBitmap(&D2D1::Point2U((column - 1) * TILE_SIZE, (low - 1) * TILE_SIZE), m_tileImageList[iterTile->second]->GetBitMap(), &D2D1::RectU(tileColumn * TILE_SIZE, tileLow * TILE_SIZE, (tileColumn + 1) * TILE_SIZE, (tileLow + 1) * TILE_SIZE));
		}//end for column
	}//end for low

	//렌더타겟 다시 백버퍼로
	ResetTRS();

	CImage* img = new CImage(bitmap, imageWidth, imageHeight);
	return img;
};

CImage* ImageManager::AddTileSheet(eTileSheetKey key, std::wstring path)
{
	UINT _width = 0, _height = 0;
	CImage* img = new CImage(AddBitmap(path, &_width, &_height), _width, _height);
	m_tileImageList[key] = img;
	return img;
}

CImage* ImageManager::FindImage(const std::string key)
{
	auto find = m_imageList.find(key);
	if (find != m_imageList.end())
	{
		return find->second;
	}
	cout << "이미지 key 없음 : " << key << "\n";
	return nullptr;
}

CImage* ImageManager::FindTileSheet(eTileSheetKey key)
{
	if (m_tileImageList[key] != nullptr)
	{
		return m_tileImageList[key];
	}
	cout << "이미지 key 없음 : " << key << "\n";
	return nullptr;
};

void ImageManager::DrawCircle(float x, float y, float width)
{
	D2D1_ELLIPSE ellipse;
	ellipse.radiusX = width;
	ellipse.radiusY = width;
	ellipse.point.x = 0;
	ellipse.point.y = 0;
	D2D1_COLOR_F color;
	color.b = 255;
	color.a = 0.5f;
	color.r = 255;
	color.g = 255;

	D2D1_MATRIX_3X2_F mat;
	mat = D2D1::Matrix3x2F::Translation(x - width / 2, y - width / 2);
}

void ImageManager::DrawCircleCamera(float x, float y, float width)
{
	D2D1_ELLIPSE ellipse;
	ellipse.radiusX = width;
	ellipse.radiusY = width;
	ellipse.point.x = 0;
	ellipse.point.y = 0;
	D2D1_COLOR_F color;
	color.b = 255;
	color.a = 0.5f;
	color.r = 255;
	color.g = 255;
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(((x - width / 2) - CAMERA->GetRenderTargetX()) * CAMERA->GetScale(), ((y - width / 2) - CAMERA->GetRenderTargetY()) * CAMERA->GetScale());
	matR = D2D1::Matrix3x2F::Rotation(0, { 0 ,0 });
	matS = D2D1::Matrix3x2F::Scale(CAMERA->GetScale(), CAMERA->GetScale());
	m_brush->SetColor(color);
	m_d2dContext->SetTransform((matS * matR * matT));

	m_d2dContext->DrawEllipse(ellipse, m_brush);
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
};

void ImageManager::DrawRect(RECT rt)
{
	D2D1_COLOR_F color;
	color.a = 255;
	color.r = 255;
	color.g = 255;
	color.b = 255;

	D2D1_MATRIX_3X2_F mat;
	mat = D2D1::Matrix3x2F::Translation(0, 0);
	m_d2dContext->SetTransform(&mat);
	m_d2dContext->CreateSolidColorBrush(color, &m_brush);

	m_d2dContext->FillRectangle(D2D1_RECT_F{ (float)rt.left ,(float)rt.top,(float)rt.right,(float)rt.bottom }, (m_brush));
}

void ImageManager::DrawRectCenter(RECT rt, CImage* img)
{
	D2D1_COLOR_F color;
	color.b = 255;
	color.a = 255;
	color.r = 255;
	color.g = 255;

	D2D1_MATRIX_3X2_F mat;
	mat = D2D1::Matrix3x2F::Translation(0, 0);
}

void ImageManager::DrawColorRender(CImage* img, float x, float y, float sizeX, float sizeY, float rot, bool isReverse, D2D1_COLOR_F colr)
{
	D2D1_MATRIX_3X2_F matT, matR, matS;

	if (isReverse == false)
	{
		matT = D2D1::Matrix3x2F::Translation((x - img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(sizeX, sizeY);
	}
	else
	{
		matT = D2D1::Matrix3x2F::Translation((x + img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(-sizeX, sizeY);
	}
}

void ImageManager::BitmapRenderUseMatrix(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix, float alpha)
{
	m_d2dContext->SetTransform(matrix);
	m_d2dContext->DrawBitmap(bitmap, 0, alpha);
};

void ImageManager::Render(CImage* img, float x, float y, float sizeX, float sizeY, float rot, float alpha) // 카메라 O
{
	D2D1_MATRIX_3X2_F a;
	m_d2dContext->GetTransform(&a);
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(-CAMERA->GetRenderTargetX() * CAMERA->GetScale(), -CAMERA->GetRenderTargetY() * CAMERA->GetScale());
	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	matS = D2D1::Matrix3x2F::Scale(sizeX * CAMERA->GetScale(), sizeY * CAMERA->GetScale());

	m_d2dContext->SetTransform(matS * matR * matT);
	m_d2dContext->DrawBitmap(img->GetBitMap(), D2D1::RectF(x, y, img->GetWidth() + x, img->GetHeight() + y), alpha, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void ImageManager::CropRender(CImage* img, float x, float y, RECT cropRc, bool isReversed, float sizeX, float sizeY, float rot, float alpha)
{
	// 이미지(그려진 비트맵)가져와서 랜더
	D2D_RECT_F destRect;
	D2D_RECT_F cropRect;
	// rect의 래프트 탑 라이트, 바텀(D2D_RECT_F <- 그려질영역 표시 렉트)
	cropRect = { (float)cropRc.left,(float)cropRc.top,(float)cropRc.right,(float)cropRc.bottom };

	destRect = { 0,0,cropRect.right - cropRect.left,cropRect.bottom - cropRect.top };
	// 각도, 회전축
	D2D1_MATRIX_3X2_F t, r, s;
	r = D2D1::Matrix3x2F::Rotation(rot, { (cropRect.right - cropRect.left) / 2,(cropRect.bottom - cropRect.top) / 2 });
	if (isReversed == false)
	{
		t = D2D1::Matrix3x2F::Translation(x, y);
		s = D2D1::Matrix3x2F::Scale(sizeX, sizeY);
	}
	else
	{
		s = D2D1::Matrix3x2F::Scale(-sizeX, sizeY);
		t = D2D1::Matrix3x2F::Translation(x + img->GetWidth(), y);
		// 플롯 이미 받아주는 변수에 더하는건 형변환 안해줘도됨
	}
	// 곱순서 정해져있음
	m_d2dContext->SetTransform(s * t * r);
	//첫번쨰 인자 : 비트맵, 두번째인자 그릴위치, 세번째인자 투명도, 이미지 값에 대한 옵션(근사값넣어라 (기본)),
	m_d2dContext->DrawBitmap(img->GetBitMap(), destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, cropRect);
};

void ImageManager::SampleRender(CImage* img, float x, float y, float sizeX, float sizeY, float rot, float alpha) // 카메라 O
{
	D2D1_POINT_2U targetPoint = { x,y };
	D2D1_RECT_F srcRect = { 0,0,500, 1500 };
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(targetPoint.x + 1000, targetPoint.y + 100);
	matR = D2D1::Matrix3x2F::Rotation(rot, { (targetPoint.x + 1000.0f) / 2,0 });
	matS = D2D1::Matrix3x2F::Scale(-sizeX, sizeY);
	m_d2dContext->SetTransform(matS * matT * matR);
	//m_mapRenderBitmap->CopyFromBitmap(NULL, img->GetBitMap(), &srcRect);
	m_d2dContext->DrawBitmap(img->GetBitMap(), D2D1::RectF(x, y, (x - 500), img->GetHeight() + y), alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
	matT = D2D1::Matrix3x2F::Translation(0, 0);
	matR = D2D1::Matrix3x2F::Rotation(0, { x,y });
	matS = D2D1::Matrix3x2F::Scale(1, 1);
	m_d2dContext->SetTransform(matS * matT * matR);
	ResetTRS();
}

void ImageManager::CenterRender(CImage* img, float x, float y, float sizeX, float sizeY, float rot, bool isReverse, float alpha)
{
	D2D1_MATRIX_3X2_F matT, matR, matS;

	if (isReverse == false)
	{
		matT = D2D1::Matrix3x2F::Translation((x - img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(sizeX, sizeY);
	}
	else
	{
		matT = D2D1::Matrix3x2F::Translation((x + img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(-sizeX, sizeY);
	}
	m_d2dContext->SetTransform((matS * matT * matR));
	ID2D1BitmapBrush* color;
	m_d2dContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	m_d2dContext->CreateBitmapBrush(img->GetBitMap(), &color);

	color->SetBitmap(img->GetBitMap());
	color->SetOpacity(alpha);
	m_d2dContext->FillOpacityMask(img->GetBitMap(), color, D2D1_OPACITY_MASK_CONTENT_GRAPHICS, D2D1::RectF(0.0f, 0.0f, img->GetWidth(), img->GetHeight()), D2D1::RectF(0.0f, 0.0f, img->GetWidth(), img->GetHeight()));
	color->Release();
}

void ImageManager::CenterAniRender(CImage* img, float renderTargetX, float renderTargetY, Animation* ani, eLayer layer, bool isReversed,float rot, float alpha)
{
	float fW = ani->GetFrameWidth() * 0.5f;
	float fH = ani->GetFrameHeight() * 0.5f;
	POINT p = ani->GetFramePos();

	D2D1_MATRIX_3X2_F matT, matR, matS;
	if (isReversed)
	{
		matT = D2D1::Matrix3x2F::Translation(((renderTargetX + fW) - CAMERA->GetRenderTargetX()) * CAMERA->GetScale(), ((renderTargetY - fH) - CAMERA->GetRenderTargetY()) * CAMERA->GetScale());
		matR = D2D1::Matrix3x2F::Rotation(rot, { fW ,fH });
		matS = D2D1::Matrix3x2F::Scale(-CAMERA->GetScale(), CAMERA->GetScale()/*, { fW,fH }*/);
	}
	else
	{
		matT = D2D1::Matrix3x2F::Translation(((renderTargetX - fW) - CAMERA->GetRenderTargetX()) * CAMERA->GetScale(), ((renderTargetY - fH) - CAMERA->GetRenderTargetY()) * CAMERA->GetScale());
		matR = D2D1::Matrix3x2F::Rotation(rot, { 0 ,0 });
		matS = D2D1::Matrix3x2F::Scale(CAMERA->GetScale(), CAMERA->GetScale()/*, { fW,fH }*/);
	}
	m_d2dContext->SetTransform((matS * matR * matT));
	m_d2dContext->SetTarget(m_layer[layer]);

	m_d2dContext->DrawBitmap
	(
		img->GetBitMap(),
		D2D1::RectF(0, 0, fW * 2, fH * 2),
		alpha, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(p.x, p.y, p.x + fW * 2, p.y + fH * 2)
	);
	if (KEYMANAGER->isToggleKey(VK_F7))
		m_d2dContext->DrawRectangle(D2D1::RectF(0, 0, fW * 2, fH * 2), m_brush);
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dContext->SetTarget(Direct2DBackBuffer);
}
void ImageManager::FrameRender(CImage* img, float x, float y, int frameX, int frameY, float sizeX, float sizeY, float rot, bool isReverse, float alpha)
{
	D2D1_MATRIX_3X2_F matT, matR, matS;

	if (isReverse == false)
	{
		matT = D2D1::Matrix3x2F::Translation((x - img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(1, 1);

	}
	else
	{
		matT = D2D1::Matrix3x2F::Translation((x + img->GetWidth() * sizeX / 2), (y - img->GetHeight() * sizeY / 2));
		matR = D2D1::Matrix3x2F::Rotation(rot, { x,y });
		matS = D2D1::Matrix3x2F::Scale(1, 1);
	}
	m_d2dContext->SetTransform((matS * matT * matR));
	ID2D1BitmapBrush* color;
	m_d2dContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	m_d2dContext->CreateBitmapBrush(img->GetBitMap(), &color);
	color->SetBitmap(img->GetBitMap());
	color->SetOpacity(alpha);
}

void ImageManager::CenterFrameRender(CImage* img, float x, float y, int frameX, int frameY, eLayer layer, float scale, float rot, float alpha)
{
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(((x - img->GetFrameWidth() * 0.5f) - CAMERA->GetRenderTargetX()) * CAMERA->GetScale(), ((y - img->GetFrameHeight() * 0.5f) - CAMERA->GetRenderTargetY()) * CAMERA->GetScale());
	matR = D2D1::Matrix3x2F::Rotation(rot, { (float)img->GetFrameWidth(),(float)img->GetFrameHeight()});
	matS = D2D1::Matrix3x2F::Scale(scale*CAMERA->GetScale(), scale*CAMERA->GetScale()/*, { img->GetFrameWidth() * 0.5f,img->GetFrameHeight() * 0.5f }*/);
	if (frameX < 0) frameX = 0;
	else if (frameX > img->GetMaxFrameX()) frameX = img->GetMaxFrameX();
	if (frameY < 0) frameY = 0;
	else if (frameY > img->GetMaxFrameY()) frameY = img->GetMaxFrameY();

	m_d2dContext->SetTransform(matS * matR * matT);
	m_d2dContext->SetTarget(m_layer[layer]);

	m_d2dContext->DrawBitmap(img->GetBitMap(),
		D2D1::RectF(0, 0, img->GetFrameWidth(), img->GetFrameHeight()),
		alpha, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(img->GetFrameWidth() * frameX, img->GetFrameHeight() * frameY, img->GetFrameWidth() * (frameX + 1), img->GetFrameHeight() * (frameY + 1)));

	float a = img->GetFrameWidth();

	if (KEYMANAGER->isToggleKey(VK_F7))
		m_d2dContext->DrawRectangle(D2D1::RectF(0, 0, img->GetFrameWidth(), img->GetFrameHeight()), m_blackBrush);
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dContext->SetTarget(Direct2DBackBuffer);
};


void ImageManager::TileRender(eTileSheetKey key, int tileNumber, int low, int column, int tileLineMaxNumber)
{
	int tileLow = (tileNumber - 1) / tileLineMaxNumber;
	int tileColumn = tileNumber - 1 - (tileLow * tileLineMaxNumber);

	//타일 위치 설정하기
	float positionX = column * TILE_SIZE * IMAGE_SCALE;
	float positionY = low * TILE_SIZE * IMAGE_SCALE;
	if (positionY > WINSIZE_Y || positionY < 0 - TILE_SIZE) return;
	if (positionX > WINSIZE_X || positionX < 0 - TILE_SIZE) return;
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation((column - tileColumn - 1) * TILE_SIZE, (low - tileLow) * TILE_SIZE);	//타일 출력 위치 설정
	matR = D2D1::Matrix3x2F::Rotation(NULL);
	matS = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1, 1));
	m_d2dContext->SetTransform(matS * matR * matT);

	//타일 시트에서 잘라오기
	m_cropEffect->SetInput(0, m_tileImageList[key]->GetBitMap());
	m_cropEffect->SetValue(D2D1_CROP_PROP_RECT, D2D1::RectF(tileColumn * TILE_SIZE, tileLow * TILE_SIZE, (tileColumn + 1) * TILE_SIZE, (tileLow + 1) * TILE_SIZE));

	m_d2dContext->DrawImage(m_cropEffect.Get());
}

void ImageManager::ImageLoad()
{
	AddTileSheet(eTileSheetKey::tile_overworld, L"Resources/Image/Tile/overworld.png");
	AddTileSheet(eTileSheetKey::tile_overworld2, L"Resources/Image/Tile/overworld2.png");
	AddTileSheet(eTileSheetKey::tile_overworld3, L"Resources/Image/Tile/overworld3.png");
	AddTileSheet(eTileSheetKey::tile_underworld, L"Resources/Image/Tile/underworld.png");
	AddTileSheet(eTileSheetKey::tile_jam, L"Resources/Image/Tile/jam.png");
	AddTileSheet(eTileSheetKey::tile_spectiles, L"Resources/Image/Tile/spectiles.png");

	AddImage("PlayerImageSheet", L"Resources/Image/Tile/player.png", 32, 32);
	AddImage("BowImageSheet", L"Resources/Image/Tile/bow.png", 32, 32);
	AddImage("ArrowImageSheet", L"Resources/Image/Tile/arrow.png", 32, 32);
	AddImage("YetiImageSheet", L"Resources/Image/Boss/YETI/yeti.png", 10, 10);
	AddImage("YetiShadow", L"Resources/Image/Boss/YETI/yetiShadow.png", 1, 1);
	AddImage("YetiEffect", L"Resources/Image/Boss/YETI/yetiEffectParticle.png", 9, 2);
	AddImage("YetiProjectile", L"Resources/Image/Boss/YETI/yetiAttack.png", 4, 3);

	m_spectiles = new GImage;
	m_spectiles->Init("Resources/Image/Tile/spectiles.bmp", 256, 256);
	COLORREF color = GetPixel(m_spectiles->_imageInfo->hMemDC, 17, 0);
	int R = GetRValue(color);
	int G = GetGValue(color);
	int B = GetBValue(color);
	cout << "spectileSheet 17, 0 : " << R << ", " << G << ", " << B << endl;
}

GImage* ImageManager::AddBackBuffer(string key, float width, float height)
{
	GImage* img;
	img = new GImage;
	img->Init(width, height);
	return img;
}
