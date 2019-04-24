#include "EngineHeader.h"
#include "ParticleManager.h"
#include "Component/ParticleEmitter.h"
#include "Component/Particle.h"
#include "Resource/ResourcesManager.h"
#include "GameObject.h"


using namespace PUN;

DEFINITION_SINGLE(CParticleManager);

bool PUN::CParticleManager::Init()
{
	std::string strProtName = "Particle_Prot_1";
	const char* strKey = "boom1";
	CResourcesManager::GetInst()->CreateTexture(strKey, TEXT("Boom1.png"));
	float fMinLifeTime = 1.5f;
	float fMaxLifeTime = 3.f;
	Vector3 vGrav(0.f, -5.f, 0.f);
	Vector3 vFriction(.5f, .5f, .5f);
	Vector3 vRotFric(5.f, 5.f, 5.f);
	Vector3 vMinSize(0.8f, 0.8f, 0.f);
	Vector3 vMaxSize(1.f, 1.f, 0.f);
	int iArrFlag = (int)EPT_ADDITIVE | (int)EPT_BILLBOARD;

	AddParticleProt(strProtName, &strKey, &iArrFlag, &fMinLifeTime, &fMaxLifeTime, 1, &vGrav, &vFriction, &vRotFric,
		&vMinSize, &vMaxSize);


	return true;
}

void PUN::CParticleManager::AddParticleProt(std::string strProtKey, const char ** pArrTexKeys, int * pArrFlags, float * pArrMinLifeTime, float * pArrMaxLifeTime, int iCnt, Vector3 * vGravity, Vector3 * vMoveFriction, Vector3 * vRotFriction,
	Vector3 *vMinSize, Vector3 *vMaxSize)
{
	PUN::CParticleProt *pProt = new PUN::CParticleProt;
	pProt->m_strProtName = strProtKey;
	pProt->InputParticleTypes(pArrTexKeys, pArrFlags, pArrMinLifeTime, pArrMaxLifeTime, iCnt, vGravity, vMoveFriction,
		vRotFriction, vMinSize, vMaxSize);

	m_vecPartProt.push_back(pProt);

	m_mapPartProtID.insert(make_pair(strProtKey, m_vecPartProt.size() - 1));
}

void PUN::CParticleManager::DeleteParticleProt(std::string strProtKey)
{
	std::unordered_map<std::string, int>::iterator itrMapEnd = m_mapPartProtID.end();
	std::vector<PUN::CParticleProt*>::iterator itrVecEnd = m_vecPartProt.end();

	std::unordered_map<std::string, int>::iterator itrMapFind = m_mapPartProtID.find(strProtKey);
	if (itrMapFind == itrMapEnd)
		return;

	int iKey = itrMapFind->second;
	m_mapPartProtID.erase(itrMapFind);
	delete (m_vecPartProt[iKey]);
	m_vecPartProt[iKey] = nullptr;
	std::vector<PUN::CParticleProt*>::iterator itrDataActual = m_vecPartProt.begin();

	int iIdx = 0;
	for (; itrDataActual != itrVecEnd; ++itrDataActual)
	{
		if (iIdx == iKey)
			break;
		++iIdx;
	}

	m_vecPartProt.erase(itrDataActual);
}

void PUN::CParticleManager::DeleteParticleProt(int iBeginIdx, int iEndIdx)
{
	std::vector<PUN::CParticleProt*>::iterator itrVecEnd = m_vecPartProt.end();
	
	if (iEndIdx <= 0)
	{
		for (auto itrVec = m_vecPartProt.begin(); itrVec != itrVecEnd; ++itrVec)
		{
			delete (*itrVec);
		}
		m_vecPartProt.clear();
		m_mapPartProtID.clear();
	}

	else
	{
		std::vector<PUN::CParticleProt*>::iterator itrDelStart;
		std::vector<PUN::CParticleProt*>::iterator itrDelEnd;

		int iIdx = 0;
		for (auto itrVec = m_vecPartProt.begin(); itrVec != itrVecEnd; ++itrVec)
		{
			if (iIdx == iBeginIdx)
				itrDelStart = itrVec;

			else if (iIdx == iEndIdx)
				itrDelEnd = itrVec;
			++iIdx;
		}

		m_vecPartProt.erase(itrDelStart, itrDelEnd);
		
		int iMapIdx = 0;

		std::unordered_map<std::string, int>::iterator itrMapEnd = m_mapPartProtID.end();

		for (auto itrMap = m_mapPartProtID.begin(); itrMap != itrMapEnd; ++itrMap)
		{
			if (iMapIdx >= iBeginIdx && iMapIdx <= iEndIdx)
			{
				m_mapPartProtID.erase(itrMap);
			}
			else
			{
				(itrMap->second) = iMapIdx;
				++iMapIdx;
			}
			
		}
	}
}

CParticleProt * PUN::CParticleManager::GetParticleProt(std::string strProtKey)
{
	int iKey = -1;

	auto itrMapEnd = m_mapPartProtID.end();

	std::unordered_map<std::string, int>::iterator itrMap = m_mapPartProtID.find(strProtKey);

	if (itrMap == itrMapEnd)
		return nullptr;

	iKey = itrMap->second;

	return m_vecPartProt[iKey];
}

CParticleProt * PUN::CParticleManager::GetParticleProt(int iProtId)
{
	return m_vecPartProt[iProtId];
}

void PUN::CParticleManager::SaveParticleEmitter(std::string strProtEmit, std::string strProtPart, int iEmitCnt, float fEmitTime, const Vector3 & vEmitArea1, const Vector3 & vEmitArea2, const Vector3 & vInitMoveSpdMin, const Vector3 & vInitMoveSpdMax, const Vector3 & vInitRotMin, const Vector3 & vinitRotMax, const Vector3 & vMoveAccelMin, const Vector3 & vMoveAccelMax, const Vector3 & vRotAccelMin, const Vector3 & vRotAccelMax, const Vector4 & vColor1, const Vector4 & vColor2)
{
}

CParticleManager * PUN::CParticleManager::CloneParticleEmitProt(std::string strKey)
{
	return nullptr;
}

CParticleManager * PUN::CParticleManager::CloneParticleEmitProt(int iKey)
{
	return nullptr;
}

void PUN::CParticleManager::ClearAllEmitterProt()
{
	std::vector<CGameObject *>::iterator itrVecEnd = m_vecPartEmitter.end();

	for (std::vector<CGameObject *>::iterator itr = m_vecPartEmitter.begin(); itr != itrVecEnd; ++itr)
	{
		SAFE_RELEASE((*itr));
	}
}

PUN::CParticleManager::CParticleManager() 
{

}

PUN::CParticleManager::~CParticleManager()
{
	auto itrEnd = m_vecPartProt.end();

	for (std::vector<CParticleProt*>::iterator itr = m_vecPartProt.begin(); itr != itrEnd; ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
}
