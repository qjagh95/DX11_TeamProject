#pragma once
#include "Ref.h"

PUN_BEGIN

class PUN_DLL CLayer :
	public CRef
{
	friend class CScene;

private:
	CLayer();
	~CLayer();

private:
	class CScene*	m_pScene;
	int				m_iZOrder;
	list<class CGameObject*>	m_ObjList;

public:
	int GetZOrder()	const;
	class CScene*	GetScene()	const;

public:
	void SetZOrder(int iZOrder);

public:
	void Start();
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	void AddObject(class CGameObject* pObj);
	class CGameObject* FindObject(const string & strTag);
	void EraseObject(class CGameObject* pObj);
	bool ListEmpty()
	{
		return m_ObjList.empty();
	}
};

PUN_END