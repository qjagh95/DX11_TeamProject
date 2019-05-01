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
	m_pTransform->SetWorldScale(100.f, 100.f, 100.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");

	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("HealingPack_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.5f);
	pCollider->SetCollisionCallback(CCT_ENTER, this, &CHealingPack::HitEnter);
	pCollider->SetCollisionCallback(CCT_STAY,  this, &CHealingPack::HitStay);
	SAFE_RELEASE(pCollider);

	/**********************************************************************************************/

	// 'Inventory' ��ü ���
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
	return 0;
}

void CHealingPack::SetMesh(const string& _meshKey, const TCHAR* _meshName)
{
	// .msh ���Ͼȿ� MAX ���α׷����� ����(Material, Texture)�� �����Ų ���̴�.
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
	string strMessage = "Hit Enter : " +  m_pObject->GetTag() + "\n";
	std::wstring cvtMessage = std::wstring(strMessage.begin(), strMessage.end());
	OutputDebugString((cvtMessage.c_str()));
}

void CHealingPack::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (KEYPRESS("LButton") == true)
	{
		if (m_isInvenInItem == false)
		{
			m_isInvenInItem = true;

			// ������ ������ ����
			// 'HealingPackIcon' ��ü ����
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

			// ���� ȭ�鿡�� �������.
			m_pObject->SetEnable(false);
		}
	}
}