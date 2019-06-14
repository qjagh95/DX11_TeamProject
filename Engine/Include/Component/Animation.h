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
	int SelectIndex;
	function<void(float)> func;
	bool	bCall;

	_tagAnimationCallback()
	{
		SelectIndex = -1;
		bCall = false;
	}

}ANIMATIONCALLBACK, *PANIMATIONCALLBACK;

typedef struct PUN_DLL _tagAnimationClip
{
	ANIMATION_OPTION	eOption;
	string				strName;
	bool				bUpdateRootTransform;
	int					iRootTransformBoneIdx;
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
	int					iFrame;
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
		fPlayTime(1.f),
		iFrame(0),
		bUpdateRootTransform(false),
		iRootTransformBoneIdx(-1)
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

	void SetRootTransformBone(int idx);
	void UseRootTransformBone(bool bOn);
}ANIMATIONCLIP, *PANIMATIONCLIP;


typedef struct PUN_DLL _tagPartialClip
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
	vector<XMVECTOR> vecXmRotVector;

	_tagPartialClip() :
		eOption(AO_LOOP),
		fStartTime(0.f),
		fEndTime(0.f),
		fTimeLength(0.f),
		fFrameTime(0.f),
		fPlayTime(0.f),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0),
		iFrameMode(30),
		iChangeFrame(0)
	{
		vecCallback.clear();
		vecKeyFrame.clear();
		vecXmRotVector.clear();
	}

	~_tagPartialClip()
	{
		Safe_Delete_VecList(vecKeyFrame);
	}
}PARTCLIP, *PPARTCLIP;

typedef struct PUN_DLL _tagPartialAnim
{
	
	bool bActivated = false;
	std::unordered_map<std::string, PPARTCLIP> mapPartClips;
	std::vector<int> vecPartIdx;
	std::vector<std::string> strAddClipName;
	//std::vector<Matrix>			vecPartBoneMatrix;

	int iRootParentIndex = -1;

	PPARTCLIP			pDefaultClip = nullptr;
	PPARTCLIP			pCurClip = nullptr;
	PPARTCLIP			pNextClip = nullptr;
	bool				bKeepBlending = false;
	bool				bEnd;
	float				fPartAnimationGTime;
	float				fClipProgress = 0.f;
	float				fChangeTime = 0.25f;
	float				fChangeLimitTime = 0.f;
	int					iFrameMode = 0;
	Vector3				vBlendScale;
	Vector4				vBlendRot;
	Vector3				vBlendPos;
	int					iRefCnt;
	Matrix				matCustomParent;
	bool				bUseCustomParent;

	//vecPartIdx, iRootParentIndex�� ���� ������ ���� ������ ��
	bool LoadPartAnimFromExistingClip(PANIMATIONCLIP pAnim);
	bool LoadPartAnimFromNewFile(const std::wstring& filePath, const std::string& strPathTag = MESH_DATA_PATH);
	bool LoadPartClipFromFile(const std::string& strClipKey, std::wstring& filePath, const std::string& strPathTag = MESH_DATA_PATH);

	_tagPartialAnim() :
		pDefaultClip(nullptr),
		pCurClip(nullptr),
		pNextClip(nullptr),
		bKeepBlending(false),
		bEnd(false),
		fPartAnimationGTime(0.f),
		fClipProgress(0.f),
		fChangeTime(0.25f),
		fChangeLimitTime(0.f),
		iFrameMode(0),
		iRefCnt(1),
		bUseCustomParent(false)
	{
		matCustomParent.Identity();
	}
	~_tagPartialAnim()
	{
		Safe_Delete_Map(mapPartClips);
	}

	void AddRef()
	{
		++iRefCnt;
	}

	void Release()
	{
		--iRefCnt;
		if (iRefCnt < 1)
			delete this;
	}

	/*
	������ ANIMATIONCLIP �� ������ ���� �θ𿡼� ���� ��ӹ��� ��ġ�� ������� �Ѿ�� ������
	�κ� �ִϸ��̼��� ������ ���ؼ��� �� �θ��� ����� ����ĵ��� ������� �Ѵ�.
	�׷��ϱ� �����ϸ� �κ� �ִϸ��̼��� �������� ����....

	*/

}PARTANIM, *PPARTANIM;


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

	std::vector<PPARTANIM> m_vecPartialAnim;

	vector<PBONE>		m_vecBones;
	ID3D11Texture2D*	m_pBoneTex;
	ID3D11ShaderResourceView*	m_pBoneRV;
	unordered_map<string, PANIMATIONCLIP>	m_mapClip;
	list<string>			m_strAddClipName;
	PANIMATIONCLIP			m_pDefaultClip;
	PANIMATIONCLIP			m_pCurClip;
	PANIMATIONCLIP			m_pNextClip;
	bool					m_bKeepBlending;
	bool					m_bEnd;
	bool					m_bCurClipEnd;
	bool					m_bCurClipStart;
	float					m_fAnimationGlobalTime;
	float					m_fClipProgress;
	float					m_fChangeTime;
	float					m_fChangeLimitTime;
	bool					m_bSkipBlending;
	float					m_fFrameTime;
	int						m_iFrameMode;
	vector<Matrix*>			m_vecBoneMatrix;
	Vector3					m_vBlendScale;
	Vector4					m_vBlendRot;
	Vector3					m_vBlendPos;

	bool					m_bRootBoneTransformChange;
	Vector3					m_vRootBonePosBuf;
	Vector3					m_vRootBoneRotBuf;
	Vector3					m_vArrRootBoneAxis[3];

