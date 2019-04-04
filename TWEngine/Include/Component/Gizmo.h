#pragma once
#include "Component.h"

PUN_BEGIN
class CRenderState;
class PUN_DLL CGizmo :
	public CComponent
{
private:
	class CMesh* m_pCylinderMesh;
	class CMesh* m_pTriangleMesh;
	class CTransform* m_pTargetTr;
	GIZMO_TYPE		  m_eGizmoType;
	bool			  m_bPick;
	CRenderState*	  m_pDepthDisable;

public:
	void Start();
	void AfterClone();
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	CGizmo* Clone();


public:
	void SetTargetTr(class CTransform* _pTr);
	void SetTarget(class CGameObject* _pObj);
	void SetGizmoType(GIZMO_TYPE _eType);
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);

public:
	CGizmo();
	CGizmo(const CGizmo& _Gizmo);
	~CGizmo();
};

PUN_END