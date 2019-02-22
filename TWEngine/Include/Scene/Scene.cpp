#include "EngineHeader.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneComponent.h"
#include "../GameObject.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Device.h"
#include "../Input.h"
#include "../SoundManager.h"
#include "../Component/Light.h"
#include "../Component/Material.h"
#include "../Component/Renderer.h"

PUN_USING

CScene::CScene()
{
	m_pSkyObj = nullptr;
	m_pSkyMtrl = nullptr;
}

CScene::~CScene()
{
	SAFE_RELEASE(m_pSkyObj);
	SAFE_RELEASE(m_pSkyMtrl);

	GET_SINGLE(CSoundManager)->DeleteSound(this);

	CGameObject::DestroyPrototype(this);
	Safe_Release_VecList(m_LayerList);
	Safe_Release_VecList(m_SceneComponentList);

	Safe_Release_Map(m_mapCamera);
	SAFE_RELEASE(m_pMainCameraObj);
	SAFE_RELEASE(m_pMainCameraTr);
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pUICameraObj);
	SAFE_RELEASE(m_pUICameraTr);
	SAFE_RELEASE(m_pUICamera);
}

CGameObject * CScene::GetMainCameraObj() const
{
	m_pMainCameraObj->AddRef();
	return m_pMainCameraObj;
}

CGameObject * CScene::GetMainCameraObjNonCount() const
{
	return m_pMainCameraObj;
}

CCamera * CScene::GetMainCamera() const
{
	m_pMainCamera->AddRef();
	return m_pMainCamera;
}

CCamera * CScene::GetMainCameraNonCount() const
{
	return m_pMainCamera;
}

CTransform * CScene::GetMainCameraTransform() const
{
	m_pMainCameraTr->AddRef();
	return m_pMainCameraTr;
}

CTransform * CScene::GetMainCameraTransformNonCount() const
{
	return m_pMainCameraTr;
}


CGameObject * CScene::GetUICameraObj() const
{
	m_pUICameraObj->AddRef();
	return m_pUICameraObj;
}

CGameObject * CScene::GetUICameraObjNonCount() const
{
	return m_pUICameraObj;
}

CCamera * CScene::GetUICamera() const
{
	m_pUICamera->AddRef();
	return m_pUICamera;
}

CCamera * CScene::GetUICameraNonCount() const
{
	return m_pUICamera;
}

CTransform * CScene::GetUICameraTransform() const
{
	m_pUICameraTr->AddRef();
	return m_pUICameraTr;
}

CTransform * CScene::GetUICameraTransformNonCount() const
{
	return m_pUICameraTr;
}

CGameObject * CScene::GetSkyObj() const
{
	m_pSkyObj->AddRef();
	return m_pSkyObj;
}

CMaterial * CScene::GetSkyMaterial() const
{
	m_pSkyMtrl->AddRef();
	return m_pSkyMtrl;
}

void CScene::Start()
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->Start();
	}
}

bool CScene::Init()
{
	AddLayer("Stage", INT_MIN + 1);
	AddLayer("Default", 0);
	AddLayer("UI", INT_MAX - 1);

	m_pMainCameraObj = CreateCamera("MainCamera", Vector3(0.f, 0.f, -5.f), CT_PERSPECTIVE, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 45.f, 0.03f, 1000.f);	m_pMainCameraTr = m_pMainCameraObj->GetTransform();
	m_pMainCamera = m_pMainCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);
	m_pUICameraObj = CreateCamera("UICamera", Vector3(0.f, 0.f, 0.f), CT_ORTHO, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 60.f, 0.f, 1000.f);

	m_pUICameraTr = m_pUICameraObj->GetTransform();
	m_pUICamera = m_pUICameraObj->FindComponentFromType<CCamera>(CT_CAMERA);

	CLayer*	pLayer = FindLayer("Default");
	SAFE_RELEASE(pLayer);

	//m_pSkyObj = CGameObject::CreateObject("Sky");

	//m_pSkyObj->SetScene(this);

	//CTransform*	pSkyTr = m_pSkyObj->GetTransform();

	//pSkyTr->SetWorldScale(100000.f, 100000.f, 100000.f);
	//pSkyTr->Update(0.f);

	//SAFE_RELEASE(pSkyTr);

	//CRenderer*	pRenderer = m_pSkyObj->AddComponent<CRenderer>("SkyRenderer");

	//pRenderer->SetMesh("Sky");
	//pRenderer->SetRenderState(DEPTH_LESSEQUAL);
	//pRenderer->SetRenderState(CULL_NONE);

	//SAFE_RELEASE(pRenderer);

	//m_pSkyMtrl = m_pSkyObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	//m_pSkyMtrl->SetDiffuseTex(10, "SkyDefault", TEXT("Sky.dds"));
	//m_pSkyMtrl->SetSampler(10, SAMPLER_LINEAR);

	return true;
}

int CScene::Input(float fTime)
{
	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetActive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_SceneComponentList.erase(iter1);
			continue;
		}

		else if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Input(fTime);
		++iter1;
	}

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);
		++iter;
	}

	m_pMainCameraObj->Input(fTime);

	return 0;
}

