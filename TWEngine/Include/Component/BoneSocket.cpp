#include "../EngineHeader.h"
#include "BoneSocket.h"
#include "../Resource/Mesh.h"
#include "Renderer.h"

#define ZYX 1
void toEulerAngle(const Vector4 & q, Vector3 & vEuler)
{
	//Quaternion -> Euler ��ȯ�� ���Ϸ� �� ������ ���� �߿��ϴ�(�̰� ������ ����� ���� ����� �ָ���)
	//Matrix.Rotation �Լ��� ���캸�� �츮 ������ �и��� XYZ ������ ����ϰ� �ִ�
	//�߰� �м� : XYZ ������ ��Ʈ������ �������, �ű⼭ �Ļ��� Quaternion�� �����ϱ� ���� ���� ���
	//���� �� ���ʹϾ��� ���� ������ ZYX�� �Ǿ�� �Ѵٴ� ������ ������ ��� �̰��� ��ȿ�ߴ�
	//���ͳݿ� ���ƴٴϴ� ������ �ڷ���� ��� Quaternion�� Euler�� ����� ���� Ȯ��������,
	//Euler ���� �����ϴ� ������ ���� ���� �޶����� ������ XYZ, XZY, YXZ, YZX, ZXY, ZYX ���� ���� ��쿡��
	//�����Ϸ��� ���� �������� Ȯ���� �� �ξ�� �Ѵ�.

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

void EulerToQuatZYX(const Vector3& vEuler, Vector4& vQuat)
{
	float c1 = cosf(vEuler.x * 0.5f);
	float c2 = cosf(vEuler.y * 0.5f);
	float c3 = cosf(vEuler.z * 0.5f);
	float s1 = sinf(vEuler.x * 0.5f);
	float s2 = sinf(vEuler.y * 0.5f);
	float s3 = sinf(vEuler.z * 0.5f);

	vQuat = Vector4(
		s1 * c2 * c3 - c1 * s2 * s3,
		c1 * s2 * c3 + s1 * c2 * s3,
		c1 * c2 * s3 - s1 * s2 * c3,
		c1 * c2 * c3 + s1 * s2 * s3
	);

}
PUN_USING

CBoneSocket::CBoneSocket():
	m_pObject(nullptr)
{
	//m_pObject = NULLPTR;
}

CBoneSocket::~CBoneSocket()
{
	SAFE_RELEASE(m_pObject);
}

bool CBoneSocket::Init()
{
	return true;
}

void CBoneSocket::Update(float fTime, const Matrix& matBone, class CTransform* motherTr)
{
	if (m_pObject)
	{
		CTransform*	pTransform = m_pObject->GetTransformNoneCount();
		if (!m_pObject->GetActive())
		{
			SAFE_RELEASE(m_pObject);
			return;
		}
		
		DirectX::XMMATRIX xmOffset = DirectX::XMMatrixTranslation(m_vOffset.x, m_vOffset.y, m_vOffset.z);

		DirectX::XMVECTOR xmRot;
		DirectX::XMVECTOR xmPos;
		DirectX::XMVECTOR xmScale;

		PUN::CTransform *pLocalRot = motherTr;

		Matrix matLocalRot = motherTr->GetLocalMatrix();

		DirectX::XMMATRIX xmSized = xmOffset * (matBone * motherTr->GetWorldRotMatrix() * matLocalRot).matrix;

		//�������� ������ �ӱݱ�..
		DirectX::XMMatrixDecompose(&xmScale, &xmRot, &xmPos, xmSized);

		Vector4 vQuat(xmRot);
		
		Vector3 vPos(xmPos);
		
		Vector3 vWorld = motherTr->GetWorldPos();

		vPos *= motherTr->GetWorldScale();
		vPos += vWorld;
		
		Vector4 vQuat_m_vRotation;
		Vector3 vRotationRad(DegreeToRadian(m_vRotation.x), DegreeToRadian(m_vRotation.y), DegreeToRadian(m_vRotation.z));

		EulerToQuatZYX(vRotationRad, vQuat_m_vRotation);

		xmRot = DirectX::XMQuaternionMultiply(xmRot, vQuat_m_vRotation.Convert());

		xmRot = DirectX::XMQuaternionNormalize(xmRot);

		xmRot = DirectX::XMQuaternionNormalize(xmRot);

		Vector3 vRot;
		Vector3 vRotWithLoc;
		toEulerAngle(Vector4(xmRot), vRot);

		vRot.x = RadianToDegree(vRot.x);
		vRot.y = RadianToDegree(vRot.y);
		vRot.z = RadianToDegree(vRot.z);
		
		pTransform->SetWorldRot(vRot);

		pTransform->SetWorldPos(vPos);
		
	}
}

void CBoneSocket::SetObject(CGameObject * pObj)
{
	SAFE_RELEASE(m_pObject);

	if (pObj)
		pObj->AddRef();

	m_pObject = pObj;
}

void CBoneSocket::SetOffset(const Vector3 & vOffset)
{
	m_vOffset = vOffset;
}

void CBoneSocket::SetOffset(float x, float y, float z)
{
	m_vOffset = Vector3(x, y, z);
}

void CBoneSocket::SetRotation(const Vector3 & vRot)
{
	m_vRotation = vRot;
}

void CBoneSocket::SetRotation(float x, float y, float z)
{
	m_vRotation = Vector3(x, y, z);
}

void CBoneSocket::SetRotationX(float x)
{
	m_vRotation.x = x;
}

void CBoneSocket::SetRotationY(float y)
{
	m_vRotation.y = y;
}

void CBoneSocket::SetRotationZ(float z)
{
	m_vRotation.z = z;
}
