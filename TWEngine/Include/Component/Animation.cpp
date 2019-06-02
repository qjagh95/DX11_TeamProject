#include "../EngineHeader.h"
#include "Animation.h"
#include "../Device.h"
#include "../GameObject.h"
#include "../PathManager.h"
#include "../Resource/FbxLoader.h"
#include "BoneSocket.h"
#include "../Timer.h"

//#define PART_METHOD_1 1
#define PART_METHOD_2 1
#define ZYX 1
void to_Euler_Angle(const Vector4 & q, Vector3 & vEuler)
{
	//Quaternion -> Euler 변환은 오일러 축 순서가 ㅈㄴ 중요하다(이것 때문에 기생수 많이 만들고 애먹음)
	//Matrix.Rotation 함수를 살펴보면 우리 엔진은 분명히 XYZ 순서를 사용하고 있다
	//추가 분석 : XYZ 순서로 매트릭스를 만들었고, 거기서 파생된 Quaternion을 분해하기 위해 만든 기능
	//따라서 이 쿼터니언의 분해 순서는 ZYX가 되어야 한다는 가설을 세워본 결과 이것이 유효했다
	//인터넷에 돌아다니는 수많은 자료들이 모두 Quaternion을 Euler로 만드는 것이 확실하지만,
	//Euler 각은 적용하는 순서에 따라 값이 달라지기 때문에 XYZ, XZY, YXZ, YZX, ZXY, ZYX 여섯 가지 경우에서
	//적용하려는 것이 무엇인지 확실히 해 두어야 한다.

#ifdef XYZ
	float fSingularityTest = 2.f * ((q.x * q.z) + (q.w * q.y));

	if (fSingularityTest > 0.999998f)
	{
		vEuler.y = 2.f * atan2f(q.x, q.w);
		vEuler.z = 0.f;
		vEuler.x = PUN_PI / 2.f;
		return;
	}
	else if (fSingularityTest < -0.999998f)
	{
		vEuler.y = -2.f * atan2f(q.x, q.w);
		vEuler.z = 0.f;
		vEuler.x = -PUN_PI / 2.f;
		return;
	}

	float fSqrQx = q.x * q.x;
	float fSqrQy = q.y * q.y;
	float fSqrQz = q.z * q.z;
	float fSqrQw = q.w * q.w;

	vEuler.z = atan2f(-2.f *(q.x * q.y - q.w * q.z), fSqrQw + fSqrQx - fSqrQy - fSqrQz);
	vEuler.y = asinf(fSingularityTest);
	vEuler.x = atan2f(-2.f*((q.y*q.z) - (q.w*q.x)), fSqrQw - fSqrQx - fSqrQy + fSqrQz);
#else
#ifdef ZYX

	float fSqrQx = q.x * q.x;
	float fSqrQy = q.y * q.y;
	float fSqrQz = q.z * q.z;
	float fSqrQw = q.w * q.w;

	float fSingularityTest = -2.f * ((q.x * q.z) - (q.w * q.y));

	if (fSingularityTest > 0.999998f || fSingularityTest < -0.999998f)
	{
		vEuler.y = copysignf(PUN_PI * 0.5f, fSingularityTest);
	}
	else
	{
		vEuler.y = asinf(fSingularityTest);
	}


	vEuler.z = atan2f(2.f *(q.x * q.y + q.w * q.z), fSqrQw + fSqrQx - fSqrQy - fSqrQz);

	vEuler.x = atan2f(2.f*((q.y*q.z) + (q.w*q.x)), fSqrQw - fSqrQx - fSqrQy + fSqrQz);
#endif
#endif
}

#define EULER_TO_QUAT_ZYX 1

void EulertoQuat(const Vector3& vEuler, Vector4& vQuat)
{
	float c1 = cosf(vEuler.x * 0.5f);
	float c2 = cosf(vEuler.y * 0.5f);
	float c3 = cosf(vEuler.z * 0.5f);
	float s1 = sinf(vEuler.x * 0.5f);
	float s2 = sinf(vEuler.y * 0.5f);
	float s3 = sinf(vEuler.z * 0.5f);

#ifdef EULER_TO_QUAT_ZYX
	vQuat = Vector4(
		s1 * c2 * c3 - c1 * s2 * s3,
		c1 * s2 * c3 + s1 * c2 * s3,
		c1 * c2 * s3 - s1 * s2 * c3,
		c1 * c2 * c3 + s1 * s2 * s3
	);
#else
#ifdef EULER_TO_QUAT_XYZ

	vQuat = Vector4(
		s1 * c2 * c3 + c1 * s2 * s3,
		c1 * s2 * c3 - s1 * c2 * s3,
		c1 * c2 * s3 + s1 * s2 * c3,
		c1 * c2 * c3 - s1 * s2 * s3
	);
#endif
#endif
}

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
	m_iFrameMode(0),
	m_bKeepBlending(false),
	m_bCurClipEnd(false),
	m_bSkipBlending(false),
	m_bRootBoneTransformChange(false)
{
	m_eComType = CT_ANIMATION;
	m_bCurClipStart = false;
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
	m_bKeepBlending = false;
	m_vecBones = anim.m_vecBones;

	for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	{
		++m_vecBones[i]->iRefCount;
	}

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
	Safe_Delete_VecList(m_vecPartialAnim);
	Safe_Delete_Map(m_mapClip);
	Safe_Delete_VecList(m_vecBoneMatrix);

	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->iRefCount;

		if (m_vecBones[i]->iRefCount == 0)
		{
			Safe_Delete_VecList(m_vecBones[i]->SocketList);
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

bool CAnimation::AddPartialClip(PPARTANIM partAnim)
{
	m_vecPartialAnim.push_back(partAnim);
	partAnim->fChangeLimitTime = m_fChangeLimitTime;
	//partAnim->fChangeTime = m_fChangeTime;

	return true;
}

void CAnimation::SetBlendSkip(bool bOn)
{
	m_bSkipBlending = bOn;
}

PANIMATIONCLIP CAnimation::GetAnimClip(const std::string & strKey)
{
	return FindClip(strKey);
}

const Matrix * CAnimation::GetBoneMatrix(int iBoneIdx)
{
	if (!m_pCurClip)
		return nullptr;

	if (iBoneIdx >= m_vecBones.size())
		return nullptr;

	if (iBoneIdx < 0)
		return nullptr;

	return m_vecBoneMatrix[iBoneIdx];
}

Matrix* CAnimation::GetBone(const string& BoneName)
{
	int Index = FindBoneIndex(BoneName);

	return m_vecBones[Index]->matBone;
}

const PBONE CAnimation::GetBone(int iBoneIdx)
{
	return m_vecBones[iBoneIdx];
}

float CAnimation::GetCurrentClipTime()
{
	return m_fAnimationGlobalTime;
}

void CAnimation::KeepBlendSet(bool on)
{
	m_bKeepBlending = on;
}

void CAnimation::AddBone(PBONE pBone)
{
	m_vecBones.push_back(pBone);
}

void CAnimation::SetDefaultClip(const std::string & strDefKey)
{
	PANIMATIONCLIP pAnim = FindClip(strDefKey);
	if (pAnim)
		m_pDefaultClip = pAnim;
}

void CAnimation::SetClipOption(const std::string& strKey, ANIMATION_OPTION eOpt)
{
	PANIMATIONCLIP pAnim = FindClip(strKey);
	if (pAnim)
		pAnim->eOption = eOpt;
}

bool CAnimation::CreateBoneTexture()
{
	SAFE_RELEASE(m_pBoneTex);
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //D3D11_USAGE_DYNAMIC 은 절대 D3D_CPU_ACCESS_WRITE만 가능함
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

bool CAnimation::IsCurAnimEnd() const
{
	return m_bCurClipEnd;
}

bool CAnimation::IsCurAnimEnd(const string & strName) const
{
	if (m_pCurClip->strName != strName)
		return false;

	return m_bCurClipEnd;
}

bool CAnimation::IsCurAnimStart() const
{
	return m_bCurClipStart;
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
	for (int i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strBoneName)
			return i;
	}

	return -1;
}

void CAnimation::SetClipUseBoneTransform(const string & strClipName, const string & strBoneName)
{
	PANIMATIONCLIP pClip = FindClip(strClipName);
	int iBoneIdx = FindBoneIndex(strBoneName);

	pClip->bUpdateRootTransform = true;
	pClip->iRootTransformBoneIdx = iBoneIdx;
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

ID3D11Texture2D * CAnimation::GetBoneTex2D()
{
	return m_pBoneTex;
}

void CAnimation::SetBoneTexture(ID3D11ShaderResourceView ** ppSRV, ID3D11Texture2D * ppBoneTex)
{
	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);
	m_pBoneRV = *ppSRV;
	m_pBoneRV->AddRef();

	m_pBoneTex = ppBoneTex;
	m_pBoneTex->AddRef();
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
		//std::cout << strClipName << std::endl;
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

bool CAnimation::LoadFileAsClip(const char * clipName, const TCHAR * pFileName)
{
	FILE *pFile = nullptr;

	std::wstring str;

	str = PUN::CPathManager::GetInst()->FindPath(MESH_DATA_PATH);
	str += pFileName;

	_wfopen_s(&pFile, str.c_str(), TEXT("rb"));

	if (!pFile)
		return false;

	char strBuf[1024] = {};

	fread(strBuf, sizeof(float), 1, pFile);
	fread(strBuf, sizeof(float), 1, pFile);

	// 애니메이션 클립정보 >> 불필요
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
		//std::cout << strClipName << std::endl;
		m_strAddClipName.push_back(clipName);

		m_mapClip.insert(make_pair(clipName, pClip));

		pClip->strName = clipName;
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


	fclose(pFile);

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

		//std::cout << strBoneName << endl;
		pBone->strName = strBoneName;

		fread(&pBone->iDepth, sizeof(int), 1, pFile);
		fread(&pBone->iParentIndex, sizeof(int), 1, pFile);
		fread(&pBone->matOffset->matrix, sizeof(XMMATRIX), 1, pFile);
		fread(&pBone->matBone->matrix, sizeof(XMMATRIX), 1, pFile);
	}

	/*
	auto itrBoneEnd = m_vecBones.end();
	std::cout << "\n\n\tBone Of :\t" << m_strTag << std::endl;
	int iii = 0;
	for (auto itr = m_vecBones.begin(); itr != itrBoneEnd; ++itr)
	{
		std::cout << "\t" << iii << "," << (*itr)->strName << "," << (*itr)->iParentIndex << std::endl;
		++iii;
	}
	*/


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

bool CAnimation::AddSocket(const string & strBoneName, const string & strSocketName)
{
	BONE* pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
				return false;
		}
	}

	CBoneSocket* pSocket = new CBoneSocket;
	
	pSocket->Init();
	pSocket->SetName(strSocketName);
	pBone->SocketList.push_back(pSocket);

	return true;
}

void CAnimation::ClearAllSockets()
{
	auto itrBoneEnd = m_vecBones.end();
	for (auto itr = m_vecBones.begin(); itr != itrBoneEnd; ++itr)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = (*itr)->SocketList.end();
		for (iter = (*itr)->SocketList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->SetObject(nullptr);
		}
	}
}

