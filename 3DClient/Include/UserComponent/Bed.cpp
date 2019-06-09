#include "../ClientHeader.h"
#include "Bed.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Human_Player.h"
#include <Input.h>

float CBed::fPlayerLocalOffsetY = -100.f;

CBed::CBed()
{
	m_eComType = (COMPONENT_TYPE)UT_HIDABLE;
	m_eType = HT_BED;
}

CBed::CBed(const CBed & battery)
{
}


CBed::~CBed()
{
}

void CBed::AfterClone()
{
	CColliderOBB3D *pCol = FindComponentFromType<CColliderOBB3D>(PUN::CT_COLLIDER);
	pCol->SetCollisionCallback(PUN::CCT_STAY, this, &CBed::OnCollisionWithPlayer);
	pCol->SetCollisionCallback(PUN::CCT_LEAVE, this, &CBed::OnCollisionExitWithPlayer);

	SAFE_RELEASE(pCol);
}

bool CBed::Init()
{

	CRenderer* pRD = FindComponentFromType<CRenderer>(CT_RENDERER);
	if (!pRD)
	{
		pRD = m_pObject->AddComponent<CRenderer>("BedRenderer");
		pRD->SetMesh("bed", TEXT("Bed_No_Sheet.msh"));

		m_pTransform->SetWorldScale(0.0375f, 0.0375f, 0.0375f);
		m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
		//m_pTransform->SetWorldPivot(0.5f, 0.f, 0.5f);
		m_pTransform->SetLocalPos(0.f, 0.f, 32.f);
	}

	CColliderOBB3D *pCol = AddComponent<CColliderOBB3D>("Collider");

	pCol->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.5f, 3.f, 3.f));
	pCol->SetColliderID(UCI_BED);

	SAFE_RELEASE(pCol);

	SAFE_RELEASE(pRD);

	std::string matressStr = "matress_";
	matressStr += to_string((unsigned int)this);
	PUN::CGameObject *pMatress = PUN::CGameObject::CreateObject(matressStr, m_pLayer);

	pRD = pMatress->AddComponent<PUN::CRenderer>("renderer");
	pRD->SetMesh("matress", TEXT("matress.msh"));
	SAFE_RELEASE(pRD);

	
	PUN::CTransform *pTR = pMatress->GetTransform();
	pTR->SetWorldScale(.1f, .10f, .0825f);
	pTR->SetLocalPos(0.f, 18.f, 15.f);

	SAFE_RELEASE(pTR);
	m_pObject->AddChild(pMatress);
	SAFE_RELEASE(pMatress);

	AfterClone();

	return true;
}

int CBed::Input(float fTime)
{
	return 0;
}

int CBed::Update(float fTime)
{
	return 0;
}

int CBed::LateUpdate(float fTime)
{
	return 0;
}

void CBed::Collision(float fTime)
{
}

void CBed::Render(float fTime)
{
}

CBed * CBed::Clone()
{
	return new CBed(*this);
}

void CBed::OnCollisionWithPlayer(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	CHuman_Player *pPlayer = pDest->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	if (pPlayer)
	{
		unsigned int iPlayerState = pPlayer->GetState();
		if (iPlayerState & PSTATUS_BED) 
		{
			if ((iPlayerState & PSTATUS_HIDEINTERACT) == 0)
			{
				PUN::CGameObject *pBedObj = pSrc->GetGameObject();
				pBedObj->SetFrustrumCullUse(false);
				SAFE_RELEASE(pBedObj);
				PUN::CTransform *pTr = pPlayer->GetTransform();
				PUN::CTransform *pBedTr = pSrc->GetTransform();
				Vector3 vPos = pTr->GetWorldPos();
				Vector3 vBedPos = pBedTr->GetWorldPos();

				if (vPos != vBedPos)
				{
					float fSpeed = fTime * 16.f;

					if (vPos.x > vBedPos.x)
					{
						float AfterAddVal = vPos.x - fSpeed;

						if (AfterAddVal <= vBedPos.x)
							AfterAddVal = vBedPos.x;

						vPos.x = AfterAddVal;
					}
					else if (vPos.x < vBedPos.x)
					{
						float AfterAddVal = vPos.x + fSpeed;

						if (AfterAddVal >= vBedPos.x)
							AfterAddVal = vBedPos.x;

						vPos.x = AfterAddVal;
					}

					if (vPos.y > vBedPos.y)
					{
						float AfterAddVal = vPos.y - fSpeed;

						if (AfterAddVal <= vBedPos.y)
							AfterAddVal = vBedPos.y;

						vPos.y = AfterAddVal;
					}
					else if (vPos.y < vBedPos.y)
					{
						float AfterAddVal = vPos.y + fSpeed;

						if (AfterAddVal >= vBedPos.y)
							AfterAddVal = vBedPos.y;

						vPos.y = AfterAddVal;
					}

					if (vPos.z > vBedPos.z)
					{
						float AfterAddVal = vPos.z - fSpeed;

						if (AfterAddVal <= vBedPos.z)
							AfterAddVal = vBedPos.z;

						vPos.z = AfterAddVal;
					}
					else if (vPos.z < vBedPos.z)
					{
						float AfterAddVal = vPos.z + fSpeed;

						if (AfterAddVal >= vBedPos.z)
							AfterAddVal = vBedPos.z;

						vPos.z = AfterAddVal;
					}

					pTr->SetWorldPos(vPos);
				}
				else
				{
					int aa = 0;
				}

				SAFE_RELEASE(pBedTr);
				SAFE_RELEASE(pTr);

			}
		}
		
		
		if (CInput::GetInst()->KeyRelease("E"))
		{
			

			if ((iPlayerState &PSTATUS_HIDEINTERACT) == 0)
			{
				if ((iPlayerState & PSTATUS_BED) == 0)
				{
					iPlayerState |= PSTATUS_BED;
					iPlayerState |= PSTATUS_HIDEINTERACT;
					//iPlayerState ^= PSTATUS_CROUCHED;
					//iPlayerState |= PSTATUS_CROUCHING;

					pPlayer->SetInteractRotationVector(m_pTransform->GetWorldRot());
					pPlayer->SetState(iPlayerState);
				}
			}
			
		}

		SAFE_RELEASE(pPlayer);
	}
	

}

void CBed::OnCollisionExitWithPlayer(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	PUN::CGameObject *pDestObj = pDest->GetGameObject();
	if (!pDestObj)
		return;
	
	CHuman_Player *pPlayer = pDest->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	if (pPlayer)
	{
		unsigned int iPlayerState = pPlayer->GetState();
		if (iPlayerState & PSTATUS_BED) //여러 가지 이유로 침대 안에 있는데 콜라이더만 삐져나올 경우
		{
			PUN::CTransform *pTr = pPlayer->GetTransform();
			PUN::CTransform *pBedTr = pSrc->GetTransform();

			pTr->SetWorldPos(pBedTr->GetWorldPos());


			SAFE_RELEASE(pBedTr);
			SAFE_RELEASE(pTr);
		}
		PUN::CGameObject *pBedObj = pSrc->GetGameObject();
		pBedObj->SetFrustrumCullUse(false);
		SAFE_RELEASE(pBedObj);

		SAFE_RELEASE(pPlayer);
	}

	SAFE_RELEASE(pDestObj);
}
