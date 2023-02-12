#pragma once
class CImage
{
private:
	float alpha;
	int	  width;
	int	  height;
	int   maxFrameX, maxFrameY;
	float frameWidth, frameHeight;
	ID2D1Bitmap1* bitmap;

public:
	CImage() :
		bitmap(nullptr), width(0), height(0),
		maxFrameX(0), maxFrameY(0), frameWidth(0), frameHeight(0)
	{
		alpha = 1;
	}
	CImage(ID2D1Bitmap1* bitmap, int width, int height, int maxFrameX = 1, int maxFrameY = 1) :
		bitmap(bitmap), width(width), height(height),
		maxFrameX(maxFrameX - 1), maxFrameY(maxFrameY - 1), frameWidth(width / maxFrameX), frameHeight(height / maxFrameY)
	{
		alpha = 1;
	}
	~CImage()
	{
		if(bitmap!=nullptr)
			bitmap->Release();
	}

	HRESULT Init(std::wstring filePath, int maxFrameX = 1, int maxFrameY = 1);

	void Render
	(
		float RenderTargetX, float RenderTargetY, 
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool reversed = false, float rot = 0.0f, 
		float alpha = 2
	);
	void RenderToDC
	(
		float RenderTargetX, float RenderTargetY, 
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool reversed = false, float rot = 0.0f, 
		float alpha = 1.0f
	);

	float GetAlpha() { return alpha; }
	int   GetWidth() { return width; }
	int   GetHeight() { return height; }
	int   GetMaxFrameX() { return maxFrameX; }
	int   GetMaxFrameY() { return maxFrameY; }
	int   GetFrameWidth() { return frameWidth; }
	int   GetFrameHeight() { return frameHeight; }

	void SetAlpha(float a) { alpha = a; }
	ID2D1Bitmap1* GetBitMap() { return bitmap; }
};