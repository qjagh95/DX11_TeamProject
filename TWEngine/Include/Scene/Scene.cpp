#include "EngineHeader.h"
#include "../Core.h"
#include "../Device.h"
#include "Layer.h"
#include "SceneComponent.h"
#include "../BinaryWriter.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"

PUN_USING

CScene::CScene()
{
	m_pSkyObj = nullptr;
	m_pSkyMtrl = nullptr;
	m_LogText = NULLPTR;

	m_bHeader = false;
	m_vecInput = NULLPTR;
	m_vecUpdate = NULLPTR;
	m_vecLateUpdate = NULLPTR;
	m_vecCollsion = NULLPTR;
	m_vecRender = NULLPTR;
}

CScene::~CScene()
{
	SAFE_RELEASE(m_pSkyObj);
	SAFE_RELEASE(m_pSkyMtrl);

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

CGameObject * CScene::GetSkyObject() const
{
	m_pSkyObj->AddRef();
	return m_pSkyObj;
}

CGameObject * CScene::GetSkyObjectNonCount() const
{
	return m_pSkyObj;
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

	GET_SINGLE(CRenderManager)->SetSkyObject(m_pSkyObj);
}

bool CScene::Init()
{
	AddLayer("Stage", INT_MIN + 1);
	AddLayer("Default", 0);
	AddLayer("Light", 50);
	AddLayer("UI", INT_MAX - 1);

	ProfileInit();

	CLayer* pLightLayer = FindLayer("Light");
	CLayer*	pLayer = FindLayer("Default");

	m_pMainCameraObj = CreateCamera("MainCamera", Vector3(0.f, 0.f, -5.f), CT_PERSPECTIVE, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 90.f, 0.03f, 1000.f);;
	m_pMainCameraTr = m_pMainCameraObj->GetTransform();
	m_pMainCamera = m_pMainCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);
	m_pMainCamera->Shadow(true);
	m_pMainCamera->SetLightLayer(pLightLayer);

	m_pUICameraObj = CreateCamera("UICamera", Vector3(0.f, 0.f, 0.f), CT_ORTHO, (float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 60.f, 0.f, 1000.f);
	m_pUICameraTr = m_pUICameraObj->GetTransform();
	m_pUICamera = m_pUICameraObj->FindComponentFromType<CCamera>(CT_CAMERA);


	m_pSkyObj = CGameObject::CreateObject("Sky");

	m_pSkyObj->SetScene(this);

	CTransform*	pSkyTr = m_pSkyObj->GetTransform();

	pSkyTr->SetWorldScale(100000.f, 100000.f, 100000.f);
	pSkyTr->Update(0.f);

	SAFE_RELEASE(pSkyTr);

	CRenderer*	pRenderer = m_pSkyObj->AddComponent<CRenderer>("SkyRenderer");

	pRenderer->SetMesh("Sky");
	pRenderer->SetRenderState(DEPTH_LESSEQUAL);
	//pRenderer->SetRenderState(DEPTH_DISABLE);
	
	pRenderer->SetRenderState(CULL_NONE);

	SAFE_RELEASE(pRenderer);

	m_pSkyMtrl = m_pSkyObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	m_pSkyMtrl->SetDiffuseTex(10, "SkyDefault", TEXT("Sky.dds"));
	m_pSkyMtrl->SetSampler(10, SAMPLER_LINEAR);

	if (CCore::GetInst()->m_bGuiMode == false)
		m_bHeader = false;
	else
		m_bHeader = true;

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	m_LogText = CCore::GetInst()->CreateFileStream(Path, "Scene", "Scene");

	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pLightLayer, true);
	CTransform* pTr = pLightObj->GetTransform();

	pTr->RotationX(45.0f);

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	pLight->SetLightType(LT_DIR);
	
	Vector3 vDir = pTr->GetWorldAxis(AXIS_Z);

	pLight->SetLightDirection(vDir);
	
	m_pMainCamera->SetShadowLight(pLight->GetTransformNonCount());
	m_pMainCamera->Shadow(true);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pLightLayer);

	return true;
}

int CScene::Input(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

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

	if (m_bHeader == false)
		return 0;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecInput->push_back(Compute);

	CCore::WriteLogText(m_LogText->Input, Compute);

	if (m_vecInput->size() >= 100)
		m_vecInput->erase(m_vecInput->begin());

	return 0;
}

