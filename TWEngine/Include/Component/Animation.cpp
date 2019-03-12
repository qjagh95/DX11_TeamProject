#include "../EngineHeader.h"
#include "Animation.h"
#include "../Device.h"
#include "../Resource/FbxLoader.h"
#include "../GameObject.h"
#include "../PathManager.h"

PUN_USING

CAnimation::CAnimation() :
	m_pBoneTex(nullptr),
	m_pBoneRV(nullptr),
	m_pCurClip(nullptr),
	m_pNextClip(nullptr),
	m_bEnd(false),
	m_fAnimationGlobalTime(0.f),
	m_fClipProgress(0.f),
	m_fChangeTime(0.f),
	m_fChangeLimitTime(0.25f),
	m_fFrameTime(1.f / 30.f),
	m_iFrameMode(0)
{
	m_eComType = CT_ANIMATION;
}


CAnimation::CAnimation(const CAnimation & anim) :
	CComponent(anim)
{
	m_pNextClip = nullptr;
	m_fAnimationGlobalTime = 0.f;
	m_fClipProgress = 0.f;
	m_fChangeTime = 0.f;
	m_fChangeLimitTime = anim.m_fChangeLimitTime;
	m_fFrameTime = anim.m_fFrameTime;
	m_iFrameMode = anim.m_iFrameMode;

	m_vecBones = anim.m_vecBones;

	for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	{
		++m_vecBones[i]->iRefCount;
	}

	/*for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	{
		PBONE	pBone = new BONE;

		*pBone = *anim.m_vecBones[i];

		pBone->matOffset = new Matrix;
		pBone->matBone = new Matrix;

		*pBone->matOffset = *anim.m_vecBones[i]->matOffset;
		*pBone->matBone = *anim.m_vecBones[i]->matBone;

		list<CBoneSocket*>::const_iterator	iterB;
		list<CBoneSocket*>::const_iterator	iterBEnd = anim.m_vecBones[i]->SocketList.end();

		for (iterB = anim.m_vecBones[i]->SocketList.begin(); iterB != iterBEnd; ++iterB)
		{
			pBone->SocketList.push_back((*iterB)->Clone());
		}

		m_vecBones.push_back(pBone);
	}*/

	m_pBoneTex = nullptr;

	CreateBoneTexture();

	unordered_map<string, PANIMATIONCLIP>::const_iterator	iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator	iterEnd = anim.m_mapClip.end();

	m_mapClip.clear();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP	pClip = new ANIMATIONCLIP;

		*pClip = *iter->second;

		pClip->vecCallback.clear();

		if (anim.m_pCurClip->strName == iter->first)
			m_pCurClip = pClip;

		if (anim.m_pDefaultClip->strName == iter->first)
			m_pDefaultClip = pClip;

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	m_vecBoneMatrix.clear();

	m_vecBoneMatrix.resize(anim.m_vecBoneMatrix.size());

	for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
	{
		m_vecBoneMatrix[i] = new Matrix;
	}
}

CAnimation::~CAnimation()
{
	Safe_Delete_Map(m_mapClip);
	Safe_Delete_VecList(m_vecBoneMatrix);

	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->iRefCount;

		if (m_vecBones[i]->iRefCount == 0)
		{
			//Safe_Release_VecList(m_vecBones[i]->SocketList);
			SAFE_DELETE(m_vecBones[i]->matBone);
			SAFE_DELETE(m_vecBones[i]->matOffset);
			SAFE_DELETE(m_vecBones[i]);
		}
	}

	m_vecBones.clear();
}

const list<string>* CAnimation::GetAddClipName() const
{
	return &m_strAddClipName;
}

void CAnimation::GetClipTagList(std::vector<std::string>* _vecstrList)
{
	unordered_map<string, PANIMATIONCLIP>::const_iterator	iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		_vecstrList->push_back(iter->second->strName);
	}
}

