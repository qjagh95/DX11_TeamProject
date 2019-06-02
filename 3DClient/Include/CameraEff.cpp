#include "ClientHeader.h"
#include "CameraEff.h"
#include <Scene/Scene.h>
#include <Component/Camera.h>
#include <Device.h>
#include <Core.h>
#include <Input.h>
#include <SoundManager.h>

CCameraEff* CCameraEff::m_pInst = nullptr;

CCameraEff::CCameraEff():
	pMainCamTransform(nullptr),
	m_bWndFocused(false),
	m_bUseFirstPersonView(false)
{
}

CCameraEff::~CCameraEff()
{
}

bool CCameraEff::Init()
{
	hWnd = PUN::CCore::GetInst()->GetWindowHandle();
	GetClientRect(hWnd, &gameWnd);
	return true;
}

int CCameraEff::Input(float fTime)
{
	

	return 0;
}

int CCameraEff::Update(float fTime)
{
	std::list<tCameraEffects>::iterator listItrEnd = m_listUpdateEffects.end();

	for (std::list<tCameraEffects>::iterator itr = m_listUpdateEffects.begin(); itr != listItrEnd;)
	{
		if ((*itr).fEffectCurrTime >= (*itr).fEffectTime)
		{
			itr = m_listUpdateEffects.erase(itr);
		}
		if (itr == listItrEnd)
			break;

		(*itr).funcEffect(fTime, (*itr).fEffectCurrTime, (*itr).fEffectTime, pMainCamTransform);
		(*itr).fEffectCurrTime += fTime;

		++itr;
	}
	return 0;
}

int CCameraEff::LateUpdate(float fTime)
{
	std::list<tCameraEffects>::iterator listItrEnd = m_listLateUpdateEffects.end();

	for (std::list<tCameraEffects>::iterator itr = m_listLateUpdateEffects.begin(); itr != listItrEnd;)
	{
		if ((*itr).fEffectCurrTime >= (*itr).fEffectTime)
		{
			itr = m_listLateUpdateEffects.erase(itr);
		}
		if (itr == listItrEnd)
			break;
		(*itr).funcEffect(fTime, (*itr).fEffectCurrTime, (*itr).fEffectTime, pMainCamTransform);
		(*itr).fEffectCurrTime += fTime;

		++itr;
	}
	return 0;
}

void CCameraEff::SetFirstPersonViewEnable()
{
	m_bUseFirstPersonView = !m_bUseFirstPersonView;

	if (m_bUseFirstPersonView)
	{
		PUN::CSoundManager::GetInst()->SetListenerTransform(pMainCamTransform);
	}
}

bool CCameraEff::IsFirstPersonEnabled() const
{
	return m_bUseFirstPersonView;
}

void CCameraEff::AddUpdateEffect(const tCameraEffects & eff)
{
	m_listUpdateEffects.push_back(eff);
}

void CCameraEff::AddLateUpdateEffect(const tCameraEffects & eff)
{
	m_listLateUpdateEffects.push_back(eff);
}

void CCameraEff::CancelUpdateEff(std::string strName)
{
	std::list<tCameraEffects>::iterator listItrEnd = m_listUpdateEffects.end();

	for (std::list<tCameraEffects>::iterator itr = m_listUpdateEffects.begin(); itr != listItrEnd;)
	{
		if ((*itr).strName == strName)
		{
			m_listUpdateEffects.erase(itr);
			break;
		}
			
	}
}

void CCameraEff::CancelLateUpdateEff(std::string strName)
{
	std::list<tCameraEffects>::iterator listItrEnd = m_listLateUpdateEffects.end();

	for (std::list<tCameraEffects>::iterator itr = m_listLateUpdateEffects.begin(); itr != listItrEnd;)
	{
		if ((*itr).strName == strName)
		{
			m_listLateUpdateEffects.erase(itr);
			break;
		}

	}
}

void CCameraEff::DeleteAllCurrEff()
{
	m_listUpdateEffects.clear();
	m_listLateUpdateEffects.clear();
}