int CScene::Update(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	CTransform*	pTr = m_pSkyObj->GetTransform();

	pTr->RotationY(3.f, fTime);
	pTr->Update(fTime);

	SAFE_RELEASE(pTr);

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

	if (m_bHeader == false)
		return 0;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecUpdate->push_back(Compute);

	CCore::WriteLogText(m_LogText->Update, Compute);

	if (m_vecUpdate->size() >= 100)
		m_vecUpdate->erase(m_vecUpdate->begin());

	return 0;
}

int CScene::LateUpdate(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

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
	
	if (m_bHeader == false)
		return 0;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecLateUpdate->push_back(Compute);

	CCore::WriteLogText(m_LogText->LateUpdate, Compute);

	if (m_vecLateUpdate->size() >= 100)
		m_vecLateUpdate->erase(m_vecLateUpdate->begin());

	return 0;
}

void CScene::Collision(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

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

	if (m_bHeader == false)
		return;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecCollsion->push_back(Compute);

	CCore::WriteLogText(m_LogText->Collsion, Compute);

	if (m_vecCollsion->size() >= 100)
		m_vecCollsion->erase(m_vecCollsion->begin());
}

void CScene::Render(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

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

	if(CCore::GetInst()->m_bGuiMode == true)
		m_bHeader = ImGui::CollapsingHeader("Scene");

	if (m_bHeader == true)
	{
		Info.End = clock();
		float Compute = (float)(Info.End - Info.Start) * 0.01f;
		m_vecRender->push_back(Compute);

		CCore::WriteLogText(m_LogText->Render, Compute);

		if (m_vecRender->size() >= 100)
			m_vecRender->erase(m_vecRender->begin());

		ImGui::PlotLines("Input", &m_vecInput->at(0), (int)m_vecInput->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Update", &m_vecUpdate->at(0), (int)m_vecUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("LateUpdate", &m_vecLateUpdate->at(0), (int)m_vecLateUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Collsion", &m_vecCollsion->at(0), (int)m_vecCollsion->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Render", &m_vecRender->at(0), (int)m_vecRender->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
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

void CScene::GetLayerTagList(vector<string>* _pVec)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		_pVec->push_back((*iter)->GetTag());
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

void CScene::ProfileInit()
{
	m_vecInput = CCore::GetInst()->AddManagerVector("SceneInput"); 
	m_vecUpdate = CCore::GetInst()->AddManagerVector("SceneUpdate");
	m_vecLateUpdate = CCore::GetInst()->AddManagerVector("SceneLateUpdate"); 
	m_vecCollsion = CCore::GetInst()->AddManagerVector("SceneCollsion"); 
	m_vecRender = CCore::GetInst()->AddManagerVector("SceneRender");
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

void CScene::Save(string _fullPath)
{
	// 파일 객체(ofstream) 생성
	BinaryWrite instBW = BinaryWrite(_fullPath.c_str());

	// 레이어 목록
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		/*
		Read(읽기)를 위해서 문자열 길이를 저장 후 그 길이만큼 읽어오는 방식으로
		저장하기 위해 문자열 길이를 먼저 저장한다.
		instBW.WriteData((char*)&strLength, sizeof(CHAR_MAX));
		instBW.WriteData(strLayerTag.c_str(), strLength);
		*/

		// BinaryWrite 클래스에선 위 작업을 함수화 시켜놨다.
		instBW.WriteData((*iter)->GetTag().c_str());

		// Layer Save 함수 호출
		(*iter)->Save(&instBW);
	}
}

void CScene::Load(string _fullPath)
{
	// 파일 객체(ifstream) 생성
	BinaryRead instBR = BinaryRead(_fullPath.c_str());

	// 레이어 목록
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		string strLayerTag = instBR.ReadString();

		// 레이어를 찾아서 m_LayerList에 없다면 추가한다.
		CLayer* pLayer = FindLayer(strLayerTag);
		if (pLayer == nullptr)
		{
			AddLayer(strLayerTag, 0); // Default ZOrder 0
		}
		SAFE_RELEASE(pLayer);
		
		// Layer Load 함수 호출
		(*iter)->Load(&instBR);
	}
}