void CAnimation::AddBone(PBONE pBone)
{
	m_vecBones.push_back(pBone);
}

bool CAnimation::CreateBoneTexture()
{
	SAFE_RELEASE(m_pBoneTex);
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.Height = 1;
	tDesc.Width = (UINT)m_vecBones.size() * 4;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SampleDesc.Count = 1;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pBoneTex)))
		return false;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBoneTex, NULL, &m_pBoneRV)))
		return false;

	/*FILE*	pFile = NULL;

	string	strFileName = m_strTag;
	strFileName += "BoneName.txt";
	fopen_s(&pFile, strFileName.c_str(), "wt");

	if (!pFile)
		return false;

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		fwrite(m_vecBones[i]->strName.c_str(), 1, m_vecBones[i]->strName.length(),
			pFile);
		fwrite("\n", 1, 1, pFile);
	}

	fclose(pFile);*/

	return true;
}

void CAnimation::AddClip(ANIMATION_OPTION eOption,
	_tagFbxAnimationClip * pClip)
{
	PANIMATIONCLIP	pAnimClip = FindClip(pClip->strName);

	if (pAnimClip)
		return;

	pAnimClip = new ANIMATIONCLIP;

	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	pAnimClip->eOption = eOption;
	pAnimClip->strName = pClip->strName;

	pAnimClip->iChangeFrame = 0;

	switch (pClip->eTimeMode)
	{
	case FbxTime::eFrames24:
		pAnimClip->iFrameMode = 24;
		break;
	case FbxTime::eFrames30:
		pAnimClip->iFrameMode = 30;
		break;
	case FbxTime::eFrames60:
		pAnimClip->iFrameMode = 60;
		break;
	}

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	pAnimClip->iStartFrame = 0; //영혁 수정 : Anm에서 매 클립 iStartFrame은 0
	pAnimClip->iEndFrame = (int)(pClip->tEnd.GetFrameCount(pClip->eTimeMode) - pClip->tStart.GetFrameCount(pClip->eTimeMode));
	pAnimClip->iFrameLength = (pAnimClip->iEndFrame) + 1; //영혁 수정 : 갯수는 iEndFrame + 1!


	pAnimClip->fFrameTime = 1.f / (float)(pAnimClip->iFrameMode); //영혁 수정 : 우리 엔진용 프레임타임은 보통 30.f
	// 시간 정보를 저장해준다.
	pAnimClip->fStartTime = 0.f;
	pAnimClip->fEndTime = (pAnimClip->iFrameLength) * (pAnimClip->fFrameTime);
	pAnimClip->fTimeLength = pAnimClip->fEndTime; //영혁 수정 : fPlayTime을 언제 입력했죠??

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < pClip->vecBoneKeyFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;

		pAnimClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(pClip->vecBoneKeyFrame[i].vecKeyFrame.size());

		for (size_t j = 0; j < pClip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			PKEYFRAME	pKeyFrame = new KEYFRAME;

			pKeyFrame->dTime = j * pAnimClip->fFrameTime;

			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix	mat = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4	vPos, vScale;
			FbxQuaternion	qRot;

			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			pKeyFrame->vScale = Vector3((float)vScale.mData[0], (float)vScale.mData[1],
				(float)vScale.mData[2]);
			pKeyFrame->vPos = Vector3((float)vPos.mData[0], (float)vPos.mData[1],
				(float)vPos.mData[2]);
			pKeyFrame->vRot = Vector4((float)qRot.mData[0], (float)qRot.mData[1],
				(float)qRot.mData[2], (float)qRot.mData[3]);

			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
		}
	}
	

	if (m_iFrameMode == 0)
	{
		m_iFrameMode = pAnimClip->iFrameMode;
		m_fFrameTime = pAnimClip->fFrameTime;
	}

	if (m_mapClip.empty())
	{
		m_pDefaultClip = pAnimClip;
		m_pCurClip = pAnimClip;
	}

	m_strAddClipName.clear();

	m_strAddClipName.push_back(pAnimClip->strName);

	m_mapClip.insert(make_pair(pAnimClip->strName, pAnimClip));
}

