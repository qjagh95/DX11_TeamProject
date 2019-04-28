#include "EngineHeader.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Camera.h"

PUN_USING

DEFINITION_SINGLE(CObjectManager)

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	Safe_Release_Map(m_DontMap);
}

void CObjectManager::PushDontDestoryObject(CGameObject* _pObject)
{
	if (FindObject(_pObject->GetTag()) != nullptr)
		return;

	_pObject->AddRef();
	m_DontMap.insert(make_pair(_pObject->GetTag(), _pObject));
}

bool CObjectManager::RemoveDontDestroyObject(CGameObject * _pObject)
{
	CGameObject* getObject = FindObject(_pObject->GetTag());

	if (getObject == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}

	m_DontMap.erase(_pObject->GetTag());
	SAFE_RELEASE(getObject);

	return true;
}

CGameObject * CObjectManager::FindObject(const string & KeyName)
{
	auto FindIter = m_DontMap.find(KeyName);
	
	if (FindIter == m_DontMap.end())
		return NULLPTR;

	return FindIter->second;
}

bool CObjectManager::Init()
{
	return true;
}

