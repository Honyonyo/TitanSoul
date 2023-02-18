#include "stdafx.h"
#include "ImageManager.h"
void ImageManager::LayerTypeTableGenerator()
{
	layerTypeTable.insert(make_pair("BG", eBG));
	layerTypeTable.insert(make_pair("WALL", eWall));
	layerTypeTable.insert(make_pair("FG", eFG));
	layerTypeTable.insert(make_pair("COL", eCOL));
	layerTypeTable.insert(make_pair("MAT", eMAT));
}
eTileLayerType ImageManager::LayerTypeStringToEnum(string type)
{
	return layerTypeTable.find(type)->second;
}
void ImageManager::TileKeyGenerator()
{
	tileKeyTable.insert(make_pair("tile_overworld",tile_overworld ));
	tileKeyTable.insert(make_pair("tile_overworld2",tile_overworld2 ));
	tileKeyTable.insert(make_pair("tile_overworld3",tile_overworld3 ));
	tileKeyTable.insert(make_pair("tile_underworld",tile_underworld ));
	tileKeyTable.insert(make_pair("tile_jam",tile_jam ));
	tileKeyTable.insert(make_pair("tile_spectiles",tile_spectiles));
}
eTileSheetKey ImageManager::TileKeyStringToEnum(string tileSheet)
{
	return tileKeyTable.find(tileSheet)->second;
}

void ImageManager::BitmapRenderToDCUseMatrix(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix)
{
	ID2D1SolidColorBrush* m_pBlackBrush;
	m_d2dContext->CreateSolidColorBrush
	(
		D2D1::ColorF(D2D1::ColorF::Black, 1.0f),
		&m_pBlackBrush
	);

	m_pDCRT->BeginDraw();
	//m_pDCRT->SetTransform(matrix);
	m_pDCRT->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pDCRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	m_pDCRT->DrawBitmap(bitmap);
	m_pDCRT->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(150.0f, 150.0f),
			100.0f,
			100.0f),
		m_pBlackBrush,
		3.0
	);

	m_pDCRT->EndDraw();
}