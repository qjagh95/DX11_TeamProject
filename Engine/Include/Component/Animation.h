#pragma once

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
	int			iRefCount;

	_tagBone() :
		iRefCount(1)
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
	int			iRefCount;

	_tagBoneKeyFrame() :
		iRefCount(1)
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
	float				fPlayTime;
	int					iStartFrame;
	int					iEndFrame;
	int					iFrameLength;
	int					iFrameMode;
	int					iChangeFrame;
	vector<PBONEKEYFRAME>		vecKeyFrame;
	vector<PANIMATIONCALLBACK>	vecCallback;

	_tagAnimationClip() :
		eOption(AO_LOOP),
		strName(""),
		iFrameMode(0),
		fStartTime(0),
		fEndTime(0),
		fTimeLength(0),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0),
		fPlayTime(1.f)
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

class PUN_DLL CAnimation :
	public CComponent
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
	list<string>			m_strAddClipName;
	PANIMATIONCLIP			m_pDefaultClip;
	PANIMATIONCLIP			m_pCurClip;
	PANIMATIONCLIP			m_pNextClip;
	bool					m_bEnd;
	float					m_fAnimationGlobalTime;
	float					m_fClipProgress;
	float					m_fChangeTime;
	float					m_fChangeLimitTime;
	float					m_fFrameTime;
	int						m_iFrameMode;
	vector<Matrix*>			m_vecBoneMatrix;
	Vector3					m_vBlendScale;
	Vector4					m_vBlendRot;
	Vector3					m_vBlendPos;
	
public:
	const list<string>* GetAddClipName()	const;
	void GetClipTagList(std::vector<std::string>* _vecstrList);
public:
	void AddBone(PBONE pBone);
	bool CreateBoneTexture();
	void AddClip(ANIMATION_OPTION eOption,
		struct _tagFbxAnimationClip* pClip);
	void AddClip(const string& strName, ANIMATION_OPTION eOption,
		int iStartFrame, int iEndFrame);
	void AddClip(const string& strName, ANIMATION_OPTION eOption,
		int iStartFrame, int iEndFrame, float fPlayTime,
		const vector<PBONEKEYFRAME>& vecFrame);
	void AddClip(const TCHAR* pFullPath);
	void AddClipFromMultibyte(const char* pFullPath);
	PANIMATIONCLIP FindClip(const string& strName);
	bool IsAnimationEnd()	const;
	PANIMATIONCLIP GetCurrentClip()	const;
	void GetCurrentKeyFrame(vector<PBONEKEYFRAME>& vecFrame);

public:
	void ChangeClipKey(const string& strOrigin, const string& strChange);
	PBONE FindBone(const string& strBoneName);
	int FindBoneIndex(const string& strBoneName);
	Matrix GetBoneMatrix(const string& strBoneName);
	bool ChangeClip(const string& strClip);
	ID3D11ShaderResourceView** GetBoneTexture();
	bool Save(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool Save(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveFromFullPath(const TCHAR* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool Load(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadFromFullPath(const TCHAR* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
	bool SaveBone(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveBone(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveBoneFromFullPath(const TCHAR* pFullPath);
	bool SaveBoneFromFullPath(const char* pFullPath);
	bool LoadBone(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadBone(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadBoneFromFullPath(const TCHAR* pFullPath);
	bool LoadBoneFromFullPath(const char* pFullPath);
	bool LoadBoneAndAnimationFullPath(const TCHAR* pFullPath);
	bool ModifyClip(const string& strKey, const string& strChangeKey,
		ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame,
		float fPlayTime, const vector<PBONEKEYFRAME>& vecFrame);
	bool DeleteClip(const string& strKey);
	bool ReturnDefaultClip();

	//디버그용 클립 넘기기 함수
	void SkipToNextClip();

private:
	void LoadFbxAnimation(const char* pFullPath);

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


