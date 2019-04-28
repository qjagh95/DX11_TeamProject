#include "Human_Player.h"
#include "DocxInven.h"
#include <Input.h>

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