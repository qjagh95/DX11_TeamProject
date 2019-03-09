#include "EngineHeader.h"
#include "EditManager.h"
#include "GameObject.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Resource/ResourcesManager.h"
#include "Component/Animation.h"

PUN_USING

CEditManager* CEditManager::m_pInst = nullptr;

CEditManager::CEditManager()
	:m_pScene(nullptr),
	 m_pActiveObject(nullptr),
	 m_pAnimation(nullptr)
{
}


CEditManager::~CEditManager()
{
	SAFE_RELEASE(m_pActiveObject);
	SAFE_RELEASE(m_pScene);
	SAFE_RELEASE(m_pAnimation);
}

bool CEditManager::Init()
{
	m_pScene = CSceneManager::GetInst()->GetScene();


	if (m_pScene == nullptr)
	{
		assert(false);
		return false;
	}

	return true;
}

void CEditManager::CreateObject(const std::string & _strTag, const std::string & _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		return;
	}

	CGameObject* pObject = CGameObject::CreateObject(_strTag, pLayer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::ObjectAddComponent(std::vector<int>& _vecComType)
{
	/*if (m_pActiveObject == nullptr)
	{
		return;
	}

	for (int i = 0; i < _vecComType.size(); ++i)
	{
		ObjectAddComponent((COMPONENT_TYPE)_vecComType[i]);
	}*/
}

void CEditManager::ObjectAddComponent(std::string& _strCompTag)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}

	std::string objTag  = "";
	std::string compTag = "";
	
	// Renderer
	if (_strCompTag.compare("renderer") == 0)
	{
		objTag = m_pActiveObject->GetTag();
		compTag = objTag + "renderer";

		// Add
		CRenderer* pRenderer = m_pActiveObject->AddComponent<CRenderer>(compTag);
		SAFE_RELEASE(pRenderer);
	}
	// Light
	else if (_strCompTag.compare("light") == 0)
	{
		objTag = m_pActiveObject->GetTag();
		compTag = objTag + "light";
	}
}

void CEditManager::ActiveObjectSetTransform(Vector3 _vPos, Vector3 _vScale, Vector3 _vRot)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}

	CTransform* pTransform = m_pActiveObject->GetTransform();

	pTransform->SetWorldPos(_vPos);
	pTransform->SetWorldScale(_vScale);
	pTransform->SetWorldRot(_vRot);

	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetPosition(double _dX, double _dY, double _dZ)
{
	if (m_pActiveObject == nullptr)
		return;

	CTransform* pTransform = m_pActiveObject->GetTransform();
	pTransform->SetWorldPos((float)_dX, (float)_dY, (float)_dZ);

	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetScale(double _dX, double _dY, double _dZ)
{
	if (m_pActiveObject == nullptr)
		return;

	CTransform* pTransform = m_pActiveObject->GetTransform();
	pTransform->SetWorldScale(Vector3((float)_dX, (float)_dY, (float)_dZ));

	SAFE_RELEASE(pTransform);

}
void CEditManager::ActiveObjectSetRotation(double _dX, double _dY, double _dZ)
{
	if (m_pActiveObject == nullptr)
		return;

	CTransform* pTransform = m_pActiveObject->GetTransform();
	pTransform->SetWorldRot(Vector3((float)_dX, (float)_dY, (float)_dZ));

	SAFE_RELEASE(pTransform);
}


void CEditManager::SetActiveObject(const std::string & _strObjTag, const std::string & _strLayerTag)
{
	if (m_pActiveObject != nullptr)
	{
		SAFE_RELEASE(m_pActiveObject);
	}

	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);

	if (pLayer)
	{
		CGameObject* pObj = pLayer->FindObject(_strObjTag);
		m_pActiveObject = pObj;
	}

	SAFE_RELEASE(pLayer);
}

void CEditManager::GetLayerListObjTag(const std::string & _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);

	if (pLayer)
	{
		m_vecstrObjList.clear();
		pLayer->GetLayerListObjTag(&m_vecstrObjList);
	}

	SAFE_RELEASE(pLayer);
}

void CEditManager::LoadClipFromFullPath(const std::wstring & _strFullPath)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}

	if (m_pAnimation)
	{ 
		SAFE_RELEASE(m_pAnimation);
		m_pActiveObject->RemoveComponentFromType(CT_ANIMATION);
	}
	
	m_pAnimation = m_pActiveObject->AddComponent<CAnimation>("Animation");

	int	iLength = _strFullPath.length();

	if (_strFullPath[iLength - 3] == 'f' &&
		_strFullPath[iLength - 2] == 'b' &&
		_strFullPath[iLength - 1] == 'x')
		m_pAnimation->AddClip(_strFullPath.c_str());

	else
		m_pAnimation->LoadBoneAndAnimationFullPath(_strFullPath.c_str());
}

std::string CEditManager::GetIndexFromObjTag(int _idx)
{
	if (_idx >= m_vecstrObjList.size())
	{
		return "";
	}

	return m_vecstrObjList[_idx];
}

void CEditManager::SetIndexFromSetObject(int _idx, const std::string& _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);

	if (pLayer)
	{
		SAFE_RELEASE(m_pActiveObject);
		m_pActiveObject = pLayer->FindObjectFromIndex(_idx);
	}

	SAFE_RELEASE(pLayer);
}

/**************************************************************************************************/

void CEditManager::AddRenderer(const std::string & _strTag)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}
	
	CRenderer* pRenderer = m_pActiveObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	if (pRenderer)
	{
		return;
	}

	pRenderer = m_pActiveObject->AddComponent<CRenderer>(_strTag);

	SAFE_RELEASE(pRenderer);
}

void CEditManager::ActiveObjectFromSetMesh(const std::string & _strMeshTag)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}

	CRenderer* pRenderer = m_pActiveObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer == nullptr)
	{
		return;
	}

	pRenderer->SetMesh(_strMeshTag);

	SAFE_RELEASE(pRenderer);
}

void CEditManager::ActiveObjectFromSetTag(const std::string & _strObjTag)
{
	if (m_pActiveObject == nullptr)
	{
		return;
	}

	m_pActiveObject->SetTag(_strObjTag);
}

void CEditManager::GetClipNameList(std::vector<std::string>* _vecstrClipList)
{
	if (m_pAnimation == nullptr)
	{
		return;
	}

	m_pAnimation->GetClipTagList(_vecstrClipList);
}

vector<string>* CEditManager::GetMeshNameList()
{
	vector<string>* vecMeshNameList = CResourcesManager::GetInst()->GetMeshNameList();
	return vecMeshNameList;
}