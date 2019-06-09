#include "../ClientHeader.h"
#include "Document.h"
#include "Device.h"

CDocument::CDocument()	:
	m_pAnimation(nullptr)
{
	m_eComType = (COMPONENT_TYPE)IT_DOCXICON;
}

CDocument::CDocument(const CDocument & doc)	:
	CUserComponent(doc)
{
}

CDocument::~CDocument()
{
	SAFE_RELEASE(m_pAnimation);
}

void CDocument::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CDocument::AfterClone()
{
}

bool CDocument::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DocRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Document", TEXT("UI/Document/MobileMessage.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(900.f, 900.f, 1.f);
	m_pTransform->SetWorldPos(_RESOLUTION.iWidth * (1.f - (1 - 830.f / 1280.f)), 
		_RESOLUTION.iHeight * (1.f - (1 - 350.f / 720.f)), 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("DocAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800, 600);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Message_Empty", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Empty_Message", TEXT("UI/Document/MessageEmpty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("MobileMessage", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"MessageMobile", TEXT("UI/Document/PH.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("SilentMessage", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"MessageSilent", TEXT("UI/Document/KeepSilence720.png"));

	vecClipFrame.clear();

	// Stage 1
	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Generator_Docx", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Docx_Generator", TEXT("UI/Document/Stage1/Generator720.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Gongji_Docx", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Docx_Gongji", TEXT("UI/Document/Stage1/Gongji720.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Message_TH720", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"TH720_Message", TEXT("UI/Document/Stage1/Message_TH720.png"));

	vecClipFrame.clear();

	// Stage 2

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Plan_A720", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"720Plan_A", TEXT("UI/Document/Stage2/Plan_A720.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("TestDocx720", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"720TestDocx", TEXT("UI/Document/Stage2/TestDocx720.png"));

	vecClipFrame.clear();

	// Stage 3

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("BeQuiet720", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"720BeQuiet", TEXT("UI/Document/Stage3/BeQuiet720.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Prepare_Lever720", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"720Prepare_Lever", TEXT("UI/Document/Stage3/Prepare_Lever720.png"));

	vecClipFrame.clear();

	return true;
}

int CDocument::Input(float fTime)
{
	return 0;
}

int CDocument::Update(float fTime)
{
	return 0;
}

int CDocument::LateUpdate(float fTime)
{
	return 0;
}

void CDocument::Collision(float fTime)
{
}

void CDocument::Render(float fTime)
{
}

CDocument * CDocument::Clone()
{
	return new CDocument(*this);
}
