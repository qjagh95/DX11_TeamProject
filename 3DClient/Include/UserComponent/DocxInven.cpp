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

	m_pDocObj = CGameObject::CreateObject("Document", m_pLayer);
	m_pDoc = m_pDocObj->AddComponent<CDocument>("Document");

	m_pDocObj->SetEnable(false);
	m_pDoc->ChangeClip("Message_Empty");

	CGameObject*	pPMObj = CGameObject::CreateObject("PhoneMessage", m_pLayer);

	CPhoneMessage*	pPM = pPMObj->AddComponent<CPhoneMessage>("PhoneMessage");

	AddItem(pPMObj);

	SAFE_RELEASE(pPM);
	SAFE_RELEASE(pPMObj);

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
}

void CDocxInven::Load(BinaryRead * _pInstBR)
{
}
