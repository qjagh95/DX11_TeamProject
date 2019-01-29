#pragma once
#include "Component.h"

PUN_BEGIN

enum TEXT_ALIGN_H
{
	TAH_LEFT,
	TAH_CENTER,
	TAH_RIGHT
};

enum TEXT_ALIGN_V
{
	TAV_TOP,
	TAV_CENTER,
	TAV_BOTTOM
};

enum TEXT_RENDER_TYPE
{
	TRT_UI,
	TRT_2D,
	TRT_3D
};

class PUN_DLL CText :
	public CComponent
{
	friend class CGameObject;

protected:
	CText();
	CText(const CText& text);
	virtual ~CText();

private:
	IDWriteTextFormat*		m_pTextFormat;
	IDWriteTextLayout*		m_pTextLayout;
	ID2D1SolidColorBrush*	m_pBrush;
	ID2D1SolidColorBrush*	m_pShadowBrush;
	ID2D1RenderTarget*		m_p2DTarget;
	bool					m_bShadow;
	TCHAR*					m_pText;
	TCHAR*					m_pFont;
	float					m_fSize;
	TEXT_ALIGN_H			m_eAlignH;
	TEXT_ALIGN_V			m_eAlignV;
	bool					m_bAlpha;
	bool					m_bAlphaShadow;
	float					m_fOpacity;
	float					m_fShadowOpacity;
	Vector4					m_vColor;
	Vector4					m_vShadowColor;
	Vector3					m_vShadowOffset;
	D2D1_RECT_F				m_tRenderArea;
	TEXT_RENDER_TYPE		m_eTextType;

public:
	void SetTextType(TEXT_RENDER_TYPE eType);
	void SetText(TCHAR* pText);
	void SetFont(TCHAR* pFont, float fSize);
	void SetSize(float fSize);
	void SetAlignH(TEXT_ALIGN_H eAlign);
	void SetAlignV(TEXT_ALIGN_V eAlign);
	void Shadow(bool bShadow);
	void SetShadowOffset(const Vector3& vOffset);
	void SetShadowColor(float r, float g, float b, float a);
	void SetShadowColor(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetShadowColor(const Vector4& vColor);
	void AlphaBlend(bool bAlpha);
	void ShadowAlphaBlend(bool bAlpha);
	void SetOpacity(float fOpacity);
	void SetShadowOpacity(float fOpacity);
	void SetColor(float r, float g, float b, float a);
	void SetColor(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetColor(const Vector4& vColor);
	void SetRenderArea(float l, float t, float r, float b);

private:
	void CreateLayout();

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CText* Clone();
};

PUN_END