#include "../ClientHeader.h"
#include "DocxInven.h"
#include "Resource/ResourcesManager.h"
#include "Document.h"
#include "Inventory.h"
#include "PhoneMessage.h"

CDocxInven::CDocxInven() :
	m_iIndex(0),
	m_iMoveIndex(0)
{
	m_eComType = (COMPONENT_TYPE)UT_DOCXINVEN;
	SetTag("DocxInven");

	m_iFlag = -1;
	m_bMouseOn = false;
	m_bOnDocx = false;
	m_bVisible = false;
	m_eState = DS_NORMAL;
	m_fItemY = 0.f;
}

CDocxInven::CDocxInven(const CDocxInven & docx)	:
	CUserComponent(docx)
{
}

CDocxInven::~CDocxInven()
{
	SAFE_RELEASE(m_pDoc);
	SAFE_RELEASE(m_pDocObj);
	SAFE_RELEASE(m_pPM);
	SAFE_RELEASE(m_pPMObj);

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecItem[i]);
	}

	Safe_Release_VecList(m_vecItem);
}

void CDocxInven::SetDocxMax(int iMax)
{
	m_iDocxMax = iMax;
}

void CDocxInven::SetVisible(bool bVisible)
{
	m_pObject->SetEnable(bVisible);	
}

void CDocxInven::SetVisible()
{
	if (m_iFlag == -1)
	{
		CSoundManager::GetInst()->SoundPlay("Docx_Open");
		PUN::CInput::GetInst()->ShowMouse(true);
		m_bVisible = true;
		m_pObject->SetEnable(true);

		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (m_vecItem[i] == nullptr)
				break;

			m_vecItem[i]->SetEnable(true);
		}
	}
	else if (m_iFlag == 1)
	{
		m_bVisible = false;
		m_pObject->SetEnable(false);
		m_pDoc->ChangeClip("Message_Empty");
		m_pDocObj->SetEnable(false);
		PUN::CInput::GetInst()->ShowMouse(false);

		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (m_vecItem[i] == nullptr)
				break;

			m_vecItem[i]->SetEnable(false);
		}
	}

	m_iFlag *= -1;
}

void CDocxInven::SetMouseOn(bool bMouseOn)
{
	m_bMouseOn = bMouseOn;
}

void CDocxInven::SetDocxnState(DOCXINVEN_STATE eState)
{
	m_eState = eState;
}

void CDocxInven::SetMouseOnDocx(bool bMouseOnInven)
{
	m_bMouseOnDocx = bMouseOnInven;
}

bool CDocxInven::GetVisible() const
{
	return m_bVisible;
}

int CDocxInven::GetDocxMax() const
{
	return m_iDocxMax;
}

bool CDocxInven::Full() const
{
	return m_iDocxMax == m_vecItem.size();
}

void CDocxInven::AddItem(CGameObject * pItem)
{
	if (Full())
		return;

	m_vecItem.push_back(pItem);

	CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

	Vector3	vDocxPos = m_pTransform->GetWorldPos();

	pItemTr->SetWorldPos(vDocxPos.x + 445.f, vDocxPos.y + 562.f - m_fItemY, 0.f);
	pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_fItemY += 60.f;

	if (m_pObject->GetEnable() == false)
	{
		m_vecItem[m_iIndex]->SetEnable(false);
	}

	++m_iIndex;

	SAFE_RELEASE(pItemTr);
}

DOCXINVEN_STATE CDocxInven::GetDocxState() const
{
	return m_eState;
}

const Vector3 CDocxInven::GetDocxPos() const
{
	return m_pTransform->GetWorldPos();
}

bool CDocxInven::GetMouseOnDocx() const
{
	return m_bMouseOnDocx;
}

void CDocxInven::AfterClone()
{
}

