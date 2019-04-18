#pragma once
#include "Component/UserComponent.h"
#include "Component/UIBar.h"
#include "../Client.h"

PUN_USING

class CHandycam :
	public CUserComponent
{
	friend class CGameObject;

private:
	CHandycam();
	CHandycam(const CHandycam& handycam);
	virtual ~CHandycam();

private:
	class CGameObject*	m_pBarObj;
	CUIBar*		m_pBar;
	class CMaterial*	m_pBarMtrl;
	class CPostEffect*	m_pPostEffect;
	int		m_iFlag;
	bool	m_bVisible;
	bool	m_bOutFlag;
	bool	m_bBatteryOut;
	bool	m_bChangeFlag;
	bool	m_bChange;
	bool	m_bCantUse;
	float	m_fBatteryTime;

public:
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetOutFlag(bool bFlag);
	void SetChangeFlag(bool bFlag);
	void SetCantUse(bool bCantUse);
	void DisableHandycam();
	void SetBatteryTime(float fBatteryTime);
	void ChangeBattery(bool bFlag);

public:
	bool	GetBatteryOut()	const;
	bool	GetBatteryChange()	const;
	bool	GetVisible()	const;
	bool GetCantUse()	const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHandycam* Clone();
};

