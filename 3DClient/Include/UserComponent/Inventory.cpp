#include "../ClientHeader.h"
#include "Inventory.h"
#include "Device.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/ColliderRect.h"
#include "GameObject.h"
#include "Input.h"
#include "Zipper.h"
#include "Component/ColliderSphere.h"
#include "Component/Item.h"
#include "BigIcon.h"
#include "DocxInven.h"

CInventory::CInventory() :
	m_iIndex(0),
	m_iMoveIndex(0),
	m_iNumberIndex(0)
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
}

CInventory::~CInventory()
{
	SAFE_RELEASE(m_pZipper);
	SAFE_RELEASE(m_pZipObj);

	SAFE_RELEASE(m_pBigIcon);
	SAFE_RELEASE(m_pBigIconObj);

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
		SAFE_RELEASE(m_pLunchBoxNumberObj);
		SAFE_RELEASE(m_pLunchBoxNumber);
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

	for (size_t i = 0; i < m_vecList.size(); ++i)
	{
		SAFE_RELEASE(m_vecList[i]);
	}

	Safe_Release_VecList(m_vecList);
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
		PUN::CInput::GetInst()->ShowMouse(true);
		CSoundManager::GetInst()->SoundPlay("Inven_Open");
		m_bVisible = true;
		m_pObject->SetEnable(true);

		m_pZipObj->SetEnable(true);
		
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
		PUN::CInput::GetInst()->ShowMouse(false);
		m_pBigIcon->ChangeClip("BigIcon_Empty");
		m_pBigIconObj->SetEnable(false);
		m_pZipper->ResetClip();
		m_pZipObj->SetEnable(false);

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

int CInventory::GetItemCountNumber(ICON_TYPE eType) const
{
	int iCount = 0;

	switch(eType)
	{
	case IT_BATTERY:
		iCount = m_iBatteryCnt;
		break;
	case IT_DAEMA:
		iCount = m_iDaemaCnt;
		break;
	case IT_MEDICALKIT:
		iCount = m_iMedicalKitCnt;
		break;
	case IT_LUNCHBOX:
		iCount = m_iLunchBoxCnt;
		break;
	case IT_TABLET:
		iCount = m_iTabletCnt;
		break;
	}

	return iCount;
}

void CInventory::SetItemCountNumber(ICON_TYPE eType, int iCount)
{
	switch (eType)
	{
	case IT_BATTERY:
		m_iBatteryCnt = iCount;
		break;
	case IT_DAEMA:
		m_iDaemaCnt = iCount;
		break;
	case IT_MEDICALKIT:
		m_iMedicalKitCnt = iCount;
		break;
	case IT_LUNCHBOX:
		m_iLunchBoxCnt = iCount;
		break;
	case IT_TABLET:
		m_iTabletCnt = iCount;
		break;
	}
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

	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	Vector3	vInvenPos = m_pTransform->GetWorldPos();
	string strTag = m_vecItem[m_iIndex]->GetTag();
	CTransform*	pItemTr = m_vecItem[m_iIndex]->GetTransform();

	if (strTag == "Icon_Battery")
	{
		m_bBatteryUse = true;
		if (m_iBatteryCnt == 0)
		{
			CGameObject* pBatteryObj = CGameObject::FindObject("Icon_Battery");
			m_pBatteryNumberObj = CGameObject::CreateObject("BatteryNumber", pUILayer, true);
			pUILayer->AddObject(m_pBatteryNumberObj);
			m_vecNumber.push_back(m_pBatteryNumberObj);
			m_vecList.push_back(pBatteryObj);
			++m_iNumberIndex;
			++m_iMoveIndex;
			SAFE_RELEASE(pBatteryObj);

			m_pBatteryNumber = m_pBatteryNumberObj->AddComponent<CNumber>("BatteryNumber");
			pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
				vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
			//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);
			m_vBatteryIconPos = pItemTr->GetWorldPos();
			m_fItemY += 151.f;
			//m_fItemY += 105.f;
		}

		++m_iBatteryCnt;
		if (m_iBatteryCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}

		m_pBatteryNumber->SetNumber(m_iBatteryCnt);
		m_pBatteryNumber->SetNumberPivot(0.5f, 0.5f, 0.f); 

		pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
			vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
		//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY + 105.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pBatteryNumberObj->GetTransform();
		pNumTr->SetWorldPos(m_vBatteryIconPos.x + 51.f, m_vBatteryIconPos.y - 51.f - 151.f, 0.f);
		//pNumTr->SetWorldPos(m_vBatteryIconPos.x + 35.f, m_vBatteryIconPos.y - 35.f, 0.f);
		SAFE_RELEASE(pNumTr);
	}

	else if (strTag == "Icon_MedicalKit")
	{
		m_bMedicalKitUse = true;
		if (m_iMedicalKitCnt == 0)
		{
			CGameObject*	pMedicalKitObj = CGameObject::FindObject("Icon_MedicalKit");
			m_pMedicalKitNumberObj = CGameObject::CreateObject("MedicalKitNumber", pUILayer, true);
			m_vecNumber.push_back(m_pMedicalKitNumberObj);
			m_vecList.push_back(pMedicalKitObj);
			++m_iNumberIndex;
			++m_iMoveIndex;
			SAFE_RELEASE(pMedicalKitObj);

			m_pMedicalKitNumber = m_pMedicalKitNumberObj->AddComponent<CNumber>("MedicalKitNumber");
			pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
				vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);
			m_vMedicalKitIconPos = pItemTr->GetWorldPos();
			m_fItemY += 151.f;
			//m_fItemY += 105.f;
		}
		++m_iMedicalKitCnt;
		if (m_iMedicalKitCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}
		m_pMedicalKitNumber->SetNumber(m_iMedicalKitCnt);
		m_pMedicalKitNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

		pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
			vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
		//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY + 105.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pMedicalKitNumberObj->GetTransform();
		pNumTr->SetWorldPos(m_vMedicalKitIconPos.x + 51.f, m_vMedicalKitIconPos.y - 51.f - 151.f, 0.f);
		//pNumTr->SetWorldPos(m_vMedicalKitIconPos.x + 35.f, m_vMedicalKitIconPos.y - 35.f, 0.f);
		SAFE_RELEASE(pNumTr);
	}

	else if(strTag == "Icon_LunchBox")
	{
		m_bLunchBoxUse = true;
		if (m_iLunchBoxCnt == 0)
		{
			CGameObject*	pLunchBoxObj = CGameObject::FindObject("Icon_LunchBox");
			m_pLunchBoxNumberObj = CGameObject::CreateObject("LunchBoxNumber", pUILayer, true);
			m_vecNumber.push_back(m_pLunchBoxNumberObj);
			m_vecList.push_back(pLunchBoxObj);
			++m_iNumberIndex;
			++m_iMoveIndex;
			SAFE_RELEASE(pLunchBoxObj);

			m_pLunchBoxNumber = m_pLunchBoxNumberObj->AddComponent<CNumber>("LunchBoxNumber");
			pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
				vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
			pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);
			m_vLunchBoxIconPos = pItemTr->GetWorldPos();
			m_fItemY += 151.f;
			//m_fItemY += 105.f;
		}

		++m_iLunchBoxCnt;
		if (m_iLunchBoxCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}
		m_pLunchBoxNumber->SetNumber(m_iLunchBoxCnt);
		m_pLunchBoxNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

		pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
			vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
		//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY + 105.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pLunchBoxNumberObj->GetTransform();
		pNumTr->SetWorldPos(m_vLunchBoxIconPos.x + 51.f, m_vLunchBoxIconPos.y - 51.f - 151.f, 0.f);
		//pNumTr->SetWorldPos(m_vLunchBoxIconPos.x + 35.f, m_vLunchBoxIconPos.y - 35.f, 0.f);
		SAFE_RELEASE(pNumTr);
	}

	else if (strTag == "Icon_Cigarette")
	{
		m_bDaemaUse = true;
		if (m_iDaemaCnt == 0)
		{
			CGameObject*	pCigaretteObj = CGameObject::FindObject("Icon_Cigarette");
			m_pDaemaNumberObj = CGameObject::CreateObject("DaemaNumber", pUILayer, true);
			m_vecNumber.push_back(m_pDaemaNumberObj);
			m_vecList.push_back(pCigaretteObj);
			++m_iNumberIndex;
			++m_iMoveIndex;
			SAFE_RELEASE(pCigaretteObj);

			m_pDaemaNumber = m_pDaemaNumberObj->AddComponent<CNumber>("DaemaNumber");
			pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
				vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
			//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);
			m_vDaemaIconPos = pItemTr->GetWorldPos();
			m_fItemY += 151.f;
			//m_fItemY += 105.f;
		}

		++m_iDaemaCnt;
		if (m_iDaemaCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}
		m_pDaemaNumber->SetNumber(m_iDaemaCnt);
		m_pDaemaNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

		pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
		 	vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
		//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY + 105.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pDaemaNumberObj->GetTransform();
		pNumTr->SetWorldPos(m_vDaemaIconPos.x + 51.f, m_vDaemaIconPos.y - 51.f - 151.f, 0.f);
		//pNumTr->SetWorldPos(m_vDaemaIconPos.x + 35.f, m_vDaemaIconPos.y - 35.f, 0.f);
		SAFE_RELEASE(pNumTr);
	}

	else if (strTag == "Icon_Tablet")
	{
		m_bTabletUse = true;
		if (m_iTabletCnt == 0)
		{
			CGameObject*	pTabletObj = CGameObject::FindObject("Icon_Tablet");
			m_pTabletNumberObj = CGameObject::CreateObject("TabletNumber", pUILayer, true);
			m_vecNumber.push_back(m_pTabletNumberObj);
			m_vecList.push_back(pTabletObj);
			++m_iNumberIndex;
			++m_iMoveIndex;
			SAFE_RELEASE(pTabletObj);

			m_pTabletNumber = m_pTabletNumberObj->AddComponent<CNumber>("TabletNumber");
			pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
				vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
			//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY, 0.f);
			pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);
			m_vTabletIconPos = pItemTr->GetWorldPos();
			m_fItemY += 151.f;
			//m_fItemY += 105.f;
		}

		++m_iTabletCnt;
		if (m_iTabletCnt >= 2)
		{
			m_vecItem[m_iIndex]->Die();
		}
		m_pTabletNumber->SetNumber(m_iTabletCnt);
		m_pTabletNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

		pItemTr->SetWorldPos(vInvenPos.x + _RESOLUTION.iWidth * (1.f - (1 - 315.f / 1280.f)),
			vInvenPos.y + _RESOLUTION.iHeight * (1.f - (1 - 646.f / 720.f)) - m_fItemY, 0.f);
		//pItemTr->SetWorldPos(vInvenPos.x + 315.f, vInvenPos.y + 562.f - m_fItemY + 105.f, 0.f);
		pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		CTransform*	pNumTr = m_pTabletNumberObj->GetTransform();
		pNumTr->SetWorldPos(m_vTabletIconPos.x + 51.f, m_vTabletIconPos.y - 51.f - 151.f, 0.f);
		//pNumTr->SetWorldPos(m_vTabletIconPos.x + 35.f, m_vTabletIconPos.y - 35.f, 0.f);
		SAFE_RELEASE(pNumTr);
	}

	if (m_pObject->GetEnable() == false)
	{
		for (size_t i = 0; i < m_vecNumber.size(); ++i)
		{
			if (m_vecNumber[i] == nullptr)
				break;

			m_vecNumber[i]->SetEnable(false);
		}

		m_vecItem[m_iIndex]->SetEnable(false);
	}	

	++m_iIndex;
	SAFE_RELEASE(pItemTr);
	SAFE_RELEASE(pUILayer);
}