bool CDocxInven::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("Docx_Open", TEXT("Document_Open.WAV"));
	CResourcesManager::GetInst()->CreateTexture("DocxInven", TEXT("UI/Document/DocxInven.png"));

	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("InventoryRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "DocxInven");
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(1280.f, 720.f, 1.f);
	m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);

	m_pObject->SetEnable(false);

	m_iDocxMax = 20;
	m_iIndex = 0;

	m_pDocObj = CGameObject::CreateObject("Document", pUILayer, true);
	m_pDoc = m_pDocObj->AddComponent<CDocument>("Document");

	m_pDocObj->SetEnable(false);
	m_pDoc->ChangeClip("Message_Empty");

	m_pPMObj = CGameObject::CreateObject("PhoneMessage", pUILayer, true);

	m_pPM = m_pPMObj->AddComponent<CPhoneMessage>("PhoneMessage");

	AddItem(m_pPMObj);

	SAFE_RELEASE(pUILayer);

	return true;
}

int CDocxInven::Input(float fTime)
{
	return 0;
}

int CDocxInven::Update(float fTime)
{
	return 0;
}

int CDocxInven::LateUpdate(float fTime)
{
	return 0;
}

void CDocxInven::Collision(float fTime)
{
}

void CDocxInven::Render(float fTime)
{
}

CDocxInven * CDocxInven::Clone()
{
	return new CDocxInven(*this);
}

void CDocxInven::Save(BinaryWrite * _pInstBW)
{
	int iSize = (int)m_vecItem.size();
	_pInstBW->WriteData(iSize);

	for (size_t i = 0; i < iSize; ++i)
	{
		_pInstBW->WriteData(m_vecItem[i]->GetTag());
		_pInstBW->WriteData(m_vecItem[i]->GetLayer()->GetTag());
		m_vecItem[i]->Save(_pInstBW);
	}

	_pInstBW->WriteData(m_iIndex);
	_pInstBW->WriteData(m_iDocxMax);

	_pInstBW->WriteData(m_pDocObj->GetTag());
	_pInstBW->WriteData(m_pDocObj->GetLayer()->GetTag());
	_pInstBW->WriteData(m_pDocObj);

	_pInstBW->WriteData(m_pDoc->GetTag());

	_pInstBW->WriteData(m_pPMObj->GetTag());
	_pInstBW->WriteData(m_pPMObj->GetLayer()->GetTag());
	_pInstBW->WriteData(m_pPMObj);

	_pInstBW->WriteData(m_pPM->GetTag());
}

void CDocxInven::Load(BinaryRead * _pInstBR)
{
	int iSize = _pInstBR->ReadInt();

	for (size_t i = 0; i < iSize; ++i)
	{
		string ObjectTag = _pInstBR->ReadString();
		string LayerTag = _pInstBR->ReadString();
		CLayer* getLayer = CSceneManager::GetInst()->FindLayer(LayerTag);

		CGameObject* pItem = CGameObject::CreateObject(ObjectTag, getLayer, true);
		pItem->Load(_pInstBR);

		SAFE_RELEASE(getLayer);
		SAFE_RELEASE(pItem);
	}

	_pInstBR->ReadInt();
	_pInstBR->ReadInt();

	string DocObjTag = _pInstBR->ReadString();
	string strLayerTag = _pInstBR->ReadString();
	CLayer*	pLayer = GET_SINGLE(CSceneManager)->FindLayer(strLayerTag);

	CGameObject*	pDocument = CGameObject::CreateObject(DocObjTag, pLayer, true);
	pDocument->Load(_pInstBR);

	string DocTag = _pInstBR->ReadString();

	CDocument*	pDoc = pDocument->AddComponent<CDocument>(DocTag);

	SAFE_RELEASE(pDoc);
	SAFE_RELEASE(pDocument);

	string PMObjTag = _pInstBR->ReadString();
	string strPMLayerTag = _pInstBR->ReadString();

	CGameObject*	pPMObj = CGameObject::CreateObject(PMObjTag, pLayer, true);
	pPMObj->Load(_pInstBR);

	string PMTag = _pInstBR->ReadString();

	CDocument*	pPM = pPMObj->AddComponent<CDocument>(PMTag);

	SAFE_RELEASE(pPM);
	SAFE_RELEASE(pPMObj);
	SAFE_RELEASE(pLayer);
}

void CDocxInven::AddUILayer()
{
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (m_vecItem[i] == nullptr)
			break;

		pUILayer->AddObject(m_vecItem[i]);
	}

	SAFE_RELEASE(pUILayer);
}
