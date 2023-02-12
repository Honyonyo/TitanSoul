#include "stdafx.h"
#include "TextManager.h"


TextManager::TextManager()
{
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(_DWriteFactory),
		reinterpret_cast<IUnknown**>(&_DWriteFactory)
	);
};
TextManager::~TextManager()
{

};

HRESULT TextManager::Init()
{

	LoadFont();
	MakeTextFormat();
	FontNamePrinter();

	//AddTextFormat("");

	return S_OK;
};

HRESULT TextManager::MakeTextFormat()
{
	TEXTMANAGER->AddTextFormat("��Ʈ_50", L"DeterminationSansK2", 50);
	TEXTMANAGER->AddTextFormat("��Ʈ_10", L"DeterminationSansK2", 10);
	TEXTMANAGER->AddTextFormat("��Ʈ_20", L"DeterminationSansK2", 20);
	TEXTMANAGER->AddTextFormat("��Ʈ_30", L"DeterminationSansK2", 30);

	return S_OK;
};



void TextManager::FontNamePrinter()
{
	cout << "Font Name Print Start" << endl;
	for (int i = 0; i < m_fontCollection->GetFontFamilyCount(); i++)
	{
		ComPtr<IDWriteFontFamily> fontFamily;
		ComPtr<IDWriteLocalizedStrings> localizedFontName;
		WCHAR fontName[64] = {};
		m_fontCollection->GetFontFamily(i, &fontFamily);
		fontFamily->GetFamilyNames(&localizedFontName);
		localizedFontName->GetString(0, fontName, 60);
		std::wstring tmpname;
		tmpname.assign(fontName);
		string name;
		name.assign(tmpname.begin(), tmpname.end());

		cout << "\t" << i << " : " << name << endl;
	}
	cout << "Font Name Print End" << endl;
};

IDWriteTextFormat* TextManager::FindTextFormat(string Key)
{
	auto find = m_tfList.find(Key);
	if (find != m_tfList.end())
	{
		return find->second;
	}
	cout << "�ؽ�Ʈ���� key ���� : " << Key << "\n";
	return nullptr;
};

void TextManager::SetTextFormatAlignment(string key, DWRITE_TEXT_ALIGNMENT tAlignment, DWRITE_PARAGRAPH_ALIGNMENT pAlignment)
{
	IDWriteTextFormat* tf = FindTextFormat(key);
	tf->SetTextAlignment(tAlignment);
	tf->SetParagraphAlignment(pAlignment);
}

void TextManager::TextDraw(string text, string tfKey, RECT drawArea, COLORREF rgbColor, 
	DWRITE_TEXT_ALIGNMENT tAlignment, DWRITE_PARAGRAPH_ALIGNMENT pAlignment)
{
	IDWriteTextFormat* tf = FindTextFormat(tfKey);
	if (tf == nullptr) return;
	tf->SetTextAlignment(tAlignment);
	tf->SetParagraphAlignment(pAlignment);

	WCHAR wchar[512] = {};
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), text.size(), wchar, text.size());

	if (rgbColor == 0x000000)	//�����ϰ��
	{
		(*IMAGEMANAGER->GetDeviceContext())
			->DrawTextA(wchar, text.size(),
				tf, D2D1::RectF(drawArea.left, drawArea.top, drawArea.right , drawArea.bottom), 
				IMAGEMANAGER->GetBlackBrush());
	}
	else
	{
		(*IMAGEMANAGER->GetDeviceContext())
			->DrawTextA(wchar, text.size(), 
				tf, D2D1::RectF(drawArea.left, drawArea.top, drawArea.right, drawArea.bottom), 
				IMAGEMANAGER->SetBrushColor({ 1.0f / 255 * GetRValue(rgbColor),1.0f / 255 * GetGValue(rgbColor),1.0f / 255 * GetBValue(rgbColor),1 }));
	}
}