void CAnimation::AddClip(const string & strName, ANIMATION_OPTION eOption, 
	int iStartFrame, int iEndFrame)
{
}

void CAnimation::AddClip(const string & strName, ANIMATION_OPTION eOption, 
	int iStartFrame, int iEndFrame, float fPlayTime, 
	const vector<PBONEKEYFRAME>& vecFrame)
{
	PANIMATIONCLIP	pAnimClip = FindClip(strName);

	if (pAnimClip)
		return;

	pAnimClip = new ANIMATIONCLIP;

	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	pAnimClip->eOption = eOption;
	pAnimClip->strName = strName;

	pAnimClip->iChangeFrame = 0;

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	pAnimClip->iStartFrame = 0;
	pAnimClip->iEndFrame = iEndFrame - iStartFrame;
	pAnimClip->iFrameLength = pAnimClip->iEndFrame - pAnimClip->iStartFrame;

	pAnimClip->fPlayTime = fPlayTime;
	pAnimClip->fFrameTime = fPlayTime / pAnimClip->iFrameLength;

	// 시간 정보를 저장해준다.
	pAnimClip->fStartTime = 0.f;
	pAnimClip->fEndTime = pAnimClip->fPlayTime;
	pAnimClip->fTimeLength = pAnimClip->fPlayTime;


	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;

		pAnimClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(pAnimClip->iFrameLength);

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = iStartFrame; j <= iEndFrame; ++j)
			{
				PKEYFRAME	pKeyFrame = new KEYFRAME;

				pKeyFrame->dTime = (j - iStartFrame) * pAnimClip->fFrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
			}
		}
	}

	if (m_mapClip.empty())
	{
		m_pDefaultClip = pAnimClip;
		m_pCurClip = pAnimClip;
	}

	m_strAddClipName.clear();

	m_strAddClipName.push_back(pAnimClip->strName);

	m_mapClip.insert(make_pair(pAnimClip->strName, pAnimClip));
}

void CAnimation::AddClip(const TCHAR * pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		0, 0);
#else
	strcpy_s(strFullPath, pFullPath);
#endif // UNICODE

	AddClipFromMultibyte(strFullPath);
}

void CAnimation::AddClipFromMultibyte(const char * pFullPath)
{
	char	strExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
	{
		LoadFbxAnimation(pFullPath);
	}

	else
	{
		LoadFromFullPath(pFullPath);
	}
}

PANIMATIONCLIP CAnimation::FindClip(const string & strName)
{
	unordered_map<string, PANIMATIONCLIP>::iterator	iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

bool CAnimation::IsAnimationEnd() const
{
	return m_bEnd;
}

PANIMATIONCLIP CAnimation::GetCurrentClip() const
{
	return m_pCurClip;
}

void CAnimation::GetCurrentKeyFrame(vector<PBONEKEYFRAME>& vecFrame)
{
	for (size_t i = 0; i < m_pCurClip->vecKeyFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneFrame = new BONEKEYFRAME;
		vecFrame.push_back(pBoneFrame);

		pBoneFrame->iBoneIndex = m_pCurClip->vecKeyFrame[i]->iBoneIndex;

		for (size_t j = 0; j < m_pCurClip->vecKeyFrame[i]->vecKeyFrame.size(); ++j)
		{
			PKEYFRAME	pFrame = new KEYFRAME;

			pFrame->dTime = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->dTime;
			pFrame->vScale = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vScale;
			pFrame->vPos = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vPos;
			pFrame->vRot = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vRot;

			pBoneFrame->vecKeyFrame.push_back(pFrame);
		}
	}
}

void CAnimation::ChangeClipKey(const string & strOrigin,
	const string & strChange)
{
	unordered_map<string, PANIMATIONCLIP>::iterator	iter = m_mapClip.find(strOrigin);

	if (iter == m_mapClip.end())
		return;

	PANIMATIONCLIP	pClip = iter->second;

	m_mapClip.erase(iter);

	m_mapClip.insert(make_pair(strChange, pClip));

	if (strOrigin == m_pCurClip->strName)
	{
		m_pCurClip->strName = pClip->strName;
	}

	if (strOrigin == m_pDefaultClip->strName)
	{
		m_pDefaultClip->strName = pClip->strName;
	}
}

PBONE CAnimation::FindBone(const string & strBoneName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strBoneName)
			return m_vecBones[i];
	}

	return NULL;
}

