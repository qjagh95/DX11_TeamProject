#include "../ClientHeader.h"
#include "Inventory.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/ColliderRect.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "Zipper.h"
#include "Component/ColliderSphere.h"
#include "Component/Item.h"

CInventory::CInventory() :
	m_iIndex(0),
	m_iMoveIndex(0)
{
	m_eComType = (COMPONENT_TYPE)UT_INVENTORY;
	m_eType = IT_NONE;
	SetTag("Inventory");

	m_iFlag = -1;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bVisible = false;
	m_eState = INS_NORMAL;
	m_fItemY = 0.f;
	m_iZipCount = 0;
	m_iCount = 0;

	m_iBatteryCnt = 0;
	m_iMedicalKitCnt = 0;
	m_iLunchBoxCnt = 0;
	m_iTabletCnt = 0;
	m_iDaemaCnt = 0;
	m_bBatteryUse = false;
	m_bMedicalKitUse = false;
	m_bTabletUse = false;
	m_bLunchBoxUse = false;
	m_bDaemaUse = false;
}

CInventory::CInventory(const CInventory & inven)	:
	CUserComponent(inven)
{
	//*this = inven;
}

CInventory::~CInventory()
{
	if (m_bBatteryUse)
	{
		SAFE_RELEASE(m_pBatteryNumberObj);
		SAFE_RELEASE(m_pBatteryNumber);
	}

	if (m_bMedicalKitUse)
	{
		SAFE_RELEASE(m_pMedicalKitNumberObj);
		SAFE_RELEASE(m_pMedicalKitNumber);
	}

	if (m_bTabletUse)
	{
		SAFE_RELEASE(m_pTabletNumberObj);
		SAFE_RELEASE(m_pTabletNumber);
	}

	if (m_bLunchBoxUse)
	{
		SAFE_RELEASE(m_punchBoxNumberObj);
		SAFE_RELEASE(m_punchBoxNumber);
	}

	if (m_bDaemaUse)
	{
		SAFE_RELEASE(m_pDaemaNumberObj);
		SAFE_RELEASE(m_pDaemaNumber);
	}

	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		SAFE_RELEASE(m_vecNumber[i]);
	}

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecItem[i]);
	}

	Safe_Release_VecList(m_vecNumber);
	Safe_Release_VecList(m_vecItem);
}

void CInventory::SetInvenMax(int iMax)
{
	m_iInvenMax = iMax;
}

void CInventory::SetVisible(bool bVisible)
{
	m_pObject->SetEnable(bVisible);
}

void CInventory::SetVisible()
{
	if (m_iFlag == -1)
	{
		CSoundManager::GetInst()->SoundPlay("Inven_Open");
		m_bVisible = true;
		m_pObject->SetEnable(true);
		
		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (m_vecItem[i] == nullptr)
				break;

			m_vecItem[i]->SetEnable(true);
		}

		for (size_t i = 0; i < m_vecNumber.size(); ++i)
		{
			if (m_vecNumber[i] == nullptr)
				break;

			m_vecNumber[i]->SetEnable(true);
		}
	}
	else if (m_iFlag == 1)
	{
		m_bVisible = false;
		m_pObject->SetEnable(false);
		m_iZipCount = 0;

		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (m_vecItem[i] == nullptr)
				break;

			m_vecItem[i]->SetEnable(false);
		}

		for (size_t i = 0; i < m_vecNumber.size(); ++i)
		{
			if (m_vecNumber[i] == nullptr)
				break;

			m_vecNumber[i]->SetEnable(false);
		}
	}

	m_iFlag *= -1;
}

void CInventory::SetMouseOn(bool bMouseOn)
{
	m_bMouseOn = bMouseOn;
}

void CInventory::SetInvenState(INVENTORY_STATE eState)
{
	m_eState = eState;
}

bool CInventory::GetVisible() const
{
	return m_bVisible;
}

int CInventory::GetInvenMax() const
{
	return m_iInvenMax;
}

bool CInventory::Full() const
{
	return m_iInvenMax == m_vecItem.size();
}

