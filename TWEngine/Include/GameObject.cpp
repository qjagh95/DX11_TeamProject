#include "EngineHeader.h"
#include "GameObject.h"
#include "Scene/Layer.h"
#include "Component/Transform.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "CollisionManager.h"
#include "ObjectManager.h"
#include "Component/Animation.h"

PUN_USING

unordered_map<class CScene*, unordered_map<string, CGameObject*>> CGameObject::m_mapPrototype;

CGameObject::CGameObject() :
	m_pTransform(nullptr),
	m_pParent(nullptr)
{
	SetTag("GameObject");
	m_eRenderGroup = RG_NORMAL;
}

CGameObject::CGameObject(const CGameObject & obj)
{
	*this = obj;

	m_iRefCount = 1;

	m_FindList.clear();

	m_pTransform = obj.m_pTransform->Clone();
	m_pTransform->m_pTransform = m_pTransform;

	m_ComList.clear();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = obj.m_ComList.end();

	for (iter = obj.m_ComList.begin(); iter != iterEnd; ++iter)
	{
		CComponent*	pCom = (*iter)->Clone();

		pCom->m_pObject = this;
		pCom->m_pTransform = m_pTransform;

		m_ComList.push_back(pCom);
	}

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		pRenderer->CheckComponent();

		CAnimation*	pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

		if (pAnimation)
		{
			pRenderer->SetBoneTexture(pAnimation->GetBoneTexture());

			SAFE_RELEASE(pAnimation);
		}

		SAFE_RELEASE(pRenderer);
	}

	m_ChildList.clear();

	list<CGameObject*>::const_iterator	iter1;
	list<CGameObject*>::const_iterator	iter1End = obj.m_ChildList.end();

	m_pTransform->m_ChildList.clear();

	for (iter1 = obj.m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		CGameObject*	pChild = (*iter1)->Clone();

		pChild->m_pParent = this;
		pChild->m_pTransform->m_pParent = m_pTransform;
		pChild->m_pTransform->SetParentFlag(TPF_ROT | TPF_POS);
		pChild->m_pTransform->AddRef();
		m_pTransform->m_ChildList.push_back(pChild->m_pTransform);

		m_ChildList.push_back(pChild);
	}
}

CGameObject::~CGameObject()
{
	Safe_Release_VecList(m_ChildList);
	Safe_Release_VecList(m_FindList);
	SAFE_RELEASE(m_pTransform);
	Safe_Release_VecList(m_ComList);
}

CGameObject * CGameObject::CreateObject(const string & strTag, CLayer * pLayer,
	bool bDontDestroy)
{
	CGameObject*	pObj = GET_SINGLE(CObjectManager)->FindDontDestroyObj(strTag);

	if (pObj)
		return pObj;

	pObj = new CGameObject;

	pObj->SetTag(strTag);

	if (bDontDestroy)
		pObj->DontDestroyOnLoad();

	if (!pObj->Init())
	{
		SAFE_RELEASE(pObj);
		return nullptr;
	}

	if (pLayer)
	{
		if (!bDontDestroy)
			pLayer->AddObject(pObj);

		else
		{
			class CScene* pScene = pLayer->GetScene();
			pObj->SetScene(pScene);
			pObj->SetLayer(pLayer);
			SAFE_RELEASE(pScene);
		}
	}

	return pObj;
}

CGameObject * CGameObject::CreatePrototype(const string & strTag,
	bool bCurrent)
{
	CScene*	pScene = nullptr;

	if (bCurrent)
		pScene = GET_SINGLE(CSceneManager)->GetScene();

	else
		pScene = GET_SINGLE(CSceneManager)->GetNextScene();

	CGameObject*	pPrototype = FindPrototype(pScene, strTag);

	if (pPrototype)
		return nullptr;

	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter =
		m_mapPrototype.find(pScene);

	unordered_map<string, CGameObject*>*	pMap = nullptr;

	if (iter == m_mapPrototype.end())
	{
		unordered_map<string, CGameObject*>	map;
		m_mapPrototype.insert(make_pair(pScene, map));

		iter = m_mapPrototype.find(pScene);
	}

	pMap = &iter->second;

	SAFE_RELEASE(pScene);

	pPrototype = new CGameObject;

	pPrototype->SetTag(strTag);

	if (!pPrototype->Init())
	{
		SAFE_RELEASE(pPrototype);
		return nullptr;
	}

	pPrototype->AddRef();
	pMap->insert(make_pair(strTag, pPrototype));

	return pPrototype;
}

CGameObject * CGameObject::CreateClone(const string & strTag,
	const string & strProto, CLayer* pLayer, bool bCurrent)
{
	class CScene*	pScene = nullptr;

	if (bCurrent)
		pScene = GET_SINGLE(CSceneManager)->GetScene();

	else
		pScene = GET_SINGLE(CSceneManager)->GetNextScene();

	CGameObject*	pProto = FindPrototype(pScene, strProto);

	SAFE_RELEASE(pScene);

	if (!pProto)
		return nullptr;

	CGameObject*	pObj = pProto->Clone();

	pObj->SetTag(strTag);

	pObj->AfterClone();

	if (pLayer)
		pLayer->AddObject(pObj);

	return pObj;
}