int CScene::Update(float fTime)
{
	//CTransform*	pTr = m_pSkyObj->GetTransform();

	//pTr->RotationY(3.f, fTime);
	//pTr->Update(fTime);

	//SAFE_RELEASE(pTr);


	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetActive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_SceneComponentList.erase(iter1);
			continue;
		}

		else if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Update(fTime);
		++iter1;
	}

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);
		++iter;
	}

	m_pMainCameraObj->Update(fTime);
#ifdef _DEBUG
	Debug();
#endif // DEBUG

	return 0;
}

int CScene::LateUpdate(float fTime)
{
	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetActive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_SceneComponentList.erase(iter1);
			continue;
		}

		else if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->LateUpdate(fTime);
		++iter1;
	}

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);
		++iter;
	}

	m_pMainCameraObj->LateUpdate(fTime);

	return 0;
}

void CScene::Collision(float fTime)
{
	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetActive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_SceneComponentList.erase(iter1);
			continue;
		}

		else if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Collision(fTime);
		++iter1;
	}

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Collision(fTime);
		++iter;
	}

	// 마지막으로 마우스 충돌체를 넣어준다.
	GET_SINGLE(CInput)->AddMouseCollision();

	GET_SINGLE(CCollisionManager)->Collision(fTime);
}

void CScene::Render(float fTime)
{
	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetActive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_SceneComponentList.erase(iter1);
			continue;
		}

		else if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Render(fTime);
		++iter1;
	}

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);
		++iter;
	}
}

void CScene::AddLayer(const string & strTag, int iZOrder)
{
	CLayer*	pLayer = new CLayer;

	pLayer->m_pScene = this;

	pLayer->SetTag(strTag);

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return;
	}

	m_LayerList.push_back(pLayer);

	pLayer->SetZOrder(iZOrder);
}

void CScene::ChangeLayerZOrder(const string & strTag, int iZOrder)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->SetZOrder(iZOrder);
			return;
		}
	}
}

CLayer * CScene::FindLayer(const string & strTag)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}

void CScene::SortLayer()
{
	m_LayerList.sort(CScene::SortLayerZOrder);
}

void CScene::EnableLayer(const string & strTag, bool bEnable)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->SetEnable(bEnable);
			return;
		}
	}
}

CGameObject * CScene::FindObject(const string & strTag)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		CGameObject*	pObj = (*iter)->FindObject(strTag);

		if (pObj)
			return pObj;
	}

	return nullptr;
}
CGameObject* CScene::FindObjectNonCount(const string & strTag)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		CGameObject*	pObj = (*iter)->FindObjectNonCount(strTag);

		if (pObj)
			return pObj;
	}

	return nullptr;
}

CGameObject * CScene::CreateCamera(const string & strTag,
	const Vector3& vPos,
	CAMERA_TYPE eType, float fWidth, float fHeight, float fViewAngle,
	float fNear, float fFar)
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

void CScene::ChangeCamera(const string & strTag)
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

CGameObject * CScene::FindCamera(const string & strTag)
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapCamera.find(strTag);

	if (iter == m_mapCamera.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CScene::SortLayerZOrder(const CLayer* pSrc, const CLayer* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}

void CScene::EnableSceneComponent(const string & strTag, bool bEnable)
{
	list<CSceneComponent*>::iterator	iter1;
	list<CSceneComponent*>::iterator	iter1End = m_SceneComponentList.end();

	for (iter1 = m_SceneComponentList.begin(); iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->GetTag() == strTag)
		{
			(*iter1)->SetEnable(bEnable);
			return;
		}
	}
}

void CScene::Debug()
{
	ImGui::Text("GlobalLight");
	ImGui::BeginTabBar("AA");
	ImGui::EndTabBar();

	static int GlobalLightType = 0;

	CGameObject* getObject = FindObjectNonCount("GlobalLight");
	CLight* getLight = getObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	const char* Items[3] = { "Direction", "Point", "Spot" };
	ImGui::Text("LightType");
	ImGui::Combo("", &getLight->m_tInfo.iLightType, Items, 3);

	ImGui::Text("LightInfo");

	ImGui::SliderFloat4("Ambient", (float*)&getLight->m_tInfo.vAmb, -1.0f, 1.0f);
	ImGui::SliderFloat4("Diffuse", (float*)&getLight->m_tInfo.vDif, -1.0f, 1.0f);
	ImGui::SliderFloat3("Specular", (float*)&getLight->m_tInfo.vSpc, -1.0f, 1.0f);
	ImGui::SliderFloat3("Direction", (float*)&getLight->m_tInfo.vDir, -1.0f, 1.0f);
	ImGui::SliderFloat("Range", (float*)&getLight->m_tInfo.fRange, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", (float*)&getLight->m_tInfo.fFallOff, 0.0f, 20.0f);

	ImGui::Text("LightPos");

	ImGui::SliderFloat3("Pos", (float*)&getLight->m_tInfo.vPos, -1.0f, 20.0f);

	ImGui::BeginTabBar("BB");
	ImGui::EndTabBar();

	ImGui::Text("Camera");

	static Vector3 CameraPos;
	//ImGui::SliderFloat3("CameraPos", (float*)&CameraPos, -500.f, 50.0f);
	//m_pMainCameraTr->SetWorldPos(CameraPos);

	ImGui::BeginTabBar("Camera");
	ImGui::EndTabBar();
}
