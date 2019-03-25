#pragma once
#include "Ref.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
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

	vector<float>* m_vecInput;
	vector<float>* m_vecUpdate;
	vector<float>* m_vecLateUpdate;
	vector<float>* m_vecCollsion;
	vector<float>* m_vecRender;

	bool m_bHeader;
	string m_HeaderName;
	FileStream m_LogText;

public:
	int GetZOrder()	const;
	class CScene*	GetScene()	const;
	class CScene * GetSceneNonCount() const;

public:
	void SetZOrder(int iZOrder);

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

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
	class CGameObject * FindObjectNonCount(const string & strTag);
	void EraseObject(class CGameObject* pObj);
	void GetLayerListObjTag(std::vector<std::string>* _vec);
	class CGameObject* FindObjectFromIndex(int _idx);
	bool ListEmpty()
	{
		return m_ObjList.empty();
	}
};

PUN_END