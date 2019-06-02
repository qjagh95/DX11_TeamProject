#include "../ClientHeader.h"
#include "NoticeMessage.h"



CNoticeMessage::CNoticeMessage()
{
	m_pAnimation = nullptr;
}

CNoticeMessage::CNoticeMessage(const CNoticeMessage & notice)	:
	CUserComponent(notice)
{
	m_pAnimation = nullptr;
}

CNoticeMessage::~CNoticeMessage()
{
	SAFE_RELEASE(m_pAnimation);
}

void CNoticeMessage::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CNoticeMessage::AfterClone()
{
}

bool CNoticeMessage::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("NMRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "NM", TEXT("UI/Tutorial/Press_Ctrl_new.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(468.f, 88.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("NMAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(468.f, 88.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Message_Empty", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Empty_Message", TEXT("UI/Tutorial/Message_Empty.png"));

	vecClipFrame.clear();

	return true;
}

int CNoticeMessage::Input(float fTime)
{
	return 0;
}

int CNoticeMessage::Update(float fTime)
{
	return 0;
}

int CNoticeMessage::LateUpdate(float fTime)
{
	return 0;
}

void CNoticeMessage::Collision(float fTime)
{
}

void CNoticeMessage::Render(float fTime)
{
}

CNoticeMessage * CNoticeMessage::Clone()
{
	return new CNoticeMessage(*this);
}
