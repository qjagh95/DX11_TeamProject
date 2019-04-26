#pragma once
#include "Component.h"

namespace PUN {

	enum ENUM_PARTICLE_TYPE
	{
		EPT_BILLBOARD = 1, //2D인지. 아니면 0
		EPT_3DMESH = 2,
		EPT_CPU = 4,
		EPT_GPU = 8, //GPU가속인지. 아니면 CPU
		EPT_ALPHABLEND = 16,
		EPT_ADDITIVE = 32,
		EPT_LIGHT_ENABLED = 64, //Light 가 적용되는지
		EPT_LIGHT_DYNAMIC = 128
	};

	class PUN_DLL CParticleProt
	{
		friend class CParticleManager;
		CParticleProt();
		~CParticleProt();
	private:
		class PUN::CGameObject **m_pProtPart;
		float *m_ArrProtRatio; //m_iTypeCnt 갯수만큼의 종류 중 확률이 얼마인지 나타냄
		int m_iTypeCnt;
		std::string m_strProtName;
		float *m_fArrMinLife;
		float *m_fArrMaxLife;
		Vector3 *m_vArrMinSize;
		Vector3 *m_vArrMaxSize;
				
	public:
		void InputParticleTypes(const char** pArrTexKeys, int *pArrFlags
			, float *pArrMinLifeTime, float *pArrMaxLifeTime, int iCnt = 1,
			Vector3 *vGravity = nullptr, Vector3 *vMoveFriction = nullptr, Vector3 *vRotFriction = nullptr,
			Vector3 *vMinSize = nullptr, Vector3 *vMaxSize = nullptr);
		void DebugParticleTypes();
		int GetTypeCnt() const;
		class PUN::CGameObject ** GetProtPartArr() const;
		const Vector3 GetInstanceSize(int idx = 0) const;
		const float GetInstanceLife(int idx = 0) const;
		const char *GetProtName() const;
	};

	class PUN_DLL CParticleEmitter:
		public PUN::CComponent
	{
		friend CParticleProt;
		friend class PUN::CGameObject;

		CParticleEmitter();
		CParticleEmitter(const CParticleEmitter& pEmit);
		~CParticleEmitter();

	private:
		
		float m_fEmitterLifeTime;
		float m_fTimer;
		CParticleProt *m_pPartProt; //Particle 프로토타입 저장
		Vector3 m_vEmitAreaMin; //파티클 생성 지역
		Vector3 m_vEmitAreaMax;
		float m_fEmitTime; //매 파티클 생성 시간. LifeTime / ParticleCnt 로 자동설정.
		float m_fMoveAccelTimeMin; //초반 추진되는 가속도(최소)
		float m_fMoveAccelTimeMax; //초반 추진되는 가속도(최대)
		float m_fRotAccelTimeMin; //초반 추진될 회전 가속도(최소)
		float m_fRotAccelTimeMax; //초반 추진될 회전 가속도(최대)
		Vector3 m_vInitMoveThrustMin; //처음 이동 속도(최소)
		Vector3 m_vInitMoveThrustMax; //처음 이동 속도(최대)
		Vector3 m_vMoveAccelerationMin; //처음 속도에 m_fMoveAccelTime만큼 적용될 가속도(최소)
		Vector3 m_vMoveAccelerationMax; //처음 속도에 m_fMoveAccelTime만큼 적용될 가속도(최대)
		
		Vector4 m_vColor1; //개별 파티클에 정해지는 Diffuse 색깔. 기본 white(최소)
		Vector4 m_vColor2; //개별 파티클에 정해지는 Diffuse 색깔. 기본 white(최대)
		Vector3 m_vInitTorqueMin; //오일러 회전 속도(최소)
		Vector3 m_vInitTorqueMax; //오일러 회전 속도(최대)
		Vector3 m_vRotAccelerationMin; //처음 속도에 m_fRotAccelTime 시간만큼 적용될 회전가속도(최소)
		Vector3 m_vRotAccelerationMax; //처음 속도에 m_fRotAccelTime 시간만큼 적용될 회전가속도(최대)
		
		int m_iParticleType; //파티클 타입. 비트플래그 사용. 자세한 사항은 ParticleEmitter.h 참고
		int m_iParticleCnt; //방출될 파티클 총 갯수
		int m_iEmittedParticleCnt; //생성된 파티클 갯수

	public:
		bool Init();
		void AfterClone();
		int Update(float fTime);
		int LateUpdate(float fTime);
		void Collision(float fTime);
		void Render(float fTime);
		PUN::CParticleEmitter *Clone();
		void PartInstance(CParticleProt *prot);
		void PartInstance(std::string strKey);
		void PartInstance(int iKey);

	public:
		void SetParticleCount(int iCount);
		int GetParticleCount() const;
		void SetEmitTime(float time);
		void SetEmitArea(const Vector3& vMin, const Vector3& vMax);
		void SetInitMoveVel(const Vector3& vMin, const Vector3& vMax);
		void SetInitRotVel(const Vector3& vMin, const Vector3& vMax);
		void SetMoveAccel(const Vector3& vMin, const Vector3& vMax);
		void SetRotAccel(const Vector3& vMin, const Vector3& vMax);
		void SetColorRange(const Vector4& vColor1, const Vector4& vColor2);
	};

}
