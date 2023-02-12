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

	//key ��Ģ�� ���Ͻô°��� �����ϴ�
	//���� : ��Ʈ�̸�_��Ʈ������ (�ձٸ��_10)
	IDWriteTextFormat* AddTextFormat
	(
		string textFormatKey,
		WCHAR* fontName, float fontSize,
		IDWriteFontCollection* = nullptr,	//��Ʈ�йи� (NULL�� ��� �ý��۱⺻��Ʈ�йи�)
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,	//�۲� �β� (https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_weight)
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,	//�۲� ��Ÿ�� (�븻/���Ÿ�/����ũ)(https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_style)
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,	//���� �ʺ�(0~9, NOMAL = 5) (https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_font_stretch)
		WCHAR* localeName = L"ko-kr"	//������
	);

	IDWriteTextFormat* FindTextFormat(string Key);

	void SetTextFormatAlignment(string key, DWRITE_TEXT_ALIGNMENT tAlignment, DWRITE_PARAGRAPH_ALIGNMENT pAlignment);
	void TextDraw
	(
		string text, string tfKey, RECT drawArea, COLORREF rgbColor = RGB(0, 0, 0),
		DWRITE_TEXT_ALIGNMENT tAlignment = DWRITE_TEXT_ALIGNMENT_LEADING,	//�������� �⺻ ������(��->�������� ���� ���� ������ ����ϹǷ� ��������) https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_text_alignment
		DWRITE_PARAGRAPH_ALIGNMENT pAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR	//�������� �⺻ ��� https://learn.microsoft.com/ko-kr/windows/win32/api/dwrite/ne-dwrite-dwrite_paragraph_alignment
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
