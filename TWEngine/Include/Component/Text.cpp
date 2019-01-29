#include "stdafx.h"
#include "Text.h"
#include "../FontManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/RenderState.h"
#include "../Rendering/Shader.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Mesh.h"
#include "../Scene/Scene.h"
#include "Camera.h"
#include "../Device.h"
#include "Transform.h"

PUN_USING

CText::CText()	:
	m_pTextFormat(nullptr)
	, m_pBrush(nullptr)
	, m_pShadowBrush(nullptr)
	, m_pTextLayout(nullptr)
	, m_p2DTarget(nullptr)
	, m_fSize(0.f)
	, m_fOpacity(1.f)
	, m_fShadowOpacity(1.f)
{
	m_eComType = CT_TEXT;
	SetTag("Text");

	m_eTextType = TRT_UI;
	m_pText = (TCHAR*)TEXT("Text");
	m_bShadow = false;
	m_bAlpha = false;
	m_bAlphaShadow = false;
	m_vShadowOffset = Vector3(1.f, -1.f, 0.f);

	m_vColor = Vector4::White;
	m_vShadowColor = Vector4::Black;
	m_pFont = (TCHAR*)TEXT("±Ã¼­Ã¼");
	m_fSize = 20.f;

	m_pTextFormat = GET_SINGLE(CFontManager)->FindFont("±Ã¼­Ã¼20");
	m_pBrush = GET_SINGLE(CFontManager)->CreateBrush(m_vColor);
	m_pShadowBrush = GET_SINGLE(CFontManager)->CreateBrush(m_vShadowColor);

	m_pTextLayout = GET_SINGLE(CFontManager)->CreateTextLayout(TEXT(""),
		m_pTextFormat, 100.f, 100.f);

	SetRenderArea(0.f, 0.f, 100.f, 50.f);
	SetAlignH(TAH_LEFT);
	SetAlignV(TAV_TOP);

	m_p2DTarget = GET_SINGLE(CDevice)->Get2DRenderTarget();
}

CText::CText(const CText & text)	:
	CComponent(text)
{
	*this = text;
	m_pTextLayout = nullptr;
	CreateLayout();
}

CText::~CText()
{
	SAFE_RELEASE(m_pTextLayout);
}

void CText::SetTextType(TEXT_RENDER_TYPE eType)
{
	m_eTextType = eType;
}

void CText::SetText(TCHAR * pText)
{
	m_pText = pText;

	CreateLayout();
}

void CText::SetFont(TCHAR * pFont, float fSize)
{
	m_pFont = pFont;
	m_fSize = fSize;
	m_vColor = Vector4::Black;

	m_pTextFormat = GET_SINGLE(CFontManager)->MakeFont(m_pFont,
		fSize, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED,
		fSize, L"ko");

	m_pBrush = GET_SINGLE(CFontManager)->CreateBrush(m_vColor.x,
		m_vColor.y, m_vColor.z, m_vColor.w);

	CreateLayout();
}

void CText::SetSize(float fSize)
{
	m_fSize = fSize;

	CreateLayout();
}

void CText::SetAlignH(TEXT_ALIGN_H eAlign)
{
	m_eAlignH = eAlign;

	if (!m_pTextLayout)
		CreateLayout();

	if (m_pTextFormat)
	{
		switch (eAlign)
		{
		case TAH_LEFT:
			m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case TAH_CENTER:
			m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case TAH_RIGHT:
			m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
		}
	}
}

void CText::SetAlignV(TEXT_ALIGN_V eAlign)
{
	m_eAlignV = eAlign;

	if (!m_pTextLayout)
		CreateLayout();

	switch (eAlign)
	{
	case TAV_TOP:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TAV_CENTER:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TAV_BOTTOM:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}
}

void CText::Shadow(bool bShadow)
{
	m_bShadow = bShadow;
}

void CText::SetShadowOffset(const Vector3 & vOffset)
{
	m_vShadowOffset = vOffset;
}

void CText::SetShadowColor(float r, float g, float b, float a)
{
	m_vShadowColor = Vector4(r, g, b, a);


	m_pShadowBrush = GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);
}

void CText::SetShadowColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vShadowColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pShadowBrush = GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);
}

void CText::SetShadowColor(const Vector4 & vColor)
{
	m_vShadowColor = vColor;

	m_pShadowBrush = GET_SINGLE(CFontManager)->CreateBrush(m_vShadowColor);
}

void CText::AlphaBlend(bool bAlpha)
{
	m_bAlpha = bAlpha;
}

void CText::ShadowAlphaBlend(bool bAlpha)
{
	m_bAlphaShadow = bAlpha;
}

void CText::SetOpacity(float fOpacity)
{
	m_fOpacity = fOpacity;
}

void CText::SetShadowOpacity(float fOpacity)
{
	m_fShadowOpacity = fOpacity;
}

void CText::SetColor(float r, float g, float b, float a)
{
	m_vColor = Vector4(r, g, b, a);

	m_pBrush = GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);
}

void CText::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pBrush = GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);
}

void CText::SetColor(const Vector4 & vColor)
{
	m_vColor = vColor;

	m_pBrush = GET_SINGLE(CFontManager)->CreateBrush(m_vColor.x,
		m_vColor.y, m_vColor.z, m_vColor.w);

	CreateLayout();
}

void CText::SetRenderArea(float l, float t, float r, float b)
{
	m_tRenderArea = D2D1::RectF(l, t, r, b);

	CreateLayout();
}

void CText::CreateLayout()
{
	SAFE_RELEASE(m_pTextLayout);
	m_pTextLayout = GET_SINGLE(CFontManager)->CreateTextLayout(m_pText,
		m_pTextFormat, m_tRenderArea.right - m_tRenderArea.left,
		m_tRenderArea.bottom - m_tRenderArea.top);

	DWRITE_TEXT_RANGE	tRange;
	tRange.startPosition = 0;
	tRange.length = lstrlen(m_pText);

	m_pTextLayout->SetFontSize(m_fSize, tRange);
}

void CText::AfterClone()
{
}

bool CText::Init()
{
	return true;
}

int CText::Input(float fTime)
{
	return 0;
}

int CText::Update(float fTime)
{
	return 0;
}

int CText::LateUpdate(float fTime)
{
	return 0;
}

void CText::Collision(float fTime)
{
}

void CText::Render(float fTime)
{
	Vector3 vPos = m_pTransform->GetWorldPos();

	float	fHeight = m_tRenderArea.bottom - m_tRenderArea.top;

	m_p2DTarget->BeginDraw();

	if (m_eTextType != TRT_UI)
	{
		CTransform*	pCameraTr = m_pScene->GetMainCameraTransform();

		vPos -= pCameraTr->GetWorldPos();

		SAFE_RELEASE(pCameraTr);
	}

	if (m_bShadow)
	{
		Vector3 vShadowPos = vPos + m_vShadowOffset;

		if (m_bAlphaShadow)
			m_pShadowBrush->SetOpacity(m_fShadowOpacity);

		else
			m_pShadowBrush->SetOpacity(1.f);

		vShadowPos.y = _RESOLUTION.iHeight - vShadowPos.y - fHeight;

		m_p2DTarget->DrawTextLayout(D2D1::Point2F(vShadowPos.x, vShadowPos.y),
			m_pTextLayout, m_pShadowBrush);
	}

	vPos.y = _RESOLUTION.iHeight - vPos.y - fHeight;
	m_p2DTarget->DrawTextLayout(D2D1::Point2F(vPos.x, vPos.y),
		m_pTextLayout, m_pBrush);
}

CText * CText::Clone()
{
	return new CText(*this);
}
