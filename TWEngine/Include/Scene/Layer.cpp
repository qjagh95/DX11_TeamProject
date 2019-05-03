#include "EngineHeader.h"
#include "Layer.h"
#include "Core.h"
#include "EditManager.h"

PUN_USING

CLayer::CLayer() :
	m_iZOrder(-1)
{
	m_vecInput = NULLPTR;
	m_vecUpdate = NULLPTR;
	m_vecLateUpdate = NULLPTR;
	m_vecCollsion = NULLPTR;
	m_vecRender = NULLPTR;
	m_LogText = NULLPTR;
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
	m_AllChildList.clear();

	// 오브젝트 개수
	// (툴이 아닌 클라이언트에서 바로 로드될 경우 개수를 알 수 없다.)
	int iObjListSize = 0;
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetSave() == true)
			++iObjListSize;
	}
	_pInstBW->WriteData(iObjListSize);
	if (iObjListSize == 0)
		return;

	// 오브젝트 목록
	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetSave() == true)
		{
			string strTag = (*iter)->GetTag();
			string strLayerTag = this->GetTag();
			bool isDontDestroy = (*iter)->GetDontDestroy();
			bool isEnable = (*iter)->GetEnable();

			_pInstBW->WriteData(strTag);		// 태그
			_pInstBW->WriteData(strLayerTag);   // 오브젝트가 속해있는 레이어 태그
			_pInstBW->WriteData(isDontDestroy); // 오브젝트 삭제 여부
			_pInstBW->WriteData(isEnable);      // 활성화 여부

			// Object Save 함수 호출
			(*iter)->Save(_pInstBW);
		}
	}
	_pInstBW->WriteData((int)m_AllChildList.size());
	for (size_t i = 0; i < m_AllChildList.size(); i++)
	{
		_pInstBW->WriteData(m_AllChildList[i]->GetParent()->GetTag());
		_pInstBW->WriteData(m_AllChildList[i]->GetTag());
	}
}

void CLayer::Load(BinaryRead* _pInstBR)
{
	m_AllChildList.clear();

	// 오브젝트 목록
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if ((*iter)->GetSave())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			continue;
		}
		++iter;
	}

	// 오브젝트 목록 개수
	int objListSize = _pInstBR->ReadInt();
	if (objListSize == 0)
	{
		return;
	}

	if (CCore::GetInst()->m_bEditorMode == true && m_strTag == "Default")
	{
		CEditManager::GetInst()->PrivateEditObjSettingLayer();
	}

	for (int i = 0; i < objListSize; ++i)
	{
		CGameObject* newObject	= NULLPTR;
		string strObjTag		= _pInstBR->ReadString();
		string strLayerTag		= _pInstBR->ReadString();
		bool isDontDestroy		= _pInstBR->ReadBool();
		bool isEnable			= _pInstBR->ReadBool();

		// 생성
		newObject = CGameObject::CreateObject(strObjTag, this, isDontDestroy);
		newObject->SetEnable(isEnable);
		
		// 오브젝트 Load 호출
		newObject->Load(_pInstBR);

		m_AllChildList.push_back(newObject);
		SAFE_RELEASE(newObject);
	}

	int AllSize = _pInstBR->ReadInt();
	for (size_t i = 0; i < AllSize; i++)
	{
		string ParentTag = _pInstBR->ReadString();
		string ChildTag  = _pInstBR->ReadString();
		CGameObject* ParentObject = FindObjectLoadVersion(ParentTag);
		CGameObject* ChildObject = FindObjectLoadVersion(ChildTag);

		//EraseObjectNoRelease(ChildObject);
		ParentObject->AddChild(ChildObject, true);
	}
	m_AllChildList.clear();
}

void CLayer::Start()
{
	list<CGameObject*>::iterator   iter;
	list<CGameObject*>::iterator   iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CLayer::Init()
{
	m_HeaderName = m_strTag + "Layer";

	m_vecInput = CCore::GetInst()->AddManagerVector(m_HeaderName + " Input"); 
	m_vecUpdate = CCore::GetInst()->AddManagerVector(m_HeaderName + " Update");
	m_vecLateUpdate = CCore::GetInst()->AddManagerVector(m_HeaderName + " LateUpdate");
	m_vecCollsion = CCore::GetInst()->AddManagerVector(m_HeaderName + " Collsion");
	m_vecRender = CCore::GetInst()->AddManagerVector(m_HeaderName + " Render");

	if (CCore::GetInst()->m_bGuiMode == false)
		m_bHeader = false;
	else
		m_bHeader = true;

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	m_LogText = CCore::GetInst()->CreateFileStream(Path, m_HeaderName, m_HeaderName);

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

	CCore::WriteLogText(m_LogText->Input, Compute);

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

	CCore::WriteLogText(m_LogText->Update, Compute);

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

	CCore::WriteLogText(m_LogText->LateUpdate, Compute);

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

	CCore::WriteLogText(m_LogText->Collsion, Compute);

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

		CCore::WriteLogText(m_LogText->Render, Compute);

		if (m_vecRender->size() >= 100)
			m_vecRender->erase(m_vecRender->begin());

		ImGui::PlotLines("Input", &m_vecInput->at(0), (int)m_vecInput->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Update", &m_vecUpdate->at(0), (int)m_vecUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("LateUpdate", &m_vecLateUpdate->at(0), (int)m_vecLateUpdate->size(), 0, NULLPTR, 0.0f, 0.5f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Collsion", &m_vecCollsion->at(0), (int)m_vecCollsion->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
		ImGui::PlotLines("Render", &m_vecRender->at(0), (int)m_vecRender->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
	}

}

void CLayer::SetPickingColliderEnable(bool _bEnable)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_ObjList.end();

	for (Iter = m_ObjList.begin(); Iter != EndIter; ++Iter)
	{
		(*Iter)->SetPickingColliderEnable(_bEnable);
	}
}

void CLayer::SetColliderEnable(bool _bEnable)
{
	std::list<CGameObject*>::iterator Iter;
	std::list<CGameObject*>::iterator EndIter = m_ObjList.end();

	for (Iter = m_ObjList.begin(); Iter != EndIter; ++Iter)
	{
		if (strstr((*Iter)->GetTag().c_str() , "Gizmo") == false && strstr((*Iter)->GetTag().c_str() , "Mouse") == false)
		{
			(*Iter)->SetColliderEnable(_bEnable);
		}
	}
}

void CLayer::AddObject(CGameObject * pObj, bool _isChild)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->SetObjectListIndex((int)m_ObjList.size());

	if (_isChild)
		return;

	pObj->AddRef();
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

CGameObject * CLayer::FindObjectLoadVersion(const string & strTag)
{
	vector<CGameObject*>::iterator	iter;
	vector<CGameObject*>::iterator	iterEnd = m_AllChildList.end();

	for (iter = m_AllChildList.begin(); iter != iterEnd; ++iter)
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
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CLayer::EraseObjectNoRelease(CGameObject * pObj)
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			iter = m_ObjList.erase(iter);
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

list<CGameObject*>* CLayer::GetObjectList() 
{
	return &m_ObjList;
}
