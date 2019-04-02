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

void CPostEffect::SetAdaptation(float fAdaptation)
{
	m_tAdaptInfo.fAdaptation = fAdaptation;
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
	m_tDownScaleCBInfo.iGroupSize = (int)ceil((iWidth * iHeight / 16) / 1024.f);
}

void CPostEffect::SetFinalPassCB(float fMiddleGrey, float fLumWhite, float fTime)
{
	m_tFinalPassCBInfo.fMiddleGrey = fMiddleGrey;
	m_tFinalPassCBInfo.fLumWhite = fLumWhite;
	m_tFinalPassCBInfo.fMiddleGrey *= m_tFinalPassCBInfo.fMiddleGrey;
	m_tFinalPassCBInfo.fLumWhite *= m_tFinalPassCBInfo.fLumWhite;
}

void CPostEffect::SetAdaptationCB(float fAdaptation, float fTime)
{
	m_tAdaptInfo.fAdaptation = fAdaptation;
	//GET_SINGLE(CShaderManager)->UpdateCBuffer("Adaptation", &m_tAdaptInfo);
}

void CPostEffect::SetAdaptationCB(float fAdaptation, float fBloomThreshold, float fTime)
{
	m_tAdaptInfo.fAdaptation = fAdaptation;
}

void CPostEffect::SetBloomThresholdCB(float fThreshold, float fTime)
{
	m_tBloomThresholdInfo.fBloomThreshold = fThreshold;
}

void CPostEffect::SetBloomCB(float fBloomScale, float fTime)
{
	m_tBloomInfo.fBloomScale = fBloomScale;
}

void CPostEffect::UpdateCBuffer(int iPass)
{
	switch (iPass)
	{
	case 0:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("HDRFirst", &m_tDownScaleCBInfo);

		break;
	case 1:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("HDRSecond", &m_tFinalPassCBInfo);

		break;
	case 2:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("Adaptation",	&m_tAdaptInfo);
		break;
	case 3:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("BloomThreshold", &m_tBloomThresholdInfo);
		break;
	case 4:
		GET_SINGLE(CShaderManager)->UpdateCBuffer("Bloom", &m_tBloomInfo);
		break;
	}
}

bool CPostEffect::Init()
{
	m_tAdaptInfo.fAdaptation = 1.f;
	m_fHeight = (float)(_RESOLUTION.iHeight / 4);
	m_fWidth = (float)(_RESOLUTION.iWidth / 4);

	m_fMiddleGrey = 0.863f;
	m_fLumWhite = 1.53f;

	m_tDownScaleCBInfo.iHeight = (int)m_fHeight;
	m_tDownScaleCBInfo.iWidth = (int)m_fWidth;
	m_tDownScaleCBInfo.iTotalPixels = (int)(m_fHeight * m_fWidth);
	m_tDownScaleCBInfo.iGroupSize = (int)ceil(m_fHeight * m_fWidth / 1024.0f);

	m_tFinalPassCBInfo.fLumWhite = m_fLumWhite * m_fLumWhite;
	m_tFinalPassCBInfo.fMiddleGrey = m_fMiddleGrey * m_fMiddleGrey;

	m_tBloomThresholdInfo.fBloomThreshold = 1.1f;

	return true;
}

int CPostEffect::Update(float fTime)
{
	return 0;
}