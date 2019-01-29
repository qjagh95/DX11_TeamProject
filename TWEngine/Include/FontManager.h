#pragma once

#include "Engine.h"

PUN_BEGIN

class PUN_DLL CFontManager
{
private:
	IDWriteFactory*	m_pWriteFactory;
	unordered_map<string, IDWriteTextFormat*>	m_mapFont;
	unordered_map<unsigned int, ID2D1SolidColorBrush*>	m_mapBrush;

public:
	bool Init();
	IDWriteTextFormat* MakeFont(const TCHAR* pFontName,
		int iWeight, int iStyle, int iStretch, float fSize,
		const TCHAR* pLocalName);
	ID2D1SolidColorBrush*	CreateBrush(float r, float g, float b, float a);
	ID2D1SolidColorBrush*	CreateBrush(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	ID2D1SolidColorBrush*	CreateBrush(const Vector4& vColor);
	IDWriteTextLayout*		CreateTextLayout(const TCHAR* pText,
		IDWriteTextFormat* pFormat, float fWidth, float fHeight);
	IDWriteTextLayout*		CreateTextLayout(const TCHAR* pText,
		const string& strFontKey, float fWidth, float fHeight);

public:
	IDWriteTextFormat*		FindFont(const string& strKey);
	ID2D1SolidColorBrush*	FindBrush(float r, float g, float b, float a);
	ID2D1SolidColorBrush*	FindBrush(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	ID2D1SolidColorBrush*	FindBrush(const Vector4& vColor);

	unsigned int CreateColorKey(float r, float g, float b, float a);
	unsigned int CreateColorKey(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);

	DECLARE_SINGLE(CFontManager)
};

PUN_END