void CCameraEff::SetCamMainPos(const Vector3 & vPos)
{
	if (pMainCamTransform)
		pMainCamTransform->SetWorldPos(vPos);
}

void CCameraEff::SetCamMainRot(const Vector3 & vRot)
{
	if (pMainCamTransform)
		pMainCamTransform->SetWorldRot(vRot);
}

float CCameraEff::FirstPersonView(float fYMax, float fYMin, PUN::CTransform *_Body, const Vector3& vEyePos, const Vector3 &vEyeRot)
{
	if (!m_bUseFirstPersonView)
		return 0.f;

	if (pMainCamTransform)
	{
		pMainCamTransform->SetWorldPos(vEyePos);
		//PUN::CSoundManager::GetInst()->SetListenerTransform(pMainCamTransform);
	}
	PUN::CInput *_Input = PUN::CInput::GetInst();
	if (!_Input->IsFocused())
	{
		m_bWndFocused = false;
		return false;
	}
	else
	{
		if (!m_bWndFocused)
		{
			GetClientRect(hWnd, &gameWnd);
			GetWindowRect(hWnd, &windowWnd);
			
			//윈도우 윗단 제거하기 : ClentRect
			int iWindowMenuThickness = (int)(windowWnd.bottom - windowWnd.top) - (int)gameWnd.bottom;

			int iXCenter = ((int)windowWnd.left + (int)windowWnd.right) >> 1;
			int iYCenter = ((gameWnd.bottom - gameWnd.top) >> 1) + (int)windowWnd.top + iWindowMenuThickness;

			SetCursorPos(iXCenter, iYCenter);
			m_bWndFocused = true;
		}
	}
	GetClientRect(hWnd, &gameWnd);
	GetWindowRect(hWnd, &windowWnd);
	//윈도우 윗단 제거하기 : ClentRect
	int iWindowMenuThickness = (int)(windowWnd.bottom - windowWnd.top) - (int)gameWnd.bottom;

	int iXCenter = ((int)windowWnd.left + (int)windowWnd.right) >> 1;
	int iYCenter = ((gameWnd.bottom - gameWnd.top) >> 1) + (int)windowWnd.top + iWindowMenuThickness;

	POINT _curMousePoint = {};
	GetCursorPos(&_curMousePoint);

	int iXPoint = (int)_curMousePoint.x - iXCenter;
	int iYPoint = (int)_curMousePoint.y - iYCenter;

	int iTag = iXPoint | iYPoint;

	if (pMainCamTransform)
	{
		Vector3 vCamRot = pMainCamTransform->GetWorldRot();
		Vector3 vBodRot = _Body->GetWorldRot();

		if (iTag)
		{
			SetCursorPos(iXCenter, iYCenter);
			float fValueW = 180.f / PUN::CDevice::GetInst()->GetResolution().iWidth;
			float fValueH = 180.f / PUN::CDevice::GetInst()->GetResolution().iHeight;
			float fAngleX = ((float)iXPoint) * fValueW;
			float fAngleY = ((float)iYPoint) * fValueH;
			_Body->RotationY(fAngleX);

			Vector3 vCamRot = _Body->GetWorldRot();
			vCamRot.x = pMainCamTransform->GetWorldRot().x;
			vCamRot.z = 0.f;
			pMainCamTransform->SetWorldRot(vCamRot);
			float fNewAngleY = fAngleY + vCamRot.x;
			if (fNewAngleY < fYMax && fNewAngleY > fYMin)
			{
				pMainCamTransform->RotationX(fAngleY);
			}

			return fAngleX;
		}

	}

	return 0.f;
}

void CCameraEff::SetCamTransform(PUN::CTransform * pMainCamTr)
{
	pMainCamTransform = pMainCamTr;
	PUN::CSoundManager::GetInst()->SetListenerTransform(pMainCamTransform);
}

CCameraEff * CCameraEff::GetInst()
{
	if (!m_pInst)
		m_pInst = new CCameraEff;
		
	return m_pInst;
}

void CCameraEff::DestroyInst()
{
	delete m_pInst;
	m_pInst = nullptr;
}