void CInventory::AddItem(CGameObject * pItem)
{
	if (Full())
		return;

	m_vecItem[m_iIndex] = pItem;

	string strTag = m_vecItem[m_iIndex]->GetTag();
	CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

	if (strTag == "Icon_Battery")
	{
		m_bBatteryUse = true;
		if (m_iBatteryCnt == 0)
		{
			m_pBatteryNumberObj = CGameObject::CreateObject("BatteryNumber", m_pLayer);

			m_vecNumber.push_back(m_pBatteryNumberObj);

			m_pBatteryNumber = m_pBatteryNumberObj->AddComponent<CNumber>("BatteryNumber");

			Vector3	vInvenPos = m_pTransform->GetWorldPos();
			
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

			m_fItemY += 100.f;
		}

		++m_iBatteryCnt;

		if (m_iBatteryCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}

		m_pBatteryNumber->SetNumber(m_iBatteryCnt);
		m_pBatteryNumber->SetNumberPivot(0.5f, 0.5f, 0.f); 

		Vector3	vInvenPos = m_pTransform->GetWorldPos();

		pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pBatteryNumberObj->GetTransform();

		Vector3	vIconPos = pItemTr->GetWorldPos();

		pNumTr->SetWorldPos(vIconPos.x + 35.f, vIconPos.y - 15.f, 0.f);

		SAFE_RELEASE(pNumTr);
		SAFE_RELEASE(pItemTr);
	}

	else if (strTag == "MedicalKit")
	{
		if (m_iMedicalKitCnt == 0)
		{
			CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

			Vector3	vInvenPos = m_pTransform->GetWorldPos();

			pItemTr->AddParentFlag(TPF_ROT);
			pItemTr->AddParentFlag(TPF_POS);
			pItemTr->AddParentFlag(TPF_SCALE);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

			SAFE_RELEASE(pItemTr);
			m_fItemY += 100.f;
		}

		++m_iMedicalKitCnt;
	}

	else if(strTag == "LunchBox")
	{
		if (m_iLunchBoxCnt == 0)
		{
			CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

			Vector3	vInvenPos = m_pTransform->GetWorldPos();

			pItemTr->AddParentFlag(TPF_ROT);
			pItemTr->AddParentFlag(TPF_POS);
			pItemTr->AddParentFlag(TPF_SCALE);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

			SAFE_RELEASE(pItemTr);
			m_fItemY += 100.f;
		}

		++m_iLunchBoxCnt;
	}

	else if (strTag == "Daema")
	{
		if (m_iDaemaCnt == 0)
		{
			CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

			Vector3	vInvenPos = m_pTransform->GetWorldPos();

			pItemTr->AddParentFlag(TPF_ROT);
			pItemTr->AddParentFlag(TPF_POS);
			pItemTr->AddParentFlag(TPF_SCALE);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

			SAFE_RELEASE(pItemTr);
			m_fItemY += 100.f;
		}

		++m_iDaemaCnt;
	}

	else if (strTag == "Tablet")
	{
		if (m_iTabletCnt == 0)
		{
			CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

			Vector3	vInvenPos = m_pTransform->GetWorldPos();

			pItemTr->AddParentFlag(TPF_ROT);
			pItemTr->AddParentFlag(TPF_POS);
			pItemTr->AddParentFlag(TPF_SCALE);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

			SAFE_RELEASE(pItemTr);
			m_fItemY += 100.f;
		}

		++m_iTabletCnt;
	}

	if (m_pObject->GetEnable() == false)
	{
		m_vecItem[m_iIndex]->SetEnable(false);
	}	

	++m_iIndex;
}

void CInventory::UseItem(CGameObject * pItem)
{
	m_vecItem[m_iIndex] = pItem;

	string strTag = m_vecItem[m_iIndex]->GetTag();

	if (strTag == "Icon_Battery")
	{
		--m_iBatteryCnt;
		m_pBatteryNumber->SetNumber(m_iBatteryCnt);

		if (m_iBatteryCnt == 0)
		{
			m_pBatteryNumberObj->Die();
			m_vecItem[m_iIndex]->Die();
			m_vecItem[m_iIndex] = nullptr;
			m_fItemY += 100.f;
			SAFE_RELEASE(m_vecItem[m_iIndex]);
		}
	}

	else if (strTag == "MedicalKit")
	{
	}

	else if (strTag == "LunchBox")
	{
	}

	else if (strTag == "Daema")
	{
	}

	else if (strTag == "Tablet")
	{
	}
	
	--m_iIndex;
}

INVENTORY_STATE CInventory::GetInvenState() const
{
	return m_eState;
}

const Vector3 CInventory::GetInvenPos() const
{
	return m_pTransform->GetWorldPos();
}

void CInventory::AfterClone()
{
}

bool CInventory::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("Inven_Open", TEXT("Zipper_Open.WAV"));

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("InventoryRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Inventory", TEXT("UI/Inven/InvenBack.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*	pTr = m_pObject->GetTransform();

	pTr->SetWorldScale(1280.f, 720.f, 1.f);
	pTr->SetWorldPivot(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTr);

	m_pObject->SetEnable(false);

	m_iInvenMax = 20;
	m_iIndex = 0;

	m_vecItem.resize(20);

	return true;
}

int CInventory::Input(float fTime)
{
	return 0;
}

int CInventory::Update(float fTime)
{	
	if (m_bVisible)
	{
		if (m_iZipCount < 1.f)
		{
			CGameObject*	pZipObj = CGameObject::CreateObject("InvenZipper", m_pLayer);

			CZipper*	pZip = pZipObj->AddComponent<CZipper>("InvenZipper");

			CTransform*	pZipTr = pZipObj->GetTransform();

			pZipTr->SetWorldPos(255.f, 80.f, 0.f);

			SAFE_RELEASE(pZipTr);
			SAFE_RELEASE(pZip);
			SAFE_RELEASE(pZipObj);
			++m_iZipCount;
		}
	}

	return 0;
}

int CInventory::LateUpdate(float fTime)
{
	return 0;
}

void CInventory::Collision(float fTime)
{
}

void CInventory::Render(float fTime)
{
}

CInventory * CInventory::Clone()
{
	return new CInventory(*this);
}

