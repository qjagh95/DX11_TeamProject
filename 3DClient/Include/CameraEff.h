#pragma once

#include <Component/Transform.h>

struct tCameraEffects
{
	std::function<void(float, PUN::CTransform*)> funcEffect;
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
public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	
public:
	void AddUpdateEffect(const tCameraEffects& eff);
	void AddLateUpdateEffect(const tCameraEffects &eff);
	void CancelUpdateEff(std::string strName);
	void CancelLateUpdateEff(std::string strName);
	void DeleteAllCurrEff();
	void SetCamMainPos(const Vector3& vPos);
	void SetCamMainRot(const Vector3& vRot);
	void FirstPersonView(float fYMax, float fYMin, PUN::CTransform *_Body = nullptr);

public:
	void SetCamTransform(PUN::CTransform *pMainCamTr);
	static CCameraEff* GetInst();
	static void DestroyInst();
};