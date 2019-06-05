#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CAnimation2D :
	public CComponent
{
	friend class CGameObject;

public:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& anim);
	~CAnimation2D();

private:
	unordered_map<string, PAnimationClip2D> m_mapClip;
	PAnimationClip2D	m_pCurClip;
	string				m_strDefaultClip;
	class CRenderer*	m_pRenderer;

public:
	void AddClip(const string& strName, ANIMATION2D_TYPE eType,
		ANIMATION_OPTION eOption, float fPlayLimitTime,
		const vector<Clip2DFrame>& vecFrame, const string& strTexKey,
		const TCHAR* pFileName, const string& strPathKey = TEXTURE_PATH);
	void AddClip(const string& strName, ANIMATION2D_TYPE eType,
		ANIMATION_OPTION eOption, float fPlayLimitTime,
		const vector<Clip2DFrame>& vecFrame, const string& strTexKey,
		const vector<const TCHAR*>& vecFileName, const string& strPathKey = TEXTURE_PATH);
	void SetDefaultClip(const string& strClip);
	void ChangeClip(const string& strName);
	void ResetClip();
	
private:
	PAnimationClip2D FindClip(const string& strName);

public:
	void CurrentClipJumpToFrame(int iFrame);
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CAnimation2D* Clone();
};

PUN_END