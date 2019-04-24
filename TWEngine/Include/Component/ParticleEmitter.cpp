#include "../EngineHeader.h"
#include "ParticleEmitter.h"
#include "../GameObject.h"
#include "Particle.h"
#include "Renderer.h"
#include "Transform.h"
#include "Material.h"
#include "../Scene/Layer.h"
#include "../ParticleManager.h"
#include "../Rendering/RenderState.h"
#include "Light.h"


PUN::CParticleEmitter::CParticleEmitter():
	m_iEmittedParticleCnt(0),
	m_fTimer(0.f)
{
	m_iParticleType = EPT_BILLBOARD | EPT_CPU | EPT_ALPHABLEND;
}

PUN::CParticleEmitter::CParticleEmitter(const CParticleEmitter & pEmit)
	:PUN::CComponent(pEmit)
{
	*this = pEmit;
	m_iEmittedParticleCnt = 0;
	m_fTimer = 0.f;
}

PUN::CParticleEmitter::~CParticleEmitter()
{
}

bool PUN::CParticleEmitter::Init()
{
	
	return true;
}

void PUN::CParticleEmitter::AfterClone()
{
	m_fEmitTime = m_fEmitterLifeTime / ((float)m_iParticleCnt);

	//파티클 프로토타입의 중력 설정		--- CParticleProt 에서 진행됨
	//파티클 프로토타입의 이동 마찰 설정	--- CParticleProt 에서 진행됨
	//파티클 프로토타입의 회전 마찰 설정	--- CParticleProt 에서 진행됨
}

int PUN::CParticleEmitter::Update(float fTime)
{
	if (!m_pPartProt)
		return 0;
	if (m_fEmitterLifeTime >= m_fTimer)
	{
		if (m_fTimer >= (m_fEmitTime) * (float)(m_iEmittedParticleCnt))
		{
			//파티클 프로토타입 종류 중 랜덤하게 하나를 뽑는다
			int iRandKey = 0;

			int iTypeCnt = m_pPartProt->GetTypeCnt();

			if(iTypeCnt > 1)
				iRandKey = rand() % (m_pPartProt->GetTypeCnt());

			PUN::CGameObject *pPart = m_pPartProt->GetProtPartArr()[iRandKey]->Clone();
			m_pLayer->AddObject(pPart);
			PUN::CTransform *pTrans = pPart->GetTransform();
			PUN::CParticle *pPartCompo = pPart->FindComponentFromTag<PUN::CParticle>("particle");

			//파티클 크기 설정
			pTrans->SetWorldScale(m_pPartProt->GetInstanceSize());

			//파티클의 라이프타임 설정
			//pPartCompo->m_fLifeTime = m_pPartProt->GetInstanceLife(iRandKey);

			//파티클의 초기 위치 설정
			Vector3 vecPos = Vector3::Zero;
			vecPos.x = ((m_vEmitAreaMax.x - m_vEmitAreaMin.x) * (((float)(rand() % 100) / 50.f) - 1.f));
			vecPos.y = ((m_vEmitAreaMax.y - m_vEmitAreaMin.y) * (((float)(rand() % 100) / 50.f) - 1.f));
			vecPos.z = ((m_vEmitAreaMax.z - m_vEmitAreaMin.z) * (((float)(rand() % 100) / 50.f) - 1.f));
			pTrans->SetWorldPos((m_pTransform->GetWorldPos()) + vecPos);

			//파티클의 이동 시작 설정
			vecPos = m_vInitMoveThrustMin + ((m_vInitMoveThrustMax - m_vInitMoveThrustMin) * ((float)(rand() % 100) / 100.f));
			//pPartCompo->m_vInitMoveThrust = vecPos;

			//파티클의 이동 가속도 설정
			vecPos = m_vMoveAccelerationMin + ((m_vMoveAccelerationMax - m_vMoveAccelerationMin) * ((float)(rand() % 100) / 100.f));
			//pPartCompo->m_vMoveAcceleration = vecPos;

			//파티클의 회전 시작 설정
			vecPos = m_vInitTorqueMin + ((m_vInitTorqueMax - m_vInitTorqueMin) * ((float)(rand() % 100) / 100.f));
			//pPartCompo->m_vInitTorque = vecPos;

			//파티클의 회전 가속도 설정
			vecPos = m_vRotAccelerationMin + ((m_vRotAccelerationMax - m_vRotAccelerationMin) * ((float)(rand() % 100) / 100.f));
			//pPartCompo->m_vRotAcceleration = vecPos;

			//파티클 특수 색상 설정
			Vector4 vColor = m_vColor1 + ((m_vColor2 - m_vColor1) * ((float)(rand() % 100) / 100.f));
			//pPartCompo->m_vColor = vColor;

			SAFE_RELEASE(pTrans);
			SAFE_RELEASE(pPart);
			SAFE_RELEASE(pPartCompo);
			m_iEmittedParticleCnt++;
		}
		m_fTimer += fTime;
	}
	else
	{
		m_pObject->Die();
		this->Die();
	}

	return 0;
}

