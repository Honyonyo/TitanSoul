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
	TEXTMANAGER->AddTextFormat("도트_50", L"DeterminationSansK2", 50);
	TEXTMANAGER->AddTextFormat("도트_10", L"DeterminationSansK2", 10);
	TEXTMANAGER->AddTextFormat("도트_20", L"DeterminationSansK2", 20);
	TEXTMANAGER->AddTextFormat("도트_30", L"DeterminationSansK2", 30);

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
	cout << "텍스트포맷 key 없음 : " << Key << "\n";
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

	if (rgbColor == 0x000000)	//검정일경우
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

	if (rgbColor == 0x000000)	//검정일경우
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
	filePathVector에 폰트파일 경로를
	여기에 push_back으로 직접 추가하거나
	폰트종류가 많을 경우/제이슨어필/기능분리를 원하는 경우
	json에서 filePath를 불러와주세요.
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
		fontName,                  // 폰트 패밀리 이름의 문자열
		fontCollection,                        // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
		fontWeight,   // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
		fontStyle,    // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
		fontStretch,  // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
		fontSize,                          // 폰트 크기.
		localeName,                         // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
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
