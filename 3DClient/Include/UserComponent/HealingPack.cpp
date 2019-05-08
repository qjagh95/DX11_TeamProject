#include "../ClientHeader.h"
#include "Inventory.h"
#include "HealingPack.h"
#include "HealingPackIcon.h"
#include "Human_Player.h"
#include <Component/ColliderSphere.h>

CHealingPack::CHealingPack() :
	m_isInvenInItem(false),
	m_hpAmount(0),
	m_strMeshKey(""),
	m_pRenderer(nullptr),
	m_pObjInventory(nullptr),
	m_pInventory(nullptr),
	m_pObjItemIcon(nullptr)
{
}

CHealingPack::CHealingPack(const CHealingPack& _healingPack)
{
}

CHealingPack::~CHealingPack()
{
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObjItemIcon);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pObjInventory);
}

TCHAR* stringToTCHAR(string& _string)
{
	// string convert to TCHAR
	std::string str = _string;
	TCHAR *param = new TCHAR[str.size() + 1];
	param[str.size()] = 0;
	std::copy(str.begin(), str.end(), param);
	return param;
}

bool CHealingPack::Init()
{
	// Transform 
	//m_pTransform->SetWorldScale(20.f, 20.f, 20.f);
	m_pTransform->SetWorldScale(1.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");

	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("HealingPack_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.5f);
	pCollider->SetCollisionCallback(CCT_ENTER, this, &CHealingPack::HitEnter);
	pCollider->SetCollisionCallback(CCT_STAY,  this, &CHealingPack::HitStay);
	pCollider->SetCollisionCallback(CCT_LEAVE, this, &CHealingPack::MouseOut);
	SAFE_RELEASE(pCollider);

	/**********************************************************************************************/

	// 'Inventory' 객체 얻기
	m_pObjInventory = m_pScene->FindObject("Inven");
	m_pInventory = m_pObjInventory->FindComponentFromTag<CInventory>("Inven");

	return true;
}

int CHealingPack::Input(float _fTime)
{
	return 0;
}

int CHealingPack::Update(float _fTime)
{
	if (m_bMotion)
	{
		CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

		CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
		pPlayer->ChangeRayAnim("AimOff");

		m_bMotion = false;

		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	return 0;
}

void CHealingPack::SetMesh(const string& _meshKey, const TCHAR* _meshName)
{
	// .msh 파일안에 MAX 프로그램으로 재질(Material, Texture)를 적용시킨 모델이다.
	m_pRenderer->SetMesh(_meshKey, _meshName);
	m_strMeshKey = _meshKey;
}

const string CHealingPack::GetMeshKey()
{
	return m_strMeshKey;
}

void CHealingPack::SetHPAmount(int _amount)
{
	m_hpAmount = _amount;
}

int CHealingPack::GetHPAmount()
{
	return m_hpAmount;
}

void CHealingPack::HitEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

	CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	Vector3 vPlayerPos = pPlayerTr->GetWorldPos();

	float fDist = m_pTransform->GetWorldPos().Distance(vPlayerPos);

	if (fDist < 50.f)
	{
		if (_pDest->GetColliderID() == UCI_PLAYER_RAY)
		{
			pPlayer->ChangeRayAnim("AimOn");
			m_bGetItem = true;
		}
	}

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
	/*string strMessage = "Hit Enter : " +  m_pObject->GetTag() + "\n";
	std::wstring cvtMessage = std::wstring(strMessage.begin(), strMessage.end());
	OutputDebugString((cvtMessage.c_str()));*/
}

void CHealingPack::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (m_bGetItem)
	{
		if (KEYPRESS("LButton") == true)
		{
			if (m_isInvenInItem == false)
			{
				m_isInvenInItem = true;

				// 아이템 아이콘 생성
				// 'HealingPackIcon' 객체 생성
				string strIconName = "";
				if (m_pObject->GetTag() == "MedicalKit")
				{
					strIconName = "Icon_MedicalKit";
				}
				else if (m_pObject->GetTag() == "LunchBox")
				{
					strIconName = "Icon_LunchBox";
				}
				m_pObjItemIcon = CGameObject::CreateObject(strIconName, m_pLayer);
				CHealingPackIcon* pHealingPackIcon = m_pObjItemIcon->AddComponent<CHealingPackIcon>(strIconName);
				pHealingPackIcon->SetHealingPackInst(this);
				pHealingPackIcon->SetMaterial();
				m_pInventory->AddItem(m_pObjItemIcon);
				SAFE_RELEASE(pHealingPackIcon);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

				// 게임 화면에서 사라진다.
				m_pObject->SetEnable(false);
			}
		}
	}
}

void CHealingPack::MouseOut(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (_pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMotion = true;
		m_bGetItem = false;
	}
}