void CInventory::UseItem(CGameObject * pItem)
{
	CGameObject* pFindItem = nullptr;
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (m_vecItem[i] == pItem)
		{
			pFindItem = m_vecItem[i];
			break;
		}
	}

	bool isDestroy = false;
	string strTag = pFindItem->GetTag();
	if (strTag == "Icon_Battery")
	{
		--m_iBatteryCnt;
		m_pBatteryNumber->SetNumber(m_iBatteryCnt);
		if (m_iBatteryCnt == 0)
		{
			isDestroy = true;
			m_pBatteryNumberObj->Die();
		}
	}

	else if (strTag == "Icon_MedicalKit")
	{
		--m_iMedicalKitCnt;
		if (m_iMedicalKitCnt == 0)
		{
			m_pMedicalKitNumberObj->Die();
			isDestroy = true;
		}
	}

	else if (strTag == "Icon_LunchBox")
	{
		--m_iLunchBoxCnt;
		if (m_iLunchBoxCnt == 0)
		{
			m_pLunchBoxNumberObj->Die();
			isDestroy = true;
		}
	}

	else if (strTag == "Icon_Cigarette")
	{
		--m_iDaemaCnt;
		if (m_iDaemaCnt == 0)
		{
			m_pDaemaNumberObj->Die();
			isDestroy = true;
		}
	}

	else if (strTag == "Icon_Tablet")
	{
		--m_iTabletCnt;
		if (m_iTabletCnt == 0)
		{
			m_pTabletNumberObj->Die();
			isDestroy = true;
		}
	}

	if (isDestroy == true)
	{
		size_t iNum = 0;
		for (size_t i = 0; i < m_vecList.size(); ++i)
		{
			if (m_vecList[i] == pFindItem)
			{
				iNum = i;

				vector<CGameObject*>::iterator	iter;
				vector<CGameObject*>::iterator	iterEnd = m_vecList.end();
				for (iter = m_vecList.begin(); iter != iterEnd; ++iter)
				{
					if ((*iter) == pFindItem)
					{
						m_vecList.erase(iter);
						break;
					}
				}

				m_vecNumber[i]->Die();

				vector<CGameObject*>::iterator	iter1;
				vector<CGameObject*>::iterator	iter1End = m_vecNumber.end();
				for (iter1 = m_vecNumber.begin(); iter1 != iter1End; ++iter1)
				{
					if ((*iter1) == m_vecNumber[i])
					{
						SAFE_RELEASE((*iter1));
						m_vecNumber.erase(iter1);
						break;
					}
				}

				--m_iNumberIndex;
				--m_iMoveIndex;

				for (size_t j = iNum; j < m_iMoveIndex; ++j)
				{
					MoveItem(j);
				}
			}			
		}

		pFindItem->Die();
		pFindItem = nullptr;
		m_fItemY -= 105.f;

		SAFE_RELEASE(pFindItem);
	}

	--m_iIndex;
}

