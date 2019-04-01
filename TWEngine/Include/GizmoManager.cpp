#include "EngineHeader.h"
#include "GizmoManager.h"
#include "Component/Gizmo.h"


PUN_USING

CGizmoManager* CGizmoManager::m_pInst = nullptr;



CGizmoManager::CGizmoManager()
	: m_pXGizmo(nullptr),
	  m_pYGizmo(nullptr),
	  m_pZGizmo(nullptr)
{
}


CGizmoManager::~CGizmoManager()
{
}

void CGizmoManager::GizmoRender(float fTime)
{
	if (m_pXGizmo != nullptr)
	{
		m_pXGizmo->Render(fTime);
	}

	if (m_pYGizmo != nullptr)
	{
		m_pYGizmo->Render(fTime);
	}


	if (m_pZGizmo != nullptr)
	{
		m_pZGizmo->Render(fTime);
	}
}

void CGizmoManager::SetXGizmo(CGizmo * _pGizmo)
{
	m_pXGizmo = _pGizmo;
}

void CGizmoManager::SetYGizmo(CGizmo * _pGizmo)
{
	m_pYGizmo = _pGizmo;
}

void CGizmoManager::SetZGizmo(CGizmo * _pGizmo)
{
	m_pZGizmo = _pGizmo;
}