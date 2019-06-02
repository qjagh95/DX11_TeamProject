#include "../ClientHeader.h"
#include "EventMessage.h"

CEventMessage::CEventMessage()
{
	m_pAnimation = nullptr;
	m_bPlay = false;
}

CEventMessage::CEventMessage(const CEventMessage & eventmsg)	:
	CUserComponent(eventmsg)
{
	m_pAnimation = nullptr;
}

CEventMessage::~CEventMessage()
{
	SAFE_RELEASE(m_pAnimation);
}

void CEventMessage::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CEventMessage::SetVisible(bool bEnable)
{
	m_bPlay = bEnable;

	if (m_bPlay)
	{
		m_pObject->SetEnable(true);
	}

	else
	{
		m_pObject->SetEnable(false);
	}
}

void CEventMessage::AfterClone()
{
}

bool CEventMessage::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("EMRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "EventMessage", TEXT("UI/Tutorial/Message_Empty.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldPos(520.f, 500.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("EMAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Message_Empty", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Empty_Message", TEXT("UI/Tutorial/Message_Empty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Press_Ctrl", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Ctrl_Press", TEXT("UI/Tutorial/Press_Ctrl_new.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Press_G", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"G_Press", TEXT("UI/Tutorial/Press_G_new.png"));

	vecClipFrame.clear();	

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Press_Shift", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Shift_Press", TEXT("UI/Tutorial/Press_Shift_new.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Tutorial_Mission", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Mission_Tutorial", TEXT("UI/Tutorial/TutorialMission_new.png"));

	vecClipFrame.clear();

	return true;
}

int CEventMessage::Input(float fTime)
{
	return 0;
}

int CEventMessage::Update(float fTime)
{
	return 0;
}

int CEventMessage::LateUpdate(float fTime)
{
	return 0;
}

void CEventMessage::Collision(float fTime)
{
}

void CEventMessage::Render(float fTime)
{
}

CEventMessage * CEventMessage::Clone()
{
	return new CEventMessage(*this);
}