CGameObject * CGameObject::FindObject(const string & strTag)
{
	return GET_SINGLE(CSceneManager)->FindObject(strTag);
}

void CGameObject::DestroyPrototype(CScene * pScene)
{
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter =
		m_mapPrototype.find(pScene);

	if (iter == m_mapPrototype.end())
		return;

	Safe_Release_Map(iter->second);

	m_mapPrototype.erase(iter);
}

void CGameObject::DestroyPrototype(CScene * pScene, const string & strName)
{
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter =
		m_mapPrototype.find(pScene);

	if (iter == m_mapPrototype.end())
		return;

	unordered_map<string, CGameObject*>::iterator	iter1 =
		iter->second.find(strName);

	if (iter1 == iter->second.end())
		return;

	SAFE_RELEASE(iter1->second);

	iter->second.erase(iter1);
}

void CGameObject::DestroyPrototype()
{
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter;
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iterEnd = m_mapPrototype.end();

	for (iter = m_mapPrototype.begin(); iter != iterEnd; ++iter)
	{
		Safe_Release_Map(iter->second);
	}

	m_mapPrototype.clear();
}

CGameObject * CGameObject::FindPrototype(CScene * pScene,
	const string & strName)
{
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter =
		m_mapPrototype.find(pScene);

	if (iter == m_mapPrototype.end())
		return nullptr;

	unordered_map<string, CGameObject*>::iterator	iter1 = iter->second.find(strName);

	if (iter1 == iter->second.end())
		return nullptr;

	return iter1->second;
}

CScene * CGameObject::GetScene() const
{
	return m_pScene;
}

CLayer * CGameObject::GetLayer() const
{
	return m_pLayer;
}

string CGameObject::GetLayerName() const
{
	return m_strLayerName;
}

int CGameObject::GetLayerZOrder() const
{
	return m_iLayerZOrder;
}

void CGameObject::SetScene(CScene * pScene)
{
	m_pScene = pScene;

	m_pTransform->m_pScene = pScene;

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->m_pScene = pScene;
	}
}

void CGameObject::SetLayer(CLayer * pLayer)
{
	m_pLayer = pLayer;

	m_strLayerName = pLayer->GetTag();
	m_iLayerZOrder = pLayer->GetZOrder();

	m_pTransform->m_pLayer = pLayer;

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->m_pLayer = pLayer;
	}
}

void CGameObject::AfterClone()
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->AfterClone();
	}
}

void CGameObject::DontDestroyOnLoad()
{
	GET_SINGLE(CObjectManager)->AddDontDestroyObj(this);
}

CTransform * CGameObject::GetTransform() const
{
	m_pTransform->AddRef();
	return m_pTransform;
}

CTransform * CGameObject::GetTransformNonCount() const
{
	return m_pTransform;
}


const list<class CComponent*>* CGameObject::GetComponentList() const
{
	return &m_ComList;
}

RENDER_GROUP CGameObject::GetRenderGroup() const
{
	return m_eRenderGroup;
}

bool CGameObject::EmptyComponent() const
{
	return m_ComList.empty();
}

void CGameObject::Start()
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CGameObject::Init()
{
	m_pTransform = new CTransform;

	m_pTransform->Init();
	m_pTransform->m_pTransform = m_pTransform;

	return true;
}

int CGameObject::Input(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRenderer)
			{
				pRenderer->DeleteComponentCBuffer(*iter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*iter));
			iter = m_ComList.erase(iter);
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

	return 0;
}

int CGameObject::Update(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRenderer)
			{
				pRenderer->DeleteComponentCBuffer(*iter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*iter));
			iter = m_ComList.erase(iter);
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

	m_pTransform->Update(fTime);

	if (m_ChildList.empty() == true)
		return 0;

	//자식이 있다면 자식트랜스폼에 부모의 정보가 저장된 행렬과 자식의 Transform정보를 곱하여
	//자식의 Parent행렬에 넣어준다.

	list<CGameObject*>::iterator StartIter1 = m_ChildList.begin();
	list<CGameObject*>::iterator EndIter1 = m_ChildList.end();

	Matrix S = m_pTransform->GetWorldScaleMatrix() * m_pTransform->GetParentScale();
	Matrix R = m_pTransform->GetWorldRotMatrix() * m_pTransform->GetParentRot();
	Matrix T = m_pTransform->GetWorldPosMatrix() * m_pTransform->GetParentPos();

	for (; StartIter1 != EndIter1; StartIter1++)
	{
		(*StartIter1)->GetTransform()->SetParentScale(S);
		(*StartIter1)->GetTransform()->SetParentRot(R);
		(*StartIter1)->GetTransform()->SetParentPos(T);
		(*StartIter1)->GetTransform()->SetUpdate(true);
	}

	return 0;
}

