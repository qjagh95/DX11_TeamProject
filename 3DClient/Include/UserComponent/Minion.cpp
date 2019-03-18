#include "../ClientHeader.h"
#include "Minion.h"
#include "Input.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "GameObject.h"
#include "NavigationManager3D.h"
#include "NavigationMesh.h"

CMinion3D::CMinion3D() :
	m_pAnimation(nullptr),
	m_pTargetTr(nullptr),
	m_bPathFind(false),
	m_bMove(false)
{
	m_eAI = MAI_IDLE;
}

CMinion3D::CMinion3D(const CMinion3D & com) :
	CUserComponent(com)
{
	m_eAI = com.m_eAI;

	m_pTargetTr = com.m_pTargetTr;

	m_bMove = false;
	m_bPathFind = false;
	if (m_pTargetTr)
		m_pTargetTr->AddRef();
}

CMinion3D::~CMinion3D()
{
	SAFE_RELEASE(m_pTargetTr);
	SAFE_RELEASE(m_pAnimation);
}

void CMinion3D::Start()
{
	m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldPos(38.f, 0.f, 81.f);
}

void CMinion3D::AfterClone()
{
}

bool CMinion3D::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("BlackCow", TEXT("Monster4.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetEmissive(0.f);

	SAFE_RELEASE(pMaterial);

	m_pAnimation = m_pObject->AddComponent<CAnimation>("Animation");

	m_pAnimation->LoadBone("Monster4.bne");
	m_pAnimation->Load("Monster4.anm");

	m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldPos(38.f, 0.f, 81.f);

	return true;
}

int CMinion3D::Input(float fTime)
{
	// AI 상태를 구해준다.
	m_eAI = MAI_TRACE;

	if (m_bPathFind)
	{
		if (m_vTargetPos != m_pTargetTr->GetWorldPos())
		{
			m_vTargetPos = m_pTargetTr->GetWorldPos();
			m_bPathFind = false;
		}
	}

	CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene,
		m_pTransform->GetWorldPos());

	if (pMesh)
	{
		switch (m_eAI)
		{
		case MAI_TRACE:
			if (!m_bPathFind)
			{
				float	fDist = m_pTransform->GetWorldPos().Distance(m_pTargetTr->GetWorldPos());

				if (fDist <= 30.f)
				{
					pMesh->FindPath(m_pTransform->GetWorldPos(), m_pTargetTr->GetWorldPos());
					m_bPathFind = true;

					m_PathList.clear();

					m_PathList = pMesh->GetPathList();

					m_bMove = true;
					m_vMovePos = m_PathList.front();
					m_PathList.pop_front();
				}
			}

			if (m_bMove)
			{
				Vector3	vDir = m_vMovePos - m_pTransform->GetWorldPos();
				vDir.Normalize();

				m_pTransform->LookAtY(m_vMovePos);

				m_pTransform->Move(vDir, 5.f, fTime);

				float	fDist = m_vMovePos.Distance(m_pTransform->GetWorldPos());

				if (fDist < 0.5f)
				{
					if (m_PathList.empty())
						m_bMove = false;

					else
					{
						m_vMovePos = m_PathList.front();
						m_PathList.pop_front();
					}
				}
			}
			break;
		}

		float	fY = pMesh->GetY(m_pTransform->GetWorldPos());

		Vector3	vPos = m_pTransform->GetWorldPos();
		vPos.y = fY;
		m_pTransform->SetWorldPos(vPos);
	}

	return 0;
}

int CMinion3D::Update(float fTime)
{
	return 0;
}

int CMinion3D::LateUpdate(float fTime)
{
	return 0;
}

void CMinion3D::Collision(float fTime)
{
}

void CMinion3D::Render(float fTime)
{
}

CMinion3D * CMinion3D::Clone()
{
	return new CMinion3D(*this);
}