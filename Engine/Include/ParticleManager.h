#pragma once

namespace PUN {
	
	class PUN_DLL CParticleManager {
		
	private:
		//Particle Emitter에서 사용할 Particle 종류 저장
		std::unordered_map<std::string, int> m_mapPartProtID;
		std::vector<class CParticleProt *> m_vecPartProt;

		//Particle Emitter 저장
		std::unordered_map<std::string, int> m_mapPartEmitID;
		std::vector<class CGameObject *> m_vecPartEmitter;
	public:
		bool Init();
		void AddParticleProt(std::string strProtKey, const char** pArrTexKeys, int *pArrFlags
			, float *pArrMinLifeTime, float *pArrMaxLifeTime, int iCnt = 1,
			Vector3 *vGravity = nullptr, Vector3 *vMoveFriction = nullptr, Vector3 *vRotFriction = nullptr,
			Vector3 *vMinSize = nullptr, Vector3 *vMaxSize = nullptr);

		void DeleteParticleProt(std::string strProtKey);
		void DeleteParticleProt(int iBeginIdx = 0, int iEndIdx = 0);
		class CParticleProt *GetParticleProt(std::string strProtKey);
		class CParticleProt *GetParticleProt(int iProtId);

		void SaveParticleEmitter(std::string strProtEmit, std::string strProtPart, int iEmitCnt, float fEmitTime, const Vector3& vEmitArea1, const Vector3& vEmitArea2,
			const Vector3& vInitMoveSpdMin, const Vector3& vInitMoveSpdMax, const Vector3& vInitRotMin, const Vector3& vinitRotMax,
			const Vector3& vMoveAccelMin = Vector3::Zero, const Vector3& vMoveAccelMax = Vector3::Zero,
			const Vector3& vRotAccelMin = Vector3::Zero, const Vector3& vRotAccelMax = Vector3::Zero,
			const Vector4& vColor1 = Vector4::White, const Vector4& vColor2 = Vector4::White);

		class CParticleManager *CloneParticleEmitProt(std::string strKey);
		class CParticleManager *CloneParticleEmitProt(int iKey);

		void ClearAllEmitterProt();


		DECLARE_SINGLE(CParticleManager)

	};

}
