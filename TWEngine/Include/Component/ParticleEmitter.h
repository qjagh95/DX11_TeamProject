#pragma once
#include "Component.h"

namespace PUN {

	enum ENUM_PARTICLE_TYPE
	{
		EPT_BILLBOARD = 1, //2D����. �ƴϸ� 0
		EPT_3DMESH = 2,
		EPT_CPU = 4,
		EPT_GPU = 8, //GPU��������. �ƴϸ� CPU
		EPT_ALPHABLEND = 16,
		EPT_ADDITIVE = 32,
		EPT_LIGHT_ENABLED = 64, //Light �� ����Ǵ���
		EPT_LIGHT_DYNAMIC = 128
	};

	class PUN_DLL CParticleProt
	{
		friend class CParticleManager;
		CParticleProt();
		~CParticleProt();
	private:
		class PUN::CGameObject **m_pProtPart;
		float *m_ArrProtRatio; //m_iTypeCnt ������ŭ�� ���� �� Ȯ���� ������ ��Ÿ��
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
		CParticleProt *m_pPartProt; //Particle ������Ÿ�� ����
		Vector3 m_vEmitAreaMin; //��ƼŬ ���� ����
		Vector3 m_vEmitAreaMax;
		float m_fEmitTime; //�� ��ƼŬ ���� �ð�. LifeTime / ParticleCnt �� �ڵ�����.
		float m_fMoveAccelTimeMin; //�ʹ� �����Ǵ� ���ӵ�(�ּ�)
		float m_fMoveAccelTimeMax; //�ʹ� �����Ǵ� ���ӵ�(�ִ�)
		float m_fRotAccelTimeMin; //�ʹ� ������ ȸ�� ���ӵ�(�ּ�)
		float m_fRotAccelTimeMax; //�ʹ� ������ ȸ�� ���ӵ�(�ִ�)
		Vector3 m_vInitMoveThrustMin; //ó�� �̵� �ӵ�(�ּ�)
		Vector3 m_vInitMoveThrustMax; //ó�� �̵� �ӵ�(�ִ�)
		Vector3 m_vMoveAccelerationMin; //ó�� �ӵ��� m_fMoveAccelTime��ŭ ����� ���ӵ�(�ּ�)
		Vector3 m_vMoveAccelerationMax; //ó�� �ӵ��� m_fMoveAccelTime��ŭ ����� ���ӵ�(�ִ�)
		
		Vector4 m_vColor1; //���� ��ƼŬ�� �������� Diffuse ����. �⺻ white(�ּ�)
		Vector4 m_vColor2; //���� ��ƼŬ�� �������� Diffuse ����. �⺻ white(�ִ�)
		Vector3 m_vInitTorqueMin; //���Ϸ� ȸ�� �ӵ�(�ּ�)
		Vector3 m_vInitTorqueMax; //���Ϸ� ȸ�� �ӵ�(�ִ�)
		Vector3 m_vRotAccelerationMin; //ó�� �ӵ��� m_fRotAccelTime �ð���ŭ ����� ȸ�����ӵ�(�ּ�)
		Vector3 m_vRotAccelerationMax; //ó�� �ӵ��� m_fRotAccelTime �ð���ŭ ����� ȸ�����ӵ�(�ִ�)
		
		int m_iParticleType; //��ƼŬ Ÿ��. ��Ʈ�÷��� ���. �ڼ��� ������ ParticleEmitter.h ����
		int m_iParticleCnt; //����� ��ƼŬ �� ����
		int m_iEmittedParticleCnt; //������ ��ƼŬ ����

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
