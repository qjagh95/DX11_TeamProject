#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Pistol.h"
#include "Input.h"
#include <Scene/Scene.h>

#define VECTOR_GUN_DIR_OFFSET Vector3(0.f, 0.f, 0.f)

void toEulerAngle(const Vector4 & q, Vector3 & vEuler)
{
	//Quaternion -> Euler 변환은 오일러 축 순서가 ㅈㄴ 중요하다(이것 때문에 기생수 많이 만들고 애먹음)
	//Matrix.Rotation 함수를 살펴보면 우리 엔진은 분명히 XYZ 순서를 사용하고 있다
	//추가 분석 : XYZ 순서로 매트릭스를 만들었고, 거기서 파생된 Quaternion을 분해하기 위해 만든 기능
	//따라서 이 쿼터니언의 분해 순서는 ZYX가 되어야 한다는 가설을 세워본 결과 이것이 유효했다
	//인터넷에 돌아다니는 수많은 자료들이 모두 Quaternion을 Euler로 만드는 것이 확실하지만,
	//Euler 각은 적용하는 순서에 따라 값이 달라지기 때문에 XYZ, XZY, YXZ, YZX, ZXY, ZYX 여섯 가지 경우에서
	//적용하려는 것이 무엇인지 확실히 해 두어야 한다.
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
}

void EulerToQuat(const Vector3& vEuler, Vector4& vQuat)
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

bool CHuman_Player::Init_Weapon()
{
	m_pPistol = m_pHandGun->AddComponent<CPistol>("pistol");
	m_pHandGun->SetEnable(false);
	m_pPistol->SetEnable(false);
	PUN::CInput::GetInst()->AddKey("1", '1');
	return true;
}

void CHuman_Player::OnDestroyWeap()
{
	SAFE_RELEASE(m_pPistol);
}

void CHuman_Player::Pickup_Weapon(float fTime)
{

}
void CHuman_Player::Aim_Weapon(float fTime)
{

}
void CHuman_Player::Fire_Weapon(float fTime)
{
	if (m_pPistol->GetEnable())
		m_pPistol->Fire();
}

int CHuman_Player::WeaponInput(float fTime)
{
	if (CInput::GetInst()->KeyPress("LButton"))
	{
		Fire_Weapon(fTime);
	}
	
	if (CInput::GetInst()->KeyPress("1"))
	{
		if (!m_pPistol->GetEnable())
		{
			m_pHandGun->SetEnable(true);
			m_pPistol->SetEnable(true);
			m_pPartCamAnim->bActivated = true;
			m_pPartCamAnim->bUseCustomParent = true;
			m_pPartCamAnim->pNextClip = m_pPartCamAnim->mapPartClips.find("gun_take")->second;
			m_pPartCamAnim->pDefaultClip = m_pPartCamAnim->mapPartClips.find("gun_idle")->second;
			m_iState |= PSTATUS_GUN;
			
		}
		else
		{
			m_pPartCamAnim->bActivated = false;
			m_pPartCamAnim->bUseCustomParent = false;
			m_iState ^= PSTATUS_GUN;
			m_pPistol->SetEnable(false);
			m_pHandGun->SetEnable(false);
		}
	}
	
	
	return 0;
}
int CHuman_Player::WeaponUpdate(float fTime)
{
	if (m_pPistol->GetEnable())
	{
		//총 휘두르는 팔의 회전
		//Y회전 : 카메라의 회전과 총의 회전을 비교한다
		//가지고 있는 회전 : 목(카메라 위치를 위함)

		PUN::CTransform *pHeadTr = m_pHeadObj->GetTransform();
		PUN::CTransform *pCamTr = m_pScene->GetMainCameraTransform();
		Vector3 vRot = pCamTr->GetWorldRot();

		//오일러 각으로 회전을 잡아주면 김벌락 때문에 특수한 경우에만 각이 나온다
		//대가리의 회전과 카메라의 회전 모두를 쿼터니언각으로 변환해줘야함 ㅅㅂ

		Vector4 vCamQuat;
		Vector4 vHeadQuat;
		Vector3 vNeckRot = pHeadTr->GetWorldRot();
		vNeckRot.x = DegreeToRadian(vNeckRot.x);
		vNeckRot.y = DegreeToRadian(vNeckRot.y);
		vNeckRot.z = DegreeToRadian(vNeckRot.z);

		EulerToQuat(vNeckRot, vHeadQuat);

		vRot.x = DegreeToRadian(vRot.x);
		vRot.y = DegreeToRadian(vRot.y);
		vRot.z = DegreeToRadian(vRot.z);

		EulerToQuat(vRot, vCamQuat);

		//두 쿼터니언 회전의 사이각 구하기
		//짜잔 쿼터니언 * 카메라 쿼터니언 = 모가지 쿼터니언
		//짜잔 쿼터니언 = 모가지 쿼터니언 * 카메라 쿼터니언(Inverse)
		
		Vector4 vDiffQuat = Vector4(DirectX::XMQuaternionMultiply(vHeadQuat.Convert(), DirectX::XMQuaternionInverse(vCamQuat.Convert())));
		Vector3 vDiffRot;

		toEulerAngle(vDiffQuat, vDiffRot);

		vDiffRot.x = RadianToDegree(vDiffRot.x);
		vDiffRot.y = RadianToDegree(vDiffRot.y);
		vDiffRot.z = RadianToDegree(vDiffRot.z);

		SAFE_RELEASE(pHeadTr);

		
		std::cout << "cam rotation : (" << vRot.x << ", " << vRot.y << ", " << vRot.z << ")"<< std::endl;
		std::cout << "neck rotation : (" << vNeckRot.x << ", " <<vNeckRot.y<< ", " << vNeckRot.z << ")" << std::endl;
		std::cout << "diff : (" <<  vDiffRot.x << ", " << vDiffRot.y << ", "<< vDiffRot.z << ")" << std::endl;
		std::cout << "==================================" << std::endl;
		
		
		PUN::PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();
		Vector3 vPos = pClip->vecKeyFrame[m_pPartCamAnim->iRootParentIndex]->vecKeyFrame[0]->vPos;
		Vector4 vQuat = pClip->vecKeyFrame[m_pPartCamAnim->iRootParentIndex]->vecKeyFrame[0]->vRot;
		Vector3 vScale = pClip->vecKeyFrame[m_pPartCamAnim->iRootParentIndex]->vecKeyFrame[0]->vScale;
		
		Vector3 vEulerDir = Vector3::Zero;
		Vector4 vDirQuat;
		
		if (vRot.x != 0)
		{
			vEulerDir.x = -vRot.x;
		}

		//뭬직 남바

		if (fabsf(vDiffRot.y + 8.61f) > 0.f)
		{
			vEulerDir.y = DegreeToRadian(vDiffRot.y + 8.61f);
			//vEulerDir.z = -DegreeToRadian(vDiffRot.z);
		}
		

		EulerToQuat(vEulerDir, vDirQuat);

		XMVECTOR xmRot = DirectX::XMQuaternionMultiply(vQuat.Convert(), vDirQuat.Convert());

		XMVECTOR vZero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		m_pPartCamAnim->matCustomParent = XMMatrixAffineTransformation(vScale.Convert(), vZero, xmRot, vPos.Convert());

		SAFE_RELEASE(pCamTr);
		//X회전 : 마우스 높이는 팔에 따로 적용되므로 반드시 얻어내야 함
		//Z회전 : 엥간하면 몸뚱의의 회전을 그대로 따른다.
	}

	return 0;
}
int CHuman_Player::WeaponLateUpdate(float fTime)
{
	return 0;
}