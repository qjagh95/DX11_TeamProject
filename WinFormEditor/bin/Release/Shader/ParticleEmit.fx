#include "ComputeShare.fx"

/*
default CGameObject Buffers

Transform(b0)
Material(b1)
Component(b2)
Light(b3)

Public(b5)


default CParticle Buffers
Animation2D(b8)
Particle(b10)

<< -- 8���� 10�� �׸��� 3��/ 0���� �ǵ��� ����

*/

static const int g_iNumDispatch = 4;
static const int g_iNumThreads = 32;

static const int g_iDimSize = g_iNumDispatch * g_iNumThreads;

cbuffer ParticleEmitter: register(b2)
{
	float fParticleInstLife;
	int iParticleMax;
	int iParticleCnt;

	int iParticlePicSizeX;
	int iParticlePicSizeY;
	

	float3 vInitVelX;
	float3 vInitVelY;
	float3 vInitRotX;
	float3 vInitRotY;
	float3 vPartFrictionLinear;
	float3 vPartFrictionRot;
	float3 vPartGravityScale;

	float2 vEmpty;
	//112 bytes

}

//��ƼŬ ���� ������(ũ�� ���� �̵�) * Transform�� matProj ���
void ComputeParticle(uint3 dispatchThreadId : SV_DispatchTreadID)
{
	uint GroupIdx = dispathThreadId.x + dispatchThreadId.y * g_iDimSize;


}
