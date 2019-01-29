#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CSPBar :
	public CUI
{
	friend class CGameObject;

protected:
	CSPBar();
	CSPBar(const CSPBar& spbar);
	virtual ~CSPBar();

private:
	BarCBuffer	m_tCBuffer;
	BAR_DIR		m_eDir;
	float		m_fMinValue;
	float		m_fMaxValue;
	float		m_fValue;
	float		m_fValueLength;
	Vector3		m_vScale;
	class CColliderRect*	m_pCollider;

public:
	void SetBarDir(BAR_DIR eDir);
	void SetValue(float fValue);
	void AddValue(float fValue);
	void SetMinMaxValue(float fMin, float fMax);
	void SetScale(const Vector3& vScale);
	void SetScale(float x, float y, float z);
	void LightOn();
	void LightOff();

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CSPBar* Clone();

public:
	void Hit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

PUN_END