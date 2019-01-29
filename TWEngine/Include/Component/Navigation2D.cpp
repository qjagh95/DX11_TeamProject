#include "stdafx.h"
#include "Navigation2D.h"
#include "../NavigationManager.h"
#include "Transform.h"

PUN_USING

CNavigation2D::CNavigation2D()
{
	m_bMove = false;
	m_eComType = CT_NAVI2D;
}

CNavigation2D::CNavigation2D(const CNavigation2D & navi) :
	CComponent(navi)
{
}

CNavigation2D::~CNavigation2D()
{
}

bool CNavigation2D::Find(const Vector3 & vStart, const Vector3 & vEnd)
{
 	if (GET_SINGLE(CNavigationManager)->Find2D(vStart, vEnd))
	{
		m_PathList.clear();
		m_PathList = GET_SINGLE(CNavigationManager)->GetPathList();

		m_bMove = true;

		_cprintf("PushList : %d\n", m_PathList.size());

		m_vTargetPos = m_PathList.front();
		m_PathList.pop_front();
	}

	return true;
}

void CNavigation2D::AutoFind(bool bAuto)
{
	m_bAutoFind = bAuto;
}

void CNavigation2D::Start()
{
}

void CNavigation2D::AfterClone()
{
}

bool CNavigation2D::Init()
{
	return true;
}

int CNavigation2D::Input(float fTime)
{
	return 0;
}

int CNavigation2D::Update(float fTime)
{
	if (m_bAutoFind && m_bMove)
	{
		Vector3	vDir = m_vTargetPos - m_pTransform->GetWorldPos();
		vDir.Normalize();		

		m_pTransform->Move(vDir, m_fSpeed, fTime);

		float fDist = m_vTargetPos.Distance(m_pTransform->GetWorldPos());

		if (fDist <= 10.f)
		{
			if (!m_PathList.empty())
			{
				m_vTargetPos = m_PathList.front();
				m_PathList.pop_front();
			}

			else
			{
				m_bMove = false;
			}
		}
	}

	return 0;
}

int CNavigation2D::LateUpdate(float fTime)
{
	return 0;
}

void CNavigation2D::Collision(float fTime)
{
}

void CNavigation2D::Render(float fTime)
{
}

CNavigation2D * CNavigation2D::Clone()
{
	return new CNavigation2D(*this);
}
