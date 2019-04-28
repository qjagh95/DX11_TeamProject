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
	m_pSceneComponent = NULLPTR;
	m_LandObject = NULLPTR;

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

	SAFE_RELEASE(m_pSceneComponent);
}

CGameObject * CScene::GetMainCameraObj() const
{
	return CSceneManager::GetInst()->GetMainCameraObj();
}

CGameObject * CScene::GetMainCameraObjNonCount() const
{
	return CSceneManager::GetInst()->GetMainCameraObjNonCount();
}

CCamera * CScene::GetMainCamera() const
{
	return CSceneManager::GetInst()->GetMainCamera();
}

CCamera * CScene::GetMainCameraNonCount() const
{
	return CSceneManager::GetInst()->GetMainCameraNonCount();
}

CTransform * CScene::GetMainCameraTransform() const
{
	return CSceneManager::GetInst()->GetMainCameraTransform();
}

CTransform * CScene::GetMainCameraTransformNonCount() const
{
	return CSceneManager::GetInst()->GetMainCameraTransformNonCount();
}

CGameObject * CScene::GetUICameraObj() const
{
	return CSceneManager::GetInst()->GetUICameraObj();
}

CGameObject * CScene::GetUICameraObjNonCount() const
{
	return CSceneManager::GetInst()->GetUICameraObjNonCount();
}

CCamera * CScene::GetUICamera() const
{
	return CSceneManager::GetInst()->GetUICamera();
}

CCamera * CScene::GetUICameraNonCount() const
{
	return CSceneManager::GetInst()->GetUICameraNonCount();
}

CTransform * CScene::GetUICameraTransform() const
{
	return CSceneManager::GetInst()->GetUICameraTransform();
}

CTransform * CScene::GetUICameraTransformNonCount() const
{
	return CSceneManager::GetInst()->GetUICameraTransformNonCount();
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

	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pLightLayer);
	CTransform* pTr = pLightObj->GetTransform();

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	pLight->SetLightType(LT_DIR);

	pTr->RotationX(90.0f);

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

	m_pSceneComponent->Input(fTime);

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

	//CTransform*	pTr = m_pSkyObj->GetTransform();

	//pTr->RotationY(3.f, fTime);
	//pTr->Update(fTime);

	//SAFE_RELEASE(pTr);


	m_pSceneComponent->Update(fTime);

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

	m_pSceneComponent->LateUpdate(fTime);

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

	m_pSceneComponent->Collision(fTime);

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

	m_pSceneComponent->Render(fTime);

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

	if (CCore::GetInst()->m_bGuiMode == true)
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

CGameObject * CScene::CreateCamera(const string & strTag, const Vector3& vPos, CAMERA_TYPE eType, float fWidth, float fHeight, float fViewAngle, float fNear, float fFar)
{
	return CSceneManager::GetInst()->CreateCamera(strTag, vPos, eType, fWidth, fHeight, fViewAngle, fNear, fFar);
}

void CScene::ChangeCamera(const string & strTag)
{
	CSceneManager::GetInst()->ChangeCamera(strTag);
}

CGameObject * CScene::FindCamera(const string & strTag)
{
	return CSceneManager::GetInst()->FindCamera(strTag);
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
	m_pSceneComponent->SetEnable(bEnable);
}

void CScene::Save(string _fullPath)
{
	// Read(읽기)를 위해서 문자열 길이를 저장 후 그 길이만큼 읽어오는 방식으로
	// 저장하기 위해 문자열 길이를 먼저 저장한다.
	// instBW.WriteData((char*)&strLength, sizeof(CHAR_MAX));
	// instBW.WriteData(strLayerTag.c_str(), strLength);

	// 파일 객체(ofstream) 생성
	BinaryWrite instBW = BinaryWrite(_fullPath.c_str());

	// 레이어 목록
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		string strLayerTag = (*iter)->GetTag();
		instBW.WriteData(strLayerTag);

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
			AddLayer(strLayerTag, 0);
		}
		SAFE_RELEASE(pLayer);

		// Layer Load 함수 호출
		(*iter)->Load(&instBR);
	}
}

void CScene::SetPickingColliderEnable(bool _bEnable)
{
	std::list<CLayer*>::iterator Iter;
	std::list<CLayer*>::iterator EndIter = m_LayerList.end();

	for (Iter = m_LayerList.begin(); Iter != EndIter; ++Iter)
	{
		(*Iter)->SetPickingColliderEnable(_bEnable);
	}
}
