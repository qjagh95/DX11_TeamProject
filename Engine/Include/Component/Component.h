#pragma once
#include "../Ref.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class PUN_DLL CComponent :
	public CRef
{
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent() = 0;

protected:
	class CScene*		m_pScene;
	class CLayer*		m_pLayer;
	class CGameObject*	m_pObject;
	class CTransform*	m_pTransform;

public:
	class CScene*		GetScene()	const;
	class CLayer*		GetLayer()	const;
	class CGameObject*	GetGameObject()	const;
	class CGameObject * GetGameObjectNonCount() const;
	class CTransform*	GetTransform()	const;
	class CTransform * GetTransformNonCount() const;

protected:
	COMPONENT_TYPE	m_eComType;

public:
	COMPONENT_TYPE	GetComponentType()	const;

public:
	// Start함수는 장면이 시작되면서 딱 1번만 호출될 함수이다.
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init() = 0;
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CComponent* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void Save(BinaryWrite* _pInstBW);
	virtual void Load(BinaryRead*  _pInstBR);
	virtual void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	virtual void LoadFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);

public:
	const list<CComponent*>* FindComponentsFromTag(const string& strTag);
	const list<CComponent*>* FindComponentsFromType(COMPONENT_TYPE eType);
	bool	CheckComponentFromType(COMPONENT_TYPE eType);

	template <typename T>
	T* AddComponent(const string& strTag)
	{
		return m_pObject->AddComponent<T>(strTag);
	}

	template <typename T>
	T* FindComponentFromTag(const string& strTag)
	{
		return m_pObject->FindComponentFromTag<T>(strTag);
	}

	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE eType)
	{
		return m_pObject->FindComponentFromType<T>(eType);
	}
};

PUN_END