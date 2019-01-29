#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderPixel :
	public CCollider
{
	friend class CGameOBject;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& com);
	virtual ~CColliderPixel();

private:
	PixelInfo	m_tRelativeInfo;
	PixelInfo	m_tInfo;

public:
	PixelInfo GetInfo()	const;
	void SetInfo(const Vector3& vMin, const Pixel24& tPixel, const char* pFileName,
		const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderPixel* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END