public:
	const list<string>* GetAddClipName()	const;
	void GetClipTagList(std::vector<std::string>* _vecstrList);
	bool AddPartialClip(PPARTANIM partAnim);
	PANIMATIONCLIP GetAnimClip(const std::string& strKey);
	void SetBlendSkip(bool bOn);
	const Matrix* GetBoneMatrix(int iBoneIdx);
	Matrix* GetBone(const string& BoneName);
	const PBONE GetBone(int iBoneIdx);
	float GetCurrentClipTime();
	void KeepBlendSet(bool on);
	void AddBone(PBONE pBone);
	void SetDefaultClip(const std::string& strDefKey);
	void SetClipOption(const std::string& strKey, ANIMATION_OPTION eOpt);
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
	bool IsCurAnimEnd() const;
	bool IsCurAnimEnd(const string& strName) const;
	bool IsCurAnimStart() const;
	PANIMATIONCLIP GetCurrentClip()	const;
	void GetCurrentKeyFrame(vector<PBONEKEYFRAME>& vecFrame);

	void SetCallback(const string& ClipName, int Frame, void(*pFunc)(float));
	template <typename T>
	void SetCallback(const string& ClipName, T* Object, int Frame, void(T::*pFunc)(float))
	{
		PANIMATIONCLIP getClip = FindClip(ClipName);

		if (getClip == NULLPTR)
			return;

		PANIMATIONCALLBACK newCallback = new ANIMATIONCALLBACK();
		newCallback->SelectIndex = Frame;
		newCallback->func = bind(pFunc, Object, placeholders::_1);
		newCallback->bCall = true;

		getClip->vecCallback.push_back(newCallback);
	}

public:
	void ChangeClipKey(const string& strOrigin, const string& strChange);
	PBONE FindBone(const string& strBoneName);
	int FindBoneIndex(const string& strBoneName);
	void SetClipUseBoneTransform(const string& strClipName, const string& strBoneName);
	Matrix GetBoneMatrix(const string& strBoneName);
	bool ChangeClip(const string& strClip);
	ID3D11ShaderResourceView** GetBoneTexture();
	ID3D11Texture2D *GetBoneTex2D();

	void SetBoneTexture(ID3D11ShaderResourceView **ppSRV, ID3D11Texture2D *ppBoneTex);
	bool Save(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool Save(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveFromFullPath(const TCHAR* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const TCHAR* pFileName, const string& strPathKey = MESH_DATA_PATH);
	bool Load(const char* pFileName, const string& strPathKey = MESH_DATA_PATH);
	bool LoadFromFullPath(const TCHAR* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
	bool LoadFileAsClip(const char* clipName, const TCHAR* pFileName);
	bool SaveBone(const TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveBone(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveBoneFromFullPath(const TCHAR* pFullPath);
	bool SaveBoneFromFullPath(const char* pFullPath);
	bool LoadBone(const TCHAR* pFileName, const string& strPathKey = MESH_DATA_PATH);
	bool LoadBone(const char* pFileName, const string& strPathKey = MESH_DATA_PATH);
	bool LoadBoneFromFullPath(const TCHAR* pFullPath);
	bool LoadBoneFromFullPath(const char* pFullPath);
	bool LoadBoneAndAnimationFullPath(const TCHAR* pFullPath);
	bool ModifyClip(const string& strKey, const string& strChangeKey,
		ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame,
		float fPlayTime, const vector<PBONEKEYFRAME>& vecFrame);
	bool DeleteClip(const string& strKey);
	bool ReturnDefaultClip();

	bool AddSocket(const string& strBoneName, const string& strSocketName);
	void ClearAllSockets();
	bool SetSocketObject(const string& strBoneName, const string& strSocketName, class CGameObject* pObj);
	bool SetSocketOffset(const string& strBoneName, const string& strSocketName, const Vector3& vOffset);
	bool SetSocketOffset(const string& strBoneName, const string& strSocketName, float x, float y, float z);
	bool SetSocketRotation(const string& strBoneName, const string& strSocketName, const Vector3& vRot);
	bool SetSocketRotation(const string& strBoneName, const string& strSocketName, float x, float y, float z);
	bool SetSocketRotationX(const string& strBoneName, const string& strSocketName, float x);
	bool SetSocketRotationY(const string& strBoneName, const string& strSocketName, float y);
	bool SetSocketRotationZ(const string& strBoneName, const string& strSocketName, float z);
	class CBoneSocket* FindSocket(const string& strBoneName, const string& strSocketName);

	//����׿� Ŭ�� �ѱ�� �Լ�
	void SkipToNextClip();

	int GetCurFrame() const;
	//�κ� �ִϸ��̼� �߰�


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