int CGameObject::LateUpdate(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRenderer)
			{
				pRenderer->DeleteComponentCBuffer(*iter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*iter));
			iter = m_ComList.erase(iter);
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

	m_pTransform->LateUpdate(fTime);

	if (m_ChildList.empty() == true)
		return 0;

	//자식이 있다면 자식트랜스폼에 부모의 정보가 저장된 행렬과 자식의 Transform정보를 곱하여
	//자식의 Parent행렬에 넣어준다.

	list<CGameObject*>::iterator StartIter1 = m_ChildList.begin();
	list<CGameObject*>::iterator EndIter1 = m_ChildList.end();

	Matrix S = m_pTransform->GetWorldScaleMatrix() * m_pTransform->GetParentScale();
	Matrix R = m_pTransform->GetWorldRotMatrix() * m_pTransform->GetParentRot();
	Matrix T = m_pTransform->GetWorldPosMatrix() * m_pTransform->GetParentPos();

	for (; StartIter1 != EndIter1; StartIter1++)
	{
		(*StartIter1)->GetTransform()->SetParentScale(S);
		(*StartIter1)->GetTransform()->SetParentRot(R);
		(*StartIter1)->GetTransform()->SetParentPos(T);
		(*StartIter1)->GetTransform()->SetUpdate(true);
	}

	return 0;
}

void CGameObject::Collision(float fTime)
{
	// 충돌관리자에 오브젝트를 등록한다.
	GET_SINGLE(CCollisionManager)->AddCollision(this);

}

void CGameObject::Render(float fTime)
{
	m_pTransform->Render(fTime);

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		if (!pRenderer->GetActive())
		{
			pRenderer->Release();
			m_ComList.remove(pRenderer);
		}

		else if (pRenderer->GetEnable())
		{
			pRenderer->Render(fTime);
		}
		SAFE_RELEASE(pRenderer);
	}

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetComponentType() == CT_RENDERER)
		{
			++iter;
			continue;
		}

		else if (!(*iter)->GetActive())
		{
			CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRenderer)
			{
				pRenderer->DeleteComponentCBuffer(*iter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*iter));
			iter = m_ComList.erase(iter);
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

CGameObject * CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::AddChild(CGameObject * pChild)
{
	pChild->m_pParent = this;
	pChild->m_pTransform->m_pParent = m_pTransform;
	pChild->m_pTransform->AddRef();
	m_pTransform->m_ChildList.push_back(pChild->m_pTransform);
	pChild->m_pTransform->SetParentFlag(TPF_ROT | TPF_POS);
	pChild->AddRef();

	m_ChildList.push_back(pChild);

	m_pLayer->AddObject(pChild);
}


CComponent* CGameObject::AddComponent(CComponent* pCom)
{
	pCom->m_pScene = m_pScene;
	pCom->m_pLayer = m_pLayer;
	pCom->m_pTransform = m_pTransform;
	pCom->m_pObject = this;
	pCom->AddRef();

	if (pCom->GetComponentType() == CT_RENDERER)
	{
		CAnimation*	pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

		if (pAnimation)
		{
			((CRenderer*)pCom)->SetBoneTexture(pAnimation->GetBoneTexture());

			SAFE_RELEASE(pAnimation);
		}
	}

	else if (pCom->GetComponentType() == CT_ANIMATION)
	{
		CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer)
		{
			pRenderer->SetBoneTexture(((CAnimation*)pCom)->GetBoneTexture());

			SAFE_RELEASE(pRenderer);
		}
	}

	m_ComList.push_back(pCom);

	if (pCom->GetComponentType() == CT_UI)
		m_eRenderGroup = RG_UI;
	else if (pCom->GetComponentType() == CT_STAGE2D)
		m_eRenderGroup = RG_LANDSCAPE;
	else if (pCom->GetComponentType() == CT_LIGHT)
		m_eRenderGroup = RG_LIGHT;

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		pRenderer->CheckComponent();

		SAFE_RELEASE(pRenderer);
	}

	return pCom;
}

const list<CComponent*>* CGameObject::FindComponentsFromTag(const string & strTag)
{
	Safe_Release_VecList(m_FindList);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	return &m_FindList;
}

const list<CComponent*>* CGameObject::FindComponentsFromTagNonCount(const string & strTag)
{
	Safe_Release_VecList(m_FindList);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			m_FindList.push_back(*iter);
		}
	}

	return &m_FindList;
}

const list<CComponent*>* CGameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	Safe_Release_VecList(m_FindList);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	return &m_FindList;
}

const list<CComponent*>* CGameObject::FindComponentsFromTypeNonCount(COMPONENT_TYPE eType)
{
	Safe_Release_VecList(m_FindList);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			m_FindList.push_back(*iter);
		}
	}

	return &m_FindList;
}


bool CGameObject::CheckComponentFromType(COMPONENT_TYPE eType)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
			return true;
	}

	return false;
}

void CGameObject::RemoveComponentFromType(COMPONENT_TYPE eType)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			SAFE_RELEASE((*iter));
			m_ComList.erase(iter);
			return;
		}
	}
}

void CGameObject::Save(FILE * pFile)
{
}

void CGameObject::Load(FILE * pFile)
{
}

