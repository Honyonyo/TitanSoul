#pragma once
#include "SingletonBase.h"
class TextManager : public SingletonBase<TextManager>
{
private:
	std::map <string, IDWriteTextFormat*> m_tfList;

	IDWriteFactory3* _DWriteFactory;
	IDWriteFontCollection1* m_fontCollection;
	IDWriteFontSet* m_customFontSet;

public:
	HRESULT Init();

	HRESULT LoadFont();
	HRESULT MakeTextFormat();
	void FontNamePrinter();

	//key 규칙을 정하시는것이 좋습니다
	//예시 : 폰트이름_폰트사이즈 (둥근모꼴_10)
	IDWriteTextFormat* AddTextFormat
	(
		string textFormatKey,
		WCHAR* fontName, float fontSize,
		IDWriteFontCollection* = nullptr,	//폰트패밀리 (NULL인 경우 시스템기본폰트패밀리)
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,	//글꼴 두께 (https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_weight)
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,	//글꼴 스타일 (노말/이탤릭/오블리크)(https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_style)
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,	//글자 너비(0~9, NOMAL = 5) (https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_stretch)
		WCHAR* localeName = L"ko-kr"	//지역값
	);

	IDWriteTextFormat* FindTextFormat(string Key);

	void SetTextFormatAlignment(string key, DWRITE_TEXT_ALIGNMENT tAlignment, DWRITE_PARAGRAPH_ALIGNMENT pAlignment);
	void TextDraw
	(
		string text, string tfKey, RECT drawArea, COLORREF rgbColor = RGB(0, 0, 0),
		DWRITE_TEXT_ALIGNMENT tAlignment = DWRITE_TEXT_ALIGNMENT_LEADING,	//가로정렬 기본 시작점(좌->우측으로 글을 쓰는 포맷을 사용하므로 좌측정렬) https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_text_alignment
		DWRITE_PARAGRAPH_ALIGNMENT pAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR	//세로정렬 기본 상단 https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_paragraph_alignment
	);
	void TextDraw
	(
		std::wstring text, string tfKey, RECT drawArea, COLORREF rgbColor = RGB(0, 0, 0),
		DWRITE_TEXT_ALIGNMENT tAlignment = DWRITE_TEXT_ALIGNMENT_LEADING,	
		DWRITE_PARAGRAPH_ALIGNMENT pAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR
	);

	TextManager();
	~TextManager();
};
