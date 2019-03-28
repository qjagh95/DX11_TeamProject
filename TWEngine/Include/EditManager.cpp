#include "EngineHeader.h"
#include "Core.h"
#include "EditManager.h"
#include "GameObject.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Component/Animation.h"
#include "Resource/ResourcesManager.h"

PUN_USING

CEditManager* CEditManager::m_pInst = nullptr;

CEditManager::CEditManager()
	:m_pScene(nullptr),
	m_pObject(nullptr),
	m_pAnimation(nullptr)
{
}

CEditManager::~CEditManager()
{
	SAFE_RELEASE(m_pObject);
	SAFE_RELEASE(m_pScene);
	SAFE_RELEASE(m_pAnimation);
	Safe_Delete_VecList(m_vecDivideFrame);
}

bool CEditManager::Init()
{
	m_pScene = CSceneManager::GetInst()->GetScene();
	if (m_pScene == nullptr)
	{
		TrueAssert(true);
		return false;
	}
	return true;
}

void CEditManager::GetLayerList(vector<string>* _pVec)
{
	m_pScene->GetLayerTagList(_pVec);
}

void CEditManager::GetSelectLayerObjList(string _strLayerTag, vector<string>* _pVec)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->GetLayerListObjTag(_pVec);
	SAFE_RELEASE(pLayer);
}

void CEditManager::SetActiveObject(const string _strObjectTag, const string _strLayerTag)
{
	// 기존 선택 오브젝트 해제
	if (m_pObject != nullptr)
	{
		SAFE_RELEASE(m_pObject);
	}
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	m_pObject = pLayer->FindObject(_strObjectTag);
	SAFE_RELEASE(pLayer);
}

void CEditManager::CreateObject(const string _strObjectTag, const string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
		return;
	}
	CGameObject* pObject = CGameObject::CreateObject(_strObjectTag, pLayer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::DeleteObject(const string _strObjectTag, const string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	CGameObject* pObject = pLayer->FindObject(_strObjectTag);
	pLayer->EraseObject(pObject);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::DeleteAllObject()
{
	vector<string> vecLayerTag;
	m_pScene->GetLayerTagList(&vecLayerTag);
	for (size_t i = 0; i < vecLayerTag.size(); ++i)
	{
		CLayer* pLayer = m_pScene->FindLayer(vecLayerTag[i]);
		pLayer->EraseObjectAll();
		SAFE_RELEASE(pLayer);
	}
}

void CEditManager::ChangeObjectTag(const string _strObjectTag)
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}
	m_pObject->SetTag(_strObjectTag);
}

void CEditManager::ChangeObjectInLayer(const string _strLayerTag)
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}

	// 기존 레이어에서 삭제
	CLayer* pLayer = m_pObject->GetLayer();
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->EraseObject(m_pObject);

	// 새로운 레이어에 오브젝트 등록
	pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->AddObject(m_pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::AddComponent(std::string& _strCompTag)
{
	if (m_pObject == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object가 없습니다"), 0, MB_OK);
		return;
	}

	std::string objTag = "";
	std::string compTag = "";

	// Renderer
	if (_strCompTag.compare("renderer") == 0)
	{
		objTag = m_pObject->GetTag();
		compTag = objTag + "renderer";

		// Add
		CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>(compTag);
		SAFE_RELEASE(pRenderer);
	}
	// Light
	else if (_strCompTag.compare("light") == 0)
	{
		objTag = m_pObject->GetTag();
		compTag = objTag + "light";
	}
}

void CEditManager::ActiveObjectSetScale(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object가 없습니다"), 0, MB_OK);
		return;
	}

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetRotate(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object가 없습니다"), 0, MB_OK);
		return;
	}

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldRot(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetPosition(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldPos((float)_dX, (float)_dY, (float)_dZ);
	SAFE_RELEASE(pTransform);
}

vector<Vector3> CEditManager::GetWorldTransform(const string _strObjectTag, const string _strLayerTag, int _eType)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}

	CGameObject* pObj = pLayer->FindObject(_strObjectTag);
	if (pObj == nullptr)
	{
		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}

	CTransform* pTr = pObj->GetTransform();
	vector<Vector3> pVecTranform;
	pVecTranform.reserve(1);
	switch ((eTransformType)_eType)
	{
		case eTransformType::TT_SCALE:
		{
			pVecTranform.push_back(pTr->GetWorldScale());
			break;
		}
		case eTransformType::TT_ROTATE:
		{
			pVecTranform.push_back(pTr->GetWorldRot());
			break;
		}
		case eTransformType::TT_POSITION:
		{
			pVecTranform.push_back(pTr->GetWorldPos());
			break;
		}
	}

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pLayer);

	return pVecTranform;
}

