#include "../ClientHeader.h"
#include "Ghost.h"
#include "Input.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include <SoundManager.h>

CGhost::CGhost()
{
	m_eComType = (COMPONENT_TYPE)UT_MONSTER;
	m_pAnimation = nullptr;
	m_fPlayTime = 0.f;
	m_fVisableTime = 0.f;
}

CGhost::CGhost(const CGhost & com)	:
	CUserComponent(com)
{
	m_pAnimation = nullptr;
	m_fPlayTime = com.m_fPlayTime;
	m_fVisableTime = com.m_fVisableTime;
}

CGhost::~CGhost()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pTransform);
}

void CGhost::Start()
{
}

void CGhost::AfterClone()
{
	CSoundManager::GetInst()->SoundPlay("scream");
}

bool CGhost::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("scream", TEXT("KKamNol.wav"));
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("GhostRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->Enable2DRenderer();
	pRenderer->SetDecalEnable(false);

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	m_pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	m_pMaterial->SetDiffuseTex(0, "Ghost_Head", TEXT("Ghost/Ghost_WomanHead_Attack04.png"));
	m_pMaterial->SetSampler(0, SAMPLER_LINEAR);

	//m_pTransform->SetWorldScale(2048.f, 2048.f, 1.f);
	m_pTransform->SetWorldScale(1.f, 1.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);
	m_pTransform->SetWorldPos(500.f, -500.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("GhostAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	/*for (int i = 0; i < 3; ++i)
	{
		tFrame.vLT = Vector2(0.f + i * 2048.f, 0.f);
		tFrame.vRB = Vector2(2048.f + i * 2048.f, 2048.f);
		vecClipFrame.push_back(tFrame);
	}*/

		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(2048.f, 2048.f);
		vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Ghost_Head", A2D_ATLAS, AO_ONCE_DESTROY, 2.f, vecClipFrame,
		"Head_Ghost", TEXT("Ghost/Ghost_WomanHead_Attack04.png"));

	vecClipFrame.clear();

	m_fTime = 2.f;

	AfterClone();

	return true;
}

int CGhost::Input(float fTime)
{
	return 0;
}

int CGhost::Update(float fTime)
{
	m_fTime -= fTime;
	m_fPlayTime += fTime;

	if (m_fPlayTime >= 2.f)
	{
		m_fVisableTime += fTime;

		//m_pMaterial->SetMaterial2D(Vector4(1.f, 1.f, 1.f, 1.f - m_fVisableTime));
	}

	if (m_fTime < 0.f)
	{
		m_fPlayTime = 0.f;
	}

	return 0;
}

int CGhost::LateUpdate(float fTime)
{
	return 0;
}

void CGhost::Collision(float fTime)
{
}

void CGhost::Render(float fTime)
{
}

CGhost * CGhost::Clone()
{
	return nullptr;
}