void CInventory::MoveItem(size_t iIndex)
{
	CTransform*	pListTr = m_vecList[iIndex]->GetTransform();

	pListTr->SetWorldPos(pListTr->GetWorldPos().x, pListTr->GetWorldPos().y + 151.f, 0.f);
	//pListTr->SetWorldPos(pListTr->GetWorldPos().x, pListTr->GetWorldPos().y + 105.f, 0.f);

	SAFE_RELEASE(pListTr);

	CTransform*	pNumberTr = m_vecNumber[iIndex]->GetTransform();

	pNumberTr->SetWorldPos(pNumberTr->GetWorldPos().x, pNumberTr->GetWorldPos().y + 151.f, 0.f);
	//pNumberTr->SetWorldPos(pNumberTr->GetWorldPos().x, pNumberTr->GetWorldPos().y + 105.f, 0.f);

	SAFE_RELEASE(pNumberTr);
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
	
	m_pTransform->SetWorldScale((float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 1.f);
	m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);

	m_pObject->SetEnable(false);

	m_iInvenMax = 20;
	m_iIndex = 0;

	m_vecItem.resize(20);

	m_pBigIconObj = CGameObject::CreateObject("BigIcon", m_pLayer, true);

	m_pBigIcon = m_pBigIconObj->AddComponent<CBigIcon>("BigIcon");

	m_pZipObj = CGameObject::CreateObject("InvenZipper", m_pLayer, true);

	m_pZipper = m_pZipObj->AddComponent<CZipper>("InvenZipper");

	CTransform*	pZipTr = m_pZipObj->GetTransform();

	pZipTr->SetWorldPos(_RESOLUTION.iWidth * (1.f - (1 - 255.f / 1280.f)),
		_RESOLUTION.iWidth * (1.f - (1 - 45.f / 720.f)), 1.f);
	//pZipTr->SetWorldPos(255.f, 80.f, 0.f);

	SAFE_RELEASE(pZipTr);

	m_pZipObj->SetEnable(false);

	return true;
}

