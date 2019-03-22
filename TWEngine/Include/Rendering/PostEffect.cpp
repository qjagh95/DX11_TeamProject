#include "../EngineHeader.h"
#include "PostEffect.h"
#include "../Device.h"
#include "../Rendering/ShaderManager.h"

PUN_USING

CPostEffect::CPostEffect()
{
}

CPostEffect::~CPostEffect()
{
}

void CPostEffect::SetAdaptation(Vector4 vAdaptation)
{
	m_tAdaptInfo.vAdaptation = vAdaptation;
}

void CPostEffect::SetAdaptation(float x, float y, float z, float w)
{
	Vector4 vAdaptation = Vector4(x, y, z, w);
	m_tAdaptInfo.vAdaptation = vAdaptation;
}

float CPostEffect::SetMiddleGrey(float fMiddleGrey)
{
	return m_fMiddleGrey = fMiddleGrey;
}

float CPostEffect::SetLumWhite(float fLumWhite)
{
	return m_fLumWhite = fLumWhite;
}

void CPostEffect::SetDownScaleCB(int iWidth, int iHeight, float fTime)
{
	m_tDownScaleCBInfo.iHeight = iHeight;
	m_tDownScaleCBInfo.iWidth = iWidth;
	m_tDownScaleCBInfo.iTotalPixels = iHeight * iWidth;
	m_tDownScaleCBInfo.iGroupSize = ceil((iWidth * iHeight / 16) / 1024.f);
}

void CPostEffect::SetFinalPassCB(float fMiddleGrey, float fLumWhite, float fTime)
{
	m_tFinalPassCBInfo.fMiddleGrey = fMiddleGrey;
	m_tFinalPassCBInfo.fLumWhite = fLumWhite;
	m_tFinalPassCBInfo.fMiddleGrey *= m_tFinalPassCBInfo.fMiddleGrey;
	m_tFinalPassCBInfo.fLumWhite *= m_tFinalPassCBInfo.fLumWhite;
}

void CPostEffect::SetAdaptationCB(Vector4 vAdaptation, float fTime)
{
	m_tAdaptInfo.vAdaptation = vAdaptation;
}

void CPostEffect::UpdateCBuffer(int iPass)
{
	switch (iPass)
	{
	case 0:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("Adaptation", &m_tAdaptInfo);
		GET_SINGLE(CShaderManager)->UpdateCBuffer("DownScale", &m_tDownScaleCBInfo);

		break;
	case 1:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("FinalPass", &m_tFinalPassCBInfo);

		break;
	}
}

const Vector4& CPostEffect::GetAdaptation() const
{
	return m_tAdaptInfo.vAdaptation;
}

bool CPostEffect::Init()
{
	m_tAdaptInfo.vAdaptation = Vector4(1.f, 1.f, 1.f, 1.f);
	m_fHeight = _RESOLUTION.iHeight;
	m_fWidth = _RESOLUTION.iWidth;

	m_fMiddleGrey = 0.863f;
	m_fLumWhite = 1.53f;

	return true;
}

int CPostEffect::Update(float fTime)
{
	return 0;
}