bool CAnimation::SetSocketObject(const string & strBoneName, const string & strSocketName, CGameObject * pObj)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetObject(pObj);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketOffset(const string & strBoneName, const string & strSocketName, const Vector3 & vOffset)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetOffset(vOffset);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketOffset(const string & strBoneName, const string & strSocketName, float x, float y, float z)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetOffset(x, y, z);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketRotation(const string & strBoneName, const string & strSocketName, const Vector3 & vRot)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetRotation(vRot);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketRotation(const string & strBoneName, const string & strSocketName, float x, float y, float z)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetRotation(x, y, z);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketRotationX(const string & strBoneName, const string & strSocketName, float x)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetRotationX(x);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketRotationY(const string & strBoneName, const string & strSocketName, float y)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetRotationY(y);
				return true;
			}
		}
	}

	return false;
}

bool CAnimation::SetSocketRotationZ(const string & strBoneName, const string & strSocketName, float z)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return false;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				(*iter)->SetRotationZ(z);
				return true;
			}
		}
	}

	return false;
}

CBoneSocket * CAnimation::FindSocket(const string & strBoneName, const string & strSocketName)
{
	BONE*	pBone = FindBone(strBoneName);

	if (pBone == NULLPTR)
		return NULLPTR;

	for (size_t i = 0; i < pBone->SocketList.size(); i++)
	{
		list<CBoneSocket*>::iterator iter;
		list<CBoneSocket*>::iterator iterEnd = pBone->SocketList.end();
		for (iter = pBone->SocketList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == strSocketName)
			{
				return *iter;
			}
		}
	}

	return NULLPTR;
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
			m_vecBoneMatrix[i] = new Matrix;
	}

	m_bEnd = false;

	//Run Notify functions

	if (m_pCurClip)
	{
		float fBeforeTime = m_fAnimationGlobalTime - fTime;

		if (fBeforeTime < 0.f)
		{
			fBeforeTime = 0.f;

			std::vector<PANIMATIONCALLBACK>::iterator itrEnd = m_pCurClip->vecCallback.end();
			for (std::vector<PANIMATIONCALLBACK>::iterator itr = m_pCurClip->vecCallback.begin();
				itr != itrEnd; ++itr)
			{
				PANIMATIONCALLBACK pCallbackInfo = (*itr);
				pCallbackInfo->bCall = false;
			}
		}

		std::vector<PANIMATIONCALLBACK>::iterator itrEnd = m_pCurClip->vecCallback.end();
		for (std::vector<PANIMATIONCALLBACK>::iterator itr = m_pCurClip->vecCallback.begin();
			itr != itrEnd; ++itr)
		{
			PANIMATIONCALLBACK pCallbackInfo = (*itr);
			float fActivateTime = m_pCurClip->fEndTime * pCallbackInfo->fAnimationProgress;
			if (pCallbackInfo->bCall)
				continue;

			if (fActivateTime > fBeforeTime && fActivateTime <= m_fAnimationGlobalTime)
			{
				pCallbackInfo->func(fTime);
				pCallbackInfo->bCall = true;
			}
		}
	}
	if (m_pNextClip)
	{
		if (m_pCurClip != m_pNextClip)
			m_fChangeLimitTime = 0.25f;
		float fBeforeTime = m_fAnimationGlobalTime - fTime;
		if (fBeforeTime < 0.f)
		{
			fBeforeTime = 0.f;

			std::vector<PANIMATIONCALLBACK>::iterator itrEnd = m_pNextClip->vecCallback.end();
			for (std::vector<PANIMATIONCALLBACK>::iterator itr = m_pNextClip->vecCallback.begin();
				itr != itrEnd; ++itr)
			{
				PANIMATIONCALLBACK pCallbackInfo = (*itr);
				pCallbackInfo->bCall = false;

			}
		}

		std::vector<PANIMATIONCALLBACK>::iterator itrEnd = m_pNextClip->vecCallback.end();
		for (std::vector<PANIMATIONCALLBACK>::iterator itr = m_pNextClip->vecCallback.begin();
			itr != itrEnd; ++itr)
		{
			PANIMATIONCALLBACK pCallbackInfo = (*itr);
			float fActivateTime = m_pNextClip->fEndTime * pCallbackInfo->fAnimationProgress;
			if (pCallbackInfo->bCall)
				continue;

			if (fActivateTime > fBeforeTime && fActivateTime <= m_fAnimationGlobalTime)
			{
				pCallbackInfo->func(fTime);
				pCallbackInfo->bCall = true;
			}
		}
	}
	//Notify end

	//부분 애니메이션이 있는지?
	int iPartAnimCnt = (int)m_vecPartialAnim.size();

	//글로벌 애니메이션 프레임

	int g_iFrameIndex = 0;
	int g_iNextFrameIndex = 0;

	// 모션이 변할때
	if (m_pNextClip)
	{
		if (m_pCurClip == nullptr)
		{
			m_bCurClipStart = true;
			m_pCurClip = m_pNextClip;
			m_pNextClip = nullptr;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}
		else
		{
		m_bCurClipStart = false;
			m_fChangeTime += fTime;

			bool	bChange = false;
			if (m_fChangeTime >= m_fChangeLimitTime)
			{
				if (!m_bKeepBlending)
				{
					m_fChangeTime = m_fChangeLimitTime;
					bChange = true;
				}
			}

			float	fAnimationTime = m_fAnimationGlobalTime + m_pCurClip->fStartTime;

			int BoneSize = (int)m_vecBones.size();

			if (bChange)
			{
				//모션 관련 변환
				
				if (m_bRootBoneTransformChange)
				{
					if (m_pCurClip->bUpdateRootTransform)
					{
						int iSize = (int)m_pCurClip->vecKeyFrame.size();
						for (int i = 0; i < iSize; ++i)
						{
							if (i == m_pCurClip->iRootTransformBoneIdx)
							{
								////합성된 트랜스폼만큼 월드 트랜스폼을 이동시켜주자
								//PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]
								//	->vecKeyFrame[m_pCurClip->vecKeyFrame[i]->vecKeyFrame.size() - 1];
								//Vector3 vChangedPosLocal = pCurKey->vPos * -1.f;
								//vChangedPosLocal *= m_pTransform->GetWorldScale(); //월드스케일만큼 곱하기
								//Vector3 vChangedPos;
								//vChangedPos += m_vArrRootBoneAxis[0] * vChangedPosLocal.x;
								////vChangedPos += m_vArrRootBoneAxis[1] * vChangedPosLocal.y; //y축은 건들지 말자 : ㅈ된다
								//vChangedPos += m_vArrRootBoneAxis[2] * vChangedPosLocal.z;
								//vChangedPos += m_vRootBonePosBuf;
								//
								//Vector3 vBufRotRad;
								//vBufRotRad.x = DegreeToRadian(m_vRootBoneRotBuf.x);
								//vBufRotRad.y = DegreeToRadian(m_vRootBoneRotBuf.y);
								//vBufRotRad.z = DegreeToRadian(m_vRootBoneRotBuf.z);
								//Vector4 vBufQuat;
								//
								//EulertoQuat(vBufRotRad, vBufQuat);
								//
								//Vector4 vRotCurr(DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(pCurKey->vRot.Convert(), vBufQuat.Convert())));
								//
								//Vector3 vRotTot;
								//
								//to_Euler_Angle(vRotCurr, vRotTot);
								//vRotTot.x = RadianToDegree(vRotTot.x);
								//vRotTot.y = RadianToDegree(vRotTot.y);
								//vRotTot.z = RadianToDegree(vRotTot.z);
								//
								//vRotTot.x = 0.f;
								//vRotTot.z = 0.f;
								//
								//m_pTransform->SetWorldPos(vChangedPos);
								//m_pTransform->SetWorldRot(vRotTot);
								break;
							}
						}
						
					}

					//초기화
					m_bRootBoneTransformChange = false;
					m_vArrRootBoneAxis[AXIS_X] = m_pTransform->GetWorldAxis(AXIS_X);
					m_vArrRootBoneAxis[AXIS_Y] = m_pTransform->GetWorldAxis(AXIS_Y);
					m_vArrRootBoneAxis[AXIS_Z] = m_pTransform->GetWorldAxis(AXIS_Z);
					m_vRootBonePosBuf = m_pTransform->GetWorldPos();
					m_vRootBoneRotBuf = m_pTransform->GetWorldRot();
				}
				
				

				m_pCurClip = m_pNextClip;
				m_pNextClip = nullptr;
				m_fAnimationGlobalTime = 0.f;
				m_fChangeTime = 0.f;


				m_bRootBoneTransformChange = false;
				//m_bCurClipEnd = true;
				//m_pCurClip->iFrame = 0;
			}
			else if (!m_bKeepBlending)
			{
				parallel_for((int)0, BoneSize, [&](int i)
				{
					// 키프레임이 없을 경우
					// 매 클립의 vecKeyFrame -> 본 갯수만큼 있다
					// parallel_for의 i 번호 : 본 번호

					//생략 본들을 찾아보자
					for (int iPartIdx = 0; iPartIdx < iPartAnimCnt; ++iPartIdx)
					{
						if (!m_vecPartialAnim[iPartIdx]->bActivated)
							continue;

						std::vector<int>::iterator itr = (m_vecPartialAnim[iPartIdx])->vecPartIdx.begin();
						std::vector<int>::iterator itrEnd = (m_vecPartialAnim[iPartIdx])->vecPartIdx.end();
						for (; itr != itrEnd; ++itr)
						{
							if (i == (*itr))
							{
								if (m_vecPartialAnim[iPartIdx]->iRootParentIndex != i)
									return;
							}
						}
					}

					if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
					{
						*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
						return;
					}

					g_iFrameIndex = m_pCurClip->iChangeFrame;
					g_iNextFrameIndex = m_pNextClip->iStartFrame;

					const PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[g_iFrameIndex];
					const PKEYFRAME pNextKey = m_pNextClip->vecKeyFrame[i]->vecKeyFrame[g_iNextFrameIndex];

					float	fPercent = m_fChangeTime / m_fChangeLimitTime;

					XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
					XMVECTOR vT = DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
					XMVECTOR vR = DirectX::XMQuaternionSlerp(pCurKey->vRot.Convert(), pNextKey->vRot.Convert(), fPercent);
					XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
					Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);


					if (m_pCurClip->bUpdateRootTransform)
					{
						if (!m_bRootBoneTransformChange)
						{
							m_bRootBoneTransformChange = true;
							m_vRootBonePosBuf = m_pTransform->GetWorldPos();
							m_vRootBoneRotBuf = m_pTransform->GetWorldRot();
							m_vArrRootBoneAxis[0] = m_pTransform->GetWorldAxis(AXIS_X);
							m_vArrRootBoneAxis[1] = m_pTransform->GetWorldAxis(AXIS_Y);
							m_vArrRootBoneAxis[2] = m_pTransform->GetWorldAxis(AXIS_Z);
						}
						if (i == m_pCurClip->iRootTransformBoneIdx)
						{
							//합성된 트랜스폼만큼 월드 트랜스폼을 이동시켜주자
							
							Vector3 vChangedPosLocal(vT);
							vChangedPosLocal = vChangedPosLocal - pCurKey->vPos;
							vChangedPosLocal *= m_pTransform->GetWorldScale(); //월드스케일만큼 곱하기
							Vector3 vChangedPos;
							vChangedPos += m_vArrRootBoneAxis[0] * vChangedPosLocal.x;
							//vChangedPos += m_vArrRootBoneAxis[1] * vChangedPosLocal.y;
							vChangedPos += m_vArrRootBoneAxis[2] * vChangedPosLocal.z;
							vChangedPos += m_vRootBonePosBuf;
							
							//Vector3 vBufRotRad;
							//vBufRotRad.x = DegreeToRadian(m_vRootBoneRotBuf.x);
							//vBufRotRad.y = DegreeToRadian(m_vRootBoneRotBuf.y);
							//vBufRotRad.z = DegreeToRadian(m_vRootBoneRotBuf.z);
							//Vector4 vBufQuat;
							//
							//EulertoQuat(vBufRotRad, vBufQuat);
							//
							//Vector4 vQuatCurr(vR);
							//DirectX::XMVECTOR xmInvRotCurr = DirectX::XMQuaternionInverse(vQuatCurr.Convert());
							//
							//Vector4 vRotCurr(DirectX::XMQuaternionMultiply(pCurKey->vRot.Convert(), xmInvRotCurr));
							//
							//vRotCurr = Vector4(DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(vRotCurr.Convert(), vBufQuat.Convert())));
							//
							//Vector3 vRotTot;
							//
							//to_Euler_Angle(vRotCurr, vRotTot);
							//vRotTot.x = RadianToDegree(vRotTot.x);
							//vRotTot.y = RadianToDegree(vRotTot.y);
							//vRotTot.z = RadianToDegree(vRotTot.z);

							//vRotTot.x = 0.f;
							//vRotTot.z = 0.f;

							m_pTransform->SetWorldPos(vChangedPos);
							//m_pTransform->SetWorldRot(vRotTot);
							

							//5월 7일 노트 : 트랜스폼 이동 후의 애니메이션 역시 변해버린 트랜스폼 정보를 가지고 있을 확률이 높다
							//m_bRootBoneTransformChange 가 true인 녀석들의 0번 프레임은 기본 자세인 경우가 절대적이다
							//PKEYFRAME pZeroKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[0];
							//Vector3 vPosDiffZeroToLast(DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pZeroKey->vPos.Convert(), fPercent));
							//vPosDiffZeroToLast *= m_pTransform->GetWorldScale();
							//Vector3 vChangedPos;
							//vChangedPos += m_vArrRootBoneAxis[0] * vPosDiffZeroToLast.x;
							////vChangedPos += m_vArrRootBoneAxis[1] * vPosDiffZeroToLast.y;
							//vChangedPos += m_vArrRootBoneAxis[2] * vPosDiffZeroToLast.z;
							//vChangedPos += m_vRootBonePosBuf;
							//m_pTransform->SetWorldPos(vChangedPos);
						}
					}
					else
					{
						m_bRootBoneTransformChange = false;
						m_vRootBonePosBuf = m_pTransform->GetWorldPos();
						m_vRootBoneRotBuf = m_pTransform->GetWorldRot();
					}


					*m_vecBones[i]->matBone = matBone;
					matBone = *m_vecBones[i]->matOffset * matBone;
					*m_vecBoneMatrix[i] = matBone;
				});
			}
			else //m_pNextClip != nullptr, m_bKeep true
			{
				m_bRootBoneTransformChange = false;
				m_fAnimationGlobalTime += fTime;
				m_fClipProgress = m_fAnimationGlobalTime / m_pCurClip->fTimeLength;

				while (m_fAnimationGlobalTime >= m_pCurClip->fTimeLength)
				{
					m_fAnimationGlobalTime -= m_pCurClip->fTimeLength;

				}

				float	fAnimationTime = m_fAnimationGlobalTime +
					m_pCurClip->fStartTime;

				int	iStartFrame = m_pCurClip->iStartFrame;
				int	iEndFrame = m_pCurClip->iEndFrame;

				g_iFrameIndex = (int)(fAnimationTime / m_pCurClip->fFrameTime);

				g_iNextFrameIndex = g_iFrameIndex + 1;

				m_pCurClip->iChangeFrame = g_iFrameIndex;
				m_pCurClip->iFrame = g_iFrameIndex;

				//이부분이 돌리는 건지
				if (g_iNextFrameIndex > iEndFrame)
				{
					g_iNextFrameIndex = iStartFrame;
				}

				int BoneSize = (int)m_vecBones.size();
				parallel_for((int)0, BoneSize, [&](int i)
				{
					// 키프레임이 없을 경우
					if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
					{
						*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
						return;
					}

					//생략 본들을 찾아보자
					for (int iPartIdx = 0; iPartIdx < iPartAnimCnt; ++iPartIdx)
					{
						if (!m_vecPartialAnim[iPartIdx]->bActivated)
							continue;

						std::vector<int>::iterator itr = (m_vecPartialAnim[iPartIdx])->vecPartIdx.begin();
						std::vector<int>::iterator itrEnd = (m_vecPartialAnim[iPartIdx])->vecPartIdx.end();
						for (; itr != itrEnd; ++itr)
						{
							if (i == (*itr))
							{
								if (m_vecPartialAnim[iPartIdx]->iRootParentIndex != i)
									return;
							}
						}
					}

					const PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[g_iFrameIndex];
					const PKEYFRAME pNextKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[g_iNextFrameIndex];


					Vector3 vCurKeyPos(pCurKey->vPos);
					Vector4 vCurKeyRot(pCurKey->vRot);
					Vector3 vCurKeyScale(pCurKey->vScale);

					XMVECTOR vR = vCurKeyRot.Convert();

					Vector3 vNextKeyPos(pNextKey->vPos);
					Vector4 vNextKeyRot(pNextKey->vRot);
					XMVECTOR vNR = vNextKeyRot.Convert();
					Vector3 vNextKeyScale(pNextKey->vScale);

					if (!m_pNextClip->vecKeyFrame[i]->vecKeyFrame.empty())
					{
						int iNEndFrame = m_pNextClip->iEndFrame;
						int	iNFrameIndex = (int)(fAnimationTime / m_pNextClip->fFrameTime);
						if (iNFrameIndex > iNEndFrame)
							iNFrameIndex = g_iFrameIndex % iNEndFrame;

						int	iNNextFrameIndex = iNFrameIndex + 1;

						m_pNextClip->iChangeFrame = iNFrameIndex;

						if (iNNextFrameIndex > iNEndFrame)
							iNNextFrameIndex = iStartFrame;

						const PKEYFRAME pNCurKey = m_pNextClip->vecKeyFrame[i]->vecKeyFrame[iNFrameIndex];
						const PKEYFRAME pNNextKey = m_pNextClip->vecKeyFrame[i]->vecKeyFrame[iNNextFrameIndex];

						vCurKeyPos += pNCurKey->vPos;
						vCurKeyScale += pNCurKey->vScale;

						vCurKeyPos *= 0.5f;
						vCurKeyScale *= 0.5f;
						vR = DirectX::XMQuaternionSlerp(vR, pNCurKey->vRot.Convert(), 0.5f);
						vCurKeyRot = Vector4(vR);
						vNextKeyPos += pNNextKey->vPos;

						vNextKeyScale += pNNextKey->vScale;

						vNextKeyPos *= 0.5f;
						vNR = DirectX::XMQuaternionSlerp(vNR, pNNextKey->vRot.Convert(), 0.5f);
						vNextKeyScale *= 0.5f;

					}

					m_vBlendPos = vCurKeyPos;
					m_vBlendScale = vCurKeyScale;
					m_vBlendRot = vCurKeyRot;

					// 현재 프레임의 시간을 얻어온다.
					double	 dFrameTime = pCurKey->dTime;

					float	fPercent = (float)((fAnimationTime - dFrameTime) / m_pCurClip->fFrameTime);

					XMVECTOR vS = DirectX::XMVectorLerp(vCurKeyScale.Convert(), vNextKeyScale.Convert(), fPercent);
					XMVECTOR vT = DirectX::XMVectorLerp(vCurKeyPos.Convert(), vNextKeyPos.Convert(), fPercent);
					vR = DirectX::XMQuaternionSlerp(vR, vNR, fPercent);
					XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
					Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

					*m_vecBones[i]->matBone = matBone;
					matBone = *m_vecBones[i]->matOffset * matBone;
					*m_vecBoneMatrix[i] = matBone;
				});
			}
		}

	}

	// 기존 모션이 계속 동작될때
	else
	{
		m_bCurClipStart = false;
		m_bRootBoneTransformChange = false;
		m_fAnimationGlobalTime += fTime;
		m_fClipProgress = m_fAnimationGlobalTime / m_pCurClip->fTimeLength;

		if (m_pCurClip->fTimeLength - m_fAnimationGlobalTime < 0.0666666666667f)
		{
			if (!m_bSkipBlending)
			{
				switch (m_pCurClip->eOption)
				{
				case AO_LOOP:
					m_pNextClip = m_pCurClip;
					m_fChangeLimitTime = 0.0625f;
					break;
				case AO_ONCE_RETURN:
					m_pNextClip = m_pDefaultClip;
					m_fChangeLimitTime = 0.25f;
					break;
				}
				m_bCurClipEnd = true;
				m_pCurClip->iFrame = 0;
			}
		}
		while (m_fAnimationGlobalTime >= m_pCurClip->fTimeLength)
		{
			m_fAnimationGlobalTime -= m_pCurClip->fTimeLength;
			m_bEnd = true;

			/*for (size_t i = 0; i < m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback.size();	++i)
			{
				m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback[i]->bCall = false;
			}*/
		}

		float	fAnimationTime = m_fAnimationGlobalTime +
			m_pCurClip->fStartTime;

		int	iStartFrame = m_pCurClip->iStartFrame;
		int	iEndFrame = m_pCurClip->iEndFrame;

		//첫 프레임과 끝 프레임 위치 사이에 오차가 존재하므로 끝물부터 합성 준비해야 함(쉬벌)
		int	iFrameIndex = (int)(fAnimationTime / m_pCurClip->fFrameTime);

		m_pCurClip->iFrame = iFrameIndex;

		if (m_bEnd)
		{

			switch (m_pCurClip->eOption)
			{
			case AO_LOOP:
				iFrameIndex = iStartFrame;
				break;
			case AO_ONCE_DESTROY:
				m_pObject->Die();
			case AO_ONCE_RETURN:
				m_pNextClip = m_pDefaultClip;
				m_fAnimationGlobalTime = 0.0f;
				m_fChangeTime = 0.f;
				m_fClipProgress = 0.f;

				iFrameIndex = 0;
				break;
			}
		}
		else
		{
			int	iNextFrameIndex = iFrameIndex + 1;

			m_pCurClip->iChangeFrame = iFrameIndex;
			m_pCurClip->iFrame = iFrameIndex;

			if (iNextFrameIndex > iEndFrame)
			{
				iNextFrameIndex = iStartFrame;
				//m_bCurClipEnd = true;
				//m_pCurClip->iFrame = 0;
			}

			int BoneSize = (int)m_vecBones.size();
			parallel_for((int)0, BoneSize, [&](int i)
			{
				// 키프레임이 없을 경우
				if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
				{
					*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
					return;
				}

				//생략 본들을 찾아보자
				for (int iPartIdx = 0; iPartIdx < iPartAnimCnt; ++iPartIdx)
				{
					if (!m_vecPartialAnim[iPartIdx]->bActivated)
						continue;

					std::vector<int>::iterator itr = (m_vecPartialAnim[iPartIdx])->vecPartIdx.begin();
					std::vector<int>::iterator itrEnd = (m_vecPartialAnim[iPartIdx])->vecPartIdx.end();
					for (; itr != itrEnd; ++itr)
					{
						if (i == (*itr))
						{
							if (m_vecPartialAnim[iPartIdx]->iRootParentIndex != i)
								return;
						}
					}
				}

				const PKEYFRAME pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
				const PKEYFRAME pNextKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

				m_vBlendPos = pCurKey->vPos;
				m_vBlendScale = pCurKey->vScale;
				m_vBlendRot = pCurKey->vRot;

				// 현재 프레임의 시간을 얻어온다.
				double	 dFrameTime = pCurKey->dTime;

				float	fPercent = (float)((fAnimationTime - dFrameTime) / m_pCurClip->fFrameTime);

				XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
				XMVECTOR vT = DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
				XMVECTOR vR = DirectX::XMQuaternionSlerp(pCurKey->vRot.Convert(), pNextKey->vRot.Convert(), fPercent);
				XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
				Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

				*m_vecBones[i]->matBone = matBone;
				matBone = *m_vecBones[i]->matOffset * matBone;
				*m_vecBoneMatrix[i] = matBone;
			});
		}
	}

	//Partial Anim
	std::vector<PPARTANIM>::iterator itrPart = m_vecPartialAnim.begin();
	std::vector<PPARTANIM>::iterator itrEnd = m_vecPartialAnim.end();
	for (; itrPart != itrEnd; ++itrPart)
	{
		PPARTANIM part = *itrPart;
		part->bEnd = false;
		if (!part->bActivated)
		{
			part->fPartAnimationGTime = 0.f;

			part->fChangeTime = 0.f;
			part->fClipProgress = 0.f;
			continue;
		}

		if (part->mapPartClips.empty())
		{
			part->bEnd = true;
			part->fPartAnimationGTime = 0.f;

			part->fChangeTime = 0.f;
			part->fClipProgress = 0.f;
			continue;
		}

		//Run Partial Anim Notify functions
		if (part->pCurClip)
		{
			float fBeforeTime = part->fPartAnimationGTime - fTime;

			if (fBeforeTime < 0.f)
			{
				fBeforeTime = 0.f;

				std::vector<PANIMATIONCALLBACK>::iterator itrEnd = part->pCurClip->vecCallback.end();
				for (std::vector<PANIMATIONCALLBACK>::iterator itr = part->pCurClip->vecCallback.begin();
					itr != itrEnd; ++itr)
				{
					PANIMATIONCALLBACK pCallbackInfo = (*itr);
					pCallbackInfo->bCall = false;

				}
			}

			std::vector<PANIMATIONCALLBACK>::iterator itrEnd = part->pCurClip->vecCallback.end();
			for (std::vector<PANIMATIONCALLBACK>::iterator itr = part->pCurClip->vecCallback.begin();
				itr != itrEnd; ++itr)
			{
				PANIMATIONCALLBACK pCallbackInfo = (*itr);
				float fActivateTime = part->pCurClip->fEndTime * pCallbackInfo->fAnimationProgress;
				if (pCallbackInfo->bCall)
					continue;

				if (fActivateTime > fBeforeTime && fActivateTime <= part->fPartAnimationGTime)
				{
					pCallbackInfo->func(fTime);
					pCallbackInfo->bCall = true;
				}
			}
		}
		if (part->pNextClip)
		{
			float fBeforeTime = part->fPartAnimationGTime - fTime;
			if (fBeforeTime < 0.f)
			{
				fBeforeTime = 0.f;

				std::vector<PANIMATIONCALLBACK>::iterator itrEnd = part->pNextClip->vecCallback.end();
				for (std::vector<PANIMATIONCALLBACK>::iterator itr = part->pNextClip->vecCallback.begin();
					itr != itrEnd; ++itr)
				{
					PANIMATIONCALLBACK pCallbackInfo = (*itr);
					pCallbackInfo->bCall = false;

				}
			}

			std::vector<PANIMATIONCALLBACK>::iterator itrEnd = part->pNextClip->vecCallback.end();
			for (std::vector<PANIMATIONCALLBACK>::iterator itr = part->pNextClip->vecCallback.begin();
				itr != itrEnd; ++itr)
			{
				PANIMATIONCALLBACK pCallbackInfo = (*itr);
				float fActivateTime = part->pNextClip->fEndTime * pCallbackInfo->fAnimationProgress;
				if (pCallbackInfo->bCall)
					continue;

				if (fActivateTime > fBeforeTime && fActivateTime <= part->fPartAnimationGTime)
				{
					pCallbackInfo->func(fTime);
					pCallbackInfo->bCall = true;
				}
			}
		}
		//Partial Anim Notify end

		//Partial Anim 모션이 변할때
		if (part->pNextClip)
		{
			part->fChangeLimitTime = 0.25f;
			if (part->pCurClip == nullptr)
			{
				part->pCurClip = part->pNextClip;
				part->pNextClip = nullptr;
				part->fPartAnimationGTime = 0.f;
				part->fChangeTime = 0.f;
			}
			else
			{
				part->fChangeTime += fTime;

				bool	bChange = false;
				if (part->fChangeTime >= part->fChangeLimitTime)
				{
					if (!part->bKeepBlending)
					{
						part->fChangeTime = part->fChangeLimitTime;
						bChange = true;
					}

				}

				float	fAnimationTime = part->fPartAnimationGTime + part->pCurClip->fStartTime;

				int BoneSize = (int)part->vecPartIdx.size();

				if (bChange)
				{
					part->pCurClip = part->pNextClip;
					part->pNextClip = nullptr;
					part->fPartAnimationGTime = 0.f;
					part->fChangeTime = 0.f;
				}
				else if (!part->bKeepBlending)
				{
					parallel_for((int)0, BoneSize, [&](int i)
					{
						int iBoneIdx = part->vecPartIdx[i];
						// 키프레임이 없을 경우
						// 매 클립의 vecKeyFrame -> 본 갯수만큼 있다
						// parallel_for의 i 번호 : 본 번호


						if (part->pNextClip->vecKeyFrame[i]->vecKeyFrame.empty())
						{
							*m_vecBoneMatrix[iBoneIdx] = *m_vecBones[iBoneIdx]->matBone;
							return;
						}

						int	iFrameIndex = part->pCurClip->iChangeFrame;
						int	iNextFrameIndex = part->pNextClip->iStartFrame;

						const PKEYFRAME pCurKey = part->pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
						const PKEYFRAME pNextKey = part->pNextClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

						float	fPercent = part->fChangeTime / part->fChangeLimitTime;

						//Method 1
#ifdef PART_METHOD_1
						Vector3 vParentMove;
						Vector4 vParentQuat;

						if (part->iRootParentIndex > -1)
						{

							DirectX::XMVECTOR xmRot;
							DirectX::XMVECTOR xmPos;
							DirectX::XMVECTOR xmScale;

							//느리지만 눈물을 머금구..
							DirectX::XMMatrixDecompose(&xmScale, &xmRot, &xmPos, m_vecBones[part->iRootParentIndex]->matBone->matrix);

							vParentMove = Vector3(xmPos);
							vParentQuat = Vector4(xmRot);
						}

						XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
						XMVECTOR vT;
						//XMVECTOR vT = DirectX::XMVectorLerp((pCurKey->vPos + vParentMove).Convert(), (pNextKey->vPos + vParentMove).Convert(), fPercent);
						XMVECTOR vR = DirectX::XMQuaternionSlerp(pCurKey->vRot.Convert(),
							pNextKey->vRot.Convert(), fPercent);
						vR = DirectX::XMQuaternionMultiply(vR, vParentQuat.Convert());
						//회전 매트릭스 구하기
						XMMATRIX vRotMat = DirectX::XMMatrixRotationQuaternion(vR);

						//방향벡터 구하기 및 표준화
						Vector3 vRotVector[3] = {};
						for (int i = 0; i < 3; ++i)
						{
							vRotVector[i] = Vector3::Axis[i].TransformNormal(vRotMat);
							vRotVector[i].Normalize();
						}

						//pCurKey 와 pNextKey 사이 회전된 위치 구하기
						Vector3 vCurRotatedPos = vParentMove + (vRotVector[0] * pCurKey->vPos.x)	+ (vRotVector[1] * pCurKey->vPos.y) + (vRotVector[2] * pCurKey->vPos.z);
						Vector3 vNextRotatedPos = vParentMove + (vRotVector[0] * pNextKey->vPos.x)	+ (vRotVector[1] * pNextKey->vPos.y) + (vRotVector[2] * pNextKey->vPos.z);
						//Vector3 vCurRotatedPos = vParentMove + pCurKey->vPos;
						//Vector3 vNextRotatedPos = vParentMove + pNextKey->vPos;

						//최종 vT 구하기
						vT = DirectX::XMVectorLerp(vCurRotatedPos.Convert(), vNextRotatedPos.Convert(), fPercent);
						
						XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
						Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);
						//Method 1 end
#else
#ifdef PART_METHOD_2
						//Method 2
						XMVECTOR vRCurr = pCurKey->vRot.Convert();
						XMVECTOR vRNext = pNextKey->vRot.Convert();

						XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
						XMVECTOR vT = DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
						XMVECTOR vR = DirectX::XMQuaternionSlerp(vRCurr, vRNext, fPercent);
						XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
						Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

						//부모 본 가지고 와서 곱한다
						if (part->iRootParentIndex > -1)
						{
							if(!part->bUseCustomParent)
								matBone = matBone * (*m_vecBones[part->iRootParentIndex]->matBone);
							else
								matBone = matBone * part->matCustomParent;
						}

						//Method 2 end

#endif
#endif


						*m_vecBones[iBoneIdx]->matBone = matBone;
						matBone = *m_vecBones[iBoneIdx]->matOffset * matBone;
						*m_vecBoneMatrix[iBoneIdx] = matBone;
					});
				}
				else //part->pNextClip != nullptr
				{
					part->fPartAnimationGTime += fTime;
					part->fClipProgress = part->fPartAnimationGTime / part->pCurClip->fTimeLength;

					while (part->fPartAnimationGTime >= part->pCurClip->fTimeLength)
					{
						part->fPartAnimationGTime -= part->pCurClip->fTimeLength;

					}

					float	fAnimationTime = part->fPartAnimationGTime +
						part->pCurClip->fStartTime;

					int	iStartFrame = part->pCurClip->iStartFrame;
					int	iEndFrame = part->pCurClip->iEndFrame;

					int	iFrameIndex = (int)(fAnimationTime / part->pCurClip->fFrameTime);

					int	iNextFrameIndex = iFrameIndex + 1;

					part->pCurClip->iChangeFrame = iFrameIndex;

					if (iNextFrameIndex > iEndFrame)
						iNextFrameIndex = iStartFrame;

					int BoneSize = (int)part->vecPartIdx.size();
					parallel_for((int)0, BoneSize, [&](int i)
					{
						// 키프레임이 없을 경우
						int iBoneIdx = part->vecPartIdx[i];
						if (part->pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
						{
							*m_vecBoneMatrix[iBoneIdx] = *m_vecBones[iBoneIdx]->matBone;
							return;
						}

						const PKEYFRAME pCurKey = part->pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
						const PKEYFRAME pNextKey = part->pCurClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];


						Vector3 vCurKeyPos(pCurKey->vPos);
						Vector4 vCurKeyRot(pCurKey->vRot);
						Vector3 vCurKeyScale(pCurKey->vScale);

						XMVECTOR vR = vCurKeyRot.Convert();

						Vector3 vNextKeyPos(pNextKey->vPos);
						Vector4 vNextKeyRot(pNextKey->vRot);
						XMVECTOR vNR = vNextKeyRot.Convert();
						Vector3 vNextKeyScale(pNextKey->vScale);

						if (!part->pNextClip->vecKeyFrame[i]->vecKeyFrame.empty())
						{
							int iNEndFrame = part->pNextClip->iEndFrame;
							int	iNFrameIndex = (int)(fAnimationTime / part->pNextClip->fFrameTime);
							if (iNFrameIndex > iNEndFrame)
								iNFrameIndex = iFrameIndex % iNEndFrame;

							int	iNNextFrameIndex = iNFrameIndex + 1;

							part->pNextClip->iChangeFrame = iNFrameIndex;

							if (iNNextFrameIndex > iNEndFrame)
								iNNextFrameIndex = iStartFrame;

							const PKEYFRAME pNCurKey = part->pNextClip->vecKeyFrame[i]->vecKeyFrame[iNFrameIndex];
							const PKEYFRAME pNNextKey = part->pNextClip->vecKeyFrame[i]->vecKeyFrame[iNNextFrameIndex];

							vCurKeyPos += pNCurKey->vPos;
							vCurKeyScale += pNCurKey->vScale;

							vCurKeyPos *= 0.5f;
							vCurKeyScale *= 0.5f;
							vR = DirectX::XMQuaternionSlerp(vR, pNCurKey->vRot.Convert(), 0.5f);
							vCurKeyRot = Vector4(vR);
							vNextKeyPos += pNNextKey->vPos;

							vNextKeyScale += pNNextKey->vScale;

							vNextKeyPos *= 0.5f;
							vNR = DirectX::XMQuaternionSlerp(vNR, pNNextKey->vRot.Convert(), 0.5f);
							vNextKeyScale *= 0.5f;

						}

						part->vBlendPos = vCurKeyPos;
						part->vBlendScale = vCurKeyScale;
						part->vBlendRot = vCurKeyRot;

						// 현재 프레임의 시간을 얻어온다.
						double	 dFrameTime = pCurKey->dTime;

						float	fPercent = (float)((fAnimationTime - dFrameTime) / part->pCurClip->fFrameTime);
						//Method 1
#ifdef PART_METHOD_1
						Vector3 vParentMove;
						Vector4 vParentQuat;

						if (part->iRootParentIndex > -1)
						{

							DirectX::XMVECTOR xmRot;
							DirectX::XMVECTOR xmPos;
							DirectX::XMVECTOR xmScale;

							//느리지만 눈물을 머금구..
							DirectX::XMMatrixDecompose(&xmScale, &xmRot, &xmPos, m_vecBones[part->iRootParentIndex]->matBone->matrix);

							vParentMove = Vector3(xmPos);
							vParentQuat = Vector4(xmRot);
						}

						XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
						XMVECTOR vT;
						//XMVECTOR vT = DirectX::XMVectorLerp((pCurKey->vPos + vParentMove).Convert(), (pNextKey->vPos + vParentMove).Convert(), fPercent);
						vR = DirectX::XMQuaternionSlerp(pCurKey->vRot.Convert(),
							pNextKey->vRot.Convert(), fPercent);
						vR = DirectX::XMQuaternionMultiply(vR, vParentQuat.Convert());
						//회전 매트릭스 구하기
						XMMATRIX vRotMat = DirectX::XMMatrixRotationQuaternion(vR);

						//방향벡터 구하기 및 표준화
						Vector3 vRotVector[3] = {};
						for (int i = 0; i < 3; ++i)
						{
							vRotVector[i] = Vector3::Axis[i].TransformNormal(vRotMat);
							vRotVector[i].Normalize();
						}

						//pCurKey 와 pNextKey 사이 회전된 위치 구하기
						Vector3 vCurRotatedPos = vParentMove + (vRotVector[0] * pCurKey->vPos.x)	+ (vRotVector[1] * pCurKey->vPos.y) + (vRotVector[2] * pCurKey->vPos.z);
						Vector3 vNextRotatedPos = vParentMove + (vRotVector[0] * pNextKey->vPos.x)	+ (vRotVector[1] * pNextKey->vPos.y) + (vRotVector[2] * pNextKey->vPos.z);
						//Vector3 vCurRotatedPos = vParentMove + pCurKey->vPos;
						//Vector3 vNextRotatedPos = vParentMove + pNextKey->vPos;

						//최종 vT 구하기
						vT = DirectX::XMVectorLerp(vCurRotatedPos.Convert(), vNextRotatedPos.Convert(), fPercent);
						
						XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
						Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);
						//Method 1 end
#else
#ifdef PART_METHOD_2
						//Method 2
						XMVECTOR vRCurr = pCurKey->vRot.Convert();
						XMVECTOR vRNext = pNextKey->vRot.Convert();

						XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
						XMVECTOR vT = DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
						vR = DirectX::XMQuaternionSlerp(vRCurr, vRNext, fPercent);
						XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
						Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);


						//부모 본 가지고 와서 곱한다
						if (part->iRootParentIndex > -1)
						{
							if (!part->bUseCustomParent)
								matBone = matBone * (*m_vecBones[part->iRootParentIndex]->matBone);
							else
								matBone = matBone * part->matCustomParent;
						}
						//Method 2 end

#endif
#endif

						*m_vecBones[iBoneIdx]->matBone = matBone;
						matBone = *m_vecBones[iBoneIdx]->matOffset * matBone;
						*m_vecBoneMatrix[iBoneIdx] = matBone;
					});
				}
			}

		}

		// 기존 모션이 계속 동작될때
		else
		{
			part->fPartAnimationGTime += fTime;
			part->fClipProgress = part->fPartAnimationGTime / part->pCurClip->fTimeLength;

			if (part->pCurClip->fTimeLength - part->fPartAnimationGTime < 0.06666666667f)
			{
				switch (part->pCurClip->eOption)
				{
				case AO_LOOP:
					part->pNextClip = part->pCurClip;
					part->fChangeLimitTime = 0.0625f;
					break;
				case AO_ONCE_RETURN:
					part->pNextClip = part->pDefaultClip;
					part->fChangeLimitTime = 0.25f;
					break;
				}
			}

			while (part->fPartAnimationGTime >= part->pCurClip->fTimeLength)
			{
				part->fPartAnimationGTime -= part->pCurClip->fTimeLength;
				part->bEnd = true;

				/*for (size_t i = 0; i < m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback.size();	++i)
				{
					m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback[i]->bCall = false;
				}*/
			}

			float	fAnimationTime = part->fPartAnimationGTime +
				part->pCurClip->fStartTime;

			int	iStartFrame = part->pCurClip->iStartFrame;
			int	iEndFrame = part->pCurClip->iEndFrame;

			int	iFrameIndex = (int)(fAnimationTime / part->pCurClip->fFrameTime);

			if (part->bEnd)
			{
				switch (part->pCurClip->eOption)
				{
				case AO_LOOP:
					iFrameIndex = iStartFrame;
					break;
				case AO_ONCE_DESTROY:
					//m_pObject->Die();
					break;
				case AO_ONCE_RETURN:
					iFrameIndex = 0;
					part->fPartAnimationGTime = 0.f;
					part->pCurClip = part->pDefaultClip;
					break;
				}
			}
			else
			{
				int	iNextFrameIndex = iFrameIndex + 1;

				part->pCurClip->iChangeFrame = iFrameIndex;

				if (iNextFrameIndex > iEndFrame)
					iNextFrameIndex = iStartFrame;

				int BoneSize = (int)part->vecPartIdx.size();
				parallel_for((int)0, BoneSize, [&](int i)
				{
					int iBoneIdx = part->vecPartIdx[i];
					// 키프레임이 없을 경우
					if (part->pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
					{
						*m_vecBoneMatrix[iBoneIdx] = *m_vecBones[iBoneIdx]->matBone;
						return;
					}

					const PKEYFRAME pCurKey = part->pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
					const PKEYFRAME pNextKey = part->pCurClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

					part->vBlendPos = pCurKey->vPos;
					part->vBlendScale = pCurKey->vScale;
					part->vBlendRot = pCurKey->vRot;

					// 현재 프레임의 시간을 얻어온다.
					double	 dFrameTime = pCurKey->dTime;

					float	fPercent = (float)((fAnimationTime - dFrameTime) / part->pCurClip->fFrameTime);
					//Method 1
#ifdef PART_METHOD_1
					Vector3 vParentMove;
					Vector4 vParentQuat;

					if (part->iRootParentIndex > -1)
					{

						DirectX::XMVECTOR xmRot;
						DirectX::XMVECTOR xmPos;
						DirectX::XMVECTOR xmScale;

						//느리지만 눈물을 머금구..
						DirectX::XMMatrixDecompose(&xmScale, &xmRot, &xmPos, m_vecBones[part->iRootParentIndex]->matBone->matrix);

						vParentMove = Vector3(xmPos);
						vParentQuat = Vector4(xmRot);
					}

					XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
					XMVECTOR vT;
					//XMVECTOR vT = DirectX::XMVectorLerp((pCurKey->vPos + vParentMove).Convert(), (pNextKey->vPos + vParentMove).Convert(), fPercent);
					XMVECTOR vR = DirectX::XMQuaternionSlerp(pCurKey->vRot.Convert(),
						pNextKey->vRot.Convert(), fPercent);
					vR = DirectX::XMQuaternionMultiply(vR, vParentQuat.Convert());
					//회전 매트릭스 구하기
					XMMATRIX vRotMat = DirectX::XMMatrixRotationQuaternion(vR);

					//방향벡터 구하기 및 표준화
					Vector3 vRotVector[3] = {};
					for (int i = 0; i < 3; ++i)
					{
						vRotVector[i] = Vector3::Axis[i].TransformNormal(vRotMat);
						vRotVector[i].Normalize();
					}

					//pCurKey 와 pNextKey 사이 회전된 위치 구하기
					Vector3 vCurRotatedPos = vParentMove + (vRotVector[0] * pCurKey->vPos.x)	+ (vRotVector[1] * pCurKey->vPos.y) + (vRotVector[2] * pCurKey->vPos.z);
					Vector3 vNextRotatedPos = vParentMove + (vRotVector[0] * pNextKey->vPos.x)	+ (vRotVector[1] * pNextKey->vPos.y) + (vRotVector[2] * pNextKey->vPos.z);
					//Vector3 vCurRotatedPos = vParentMove + pCurKey->vPos;
					//Vector3 vNextRotatedPos = vParentMove + pNextKey->vPos;

					//최종 vT 구하기
					vT = DirectX::XMVectorLerp(vCurRotatedPos.Convert(), vNextRotatedPos.Convert(), fPercent);
					
					XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
					Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);
					//Method 1 end
#else
#ifdef PART_METHOD_2
					//Method 2
					XMVECTOR vRCurr = pCurKey->vRot.Convert();
					XMVECTOR vRNext = pNextKey->vRot.Convert();

					XMVECTOR vS = DirectX::XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
					XMVECTOR vT = DirectX::XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
					XMVECTOR vR = DirectX::XMQuaternionSlerp(vRCurr, vRNext, fPercent);
					XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
					Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

					//부모 본 가지고 와서 곱한다
					if (part->iRootParentIndex > -1)
					{
						if (!part->bUseCustomParent)
							matBone = matBone * (*m_vecBones[part->iRootParentIndex]->matBone);
						else
							matBone = matBone * part->matCustomParent;
					}
					//Method 2 end

#endif
#endif
					*m_vecBones[iBoneIdx]->matBone = matBone;
					matBone = *m_vecBones[iBoneIdx]->matOffset * matBone;
					*m_vecBoneMatrix[iBoneIdx] = matBone;
				});
			}
		}

		if (m_bEnd)
		{
			if (!part->bEnd)
				m_bEnd = false;
		}
	}

	if (!m_bEnd)
	{
		D3D11_MAPPED_SUBRESOURCE	tMap = {};

		CONTEXT->Map(m_pBoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		Matrix*	pMatrix = (Matrix*)tMap.pData;

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
			pMatrix[i] = *m_vecBoneMatrix[i];

		CONTEXT->Unmap(m_pBoneTex, 0);
	}

	return 0;
}

