#include "../ClientHeader.h"
#include "Inventory.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/ColliderRect.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "Zipper.h"

CInventory::CInventory() :
	m_iIndex(0),
	m_iMoveIndex(0)
{
	m_eComType = (COMPONENT_TYPE)UT_INVEN;
	SetTag("Inventory");

	m_iFlag = -1;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bVisible = false;
	m_eState = INS_NORMAL;
	m_fItemY = 0.f;
	m_iZipCount = 0;
}

CInventory::CInventory(const CInventory & inven)	:
	CUI(inven)
{
	//*this = inven;
}

CInventory::~CInventory()
{
	//SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pUILayer);

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecItem[i]);
	}

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
		m_iZipCount = 0;

		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (m_vecItem[i] == nullptr)
				break;

			m_vecItem[i]->SetEnable(false);
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

void CInventory::SetMouseOnInven(bool bMouseOnInven)
{
	m_bMouseOnInven = bMouseOnInven;
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

	if (m_pObject->GetEnable() == false)
	{
		m_vecItem[m_iIndex]->SetEnable(false);
	}

	++m_iIndex;
}

void CInventory::DeleteItem(CGameObject * pItem)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (m_vecItem[i] == pItem)
		{
			m_vecItem[i] = nullptr;
			--m_iIndex;
		}
	}
}

INVENTORY_STATE CInventory::GetInvenState() const
{
	return m_eState;
}

const Vector3 CInventory::GetInvenPos() const
{
	return m_pTransform->GetWorldPos();
}

bool CInventory::GetMouseOnInven() const
{
	return m_bMouseOnInven;
}

void CInventory::AfterClone()
{
}

bool CInventory::Init()
{
	m_pUILayer = m_pScene->FindLayer("UI");

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

void CInventory::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CInventory::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CInventory::BottomHit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CInventory::BottomInven(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CInventory::BottomOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CInventory::AddInvenList(CGameObject * pItem)
{
}