int CAnimation::FindBoneIndex(const string & strBoneName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strBoneName)
			return (int)i;
	}

	return -1;
}

Matrix CAnimation::GetBoneMatrix(const string & strBoneName)
{
	int	iBone = FindBoneIndex(strBoneName);

	if (iBone == -1)
		return Matrix();

	return *m_vecBoneMatrix[iBone];
}

bool CAnimation::ChangeClip(const string & strClip)
{
	if (m_pCurClip->strName == strClip)
		return false;

	m_pNextClip = FindClip(strClip);

	if (!m_pNextClip)
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView** CAnimation::GetBoneTexture()
{
	return &m_pBoneRV;
}

bool CAnimation::Save(const TCHAR * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return Save(strFileName, strPathKey);
}

bool CAnimation::Save(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return SaveFromFullPath(strFullPath.c_str());
}

bool CAnimation::SaveFromFullPath(const TCHAR * pFullPath)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFileName, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return SaveFromFullPath(strFileName);
}

bool CAnimation::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;	

	fwrite(&m_iFrameMode, sizeof(float), 1, pFile);
	fwrite(&m_fChangeLimitTime, sizeof(float), 1, pFile);

	// 애니메이션 클립정보를 저장한다.
	size_t iCount = m_mapClip.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	size_t iLength = m_pDefaultClip->strName.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_pDefaultClip->strName.c_str(), sizeof(char),
		iLength, pFile);

	iLength = m_pCurClip->strName.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_pCurClip->strName.c_str(), sizeof(char), iLength, pFile);

	unordered_map<string, PANIMATIONCLIP>::iterator	iter;
	unordered_map<string, PANIMATIONCLIP>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP	pClip = iter->second;

		// 애니메이션 클립 키를 저장한다.
		iLength = pClip->strName.length();
		fwrite(&iLength, sizeof(size_t), 1, pFile);
		fwrite(pClip->strName.c_str(), sizeof(char), iLength, pFile);

		fwrite(&pClip->eOption, sizeof(ANIMATION_OPTION), 1, pFile);

		fwrite(&pClip->fStartTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fEndTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fTimeLength, sizeof(float), 1, pFile);
		fwrite(&pClip->fFrameTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fPlayTime, sizeof(float), 1, pFile);
		fwrite(&pClip->iFrameMode, sizeof(int), 1, pFile);
		fwrite(&pClip->iStartFrame, sizeof(int), 1, pFile);
		fwrite(&pClip->iEndFrame, sizeof(int), 1, pFile);
		fwrite(&pClip->iFrameLength, sizeof(int), 1, pFile);

		size_t	iCount = pClip->vecKeyFrame.size();

		fwrite(&iCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iCount; ++i)
		{
			fwrite(&pClip->vecKeyFrame[i]->iBoneIndex, sizeof(int), 1,
				pFile);

			size_t	iFrameCount = pClip->vecKeyFrame[i]->vecKeyFrame.size();

			fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

			for (size_t j = 0; j < iFrameCount; ++j)
			{
				fwrite(&pClip->vecKeyFrame[i]->vecKeyFrame[j]->dTime, sizeof(double), 1, pFile);
				fwrite(&pClip->vecKeyFrame[i]->vecKeyFrame[j]->vPos, sizeof(Vector3), 1, pFile);
				fwrite(&pClip->vecKeyFrame[i]->vecKeyFrame[j]->vScale, sizeof(Vector3), 1, pFile);
				fwrite(&pClip->vecKeyFrame[i]->vecKeyFrame[j]->vRot, sizeof(Vector4), 1, pFile);
			}
		}
	}

	fclose(pFile);

	return true;
}