void TextManager::TextDraw(std::wstring text, string tfKey, RECT drawArea, COLORREF rgbColor, DWRITE_TEXT_ALIGNMENT tAlignment, DWRITE_PARAGRAPH_ALIGNMENT pAlignment)
{
	IDWriteTextFormat* tf = FindTextFormat(tfKey);
	tf->SetTextAlignment(tAlignment);
	tf->SetParagraphAlignment(pAlignment);

	WCHAR wchar[512] = {};
	for (int i = 0; i < text.size(); i++)
	{
		wchar[i] = text[i];
	}

	if (rgbColor == 0x000000)	//�����ϰ��
	{
		(*IMAGEMANAGER->GetDeviceContext())
			->DrawTextA(wchar, text.size(),
				tf, D2D1::RectF(drawArea.left, drawArea.top, drawArea.right, drawArea.bottom),
				IMAGEMANAGER->GetBlackBrush());
	}
	else
	{
		(*IMAGEMANAGER->GetDeviceContext())
			->DrawTextA(wchar, text.size(),
				tf, D2D1::RectF(drawArea.left, drawArea.top, drawArea.right, drawArea.bottom),
				IMAGEMANAGER->SetBrushColor({ 1.0f / 255 * GetRValue(rgbColor),1.0f / 255 * GetGValue(rgbColor),1.0f / 255 * GetBValue(rgbColor),1 }));
	}
}

HRESULT TextManager::LoadFont()
{
	IDWriteFontSetBuilder1* fontSetBuilder;
	_DWriteFactory->CreateFontSetBuilder(reinterpret_cast<IDWriteFontSetBuilder**>(&fontSetBuilder));

	vector<wstring> filePathVector;
	/*
	filePathVector�� ��Ʈ���� ��θ�
	���⿡ push_back���� ���� �߰��ϰų�
	��Ʈ������ ���� ���/���̽�����/��ɺи��� ���ϴ� ���
	json���� filePath�� �ҷ����ּ���.
	*/

	filePathVector.push_back(L"Resources/Font/NotoSansKR-Black.otf");
	filePathVector.push_back(L"Resources/Font/DeterminationSansK2.ttf");
	filePathVector.push_back(L"Resources/Font/NotoSansKR-Black.otf");

	for (int i = 0; i < filePathVector.size(); i++)
	{
		ComPtr<IDWriteFontFile> fontFile;
		_DWriteFactory->CreateFontFileReference(filePathVector[i].c_str(), nullptr, &fontFile);
		fontSetBuilder->AddFontFile(fontFile.Get());
	}

	IDWriteFontSet* customFontSet;
	fontSetBuilder->CreateFontSet(&customFontSet);
	_DWriteFactory->CreateFontCollectionFromFontSet(customFontSet, &m_fontCollection);
	return S_OK;
};


IDWriteTextFormat* TextManager::AddTextFormat
(	string textFormatKey,
	WCHAR* fontName, float fontSize, IDWriteFontCollection* fontCollection,
	DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch,
	WCHAR* localeName	)
{
	HRESULT hr = S_OK;
	IDWriteTextFormat* tf;

	if (fontCollection == nullptr)
	{
		fontCollection = m_fontCollection;
	}

	hr = _DWriteFactory->CreateTextFormat(
		fontName,                  // ��Ʈ �йи� �̸��� ���ڿ�
		fontCollection,                        // ��Ʈ �÷��� ��ü, NULL=�ý��� ��Ʈ �÷���
		fontWeight,   // ��Ʈ ����. LIGHT, NORMAL, BOLD ��.
		fontStyle,    // ��Ʈ ��Ÿ��. NORMAL, OBLIQUE, ITALIC.
		fontStretch,  // ��Ʈ ����. CONDENSED, NORMAL, MEDIUM, EXTEXDED ��.
		fontSize,                          // ��Ʈ ũ��.
		localeName,                         // �������� ���ڿ��� ���.  ����-�̱�=L"en-us", �ѱ���-�ѱ�=L"ko-kr"
		&tf
	);

	if (FAILED(hr))
	{
		cout << "CreateTextFormat Failed" << endl;
		cout << "\tFont Key : " << textFormatKey << endl;

		hr = m_fontCollection->FindFamilyName(fontName, NULL, NULL);
		if (FAILED(hr))
		{
			cout << "\tI think, you have the wrong Font Name..." << endl;
			cout << "*If you want to look up the correct Font Name, you can call the function" << endl;
			cout << "\"FontNamePrinter()\" <- exist in TextManager" << endl;
		}

		return nullptr;
	}

	m_tfList.insert(make_pair(textFormatKey, tf));

	return tf;
};
