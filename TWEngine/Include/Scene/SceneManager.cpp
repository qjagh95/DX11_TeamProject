#include "EngineHeader.h"
#include "SceneManager.h"
#include "Scene.h"

PUN_USING

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_pScene(nullptr),
	m_pNextScene(nullptr),
	m_bChange(false)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_RELEASE(m_pNextScene);
	SAFE_RELEASE(m_pScene);
}

CScene * CSceneManager::GetScene() const
{
	m_pScene->AddRef();
	return m_pScene;
}

CScene * CSceneManager::GetSceneNonCount() const
{
	return m_pScene;
}

CScene * CSceneManager::GetNextScene() const
{
	m_pNextScene->AddRef();
	return m_pNextScene;
}
CScene * CSceneManager::GetNextSceneNonCount() const
{
	return m_pNextScene;
}

bool CSceneManager::Init()
{
	m_pScene = new CScene;

	if (!m_pScene->Init())
		return false;

	GET_SINGLE(CInput)->ChangeMouseScene(m_pScene);

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pScene->Input(fTime);

	return ChangeScene();
}

int CSceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	return ChangeScene();
}

int CSceneManager::LateUpdate(float fTime)
{
	m_pScene->LateUpdate(fTime);

	return ChangeScene();
}

int CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);

	return ChangeScene();
}

void CSceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);
}

int CSceneManager::ChangeScene()
{
	if (m_pNextScene && m_bChange)
	{
		m_bChange = false;
		SAFE_RELEASE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;

		GET_SINGLE(CInput)->ChangeMouseScene(m_pScene);

		m_pScene->Start();

		return 1;
	}

	return 0;
}

void CSceneManager::AddLayer(const string & strTag, int iZOrder, bool bCurrent)
{
	if (bCurrent)
		m_pScene->AddLayer(strTag, iZOrder);

	else
		m_pNextScene->AddLayer(strTag, iZOrder);
}

void CSceneManager::ChangeLayerZOrder(const string & strTag, int iZOrder, bool bCurrent)
{
	if (bCurrent)
		m_pScene->ChangeLayerZOrder(strTag, iZOrder);

	else
		m_pNextScene->ChangeLayerZOrder(strTag, iZOrder);
}

CLayer * CSceneManager::FindLayer(const string & strTag, bool bCurrent)
{
	if (bCurrent)
		return m_pScene->FindLayer(strTag);

	return m_pNextScene->FindLayer(strTag);
}

CGameObject * CSceneManager::FindObject(const string & strTag)
{
	CGameObject* pObj = m_pScene->FindObject(strTag);

	if (pObj)
		return pObj;

	if (!m_pNextScene)
		return nullptr;

	return m_pNextScene->FindObject(strTag);
}

void CSceneManager::CreateNextScene(bool bChange , const std::string& _strTag)
{
	SAFE_RELEASE(m_pNextScene);
	m_pNextScene = new CScene;
	m_pNextScene->SetTag(_strTag);
	m_pNextScene->Init();

	GET_SINGLE(CObjectManager)->ChangeSceneFromDontDestroyObj(m_pNextScene);

	m_bChange = bChange;
}

void CSceneManager::ChangeEnable()
{
	m_bChange = true;
}
