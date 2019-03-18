#pragma once

#include "Component/UserComponent.h"
#include "Component/Animation.h"
#include "Component/Transform.h"

PUN_USING

enum MINION3D_AI
{
	MAI_IDLE,
	MAI_TRACE
};

class CMinion3D :
	public CUserComponent
{
	friend class CGameObject;

protected:
	CMinion3D();
	CMinion3D(const CMinion3D& com);
	virtual ~CMinion3D();

private:
	CAnimation*	m_pAnimation;
	MINION3D_AI	m_eAI;
	CTransform*	m_pTargetTr;
	Vector3		m_vTargetPos;
	bool		m_bPathFind;
	list<Vector3>	m_PathList;
	bool		m_bMove;
	Vector3		m_vMovePos;

public:
	void SetTargetTransform(CTransform* pTr)
	{
		if (pTr)
		{
			pTr->AddRef();
			m_vTargetPos = pTr->GetWorldPos();
		}
		m_pTargetTr = pTr;
	}

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMinion3D* Clone();
};
