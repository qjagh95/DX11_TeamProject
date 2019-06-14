#include "EngineHeader.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Component/Camera.h"
#include "Device.h"

PUN_USING

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_pCurScene(nullptr),
	m_bChange(false)
{
	m_bStart = false;
}

CSceneManager::~CSceneManager()
{
	SAFE_RELEASE(m_pCurScene);
	Safe_Release_Map(m_SceneMap);
	SAFE_RELEASE(m_pMainCameraObj);
	SAFE_RELEASE(m_pMainCameraTr);
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pUICameraObj);
	SAFE_RELEASE(m_pUICameraTr);
	SAFE_RELEASE(m_pUICamera);
	Safe_Release_Map(m_mapCamera);
}

CScene * CSceneManager::GetScene() const
{
	m_pCurScene->AddRef();
	return m_pCurScene;
}

CScene * CSceneManager::GetSceneNonCount() const
{
	return m_pCurScene;
}

CCamera * CSceneManager::GetMainCamera() const
{
	m_pMainCamera->AddRef();

	return m_pMainCamera;
}

CCamera * CSceneManager::GetMainCameraNonCount() const
{
	return m_pMainCamera;
}

CTransform * CSceneManager::GetMainCameraTransform() const
{
	m_pMainCameraTr->AddRef();
	return m_pMainCameraTr;
}

CTransform * CSceneManager::GetMainCameraTransformNonCount() const
{
	return m_pMainCameraTr;
}

CGameObject * CSceneManager::GetUICameraObj() const
{
	m_pUICameraObj->AddRef();
	return m_pUICameraObj;
}

CGameObject * CSceneManager::GetUICameraObjNonCount() const
{
	return m_pUICameraObj;
}

CCamera * CSceneManager::GetUICamera() const
{
	m_pUICamera->AddRef();
	return m_pUICamera;
}

CCamera * CSceneManager::GetUICameraNonCount() const
{
	return m_pUICamera;
}

CTransform * CSceneManager::GetUICameraTransform() const
{
	m_pUICameraTr->AddRef();
	return m_pUICameraTr;
}

CTransform * CSceneManager::GetUICameraTransformNonCount() const
{
	return m_pUICameraTr;
}

CGameObject * CSceneManager::CreateCamera(const string & strTag, const Vector3 & vPos, CAMERA_TYPE eType, float fWidth, float fHeight, float fViewAngle, float fNear, float fFar)
{
	CGameObject*	pCameraObj = FindCamera(strTag);

	if (pCameraObj)
		return pCameraObj;

	pCameraObj = CGameObject::CreateObject(strTag);

	CTransform*	pTr = pCameraObj->GetTransform();
	pTr->SetWorldPos(vPos);
	SAFE_RELEASE(pTr);

	CCamera*	pCamera = pCameraObj->AddComponent<CCamera>("Camera");
	pCamera->SetCameraInfo(eType, fWidth, fHeight, fViewAngle, fNear, fFar);
	SAFE_RELEASE(pCamera);

	pCameraObj->AddRef();
	m_mapCamera.insert(make_pair(strTag, pCameraObj));

	return pCameraObj;
}

void CSceneManager::ChangeCamera(const string & strTag)
{
	CGameObject*	pCameraObj = FindCamera(strTag);

	if (!pCameraObj)
		return;

	SAFE_RELEASE(m_pMainCameraObj);
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pMainCameraTr);

	m_pMainCameraObj = pCameraObj;
	m_pMainCameraTr = pCameraObj->GetTransform();
	m_pMainCamera = pCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);
}

