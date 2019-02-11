#include "Animation.h"
#include "../Device.h"

PUN_USING

CAnimation::CAnimation() :
	m_pBoneTex(nullptr),
	m_pBoneRV(nullptr),
	m_pCurClip(nullptr),
	m_pNextClip(nullptr)
{
	m_eComType = CT_ANIMATION;
}


CAnimation::CAnimation(const CAnimation & anim) :
	CComponent(anim)
{
	m_vecBones.clear();

	for (size_t i = 0; i < anim.m_vecBones.size(); ++i)
	{
		PBONE	pBone = new BONE;

		*pBone = *anim.m_vecBones[i];

		pBone->matOffset = new Matrix;
		pBone->matBone = new Matrix;

		*pBone->matOffset = *anim.m_vecBones[i]->matOffset;
		*pBone->matBone = *anim.m_vecBones[i]->matBone;

		/*list<CBoneSocket*>::const_iterator	iterB;
		list<CBoneSocket*>::const_iterator	iterBEnd = anim.m_vecBones[i]->SocketList.end();

		for (iterB = anim.m_vecBones[i]->SocketList.begin(); iterB != iterBEnd; ++iterB)
		{
			pBone->SocketList.push_back((*iterB)->Clone());
		}*/

		m_vecBones.push_back(pBone);
	}

	CreateBoneTexture();

	unordered_map<string, PANIMATIONCLIP>::const_iterator	iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator	iterEnd = anim.m_mapClip.end();

	m_mapClip.clear();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP	pClip = new ANIMATIONCLIP;


	}
}

CAnimation::~CAnimation()
{
	Safe_Delete_Map(m_mapClip);

	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		//Safe_Release_VecList(m_vecBones[i]->SocketList);
		SAFE_DELETE(m_vecBones[i]->matBone);
		SAFE_DELETE(m_vecBones[i]->matOffset);
		SAFE_DELETE(m_vecBones[i]);
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
