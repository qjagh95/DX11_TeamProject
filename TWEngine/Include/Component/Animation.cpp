#include "Animation.h"
#include "../Device.h"
#include "../Resource/FbxLoader.h"
#include "../GameObject.h"

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
	m_fChangeLimitTime(0.25f)
{
	m_eComType = CT_ANIMATION;
}


CAnimation::CAnimation(const CAnimation & anim) :
	CComponent(anim)
{
	m_fAnimationGlobalTime = 0.f;
	m_fClipProgress = 0.f;
	m_fChangeTime = 0.f;
	m_fChangeLimitTime = anim.m_fChangeLimitTime;

	m_vecBones = anim.m_vecBones;

	for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	{
		++m_vecBones[i]->iRefCount;
	}

	//for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	//{
	//	PBONE	pBone = new BONE;

	//	*pBone = *anim.m_vecBones[i];

	//	pBone->matOffset = new Matrix;
	//	pBone->matBone = new Matrix;

	//	*pBone->matOffset = *anim.m_vecBones[i]->matOffset;
	//	*pBone->matBone = *anim.m_vecBones[i]->matBone;

	//	/*list<CBoneSocket*>::const_iterator	iterB;
	//	list<CBoneSocket*>::const_iterator	iterBEnd = anim.m_vecBones[i]->SocketList.end();

	//	for (iterB = anim.m_vecBones[i]->SocketList.begin(); iterB != iterBEnd; ++iterB)
	//	{
	//		pBone->SocketList.push_back((*iterB)->Clone());
	//	}*/

	//	m_vecBones.push_back(pBone);
	//}

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
	tDesc.Width = m_vecBones.size() * 4;
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

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	pAnimClip->iStartFrame = pClip->tStart.GetFrameCount(pClip->eTimeMode);
	pAnimClip->iEndFrame = pClip->tEnd.GetFrameCount(pClip->eTimeMode);
	pAnimClip->iFrameLength = pAnimClip->iEndFrame - pAnimClip->iStartFrame;

	// 시간 정보를 저장해준다.
	pAnimClip->fStartTime = pClip->tStart.GetSecondDouble();
	pAnimClip->fEndTime = pClip->tEnd.GetSecondDouble();
	pAnimClip->fTimeLength = pAnimClip->fEndTime - pAnimClip->fStartTime;

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

			pKeyFrame->dTime = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].dTime;

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

	pAnimClip->fFrameTime = 1.f / pAnimClip->iFrameMode;

	if (m_mapClip.empty())
	{
		m_pDefaultClip = pAnimClip;
		m_pCurClip = pAnimClip;
	}

	m_mapClip.insert(make_pair(pAnimClip->strName, pAnimClip));
}

PANIMATIONCLIP CAnimation::FindClip(const string & strName)
{
	std::unordered_map<std::string, PANIMATIONCLIP>::iterator Iter = m_mapClip.find(strName);
	
	if (Iter == m_mapClip.end())
		return nullptr;

	return Iter->second;
}

bool CAnimation::IsAnimationEnd() const
{
	return m_bEnd;
}

void CAnimation::ChangeClipKey(const string & strOrigin, const string & strChange)
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
			return i;
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

ID3D11ShaderResourceView * CAnimation::GetBoneTexture() const
{
	return m_pBoneRV;
}

bool CAnimation::Save(const wchar_t * pFileName, const string & strPathKey)
{
	return false;
}

bool CAnimation::Save(const char * pFileName, const string & strPathKey)
{
	return false;
}

bool CAnimation::SaveFromFullPath(const wchar_t * pFullPath)
{
	return false;
}

bool CAnimation::SaveFromFullPath(const char * pFullPath)
{
	return false;
}

bool CAnimation::Load(const wchar_t * pFileName, const string & strPathKey)
{
	return false;
}

bool CAnimation::Load(const char * pFileName, const string & strPathKey)
{
	return false;
}

bool CAnimation::LoadFromFullPath(const wchar_t * pFullPath)
{
	return false;
}

bool CAnimation::LoadFromFullPath(const char * pFullPath)
{
	return false;
}

bool CAnimation::ModifyClip(const string & strKey, const string & strChangeKey, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame)
{
	PANIMATIONCLIP	pClip = FindClip(strKey);

	if (!pClip)
		return false;

	m_mapClip.erase(strKey);

	int	iLength = iEndFrame - iStartFrame;

	pClip->eOption = eOption;
	pClip->strName = strChangeKey;
	pClip->iStartFrame = iStartFrame;
	pClip->iEndFrame = iEndFrame;
	pClip->iFrameLength = iEndFrame - iStartFrame;
	pClip->fStartTime = 0.f;
	pClip->fEndTime = iLength / (float)pClip->iFrameMode;
	pClip->fTimeLength = pClip->fEndTime - pClip->fStartTime;

	m_mapClip.insert(make_pair(strChangeKey, pClip));

	if (m_pDefaultClip->strName == strKey)
		m_pDefaultClip->strName = strChangeKey;

	if (m_pCurClip->strName == strKey)
		m_pCurClip->strName = strChangeKey;

	return true;
}

bool CAnimation::DeleteClip(const string & strKey)
{
	unordered_map<string, PANIMATIONCLIP>::iterator	iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return false;

	SAFE_DELETE(iter->second);

	if (strKey == m_pDefaultClip->strName)
	{
		if (!m_mapClip.empty())
			m_pDefaultClip = FindClip(m_mapClip.begin()->first);
	}

	if (strKey == m_pCurClip->strName)
	{
		m_pCurClip = m_pDefaultClip;
	}

	m_mapClip.erase(iter);

	return true;
}

bool CAnimation::ReturnDefaultClip()
{
	ChangeClip(m_pDefaultClip->strName);

	return true;
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

		int	iFrameIndex = (int)(fAnimationTime * m_pCurClip->iFrameMode);

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

				float	fPercent = (fAnimationTime - dFrameTime) / m_pCurClip->fFrameTime;

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
		CDevice::GetInst()->GetContext()->Map(m_pBoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		Matrix*	pMatrix = (Matrix*)tMap.pData;

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
		{
			pMatrix[i] = *m_vecBoneMatrix[i];
		}

		CDevice::GetInst()->GetContext()->Unmap(m_pBoneTex, 0);
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
