#include "Human_Player.h"
#include "Inventory.h"
#include "DocxInven.h"
#include "Input.h"


bool CHuman_Player::Init_Items()
{

	PUN::CInput::GetInst()->AddKey("U", 'U');

	PUN::CGameObject *pDocxInvObj = PUN::CGameObject::CreateObject("DocxInven", this->m_pLayer);

	m_pDocxInven = pDocxInvObj->AddComponent<CDocxInven>("DocxInven");
	m_pDocxInven->SetDocxMax(19);

	SAFE_RELEASE(pDocxInvObj);


	return true;
}

void CHuman_Player::OnDestroyInven()
{
	SAFE_RELEASE(m_pDocxInven);
}

int CHuman_Player::Input_Items(float fTime)
{

	if (PUN::CInput::GetInst()->KeyPress("U"))
	{
		m_pDocxInven->SetVisible();
	}

	return 0;
}

int CHuman_Player::ItemUpdate(float fTime)
{
	
	return 0;
}
int CHuman_Player::ItemLateUpdate(float fTime)
{
	return 0;
}


void CHuman_Player::Pickup_Item(float fTime)
{

};
void CHuman_Player::Open_Item(float fTime)
{
	m_pInven->SetVisible();
};
void CHuman_Player::Close_Item(float fTime)
{
	m_pInven->SetVisible();
}