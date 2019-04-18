#pragma once

#include "Component/Collider.h"

PUN_BEGIN

enum COLLISION_GROUP_TYPE
{
	CGT_2D,
	CGT_3D
};

class PUN_DLL CCollisionManager
{
private:
	typedef struct _tagCollisionSection
	{
		CCollider**	pList;
		int		iSize;
		int		iCapacity;

		_tagCollisionSection()
		{
			iSize = 0;
			iCapacity = 5;

			pList = new CCollider*[iCapacity];
		}

		~_tagCollisionSection()
		{
			SAFE_DELETE_ARRAY(pList);
		}
	}CollisionSection, *PCollisionSection;

	typedef struct _tagCollisionGroup
	{
		PCollisionSection	pSectionList;
		COLLISION_GROUP_TYPE	eType;
		int		iCountX;
		int		iCountY;
		int		iCountZ;
		int		iCount;
		Vector3	vMin;
		Vector3	vMax;
		Vector3	vLength;
		Vector3	vCellLength;

		_tagCollisionGroup()
		{
		}

		~_tagCollisionGroup()
		{
			SAFE_DELETE_ARRAY(pSectionList);
		}
	}CollisionGroup, *PCollisionGroup;

private:
	unordered_map<string, PCollisionGroup>	m_mapGroup;
	class CCollider* m_pPrevMouseCollision;
	class CCollider** m_pWorldMouseList;
	int	m_iListSize;
	int	m_iListCapacity;
	bool m_isShow;

public:
	bool Init();
	bool CreateGroup(const string& strKey, const Vector3& vMin,
		const Vector3& vMax, int iCountX, int iCountY, int iCountZ,
		COLLISION_GROUP_TYPE eGroupType = CGT_3D);
	void ChangeGroupType(const string& strGroup, COLLISION_GROUP_TYPE eType);
	void AddCollision(class CGameObject* pObj);
	void ClearCollisionGroup();
	void Collision(float fTime);
	void Render(float fTime);
	void SetIsShow(bool Value) { m_isShow = Value; }
	bool GetIsShow() const { return m_isShow; }
	void OnOff();

private:
	void CollisionMouse2D(CGameObject* pMouseObj, float fTime);
	void CollisionMouse3D(CGameObject* pMouseObj, float fTime);

private:
	PCollisionGroup FindGroup(const string& strGroup);

private:
	static int SortZ(const void* pSrc, const void* pDest);

	DECLARE_SINGLE(CCollisionManager)
};

PUN_END
