#include "stdafx.h"
#include "CImage.h"

HRESULT CImage::Init(wstring filePath, int maxFrameX, int maxFrameY)
{
	UINT Width, Height;
	Width = Height = 0;
	this->bitmap = IMAGEMANAGER->AddBitmap(filePath, &Width, &Height);
	if (bitmap == nullptr)
	{
		return E_FAIL;
	}

	this->width = Width;
	this->height = Height;
	this->maxFrameX = maxFrameX - 1;
	this->maxFrameY = maxFrameY - 1;
	this->frameWidth = width / maxFrameX;
	this->frameHeight = height / maxFrameY;

	return S_OK;
}

void CImage::Render
(
	float renderTargetX, float renderTargetY,
	float scaleX , float scaleY,
	bool reversed, float rot,
	float alpha
)
{
	D2D1_POINT_2U targetPoint = { renderTargetX,renderTargetY };
	//D2D1_RECT_U srcRect = { 0,0,WINSIZE_X - RenderTargetX, WINSIZE_Y - RenderTargetY };
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(targetPoint.x, targetPoint.y);
	matR = D2D1::Matrix3x2F::Rotation(rot, { (renderTargetX + width) / 2, (renderTargetY + height)/2 });
	if (reversed == false)
	{
		matS = D2D1::Matrix3x2F::Scale(scaleX, scaleY);
	}
	else
	{
		matS = D2D1::Matrix3x2F::Scale(-scaleX, scaleY);
	}
	if (alpha < 0 || alpha>1)
	{
		alpha = this->alpha;
	}
	IMAGEMANAGER->BitmapRenderUseMatrix(bitmap,(matS * matT * matR), alpha);
	//CreateBitmapIndirect(bitmap);
};
void CImage::RenderToDC
(
	float RenderTargetX, float RenderTargetY,
	float scaleX , float scaleY,
	bool reversed, float rot,
	float alpha
)
{
	D2D1_POINT_2U targetPoint = { RenderTargetX,RenderTargetY };
	//D2D1_RECT_U srcRect = { 0,0,WINSIZE_X - RenderTargetX, WINSIZE_Y - RenderTargetY };
	D2D1_MATRIX_3X2_F matT, matR, matS;
	matT = D2D1::Matrix3x2F::Translation(targetPoint.x, targetPoint.y);
	matR = D2D1::Matrix3x2F::Rotation(rot, { (RenderTargetX + width) / 2, (RenderTargetY + height)/2 });
	if (reversed == false)
	{
		matS = D2D1::Matrix3x2F::Scale(scaleX, scaleY);
	}
	else
	{
		matS = D2D1::Matrix3x2F::Scale(-scaleX, scaleY);
	}
	IMAGEMANAGER->BitmapRenderToDCUseMatrix(bitmap,(matS * matT * matR));
	//CreateBitmapIndirect(bitmap);
};