int PUN::CParticleEmitter::LateUpdate(float fTime)
{
	return 0;
}

void PUN::CParticleEmitter::Collision(float fTime)
{
}

void PUN::CParticleEmitter::Render(float fTime)
{
}

PUN::CParticleEmitter * PUN::CParticleEmitter::Clone()
{
	return new CParticleEmitter(*this);
}

void PUN::CParticleEmitter::PartInstance(CParticleProt * prot)
{
	this->m_pPartProt = prot;
	
}

void PUN::CParticleEmitter::PartInstance(std::string strKey)
{
	this->m_pPartProt = CParticleManager::GetInst()->GetParticleProt(strKey);
}

void PUN::CParticleEmitter::PartInstance(int iKey)
{
	this->m_pPartProt = CParticleManager::GetInst()->GetParticleProt(iKey);
}

void PUN::CParticleEmitter::SetParticleCount(int iCount)
{
	m_iParticleCnt = iCount;
}

int PUN::CParticleEmitter::GetParticleCount() const
{
	return m_iParticleCnt;
}

void PUN::CParticleEmitter::SetEmitTime(float time)
{
	m_fEmitterLifeTime = time;
}

void PUN::CParticleEmitter::SetEmitArea(const Vector3 & vMin, const Vector3 & vMax)
{
	m_vEmitAreaMin = vMin;
	m_vEmitAreaMax = vMax;
}

void PUN::CParticleEmitter::SetInitMoveVel(const Vector3 & vMin, const Vector3 & vMax)
{
	m_vInitMoveThrustMin = vMin;
	m_vInitMoveThrustMax = vMax;
}

void PUN::CParticleEmitter::SetInitRotVel(const Vector3 & vMin, const Vector3 & vMax)
{
	m_vInitTorqueMin = vMin;
	m_vInitTorqueMax = vMax;
}

void PUN::CParticleEmitter::SetMoveAccel(const Vector3 & vMin, const Vector3 & vMax)
{
	m_vMoveAccelerationMin = vMin;
	m_vMoveAccelerationMax = vMax;
}

void PUN::CParticleEmitter::SetRotAccel(const Vector3 & vMin, const Vector3 & vMax)
{
	m_vRotAccelerationMin = vMin;
	m_vRotAccelerationMax = vMax;
}

void PUN::CParticleEmitter::SetColorRange(const Vector4 & vColor1, const Vector4 & vColor2)
{
	m_vColor1 = vColor1;
	m_vColor2 = vColor2;
}

PUN::CParticleProt::CParticleProt():
	m_pProtPart(nullptr),
	m_iTypeCnt(0),
	m_ArrProtRatio(nullptr),
	m_fArrMaxLife(nullptr),
	m_fArrMinLife(nullptr)
{
}

PUN::CParticleProt::~CParticleProt()
{
	for (int i = 0; i < m_iTypeCnt; ++i)
	{
		m_pProtPart[i]->Die();
		SAFE_RELEASE(m_pProtPart[i]);
	}

	delete[] m_pProtPart;
	m_pProtPart = nullptr;

	if (m_ArrProtRatio)
	{
		delete[] m_ArrProtRatio;
		m_ArrProtRatio = nullptr;
	}

	if (m_fArrMaxLife)
	{
		delete[] m_fArrMaxLife;
		m_fArrMaxLife = nullptr;
	}

	if (m_fArrMinLife)
	{
		delete[] m_fArrMinLife;
		m_fArrMinLife = nullptr;
	}

	if (m_vArrMaxSize)
	{
		delete[] m_vArrMaxSize;
		m_vArrMaxSize = nullptr;
	}

	if (m_vArrMinSize)
	{
		delete[] m_vArrMinSize;
		m_vArrMinSize = nullptr;
	}

}

