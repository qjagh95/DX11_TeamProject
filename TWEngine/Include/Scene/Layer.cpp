#include "EngineHeader.h"
#include "Layer.h"
#include "Core.h"

PUN_USING

CLayer::CLayer() :
	m_iZOrder(-1)
{
	m_vecInput = NULLPTR;
	m_vecUpdate = NULLPTR;
	m_vecLateUpdate = NULLPTR;
	m_vecCollsion = NULLPTR;
	m_vecRender = NULLPTR;
	m_bHeader = true;
}

CLayer::~CLayer()
{
	Safe_Release_VecList(m_ObjList);
}

int CLayer::GetZOrder() const
{
	return m_iZOrder;
}

CScene * CLayer::GetScene() const
{
	m_pScene->AddRef();
	return m_pScene;
}

CScene * CLayer::GetSceneNonCount() const
{
	return m_pScene;
}

void CLayer::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;

	m_pScene->SortLayer();
}

void CLayer::Save(BinaryWrite* _pInstBW)
{
	// 오브젝트 개수
	// (툴이 아닌 클라이언트에서 바로 로드될 경우 개수를 알 수 없기때문이다.)
	_pInstBW->WriteData((int)m_ObjList.size());

	// 오브젝트 목록
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		_pInstBW->WriteData((*iter)->GetTag().c_str());	// 태그
		_pInstBW->WriteData(this->GetTag().c_str());	// 오브젝트가 속해있는 레이어 태그
		bool isDontDestroy = (*iter)->GetDontDestroy();
		_pInstBW->WriteData(isDontDestroy);				// 오브젝트 삭제 여부
		_pInstBW->WriteData(GetEnable());				// (비)활성화 상태

		// Object Save 함수 호출
		(*iter)->Save(_pInstBW);
	}
}

void CLayer::Load(BinaryRead* _pInstBR)
{
	// 오브젝트 목록 개수
	int objListSize = _pInstBR->ReadInt();

	// 오브젝트 목록
	Safe_Release_VecList(m_ObjList);
	m_ObjList.clear();
	for (int i = 0; i < objListSize; ++i)
	{
		string strObjTag = _pInstBR->ReadString();
		string strLayerTag = _pInstBR->ReadString();
		bool isDontDestroy = _pInstBR->ReadBool();
		bool isEnable = _pInstBR->ReadBool();

		// 생성
		CGameObject* pObj = CGameObject::CreateObject(strObjTag, this, isDontDestroy);
		pObj->SetEnable(isEnable);

		// 오브젝트 Load 호출
		pObj->Load(_pInstBR);
		SAFE_RELEASE(pObj);
	}
}

void CLayer::Start()
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CLayer::Init()
{
	m_HeaderName = m_strTag + "Layer";

	CCore::GetInst()->AddManagerVector(m_HeaderName + " Input");
	CCore::GetInst()->AddManagerVector(m_HeaderName + " Update");
	CCore::GetInst()->AddManagerVector(m_HeaderName + " LateUpdate");
	CCore::GetInst()->AddManagerVector(m_HeaderName + " Collsion");
	CCore::GetInst()->AddManagerVector(m_HeaderName + " Render");

	m_vecInput = CCore::GetInst()->FindManagerMap(m_HeaderName + " Input");
	m_vecUpdate = CCore::GetInst()->FindManagerMap(m_HeaderName + " Update");
	m_vecLateUpdate = CCore::GetInst()->FindManagerMap(m_HeaderName + " LateUpdate");
	m_vecCollsion = CCore::GetInst()->FindManagerMap(m_HeaderName + " Collsion");
	m_vecRender = CCore::GetInst()->FindManagerMap(m_HeaderName + " Render");

	if (CCore::GetInst()->m_bGuiMode == false)
		m_bHeader = false;
	else
		m_bHeader = true;

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	m_LogText.Input.open(Path + m_HeaderName + " Input.txt");
	m_LogText.Update.open(Path + m_HeaderName + + " Update.txt");
	m_LogText.LateUpdate.open(Path + m_HeaderName + + " LateUpdate.txt");
	m_LogText.Collsion.open(Path + m_HeaderName + + " Collsion.txt");
	m_LogText.Render.open(Path + m_HeaderName + + " Render.txt");

	return true;
}

int CLayer::Input(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
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

	CCore::WriteLogText(m_LogText.Input, Compute);

	if (m_vecInput->size() >= 100)
		m_vecInput->erase(m_vecInput->begin());

	return 0;
}

int CLayer::Update(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
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

	CCore::WriteLogText(m_LogText.Update, Compute);

	if (m_vecUpdate->size() >= 100)
		m_vecUpdate->erase(m_vecUpdate->begin());

	return 0;
}

int CLayer::LateUpdate(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
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

	CCore::WriteLogText(m_LogText.LateUpdate, Compute);

	if (m_vecLateUpdate->size() >= 100)
		m_vecLateUpdate->erase(m_vecLateUpdate->begin());

	return 0;
}

void CLayer::Collision(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
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

	if (m_bHeader == false)
		return ;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecCollsion->push_back(Compute);

	CCore::WriteLogText(m_LogText.Collsion, Compute);

	if (m_vecCollsion->size() >= 100)
		m_vecCollsion->erase(m_vecCollsion->begin());
}

void CLayer::Render(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		//(*iter)->Render(fTime);
		GET_SINGLE(CRenderManager)->AddRenderObj(*iter);
		++iter;
	}

	if (CCore::GetInst()->m_bGuiMode == true)
		m_bHeader = ImGui::CollapsingHeader(m_HeaderName.c_str());

	if (m_bHeader == true)
	{
		Info.End = clock();
		float Compute = (float)(Info.End - Info.Start) * 0.01f;
		m_vecRender->push_back(Compute);

		CCore::WriteLogText(m_LogText.Render, Compute);

		if (m_vecRender->size() >= 100)
			m_vecRender->erase(m_vecRender->begin());

		ImGui::PlotLines("Input", &m_vecInput->at(0), (int)m_vecInput->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Update", &m_vecUpdate->at(0), (int)m_vecUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("LateUpdate", &m_vecLateUpdate->at(0), (int)m_vecLateUpdate->size(), 0, NULLPTR, 0.0f, 0.5f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Collsion", &m_vecCollsion->at(0), (int)m_vecCollsion->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Render", &m_vecRender->at(0), (int)m_vecRender->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
	}

}

void CLayer::AddObject(CGameObject * pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();
	pObj->SetObjectListIndex((int)m_ObjList.size());

	m_ObjList.push_back(pObj);
}

CGameObject * CLayer::FindObject(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}


CGameObject * CLayer::FindObjectNonCount(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			return *iter;
		}
	}

	return nullptr;
}

void CLayer::EraseObject(CGameObject* pObj)
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			iter = m_ObjList.erase(iter);
			SAFE_RELEASE(pObj);
			return;
		}
	}
}

void CLayer::EraseObjectAll()
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		CGameObject* pObj = *iter;
		iter = m_ObjList.erase(iter);
		SAFE_RELEASE(pObj);
	}
}

void CLayer::GetLayerListObjTag(std::vector<std::string>* _vec)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (strstr((*iter)->GetTag().c_str(), "Gizmo") == false)
		_vec->push_back((*iter)->GetTag());
	}
}

CGameObject * CLayer::FindObjectFromIndex(int _idx)
{
	if (_idx >= m_ObjList.size())
	{
		return nullptr;
	}

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetObjectListIndex() == _idx)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;


}
