#include "../EngineHeader.h"
#include "Particle.h"
#include "Camera.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Scene/Scene.h"
#include "../Rendering/ShaderManager.h"

PUN_USING

CParticle::CParticle()
{
	SetTag("Particle");
	m_eComType = CT_PARTICLE;
}

CParticle::CParticle(const CParticle & particle) :
	CComponent(particle)
{
}

CParticle::~CParticle()
{
}

void CParticle::Start()
{
}

void CParticle::AfterClone()
{
}

bool CParticle::Init()
{
	m_pObject->SetRenderGroup(RG_ALPHA3);

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("ParticleRenderer");

	pRenderer->SetMesh("Particle");
	pRenderer->CreateRendererCBuffer("Particle", sizeof(ParticleCBuffer));

	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);

	SAFE_RELEASE(pRenderer);

	return true;
}

int CParticle::Input(float fTime)
{
	return 0;
}

int CParticle::Update(float fTime)
{
	return 0;
}

int CParticle::LateUpdate(float fTime)
{
	m_tCBuffer.vCenter = m_pTransform->GetWorldPos();

	m_tCBuffer.fSizeX = m_pTransform->GetWorldScale().x;
	m_tCBuffer.fSizeY = m_pTransform->GetWorldScale().y;

	CTransform*	pCameraTr = m_pScene->GetMainCameraTransform();

	m_tCBuffer.vAxisX = pCameraTr->GetWorldAxis(AXIS_X);
	m_tCBuffer.vAxisY = pCameraTr->GetWorldAxis(AXIS_Y);

	SAFE_RELEASE(pCameraTr);

	CRenderer*	pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Particle", &m_tCBuffer, sizeof(ParticleCBuffer));

	SAFE_RELEASE(pRenderer);

	return 0;
}

void CParticle::Collision(float fTime)
{
}

void CParticle::Render(float fTime)
{
}

CParticle * CParticle::Clone()
{
	return new CParticle(*this);
}