void CEditManager::GetMeshNameList(vector<string>* _pVec)
{
	CResourcesManager::GetInst()->GetMeshNameList(_pVec);
}

void CEditManager::SetMesh(const string& _strMeshTag)
{
	if (m_pObject == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object가 없습니다"), 0, MB_OK);
		return;
	}

	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	if (pRenderer == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object의 Renderer가 없습니다"), 0, MB_OK);
		return;
	}

	pRenderer->SetMesh(_strMeshTag);
	SAFE_RELEASE(pRenderer);
}

void CEditManager::LoadClipFromFullPath(const wstring& _strFullPath)
{
	if (m_pObject == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object가 없습니다"), 0, MB_OK);
		return;
	}

	if (m_pAnimation)
	{
		SAFE_RELEASE(m_pAnimation);
		m_pObject->RemoveComponentFromType(CT_ANIMATION);
	}

	m_pAnimation = m_pObject->AddComponent<CAnimation>("Animation");
	int	iLength = (int)_strFullPath.length();

	if (_strFullPath[iLength - 3] == 'f' &&
		_strFullPath[iLength - 2] == 'b' &&
		_strFullPath[iLength - 1] == 'x')
		m_pAnimation->AddClip(_strFullPath.c_str());

	else
		m_pAnimation->LoadBoneAndAnimationFullPath(_strFullPath.c_str());
}

void CEditManager::GetClipNameList(vector<string>* _vecstrClipList)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return;
	}

	m_pAnimation->GetClipTagList(_vecstrClipList);
}

void CEditManager::SetDivideKeyFrame()
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return;
	}

	m_pAnimation->GetCurrentKeyFrame(m_vecDivideFrame);
}

void CEditManager::DeleteDivideKeyFrame()
{
	if (m_vecDivideFrame.empty())
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("Divide Clip이 없습니다"), 0, MB_OK);
		return;
	}

	Safe_Delete_VecList(m_vecDivideFrame);
}

void CEditManager::DeleteClip(const string& _strKey)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return;
	}

	m_pAnimation->DeleteClip(_strKey);
}

bool CEditManager::ModifyClip(const string& strKey, const string& strChangeKey, int iOption,
	int iStartFrame, int iEndFrame, float fPlayTime)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return false;
	}

	if (m_vecDivideFrame.empty())
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("Divide Clip이 없습니다"), 0, MB_OK);
		return false;
	}


	return m_pAnimation->ModifyClip(strKey, strChangeKey, (ANIMATION_OPTION)iOption, iStartFrame, iEndFrame, fPlayTime, m_vecDivideFrame);
}

bool CEditManager::AddClip(const string& strKey, int iOption, int iStartFrame, int iEndFrame, float fPlayTime)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return false;
	}

	if (m_vecDivideFrame.empty())
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("Divide Clip이 없습니다"), 0, MB_OK);
		return false;
	}

	m_pAnimation->AddClip(strKey, (ANIMATION_OPTION)iOption, iStartFrame, iEndFrame, fPlayTime, m_vecDivideFrame);

	return true;
}

void CEditManager::ChangeClip(const string& _strKey)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return;
	}

	m_pAnimation->ChangeClip(_strKey);
}

void CEditManager::ClipSaveFromFullPath(const string& _strFullPath)
{
	if (m_pAnimation == nullptr)
	{
		//MessageBox(CCore::GetInst()->GetWindowHandle(), TEXT("선택된 Object에 Animation이 없습니다"), 0, MB_OK);
		return;
	}

	m_pAnimation->SaveFromFullPath(_strFullPath.c_str());
}