#include "EngineHeader.h"
#include "UI.h"
#include "Renderer.h"
#include "../GameObject.h"

PUN_USING

CUI::CUI()
{
	m_eComType = CT_UI;
	SetTag("UI");
}

CUI::CUI(const CUI & ui) :
	CComponent(ui)
{
	m_eUIType = ui.m_eUIType;
}

CUI::~CUI()
{
}

void CUI::Start()
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);
}

void CUI::AfterClone()
{
}

bool CUI::Init()
{
	return true;
}

int CUI::Input(float fTime)
{
	return 0;
}

int CUI::Update(float fTime)
{
	return 0;
}

int CUI::LateUpdate(float fTime)
{
	return 0;
}

void CUI::Collision(float fTime)
{
}

void CUI::Render(float fTime)
{
}

