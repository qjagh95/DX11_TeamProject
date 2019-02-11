#pragma once
#include "../EngineHeader.h"
#include "Component.h"

PUN_BEGIN

typedef struct PUN_DLL _tagBone
{
	string		strName;
	int			iDepth;
	int			iParentIndex;
	Matrix*		matOffset;
	Matrix*		matBone;
	list<class CBoneSocket*>	SocketList;
}BONE, *PBONE;

typedef struct PUN_DLL _tagKeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
}KEYFRAME, *PKEYFRAME;

typedef struct PUN_DLL _tagBoneKeyFrame
{
	int		iBoneIndex;
	vector<PKEYFRAME>	vecKeyFrame;

	_tagBoneKeyFrame()
	{
	}

	~_tagBoneKeyFrame()
	{
		Safe_Delete_VecList(vecKeyFrame);
	}
}BONEKEYFRAME, *PBONEKEYFRAME;

typedef struct PUN_DLL _tagAnimationCallback
{
	int		iAnimationProgress;
	float	fAnimationProgress;
	function<void(float)> func;
	bool	bCall;
}ANIMATIONCALLBACK, *PANIMATIONCALLBACK;

typedef struct PUN_DLL _tagAnimationClip
{
	ANIMATION_OPTION	eOption;
	string				strName;
	float				fStartTime;
	float				fEndTime;
	float				fTimeLength;
	int					iStartFrame;
	int					iEndFrame;
	int					iFrameLength;
	int					iFrameMode;
	int					iAnimationLimitFrame;
	vector<PBONEKEYFRAME>		vecKeyFrame;
	vector<PANIMATIONCALLBACK>	vecCallback;

	_tagAnimationClip() :
		eOption(AO_LOOP),
		strName(""),
		iFrameMode(0),
		iAnimationLimitFrame(0),
		fStartTime(0),
		fEndTime(0),
		fTimeLength(0),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0)
	{
	}

	~_tagAnimationClip()
	{
		Safe_Delete_VecList(vecKeyFrame);
		Safe_Delete_VecList(vecCallback);
	}
}ANIMATIONCLIP, *PANIMATIONCLIP;

class PUN_DLL CAnimation
	:public CComponent
{
	friend class CGameObject;
	friend class CMesh;

public:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	vector<PBONE>		m_vecBones;
	ID3D11Texture2D*	m_pBoneTex;
	ID3D11ShaderResourceView*	m_pBoneRV;
	unordered_map<string, PANIMATIONCLIP>	m_mapClip;
	PANIMATIONCLIP			m_pCurClip;
	PANIMATIONCLIP			m_pNextClip;

public:
	void AddBone(PBONE pBone);
	bool CreateBoneTexture();
	void AddClip(ANIMATION_OPTION eOption,
		struct _tagFbxAnimationClip* pClip);

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CAnimation* Clone();
};

PUN_END

