#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CNavigation2D :
	public CComponent
{

	friend class CGameObject;

protected:
	CNavigation2D();
	CNavigation2D(const CNavigation2D& navi);
	~CNavigation2D();

private:
	list<Vector3>	m_PathList;
	bool			m_bAutoFind;
	bool			m_bMove;
	Vector3			m_vTargetPos;
	float			m_fSpeed;

public:
	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

public:
	bool Find(const Vector3& vStart, const Vector3& vEnd);
	void AutoFind(bool bAuto);

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CNavigation2D* Clone();
};

PUN_END