int CInventory::Input(float fTime)
{
	return 0;
}

int CInventory::Update(float fTime)
{	
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

void CInventory::Save(BinaryWrite * _pInstBW)
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
	_pInstBW->WriteData(m_iInvenMax);
	_pInstBW->WriteData(m_iMoveIndex);
	_pInstBW->WriteData(m_iNumberIndex);
	_pInstBW->WriteData(m_fItemY);

	_pInstBW->WriteData(m_iBatteryCnt);
	_pInstBW->WriteData(m_iMedicalKitCnt);
	_pInstBW->WriteData(m_iLunchBoxCnt);
	_pInstBW->WriteData(m_iTabletCnt);
	_pInstBW->WriteData(m_iDaemaCnt);

	iSize = (int)m_vecNumber.size();
	_pInstBW->WriteData(iSize);

	for (size_t i = 0; i < iSize; ++i)
	{
		_pInstBW->WriteData(m_vecNumber[i]->GetTag());
		_pInstBW->WriteData(m_vecNumber[i]->GetLayer()->GetTag());
		m_vecNumber[i]->Save(_pInstBW);
	}

	iSize = (int)m_vecList.size();
	_pInstBW->WriteData(iSize);

	for (size_t i = 0; i < iSize; ++i)
	{
		_pInstBW->WriteData(m_vecList[i]->GetTag());
		_pInstBW->WriteData(m_vecList[i]->GetLayer()->GetTag());
		m_vecList[i]->Save(_pInstBW);
	}
	   

	_pInstBW->WriteData(m_pZipObj->GetTag());
	_pInstBW->WriteData(m_pZipObj->GetLayer()->GetTag());
	m_pZipObj->Save(_pInstBW);

	_pInstBW->WriteData(m_pZipper->GetTag());
}

