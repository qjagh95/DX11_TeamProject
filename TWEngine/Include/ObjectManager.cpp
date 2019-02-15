#include "EngineHeader.h"
#include "ObjectManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"

PUN_USING

DEFINITION_SINGLE(CObjectManager)

CObjectManager::CObjectManager()
{
}


CObjectManager::~CObjectManager()
{
	Safe_Release_VecList(m_DontDestroyObjList);
}

void CObjectManager::ChangeScene(CScene * _pScene , class CGameObject* _pObject, const std::string& _strLayerName)
{
	CLayer* pLayer = _pScene->FindLayer(_strLayerName);

	pLayer->AddObject(_pObject);

	SAFE_RELEASE(pLayer);
}

void CObjectManager::ChangeSceneFromDontDestroyObj(CScene* _pScene)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_DontDestroyObjList.end();
	for (Iter = m_DontDestroyObjList.begin(); Iter != EndIter; ++Iter)
	{
		ChangeScene(_pScene, *Iter , (*Iter)->GetLayerName());
	}
}

void CObjectManager::PushDontDestoryObject(CGameObject* _pObject)
{
	if (FindDonDestoryObject(_pObject) != nullptr)
	{
		return;
	}
	
	_pObject->AddRef();
	m_DontDestroyObjList.push_back(_pObject);
}

bool CObjectManager::RemoveDontDestroyObject(CGameObject * _pObject)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_DontDestroyObjList.end();
	for (Iter = m_DontDestroyObjList.begin(); Iter != EndIter; ++Iter)
	{
		if (*Iter == _pObject)
		{
			SAFE_RELEASE((*Iter));
			m_DontDestroyObjList.erase(Iter);
			return true;
		}
	}
	
	return false;
}

bool CObjectManager::RemoveDontDestroyObject(const std::string & _strTag)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_DontDestroyObjList.end();
	for (Iter = m_DontDestroyObjList.begin(); Iter != EndIter; ++Iter)
	{
		if ((*Iter)->GetTag() == _strTag)
		{
			SAFE_RELEASE((*Iter));
			m_DontDestroyObjList.erase(Iter);
			return true;
		}
	}

	return false;
}

CGameObject * CObjectManager::FindDonDestoryObject(const std::string & _strTag)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_DontDestroyObjList.end();
	for (Iter = m_DontDestroyObjList.begin(); Iter != EndIter; ++Iter)
	{
		if ((*Iter)->GetTag() == _strTag)
		{
			return *Iter;
		}
	}
	return nullptr;
}

CGameObject * CObjectManager::FindDonDestoryObject(CGameObject * _pObject)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_DontDestroyObjList.end();
	for (Iter = m_DontDestroyObjList.begin(); Iter != EndIter; ++Iter)
	{
		if (*Iter == _pObject)
		{
			return *Iter;
		}
	}
	return nullptr;
}

bool CObjectManager::Init()
{
	return true;
}