bool CAnimation::Load(const TCHAR * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return Load(strFileName, strPathKey);
}

bool CAnimation::Load(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return LoadFromFullPath(strFullPath.c_str());
}

bool CAnimation::LoadFromFullPath(const TCHAR * pFullPath)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFileName, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return LoadFromFullPath(strFileName);
}

bool CAnimation::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	fread(&m_iFrameMode, sizeof(float), 1, pFile);
	fread(&m_fChangeLimitTime, sizeof(float), 1, pFile);

	// 애니메이션 클립정보를 저장한다.
	size_t iCount = 0, iLength = 0;
	fread(&iCount, sizeof(size_t), 1, pFile);

	char	strDefaultClip[256] = {};
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strDefaultClip, sizeof(char),
		iLength, pFile);

	char	strCurClip[256] = {};
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strCurClip, sizeof(char), iLength, pFile);

	m_strAddClipName.clear();

	for (int l = 0; l < iCount; ++l)
	{
		PANIMATIONCLIP	pClip = new ANIMATIONCLIP;

		// 애니메이션 클립 키를 저장한다.
		char	strClipName[256] = {};
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strClipName, sizeof(char), iLength, pFile);

		m_strAddClipName.push_back(strClipName);

		m_mapClip.insert(make_pair(strClipName, pClip));

		pClip->strName = strClipName;
		pClip->iChangeFrame = 0;

		fread(&pClip->eOption, sizeof(ANIMATION_OPTION), 1, pFile);

		fread(&pClip->fStartTime, sizeof(float), 1, pFile);
		fread(&pClip->fEndTime, sizeof(float), 1, pFile);
		fread(&pClip->fTimeLength, sizeof(float), 1, pFile);
		fread(&pClip->fFrameTime, sizeof(float), 1, pFile);
		fread(&pClip->fPlayTime, sizeof(float), 1, pFile);
		fread(&pClip->iFrameMode, sizeof(int), 1, pFile);
		fread(&pClip->iStartFrame, sizeof(int), 1, pFile);
		fread(&pClip->iEndFrame, sizeof(int), 1, pFile);
		fread(&pClip->iFrameLength, sizeof(int), 1, pFile);

		size_t	iFrameCount = 0;

		fread(&iFrameCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iFrameCount; ++i)
		{
			PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;
			pClip->vecKeyFrame.push_back(pBoneKeyFrame);

			fread(&pBoneKeyFrame->iBoneIndex, sizeof(int), 1,
				pFile);

			size_t	iBoneFrameCount = 0;

			fread(&iBoneFrameCount, sizeof(size_t), 1, pFile);

			for (size_t j = 0; j < iBoneFrameCount; ++j)
			{
				PKEYFRAME	pKeyFrame = new KEYFRAME;
				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

				fread(&pKeyFrame->dTime, sizeof(double), 1, pFile);
				fread(&pKeyFrame->vPos, sizeof(Vector3), 1, pFile);
				fread(&pKeyFrame->vScale, sizeof(Vector3), 1, pFile);
				fread(&pKeyFrame->vRot, sizeof(Vector4), 1, pFile);
			}
		}
	}

	m_pCurClip = FindClip(strCurClip);
	m_pDefaultClip = FindClip(strDefaultClip);

	fclose(pFile);

	CreateBoneTexture();

	return true;
}

bool CAnimation::SaveBone(const TCHAR * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return SaveBone(strFileName, strPathKey);
}