int CAnimation::LateUpdate(float fTime)
{
	if (m_vecBoneMatrix.size() > 0)
	{
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			for (size_t j = 0; j < m_vecBones[i]->SocketList.size(); j++)
			{
				list<CBoneSocket*>::iterator iter;
				list<CBoneSocket*>::iterator iterEnd = m_vecBones[i]->SocketList.end();
				for (iter = m_vecBones[i]->SocketList.begin(); iter != iterEnd; ++iter)
				{
					(*iter)->Update(fTime, *m_vecBoneMatrix[i], m_pTransform);
				}
			}
		}
	}

	return 0;
}

void CAnimation::Collision(float fTime)
{
}

void CAnimation::Render(float fTime)
{
	m_bCurClipEnd = false; 
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}

bool _tagPartialAnim::LoadPartAnimFromExistingClip(PANIMATIONCLIP pAnim)
{
	std::unordered_map<std::string, PPARTCLIP>::iterator itr = mapPartClips.find(pAnim->strName);
	if (itr != mapPartClips.end())
		return false;

	PPARTCLIP _newAnim = new PARTCLIP;
	mapPartClips.insert(std::make_pair(pAnim->strName, _newAnim));
	if (!pCurClip)
		pCurClip = _newAnim;
	//복사할 소스 클립 루프
	_newAnim->eOption = pAnim->eOption;
	_newAnim->strName = pAnim->strName;
	_newAnim->fStartTime = pAnim->fStartTime;
	_newAnim->fEndTime = pAnim->fEndTime;
	_newAnim->fTimeLength = pAnim->fTimeLength;
	_newAnim->fFrameTime = pAnim->fFrameTime;
	_newAnim->fPlayTime = pAnim->fPlayTime;
	_newAnim->iStartFrame = pAnim->iStartFrame;
	_newAnim->iEndFrame = pAnim->iEndFrame;
	_newAnim->iFrameLength = pAnim->iFrameLength;
	_newAnim->iFrameMode = pAnim->iFrameMode;
	_newAnim->iChangeFrame = pAnim->iChangeFrame;

	int ipAnimSize = (int)pAnim->vecKeyFrame.size();


	unsigned int iBoneSize = (unsigned int)vecPartIdx.size();
	_newAnim->vecKeyFrame.resize(iBoneSize);
	int iPBI = -1;


	if (iRootParentIndex > 0)
	{
		int iFrameSize = (int)pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame.size();
		if (iFrameSize > 0)
		{
			_newAnim->vecXmRotVector.resize(iFrameSize);
			for (int i = 0; i < iFrameSize; ++i)
			{
				_newAnim->vecXmRotVector[i] = DirectX::XMQuaternionInverse(pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[i]->vRot.Convert());
			}
		}
	}

	for (unsigned int i = 0; i < iBoneSize; ++i)
	{
		iPBI = vecPartIdx[i];
		_newAnim->vecKeyFrame[i] = new BONEKEYFRAME;
		_newAnim->vecKeyFrame[i]->iBoneIndex = pAnim->vecKeyFrame[iPBI]->iBoneIndex;
		int iFrameSize = (int)pAnim->vecKeyFrame[iPBI]->vecKeyFrame.size();
		_newAnim->vecKeyFrame[i]->vecKeyFrame.resize(iFrameSize);

		if (iRootParentIndex < 0)
			continue;


		if (pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame.size() > 0)
		{
			if (_newAnim->vecXmRotVector.size() < 1)
			{
				_newAnim->vecXmRotVector.resize(pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame.size());
			}

			for (int j = 0; j < iFrameSize; ++j)
			{
				_newAnim->vecKeyFrame[i]->vecKeyFrame[j] = new KEYFRAME;
				*(_newAnim->vecKeyFrame[i]->vecKeyFrame[j]) = *(pAnim->vecKeyFrame[iPBI]->vecKeyFrame[j]);
				//_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vPos -= pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vPos;
				
				//DirectX::XMVECTOR vConjugatedRootRot = DirectX::XMQuaternionInverse(pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vRot.Convert());
				//_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vRot = Vector4(DirectX::XMQuaternionMultiply(_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vRot.Convert(), vConjugatedRootRot));
				
				//_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vScale /= pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vScale;
				//세 부분을 따로 나누지 말고, Affine Matrix로 변환한 이후 부모행렬의 역행렬을 구하고 이 값을 다시 Decompose해보자
				Matrix matParentMatBone;
				XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

				Matrix matCurrPartBone;

				matParentMatBone = DirectX::XMMatrixAffineTransformation(pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vScale.Convert()
					, vZero, pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vRot.Convert(), pAnim->vecKeyFrame[iRootParentIndex]->vecKeyFrame[j]->vPos.Convert());

				matCurrPartBone = DirectX::XMMatrixAffineTransformation(_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vScale.Convert()
					, vZero, _newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vRot.Convert(), _newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vPos.Convert());

				matParentMatBone.Inverse();
				matCurrPartBone *= matParentMatBone;

				XMVECTOR xmVecPos;
				XMVECTOR xmVecRot;
				XMVECTOR xmVecScale;
				DirectX::XMMatrixDecompose(&xmVecScale, &xmVecRot, &xmVecPos, matCurrPartBone.matrix);

				_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vPos = Vector3(xmVecPos);
				_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vRot = Vector4(xmVecRot);
				_newAnim->vecKeyFrame[i]->vecKeyFrame[j]->vScale = Vector3(xmVecScale);
			}
		}

	}

	return true;
}