CGameObject * CSceneManager::FindCamera(const string & TagName)
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapCamera.find(TagName);

	if (iter == m_mapCamera.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CSceneManager::AfterInit()
{
	for (size_t i = 0;  i < m_vecTemp.size(); i++)
		m_vecTemp[i]->AfterInit();
}

CScene * CSceneManager::FindScene(const string & KeyName)
{
	auto FindIter = m_SceneMap.find(KeyName);

	if (FindIter == m_SceneMap.end())
		return NULLPTR;

	return FindIter->second;
}

void CSceneManager::ChangeScene(const string & KeyName)
{
	CScene* getScene = FindScene(KeyName);

	if (getScene == NULLPTR)
	{
		TrueAssert(true);
		return;
	}

	m_pCurScene = getScene;

	auto StartIter = CObjectManager::GetInst()->GetMap()->begin();
	auto EndIter = CObjectManager::GetInst()->GetMap()->end();

	for (; StartIter != EndIter; StartIter++)
		StartIter->second->SetScene(m_pCurScene);

	m_pCurScene->ChangeScene();

	GET_SINGLE(CRenderManager)->SetSkyObject(getScene->GetSkyObjectNonCount());

	m_bChange = true;
}

void CSceneManager::SetChange(bool bChange)
{
	m_bChange = bChange;
}

bool CSceneManager::GetChange() const
{
	return m_bChange;
}

void CSceneManager::DeleteScene(const string & SceneName)
{
	CScene* getScene = FindScene(SceneName);

	if (getScene == NULLPTR)
		return;

	m_SceneMap.erase(SceneName);
	SAFE_RELEASE(getScene);
}

CGameObject * CSceneManager::GetMainCameraObj() const
{
	m_pMainCameraObj->AddRef();
	return m_pMainCameraObj;
}

CGameObject * CSceneManager::GetMainCameraObjNonCount() const
{
	return m_pMainCameraObj;
}

bool CSceneManager::Init()
{
	m_pMainCameraObj = CreateCamera("MainCamera", Vector3(0.f, 0.f, -5.f), CT_PERSPECTIVE, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 70.f, 0.03f, 1000.f);;
	m_pMainCameraTr = m_pMainCameraObj->GetTransform();
	m_pMainCamera = m_pMainCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);

	m_pUICameraObj = CreateCamera("UICamera", Vector3(0.f, 0.f, 0.f), CT_ORTHO, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 60.f, 0.f, 1000.f);
	m_pUICameraTr = m_pUICameraObj->GetTransform();
	m_pUICamera = m_pUICameraObj->FindComponentFromType<CCamera>(CT_CAMERA);

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pMainCameraObj->Input(fTime);

	m_pCurScene->Input(fTime);

	return 0;
}

int CSceneManager::Update(float fTime)
{
	m_pMainCameraObj->Update(fTime);

	m_pCurScene->Update(fTime);

	return 0;
}

int CSceneManager::LateUpdate(float fTime)
{
	m_pMainCameraObj->LateUpdate(fTime);

	m_pCurScene->LateUpdate(fTime);

	return 0;
}

int CSceneManager::Collision(float fTime)
{
	m_pCurScene->Collision(fTime);

	return 0;
}

void CSceneManager::Render(float fTime)
{
	m_pCurScene->Render(fTime);
}

void CSceneManager::AddLayer(const string & strTag, int iZOrder, bool bCurrent)
{
	m_pCurScene->AddLayer(strTag, iZOrder);
}

void CSceneManager::ChangeLayerZOrder(const string & strTag, int iZOrder, bool bCurrent)
{
	m_pCurScene->ChangeLayerZOrder(strTag, iZOrder);
}

CLayer * CSceneManager::FindLayer(const string & strTag, bool bCurrent)
{
	return m_pCurScene->FindLayer(strTag);
}

CGameObject* CSceneManager::FindObject(const string & TagName)
{
	if (m_pCurScene != NULLPTR)
		return m_pCurScene->FindObject(TagName);

	return NULLPTR;
}

void CSceneManager::Access()
{
	for (size_t i = 0; i < m_vecTemp.size(); i++)
	{
		auto StartIter = m_vecTemp[i]->m_LayerList.begin();
		auto EndIter = m_vecTemp[i]->m_LayerList.end();

		for (; StartIter != EndIter; StartIter++)
		{
			auto StartIter1 = CObjectManager::GetInst()->GetMap()->begin();
			auto EndIter1 = CObjectManager::GetInst()->GetMap()->end();

			for (; StartIter1 != EndIter1; StartIter1++)
			{
				if (StartIter1->second->GetScene()->GetTag() == m_vecTemp[i]->GetTag())
					continue;

				if ((*StartIter)->GetTag() == StartIter1->second->GetLayer()->GetTag())
					(*StartIter)->GetObjectList()->push_back(StartIter1->second);
			}
		}
	}

	AfterInit();
}