bool CAnimation::SaveBone(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return SaveBoneFromFullPath(strFullPath.c_str());
}

bool CAnimation::SaveBoneFromFullPath(const TCHAR * pFullPath)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFileName, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return SaveBoneFromFullPath(strFileName);
}

bool CAnimation::SaveBoneFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;

	// ===================== 본 정보 저장 =====================
	size_t	iCount = m_vecBones.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		iLength = m_vecBones[i]->strName.length();
		fwrite(&iLength, sizeof(size_t), 1, pFile);
		fwrite(m_vecBones[i]->strName.c_str(), sizeof(char), iLength, pFile);

		fwrite(&m_vecBones[i]->iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->iParentIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->matOffset->matrix, sizeof(XMMATRIX), 1, pFile);
		fwrite(&m_vecBones[i]->matBone->matrix, sizeof(XMMATRIX), 1, pFile);
	}

	fclose(pFile);

	return true;
}


bool CAnimation::LoadBone(const TCHAR * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return LoadBone(strFileName, strPathKey);
}

bool CAnimation::LoadBone(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return LoadBoneFromFullPath(strFullPath.c_str());
}

bool CAnimation::LoadBoneFromFullPath(const TCHAR * pFullPath)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFileName, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return LoadBoneFromFullPath(strFileName);
}

bool CAnimation::LoadBoneFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	// ===================== 본 정보 읽기 =====================
	size_t	iCount = 0;

	fread(&iCount, sizeof(size_t), 1, pFile);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		PBONE	pBone = new BONE;
		m_vecBones.push_back(pBone);

		pBone->matBone = new Matrix;
		pBone->matOffset = new Matrix;

		char	strBoneName[256] = {};
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strBoneName, sizeof(char), iLength, pFile);
		pBone->strName = strBoneName;

		fread(&pBone->iDepth, sizeof(int), 1, pFile);
		fread(&pBone->iParentIndex, sizeof(int), 1, pFile);
		fread(&pBone->matOffset->matrix, sizeof(XMMATRIX), 1, pFile);
		fread(&pBone->matBone->matrix, sizeof(XMMATRIX), 1, pFile);
	}

	fclose(pFile);

	return true;
}

bool CAnimation::LoadBoneAndAnimationFullPath(const TCHAR * pFullPath)
{
	// 본 파일로 이름을 변환한다.
	TCHAR	strBone[MAX_PATH] = {};

	lstrcpy(strBone, pFullPath);

	int	iLength = lstrlen(strBone);

	strBone[iLength - 3] = 'b';
	strBone[iLength - 2] = 'n';
	strBone[iLength - 1] = 'e';

	if (!LoadBoneFromFullPath(strBone))
		return false;

	return LoadFromFullPath(pFullPath);
}

bool CAnimation::ModifyClip(const string & strKey,
	const string & strChangeKey, ANIMATION_OPTION eOption,
	int iStartFrame, int iEndFrame, float fPlayTime, 
	const vector<PBONEKEYFRAME>& vecFrame)
{
	PANIMATIONCLIP	pClip = FindClip(strKey);

	if (!pClip)
		return false;

	m_mapClip.erase(strKey);

	int	iLength = iEndFrame - iStartFrame;

	pClip->fPlayTime = fPlayTime;
	pClip->fFrameTime = pClip->fPlayTime / iLength;
	pClip->eOption = eOption;
	pClip->strName = strChangeKey;
	pClip->iStartFrame = 0;
	pClip->iEndFrame = iLength;
	pClip->iFrameLength = iLength;
	pClip->fStartTime = 0.f;
	pClip->fEndTime = fPlayTime;
	pClip->fTimeLength = fPlayTime;

	Safe_Delete_VecList(pClip->vecKeyFrame);

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;

		pClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(vecFrame[i]->vecKeyFrame.size());

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = iStartFrame; j <= iEndFrame; ++j)
			{
				PKEYFRAME	pKeyFrame = new KEYFRAME;

				pKeyFrame->dTime = (j - iStartFrame) * pClip->fFrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
			}
		}
	}

	m_mapClip.insert(make_pair(strChangeKey, pClip));

	return true;
}

