#include "stdafx.h"
#include "Animation2D.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Texture.h"
#include "../PathManager.h"
#include "../GameObject.h"
#include "Renderer.h"
#include "Material.h"

PUN_USING

CAnimation2D::CAnimation2D() :
	m_pCurClip(nullptr)
{
	m_eComType = CT_ANIMATION2D;
	
}

CAnimation2D::CAnimation2D(const CAnimation2D & anim) :
	CComponent(anim)
{
	m_strDefaultClip = anim.m_strDefaultClip;

	m_mapClip.clear();

	unordered_map<string, PAnimationClip2D>::const_iterator	iter;
	unordered_map<string, PAnimationClip2D>::const_iterator	iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PAnimationClip2D	pClip = new AnimationClip2D;

		*pClip = *(iter->second);

		pClip->pTexture->AddRef();

		m_mapClip.insert(make_pair(iter->first, pClip));

		if (anim.m_pCurClip->strName == iter->first)
		{
			m_pCurClip = pClip;
		}
	}
}

CAnimation2D::~CAnimation2D()
{
	unordered_map<string, PAnimationClip2D>::iterator	iter;
	unordered_map<string, PAnimationClip2D>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pTexture);
		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

void CAnimation2D::AddClip(const string & strName,
	ANIMATION2D_TYPE eType, ANIMATION_OPTION eOption,
	float fPlayLimitTime, const vector<Clip2DFrame>& vecFrame,
	const string & strTexKey, const TCHAR * pFileName,
	const string & strPathKey)
{
	PAnimationClip2D	pClip = FindClip(strName);

	if (pClip)
		return;

	pClip = new AnimationClip2D;

	pClip->strName = strName;
	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->fPlayLimitTime = fPlayLimitTime;
	pClip->vecFrame = vecFrame;
	pClip->eType = eType;
	pClip->eType = eType;
	pClip->iFrame = 0;
	pClip->fPlayTime = 0.f;

	GET_SINGLE(CResourcesManager)->CreateTexture(strTexKey,
		pFileName, strPathKey);
	pClip->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);
	pClip->iTexRegister = 0;
	pClip->fTextureWidth = pClip->pTexture->GetWidth();
	pClip->fTextureHeight = pClip->pTexture->GetHeight();

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip == "")
	{
		m_strDefaultClip = strName;
	}

	if (!m_pCurClip)
		m_pCurClip = pClip;
}

void CAnimation2D::AddClip(const string & strName, ANIMATION2D_TYPE eType,
	ANIMATION_OPTION eOption, float fPlayLimitTime,
	const vector<Clip2DFrame>& vecFrame, const string & strTexKey,
	const vector<const TCHAR*>& vecFileName, const string & strPathKey)
{
	PAnimationClip2D	pClip = FindClip(strName);

	if (pClip)
		return;

	pClip = new AnimationClip2D;

	pClip->strName = strName;
	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->fPlayLimitTime = fPlayLimitTime;
	pClip->vecFrame = vecFrame;
	pClip->eType = eType;
	pClip->eType = eType;
	pClip->iFrame = 0;
	pClip->fPlayTime = 0.f;
	pClip->iTexRegister = 1;

	GET_SINGLE(CResourcesManager)->CreateTexture(strTexKey,
		vecFileName, strPathKey);
	pClip->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);
	pClip->fTextureWidth = pClip->pTexture->GetWidth();
	pClip->fTextureHeight = pClip->pTexture->GetHeight();

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip == "")
	{
		m_strDefaultClip = strName;
	}

	if (!m_pCurClip)
		m_pCurClip = pClip;
}

void CAnimation2D::SetDefaultClip(const string & strClip)
{
	m_strDefaultClip = strClip;
}

void CAnimation2D::ChangeClip(const string & strName)
{
	if (m_pCurClip->strName == strName)
		return;

	m_pCurClip = FindClip(strName);

	m_pCurClip->iFrame = 0;
	m_pCurClip->fPlayTime = 0.f;

	CMaterial*	pMaterial = FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (pMaterial)
	{
		pMaterial->SetDiffuseTex(m_pCurClip->iTexRegister, m_pCurClip->pTexture);
		SAFE_RELEASE(pMaterial);
	}
}

PAnimationClip2D CAnimation2D::FindClip(const string & strName)
{
	unordered_map<string, PAnimationClip2D>::iterator	iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

void CAnimation2D::Start()
{
	CMaterial*	pMaterial = FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (pMaterial)
	{
		pMaterial->SetDiffuseTex(m_pCurClip->iTexRegister, m_pCurClip->pTexture);
		SAFE_RELEASE(pMaterial);
	}
}

void CAnimation2D::AfterClone()
{
}

bool CAnimation2D::Init()
{
	return true;
}

int CAnimation2D::Input(float fTime)
{
	return 0;
}

int CAnimation2D::Update(float fTime)
{
	return 0;
}

int CAnimation2D::LateUpdate(float fTime)
{
	m_pCurClip->fPlayTime += fTime;
	float	fFrameTime = m_pCurClip->fPlayLimitTime / m_pCurClip->vecFrame.size();

	while (m_pCurClip->fPlayTime >= fFrameTime)
	{
		m_pCurClip->fPlayTime -= fFrameTime;
		++m_pCurClip->iFrame;

		if (m_pCurClip->iFrame >= m_pCurClip->vecFrame.size())
		{
			m_pCurClip->iFrame = 0;
			if (m_pCurClip->eOption == AO_ONCE_DESTROY)
			{
				m_pCurClip->iFrame = 0;
				m_pObject->Die();
				break;
			}

			if (m_pCurClip->eOption == AO_ONCE_RETURN)
			{
				m_pCurClip->iFrame = 0;
				ChangeClip(m_strDefaultClip);
				break;
			}

			if (m_pCurClip->eOption == AO_ONCE_LAST)
			{
				m_pCurClip->iFrame = m_pCurClip->vecFrame.size() - 1;
				break;
			}
		}
	}

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		Animation2DCBuffer	tCBuffer = {};

		tCBuffer.iFrame = m_pCurClip->iFrame;

		pRenderer->UpdateAnimationType(m_pCurClip->eType);

		if (m_pCurClip->eType == A2D_ATLAS)
		{
			Clip2DFrame&	tFrame = m_pCurClip->vecFrame[m_pCurClip->iFrame];
			tCBuffer.vUVLT.x = tFrame.vLT.x / m_pCurClip->fTextureWidth;
			tCBuffer.vUVLT.y = tFrame.vLT.y / m_pCurClip->fTextureHeight;
			tCBuffer.vUVRB.x = tFrame.vRB.x / m_pCurClip->fTextureWidth;
			tCBuffer.vUVRB.y = tFrame.vRB.y / m_pCurClip->fTextureHeight;
		}

		else
		{
			tCBuffer.vUVLT = Vector2(0.f, 0.f);
			tCBuffer.vUVRB = Vector2(1.f, 1.f);
		}

		pRenderer->UpdateRendererCBuffer("Animation2D", &tCBuffer,
			sizeof(Animation2DCBuffer));

		SAFE_RELEASE(pRenderer);
	}

	return 0;
}

void CAnimation2D::Collision(float fTime)
{
}

void CAnimation2D::Render(float fTime)
{
}

CAnimation2D * CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}