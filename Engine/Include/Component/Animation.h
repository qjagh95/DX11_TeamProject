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
	int			iRefCount;
	list<class CBoneSocket*>	SocketList;

	_tagBone()
		:iRefCount(1)
	{

	}
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
	int     iRefCount;

	_tagBoneKeyFrame()
		:iRefCount(1)
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
	float				fFrameTime;
	int					iStartFrame;
	int					iEndFrame;
	int					iFrameLength;
	int					iFrameMode;
	int					iAnimationLimitFrame;
	int					iChangeFrame;
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
		for (size_t i = 0; i < vecKeyFrame.size(); ++i)
		{
			--vecKeyFrame[i]->iRefCount;

			if (vecKeyFrame[i]->iRefCount == 0)
			{
				SAFE_DELETE(vecKeyFrame[i]);
			}
		}

		vecKeyFrame.clear();
		//Safe_Delete_VecList(vecCallback);
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
	PANIMATIONCLIP			m_pDefaultClip;
	PANIMATIONCLIP			m_pCurClip;
	PANIMATIONCLIP			m_pNextClip;
	bool					m_bEnd;
	float					m_fAnimationGlobalTime;
	float					m_fClipProgress;
	float					m_fChangeTime;
	float					m_fChangeLimitTime;
	vector<Matrix*>			m_vecBoneMatrix;
	Vector3					m_vBlendScale;
	Vector4					m_vBlendRot;
	Vector3					m_vBlendPos;

public:
	void AddBone(PBONE pBone);
	bool CreateBoneTexture();
	void AddClip(ANIMATION_OPTION eOption,
		struct _tagFbxAnimationClip* pClip);
	PANIMATIONCLIP FindClip(const string& strName);
	bool IsAnimationEnd()	const;

public:
	void ChangeClipKey(const string& strOrigin, const string& strChange);
	PBONE FindBone(const string& strBoneName);
	int FindBoneIndex(const string& strBoneName);
	Matrix GetBoneMatrix(const string& strBoneName);
	bool ChangeClip(const string& strClip);
	ID3D11ShaderResourceView* GetBoneTexture()	const;
	bool Save(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Save(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveFromFullPath(const wchar_t* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Load(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadFromFullPath(const wchar_t* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
	bool ModifyClip(const string& strKey, const string& strChangeKey,
		ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame);
	bool DeleteClip(const string& strKey);
	bool ReturnDefaultClip();

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

