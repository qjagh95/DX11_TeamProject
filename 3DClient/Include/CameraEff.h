#pragma once

#include <Component/Transform.h>

struct tCameraEffects
{
	std::function<void(float, float, float ,PUN::CTransform*)> funcEffect;
	float fEffectTime;
	float fEffectCurrTime = 0.f;
	std::string strName;
};

class CCameraEff
{
	
private:
	CCameraEff();
	~CCameraEff();

private:
	static CCameraEff* m_pInst;
	PUN::CTransform *pMainCamTransform;
	std::list<tCameraEffects> m_listUpdateEffects;

	std::list<tCameraEffects> m_listLateUpdateEffects;
	HWND hWnd;
	RECT gameWnd;
	RECT windowWnd;
	bool m_bWndFocused;
	bool m_bUseFirstPersonView;
public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void SetFirstPersonViewEnable();
	bool IsFirstPersonEnabled() const;
public:
	//tCameraEffect -> float(델타타임), float(현재효과시간), float(끝효과시간), Transform(카메라)
	void AddUpdateEffect(const tCameraEffects& eff);
	//tCameraEffect -> float(델타타임), float(현재효과시간), float(끝효과시간), Transform(카메라)
	void AddLateUpdateEffect(const tCameraEffects &eff);
	void CancelUpdateEff(std::string strName);
	void CancelLateUpdateEff(std::string strName);
	void DeleteAllCurrEff();
	void SetCamMainPos(const Vector3& vPos);
	void SetCamMainRot(const Vector3& vRot);
	float FirstPersonView(float fYMax, float fYMin, PUN::CTransform *_Body = nullptr, const Vector3& vEyePos = Vector3::Zero, const Vector3 &vEyeRot = Vector3::Zero);

public:
	void SetCamTransform(PUN::CTransform *pMainCamTr);
	static CCameraEff* GetInst();
	static void DestroyInst();
};