bool CAnimation::DeleteClip(const string & strKey)
{
	unordered_map<string, PANIMATIONCLIP>::iterator	iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return false;

	SAFE_DELETE(iter->second);

	m_mapClip.erase(iter);


	if (!m_mapClip.empty())
	{
		m_pDefaultClip = FindClip(m_mapClip.begin()->first);
		m_pCurClip = m_pDefaultClip;
	}
	else
	{
		m_pCurClip = nullptr;
		m_pDefaultClip = nullptr;
	}

	return true;
}

bool CAnimation::ReturnDefaultClip()
{
	ChangeClip(m_pDefaultClip->strName);

	return true;
}

void CAnimation::SkipToNextClip()
{
	std::unordered_map<std::string, PUN::PANIMATIONCLIP>::iterator itr = m_mapClip.begin();

	std::unordered_map<std::string, PUN::PANIMATIONCLIP>::iterator itrEnd = m_mapClip.end();

	itr = m_mapClip.find(m_pCurClip->strName);

	++itr;

	if (itr != itrEnd)
	{
		ChangeClip(itr->second->strName);
	}
	else
	{
		itr = m_mapClip.begin();
		ChangeClip(itr->second->strName);
	}
}

void CAnimation::LoadFbxAnimation(const char * pFullPath)
{
	CFbxLoader	loader;

	loader.LoadFbx(pFullPath);

	// 애니메이션 클립을 추가한다.
	const vector<PFBXANIMATIONCLIP>* pvecClip = loader.GetClips();

	// 클립을 읽어온다.
	vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	m_strAddClipName.clear();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		AddClip(AO_LOOP, *iterC);

		m_strAddClipName.push_back((*iterC)->strName);
	}
}

void CAnimation::Start()
{
}

void CAnimation::AfterClone()
{
}

bool CAnimation::Init()
{
	return true;
}

int CAnimation::Input(float fTime)
{
	return 0;
}