bool _tagPartialAnim::LoadPartAnimFromNewFile(const std::wstring & filePath, const std::string& strPathTag)
{
	const TCHAR *path = CPathManager::GetInst()->FindPath(strPathTag);
	std::wstring _path = filePath;
	_path += path;
	FILE *pFile = nullptr;
	_wfopen_s(&pFile, _path.c_str(), TEXT("rb"));

	if (!pFile)
		return false;

	fread(&iFrameMode, sizeof(float), 1, pFile);
	fread(&fChangeLimitTime, sizeof(float), 1, pFile);

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

	strAddClipName.clear();

	int kEnd = (int)vecPartIdx.size();
	int iBoneSteps = 0;

	for (int l = 0; l < iCount; ++l)
	{
		PPARTCLIP	pClip = new PARTCLIP;

		// 애니메이션 클립 키를 저장한다.
		char	strClipName[256] = {};
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strClipName, sizeof(char), iLength, pFile);
		//std::cout << strClipName << std::endl;
		strAddClipName.push_back(strClipName);

		mapPartClips.insert(make_pair(strClipName, pClip));

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
			bool bNext = true;
			for (int k = 0; k < kEnd; ++k)
			{
				if (vecPartIdx[k] == i)
				{
					bNext = false;
					break;
				}
			}
			if (bNext)
			{
				Vector4 vEmpty;
				fread(&vEmpty, sizeof(int), 1, pFile);
				fread(&vEmpty, sizeof(size_t), 1, pFile);
				continue;
			}

			PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;
			pClip->vecKeyFrame.push_back(pBoneKeyFrame);

			fread(&pBoneKeyFrame->iBoneIndex, sizeof(int), 1,
				pFile);

			size_t	iBoneFrameCount = 0;

			fread(&iBoneFrameCount, sizeof(size_t), 1, pFile);

			iBoneSteps = 0;
			for (size_t j = 0; j < iBoneFrameCount; ++j)
			{
				bool bNext = true;
				for (int k = 0; k < kEnd; ++k)
				{
					if (vecPartIdx[k] == i)
					{
						bNext = false;
						break;
					}
				}
				if (bNext)
				{
					Vector4 vEmpty;
					fread(&vEmpty, sizeof(double), 1, pFile);
					fread(&vEmpty, sizeof(Vector3), 1, pFile);
					fread(&vEmpty, sizeof(Vector3), 1, pFile);
					fread(&vEmpty, sizeof(Vector4), 1, pFile);
					continue;
				}

				PKEYFRAME	pKeyFrame = new KEYFRAME;
				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

				fread(&pKeyFrame->dTime, sizeof(double), 1, pFile);
				fread(&pKeyFrame->vPos, sizeof(Vector3), 1, pFile);
				fread(&pKeyFrame->vScale, sizeof(Vector3), 1, pFile);
				fread(&pKeyFrame->vRot, sizeof(Vector4), 1, pFile);
			}
		}
	}


	fclose(pFile);

	return true;
}

