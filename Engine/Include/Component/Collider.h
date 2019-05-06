#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CCollider :
	public CComponent
{
	friend class CGameObject;

protected:
	CCollider();
	CCollider(const CCollider& com);
	virtual ~CCollider() = 0;

protected:
	COLLIDER_ID		m_eColliderID;
	COLLIDER_TYPE	m_eCollType;
	Vector3			m_vPivot;
	list<CCollider*>	m_PrevCollision;
	list<function<void(CCollider*, CCollider*, float)>>	m_CollisionFunc[CCT_END];
	Vector3			m_vSectionMin;
	Vector3			m_vSectionMax;
	bool			m_bUpdateCollision;
	string			m_strCollisionGroup;
	list<int>		m_CollisionSectionList;
	OBJECT_COLLISION_TYPE m_eObjectCollType;
	string m_MyTypeName;
	vector<string> m_vecContinueName;
	bool			m_bCallBackFunc;
public:
	void SetCallBackFunc(bool _bEnable)
	{
		m_bCallBackFunc = _bEnable;
	}
	bool GetCallBackFunc() const
	{
		return m_bCallBackFunc;
	}
public:
	OBJECT_COLLISION_TYPE GetObjectCollType()
	{
		return m_eObjectCollType;
	}

	class CShader*	m_pShader;
	class CMesh*	m_pMesh;
	ID3D11InputLayout*	m_pLayout;
	class CRenderState*	m_pDepthDisable;
	Vector4			m_vColor;
	class CRenderState* m_pWireFrame;

public:
	COLLIDER_ID GetColliderID() const;
	void SetColliderID(int iID);
	COLLIDER_TYPE GetColliderType()	const;
	Vector3 GetSectionMin()	const;
	Vector3 GetSectionMax()	const;
	bool GetUpdateCollision()	const;
	string GetCollisionGroup()	const;
	const list<int>* GetColisionSection()	const;
	void SetMyTypeName(const string& TypeName) { m_MyTypeName = TypeName; }
	void SetContinueTypeName(const string& ContinueTypeName) { m_vecContinueName.push_back(ContinueTypeName); }
	string GetMyTypeName() const { return m_MyTypeName; }
	string GetContinueName(size_t Index) const { return m_vecContinueName[Index]; }
	size_t GetContinueSize() const { return m_vecContinueName.size(); }

public:
	void ClearCollisionSection();
	void AddCollisionSection(int iSection);
	void SetCollisionGroup(const string& strCollisionGroup);
	void SetPivot(const Vector3& vPivot);
	void SetPivot(float x, float y, float z);
	bool CheckPrevCollision(CCollider* pDest);
	void AddPrevCollision(CCollider* pDest);
	void ErasePrevCollision(CCollider* pDest);
	void CheckPrevCollisionInSection(float fTime);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCollider* Clone() = 0;

public:
	virtual bool Collision(CCollider* pDest, float fTime) = 0;

public:
	void OnCollisionEnter(CCollider* pDest, float fTime);
	void OnCollision(CCollider* pDest, float fTime);
	void OnCollisionLeave(CCollider* pDest, float fTime);

public:
	bool CollisionRectToRect(const BoxInfo& tSrc, const BoxInfo& tDest);
	bool CollisionRectToPoint(const BoxInfo& tSrc, const Vector3& vDest);
	bool CollisionOBB2DToOBB2D(const OBB2DInfo& tSrc, const OBB2DInfo& tDest);
	bool CollisionOBB2DToRect(const OBB2DInfo& tSrc, const BoxInfo& tDest);
	bool CollisionOBB2DToPoint(const OBB2DInfo& tSrc, const Vector3& vDest);
	bool CollisionRectToPixel(BoxInfo tSrc, const PixelInfo& tDest);
	bool CollisionSphereToSphere(SphereInfo tSrc, const SphereInfo& tDest);
	bool CollisionOBB3DToOBB3D(const OBB3DInfo& tSrc, const OBB3DInfo& tDest);
	bool CollisionRayToSphere(const RayInfo& tSrc, const SphereInfo& tDest);
	bool CollisionRayToOBB3D(const OBB3DInfo& tDest, const RayInfo& tSrc, CTransform* pOBBTr);
	bool CollisionRayToAABB(const RayInfo& _tSrc, const _tagAABBInfo& _tDest);
	
public:
	void SetCollisionCallback(COLLISION_CALLBACK_TYPE eType,
		void(*pFunc)(CCollider*, CCollider*, float));

	template <typename T>
	void SetCollisionCallback(COLLISION_CALLBACK_TYPE eType,
		T* pObj, void(T::*pFunc)(CCollider*, CCollider*, float))
	{
		function<void(CCollider*, CCollider*, float)>	func;

		func = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);

		m_CollisionFunc[eType].push_back(func);
	}
};

PUN_END