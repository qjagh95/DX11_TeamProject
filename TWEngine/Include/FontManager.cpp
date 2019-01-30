#include "EngineHeader.h"
#include "FontManager.h"
#include "Device.h"

PUN_USING

DEFINITION_SINGLE(CFontManager)

CFontManager::CFontManager() :
	m_pWriteFactory(nullptr)
{
}

CFontManager::~CFontManager()
{
	SAFE_RELEASE(m_pWriteFactory);
	Safe_Release_Map(m_mapFont);
	Safe_Release_Map(m_mapBrush);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pWriteFactory), (IUnknown**)&m_pWriteFactory)))
		return false;

	MakeFont(TEXT("궁서체"), DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED,
		20.f, TEXT("ko"));

	CreateBrush(0.f, 0.f, 0.f, 1.f);
	CreateBrush(1.f, 0.f, 0.f, 1.f);
	CreateBrush(0.f, 1.f, 0.f, 1.f);
	CreateBrush(0.f, 0.f, 1.f, 1.f);
	CreateBrush(1.f, 0.8431f, 0.f, 1.f);

	return true;
}

IDWriteTextFormat * CFontManager::MakeFont(const TCHAR * pFontName,
	int iWeight, int iStyle, int iStretch, float fSize,
	const TCHAR * pLocalName)
{
	char	strKey[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFontName, -1, strKey, lstrlen(pFontName) * 2,
		0, 0);
#else
	strcpy_s(strKey, pFontName);
#endif // UNICODE
	sprintf_s(strKey, "%s%d", strKey, (int)fSize);

	IDWriteTextFormat*	pFormat = FindFont(strKey);

	if (pFormat)
		return pFormat;

	// Text Format을 설정한다.
	// 1번인자 : 폰트 이름을 입력한다. 
	// 2번인자 : 특정 폰트는 컬렉션을 가지고 있다. 예 Arial 은 Arial Black 이런식
	// 3번인자 : 폰트 굵기 4번인자 : 기울기
	// 5번인자 : 자간 6번인자 : 폰트 크기
	// 7번인자 : 언어 지역 이름을 설정한다. 한국은 ko - KR 미국은 en - us 
	// 8번인자 : 텍스트 인터페이스
	if (FAILED(m_pWriteFactory->CreateTextFormat(pFontName, nullptr,
		(DWRITE_FONT_WEIGHT)iWeight,
		(DWRITE_FONT_STYLE)iStyle,
		(DWRITE_FONT_STRETCH)iStretch,
		fSize, pLocalName, &pFormat)))
		return nullptr;

	m_mapFont.insert(make_pair(strKey, pFormat));

	return pFormat;
}

ID2D1SolidColorBrush * CFontManager::CreateBrush(float r, float g, float b,
	float a)
{
	ID2D1SolidColorBrush*	pBrush = FindBrush(r, g, b, a);

	if (pBrush)
		return pBrush;

	if (FAILED(GET_SINGLE(CDevice)->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &pBrush)))
		return nullptr;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_mapBrush.insert(make_pair(iKey, pBrush));

	return pBrush;
}

ID2D1SolidColorBrush * CFontManager::CreateBrush(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ID2D1SolidColorBrush*	pBrush = FindBrush(r, g, b, a);

	if (pBrush)
		return pBrush;

	if (FAILED(GET_SINGLE(CDevice)->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &pBrush)))
		return nullptr;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_mapBrush.insert(make_pair(iKey, pBrush));

	return pBrush;
}

ID2D1SolidColorBrush * CFontManager::CreateBrush(const Vector4 & vColor)
{
	float	r, g, b, a;
	r = vColor.r;
	g = vColor.g;
	b = vColor.b;
	a = vColor.a;

	ID2D1SolidColorBrush*	pBrush = FindBrush(r, g, b, a);

	if (pBrush)
		return pBrush;

	if (FAILED(GET_SINGLE(CDevice)->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &pBrush)))
		return nullptr;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_mapBrush.insert(make_pair(iKey, pBrush));

	return pBrush;
}

IDWriteTextLayout * CFontManager::CreateTextLayout(const TCHAR * pText,
	IDWriteTextFormat * pFormat, float fWidth, float fHeight)
{
	IDWriteTextLayout*	pLayout = nullptr;

	if (FAILED(m_pWriteFactory->CreateTextLayout(pText, lstrlen(pText),
		pFormat, fWidth, fHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

IDWriteTextLayout * CFontManager::CreateTextLayout(const TCHAR * pText,
	const string & strFontKey, float fWidth, float fHeight)
{
	IDWriteTextLayout*	pLayout = nullptr;
	IDWriteTextFormat*	pFormat = FindFont(strFontKey);

	if (FAILED(m_pWriteFactory->CreateTextLayout(pText, lstrlen(pText),
		pFormat, fWidth, fHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

IDWriteTextFormat * CFontManager::FindFont(const string & strKey)
{
	unordered_map<string, IDWriteTextFormat*>::iterator	iter = m_mapFont.find(strKey);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush * CFontManager::FindBrush(float r, float g,
	float b, float a)
{
	return FindBrush((unsigned char)(r * 255),
		(unsigned char)(g * 255), (unsigned char)(b * 255),
		(unsigned char)(a * 255));
}

ID2D1SolidColorBrush * CFontManager::FindBrush(unsigned char r, unsigned char g,
	unsigned char b, unsigned char a)
{
	unsigned int	iKey = CreateColorKey(r, g, b, a);

	unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator	iter =
		m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush * CFontManager::FindBrush(const Vector4 & vColor)
{
	unsigned int	iKey = CreateColorKey(vColor.r, vColor.g, vColor.b,
		vColor.a);

	unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator	iter =
		m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

unsigned int CFontManager::CreateColorKey(float r, float g, float b, float a)
{
	return CreateColorKey((unsigned char)(r * 255),
		(unsigned char)(g * 255), (unsigned char)(b * 255),
		(unsigned char)(a * 255));
}

unsigned int CFontManager::CreateColorKey(unsigned char r, unsigned char g,
	unsigned char b, unsigned char a)
{
	unsigned int iKey = a;
	iKey <<= 8;
	iKey |= r;
	iKey <<= 8;
	iKey |= g;
	iKey <<= 8;
	iKey |= b;

	return iKey;
}