bool _tagPartialAnim::LoadPartClipFromFile(const std::string & strClipKey, std::wstring & filePath, const std::string & strPathTag)
{
	FILE *pFile = nullptr;

	std::wstring str;

	str = PUN::CPathManager::GetInst()->FindPath(MESH_DATA_PATH);
	str += filePath;

	_wfopen_s(&pFile, str.c_str(), TEXT("rb"));

	if (!pFile)
		return false;

	char strBuf[1024] = {};

	fread(strBuf, sizeof(float), 1, pFile);
	fread(strBuf, sizeof(float), 1, pFile);

	// 애니메이션 클립정보 >> 불필요
	size_t iCount = 0, iLength = 0;
	fread(&iCount, sizeof(size_t), 1, pFile);

	char	strDefaultClip[256] = {};
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strDefaultClip, sizeof(char),
		iLength, pFile);

	char	strCurClip[256] = {};
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strCurClip, sizeof(char), iLength, pFile);

	for (int l = 0; l < iCount; ++l)
	{
		PANIMATIONCLIP	pClip = new ANIMATIONCLIP;

		// 애니메이션 클립 키를 저장한다.
		char	strClipName[256] = {};
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strClipName, sizeof(char), iLength, pFile);
		//std::cout << strClipName << std::endl;

		pClip->strName = strClipKey;
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

		//클립이 완성되었으니, 이 클립으로 부분 애니메이션을 만든다
		LoadPartAnimFromExistingClip(pClip);

		//이제 클립은 무의미하다
		delete pClip;
	}


	fclose(pFile);

	return true;
}

void _tagAnimationClip::SetRootTransformBone(int idx)
{
	iRootTransformBoneIdx = idx;
}

void _tagAnimationClip::UseRootTransformBone(bool bOn)
{
	bUpdateRootTransform = bOn;
}