void CInventory::Load(BinaryRead * _pInstBR)
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
	_pInstBR->ReadInt();
	_pInstBR->ReadInt();
	_pInstBR->ReadInt();

	_pInstBR->ReadInt();
	_pInstBR->ReadInt();
	_pInstBR->ReadInt();
	_pInstBR->ReadInt();
	_pInstBR->ReadInt();

	iSize = _pInstBR->ReadInt();

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


	iSize = _pInstBR->ReadInt();

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

	string ZipObjTag = _pInstBR->ReadString();
	string ZipLayerTag = _pInstBR->ReadString();
	CLayer* getZipLayer = CSceneManager::GetInst()->FindLayer(ZipLayerTag);

	CGameObject* pZipItem = CGameObject::CreateObject(ZipObjTag, getZipLayer, true);
	pZipItem->Load(_pInstBR);

	string ZipTag = _pInstBR->ReadString();
	CZipper*	pZip = pZipItem->AddComponent<CZipper>(ZipTag);

	SAFE_RELEASE(pZip);
	SAFE_RELEASE(getZipLayer);
	SAFE_RELEASE(pZipItem);
}

void CInventory::AddUILayer()
{
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (m_vecItem[i] == nullptr)
			break;

		pUILayer->AddObject(m_vecItem[i]);
	}

	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		if (m_vecNumber[i] == nullptr)
			break;

		pUILayer->AddObject(m_vecNumber[i]);
	}

	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		if (m_vecList[i] == nullptr)
			break;

		pUILayer->AddObject(m_vecList[i]);
	}

	SAFE_RELEASE(pUILayer);
}