int CAnimation::Update(float fTime)
{
	if (m_mapClip.empty())
		return 0;

	if (m_vecBoneMatrix.empty())
	{
		m_vecBoneMatrix.resize(m_vecBones.size());

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
		{
			m_vecBoneMatrix[i] = new Matrix;
		}
	}

	m_bEnd = false;

	// 모션이 변할때
	if (m_pNextClip)
	{
		if (m_pCurClip == nullptr)
		{
			m_pCurClip = m_pNextClip;
			m_pNextClip = nullptr;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}


		m_fChangeTime += fTime;

		bool	bChange = false;
		if (m_fChangeTime >= m_fChangeLimitTime)
		{
			m_fChangeTime = m_fChangeLimitTime;
			bChange = true;
		}

		float	fAnimationTime = m_fAnimationGlobalTime +
			m_pCurClip->fStartTime;

		// 본 수만큼 반복한다.
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			// 키프레임이 없을 경우
			if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
			{
				*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
				continue;
			}

			//int	iFrameIndex = (int)(fAnimationTime * m_iFrameMode);
			int	iFrameIndex = m_pCurClip->iChangeFrame;
			int	iNextFrameIndex = m_pNextClip->iStartFrame;

			const PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
			const PKEYFRAME pNextKey = m_pNextClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

			float	fPercent = m_fChangeTime / m_fChangeLimitTime;

			XMVECTOR	vS = XMVectorLerp(pCurKey->vScale.Convert(),
				pNextKey->vScale.Convert(), fPercent);
			XMVECTOR	vT = XMVectorLerp(pCurKey->vPos.Convert(),
				pNextKey->vPos.Convert(), fPercent);
			XMVECTOR	vR = XMQuaternionSlerp(pCurKey->vRot.Convert(),
				pNextKey->vRot.Convert(), fPercent);

			XMVECTOR	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

			Matrix	matBone = XMMatrixAffineTransformation(vS, vZero,
				vR, vT);

			*m_vecBones[i]->matBone = matBone;

			matBone = *m_vecBones[i]->matOffset *
				matBone;

			*m_vecBoneMatrix[i] = matBone;
		}

		if (bChange)
		{
			m_pCurClip = m_pNextClip;
			m_pNextClip = nullptr;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}
	}

	// 기존 모션이 계속 동작될때
	else
	{
		m_fAnimationGlobalTime += fTime;
		m_fClipProgress = m_fAnimationGlobalTime / m_pCurClip->fTimeLength;

		while (m_fAnimationGlobalTime >= m_pCurClip->fTimeLength)
		{
			m_fAnimationGlobalTime -= m_pCurClip->fTimeLength;
			m_bEnd = true;

			/*for (size_t i = 0; i < m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback.size();
				++i)
			{
				m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback[i]->bCall = false;
			}*/
		}

		float	fAnimationTime = m_fAnimationGlobalTime +
			m_pCurClip->fStartTime;

		int	iStartFrame = m_pCurClip->iStartFrame;
		int	iEndFrame = m_pCurClip->iEndFrame;

		int	iFrameIndex = (int)(fAnimationTime / m_pCurClip->fFrameTime);

		if (m_bEnd)
		{
			switch (m_pCurClip->eOption)
			{
			case AO_LOOP:
				iFrameIndex = iStartFrame;
				break;
			case AO_ONCE_DESTROY:
				m_pObject->Die();
				break;
			}
		}
		else
		{
			int	iNextFrameIndex = iFrameIndex + 1;

			m_pCurClip->iChangeFrame = iFrameIndex;

			if (iNextFrameIndex > iEndFrame)
				iNextFrameIndex = iStartFrame;

			// 본 수만큼 반복한다.
			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				// 키프레임이 없을 경우
				if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
				{
					*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
					continue;
				}

				const PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
				const PKEYFRAME pNextKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

				m_vBlendPos = pCurKey->vPos;
				m_vBlendScale = pCurKey->vScale;
				m_vBlendRot = pCurKey->vRot;

				// 현재 프레임의 시간을 얻어온다.
				double	 dFrameTime = pCurKey->dTime;

				float	fPercent = (float)((fAnimationTime - dFrameTime) / m_pCurClip->fFrameTime);

				XMVECTOR	vS = XMVectorLerp(pCurKey->vScale.Convert(),
					pNextKey->vScale.Convert(), fPercent);
				XMVECTOR	vT = XMVectorLerp(pCurKey->vPos.Convert(),
					pNextKey->vPos.Convert(), fPercent);
				XMVECTOR	vR = XMQuaternionSlerp(pCurKey->vRot.Convert(),
					pNextKey->vRot.Convert(), fPercent);

				XMVECTOR	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

				Matrix	matBone = XMMatrixAffineTransformation(vS, vZero,
					vR, vT);

				*m_vecBones[i]->matBone = matBone;

				matBone = *m_vecBones[i]->matOffset * matBone;

				//*m_vecBones[i]->matBone = matBone;

				*m_vecBoneMatrix[i] = matBone;
			}
		}
	}

	if (!m_bEnd)
	{
		D3D11_MAPPED_SUBRESOURCE	tMap = {};
		CONTEXT->Map(m_pBoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		Matrix*	pMatrix = (Matrix*)tMap.pData;

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
		{
			pMatrix[i] = *m_vecBoneMatrix[i];
		}

		CONTEXT->Unmap(m_pBoneTex, 0);
	}

	return 0;
}

int CAnimation::LateUpdate(float fTime)
{
	return 0;
}

void CAnimation::Collision(float fTime)
{
}

void CAnimation::Render(float fTime)
{
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}