void PUN::CParticleProt::InputParticleTypes(const char** pArrTexKeys, int *pArrFlags
	, float *pArrMinLifeTime, float *pArrMaxLifeTime, int iCnt,
	Vector3 *vGravity, Vector3 *vMoveFriction, Vector3 *vRotFriction, Vector3 *vMinSize, Vector3 *vMaxSize)
{
	m_iTypeCnt = iCnt;
	m_pProtPart = new PUN::CGameObject*[iCnt];
	m_ArrProtRatio = new float[iCnt];
	m_fArrMaxLife = new float[iCnt];
	m_fArrMinLife = new float[iCnt];
	m_vArrMaxSize = new Vector3[iCnt];
	m_vArrMinSize = new Vector3[iCnt];
	
	memset(m_ArrProtRatio, 0, sizeof(float) * iCnt);
	size_t ulVector3Size = sizeof(float) * 3 * iCnt;
	memset(m_fArrMaxLife, 1, sizeof(float) * iCnt);
	memset(m_fArrMinLife, 1, sizeof(float) * iCnt);
	memset(m_vArrMaxSize, 1, ulVector3Size);
	memset(m_vArrMinSize, 1, ulVector3Size);
	

	std::string strProtName;
	for (int i = 0; i < iCnt; ++i)
	{
		strProtName = m_strProtName + "_" + to_string(i);
		m_pProtPart[i] = PUN::CGameObject::CreateObject(strProtName);
		PUN::CParticle *part = m_pProtPart[i]->AddComponent<PUN::CParticle>("particle");
		//m_pProtPart[i]->SetRenderGrp(PUN::RG_ALPHA1);
		PUN::CRenderer *pRend = m_pProtPart[i]->AddComponent<PUN::CRenderer>("renderer");
		
		//2D / Billboard의 경우 텍스쳐를 직접 지정해준다
		if (pArrFlags[i] & (int)EPT_BILLBOARD)
		{
			pRend->SetMesh("ParticleTest", TEXT("plane.msh"), MESH_PATH);
			//pRend->SetMesh("ParticleRect");
			PUN::CMaterial *pMat = m_pProtPart[i]->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
			//pRend->SetShader("Particle3D_Additive");
			
			if (pArrFlags[i] & (int)EPT_ALPHABLEND)
			{
				pRend->SetShader("Particle3D");
			}
			else if (pArrFlags[i] & (int)EPT_ADDITIVE)
			{
				pRend->SetShader("Particle3D_Additive");
			}
				
			if (!pMat)
				pMat = m_pProtPart[i]->AddComponent<CMaterial>("Material");


			pMat->SetDiffuseTex(0, pArrTexKeys[i]);
			
			pMat->SetSampler(0, SAMPLER_LINEAR);
			//pMat->SetEmissive(1.f);
			pMat->SetMaterial(Vector4::White, Vector4::Gray, Vector4::White, 2.f, Vector4::White);
			SAFE_RELEASE(pMat);

		}
		//3D의 경우는 pArrTexKey를 메쉬 키로 지정한다
		else if (pArrFlags[i] | (int)EPT_3DMESH)
		{
			pRend->SetMesh(pArrTexKeys[i]);
		}

		//part->m_iParticleType = pArrFlags[i];

		this->m_fArrMaxLife[i] = pArrMaxLifeTime[i];
		this->m_fArrMinLife[i] = pArrMinLifeTime[i];

		if (vGravity)
		{
			//part->m_vGravity = vGravity[i];
		}
		else
		{
			//part->m_vGravity = Vector3::Zero;
		}

		if (vMoveFriction)
		{
			//part->m_vMoveFriction = vMoveFriction[i];
		}
		else
		{
			//part->m_vMoveFriction = Vector3::Zero;
		}

		if (vRotFriction)
		{
			//part->m_vRotFriction = Vector3::Zero;
		}

		if (vMinSize)
		{
			m_vArrMaxSize[i] = vMaxSize[i];
			m_vArrMinSize[i] = vMinSize[i];
		}

		SAFE_RELEASE(part);
		SAFE_RELEASE(pRend);
		//m_pProtPart[i]->Release();
		
	}


}

void PUN::CParticleProt::DebugParticleTypes()
{
	OutputDebugStringA("파티클 프로토타입 디버그 ---");
	char strDebugstr[128] = {};

	sprintf_s(strDebugstr, " 타입 갯수: %d\n", this->m_iTypeCnt);
	OutputDebugStringA(strDebugstr);
	memset(strDebugstr, 0, sizeof(char) * 128);

	for (int i = 0; i < this->m_iTypeCnt; ++i)
	{
		sprintf_s(strDebugstr, "\t파티클 이름 : %s\n", m_pProtPart[i]->GetTag().c_str());
		OutputDebugStringA(strDebugstr);
		memset(strDebugstr, 0, sizeof(char) * 128);
	}

	OutputDebugStringA("\t\t--------------\n");
}

int PUN::CParticleProt::GetTypeCnt() const
{
	return m_iTypeCnt;
}

PUN::CGameObject ** PUN::CParticleProt::GetProtPartArr() const
{
	return m_pProtPart;
}

const Vector3 PUN::CParticleProt::GetInstanceSize(int idx) const
{
	Vector3 vRes = m_vArrMinSize[idx] + ((m_vArrMaxSize[idx] - m_vArrMinSize[idx]) * ((float)(rand() % 100) / 100.f));
	
	return vRes;
}

const float PUN::CParticleProt::GetInstanceLife(int idx) const
{
	float fRes = m_fArrMinLife[idx] + (m_fArrMaxLife[idx] * m_fArrMinLife[idx] * ((float)(rand() % 100) / 100.f));
	return fRes;
}

const char * PUN::CParticleProt::GetProtName() const
{
	return (m_strProtName.c_str());
}
