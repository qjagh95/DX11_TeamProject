#pragma once
#include "../Client.h"
#include <Component/UserComponent.h>

PUN_USING

class CInventory;
class CHealingPackIcon;
class CHealingPack : public CUserComponent
{
private:
	friend class CGameObject;

public:
	CHealingPack();
	CHealingPack(const CHealingPack& _healingPack);
	~CHealingPack() override;

public:
	void Start()					override {};
	void AfterClone()				override {};
	virtual void AfterInit();
	bool Init()						override;
	int	 Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override { return 0; };
	void Collision(float _fTime)	override {};
	void Render(float _fTime)		override {};
	CHealingPack* Clone()			override { return nullptr; };

private:
	bool	m_isInvenInItem;
	bool	m_bMotion;
	bool	m_bGetItem;
	int		m_hpAmount;
	string	m_strMeshKey;

private:
	CRenderer*			m_pRenderer;
	CGameObject*		m_pObjInventory;
	CInventory*			m_pInventory;
	CGameObject*		m_pObjItemIcon;
	CGameObject*	m_pOutLineObj;
	CGameObject*	m_pBigObj;
	CTransform*		m_pOutLineTr;
	CTransform*		m_pBigTr;
	CRenderer*		m_pOutLineRenderer;
	CRenderer*		m_pBigRenderer;
	CMaterial*		m_pOutLineMaterial;

public:
	void SetHPAmount(int _amount);
	int  GetHPAmount();
	void SetMesh(const string& _meshKey, const TCHAR* _meshName);
	const string GetMeshKey();
	void HitEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void MouseOut(CCollider* _pSrc, CCollider* _pDest, float _fTime);

private:
	void SetOutLineVisible(bool bEnable);

public:
	void SetOutLinePos(const Vector3& vPos);
	void SetOutLinePos(float x, float y, float z);
	void SetOutLineScale(const Vector3& vScale);
	void SetOutLineScale(float x, float y, float z);
};