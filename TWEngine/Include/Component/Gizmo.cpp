#include "../EngineHeader.h"
#include "Gizmo.h"
#include "../Resource/Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "../Rendering/Shader.h"
#include "ColliderOBB3D.h"
#include "../Input.h"

PUN_USING

CGizmo::CGizmo()
{
	m_eComType = CT_GIZMO;
	m_pCylinderMesh = nullptr;
	m_pTriangleMesh = nullptr;
	m_pTargetTr = nullptr;
	m_bPick = false;
}

CGizmo::CGizmo(const CGizmo & _Gizmo)
	:CComponent(_Gizmo)
{
}

CGizmo::~CGizmo()
{
	SAFE_RELEASE(m_pCylinderMesh);
	SAFE_RELEASE(m_pTriangleMesh);
	SAFE_RELEASE(m_pTargetTr);
}

void CGizmo::Start()
{
}

void CGizmo::AfterClone()
{
}

bool CGizmo::Init()
{
	m_eGizmoType = GT_END;

	CColliderOBB3D* pOBB = m_pObject->AddComponent<CColliderOBB3D>("ColliderOBB");

	float	fLength[3] = { 0.5f, 4.f, 0.5f };

	pOBB->SetInfo(Vector3::Zero, Vector3::Axis, fLength);
	pOBB->SetCollisionCallback(CCT_STAY, this, &CGizmo::Hit);
	SAFE_RELEASE(pOBB);

	m_pTransform->SetWorldScale(0.5f, 2.5f, 0.5f);
	m_pTransform->SetWorldPivot(0.f, 0.f ,0.f);
	
	return true;
}

int CGizmo::Input(float fTime)
{
	if (m_pTargetTr == nullptr)
		return 0;

	Vector3 vAxis = m_pTargetTr->GetWorldAxis()[m_eGizmoType];

	Vector3 vCameraAxis = m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[m_eGizmoType];
	Vector3 vTargetPos = m_pTargetTr->GetWorldPos();
	Vector3 vCameraPos = m_pScene->GetMainCameraTransformNonCount()->GetWorldPos();
	Vector3 vMouseGap = CInput::GetInst()->GetMouseGap();
	//ImGui::Begin("Axis");

	//ImGui::Text("ObjAxis - X : %f Y : %f, Z : %f", vAxis.x, vAxis.y, vAxis.z);
	//ImGui::Text("CameraAxis - X : %f Y : %f, Z : %f", vCameraAxis.x, vCameraAxis.y, vCameraAxis.z);

	Vector3 vAxisDot = vAxis.Dot(vCameraAxis);
	//vAxisDot.Normalize();
	//ImGui::Text("DotObjAxis - X : %f Y : %f, Z : %f", vAxisDot.x, vAxisDot.y, vAxisDot.z);


	if (m_bPick == true)
	{
		if (CInput::GetInst()->GetMouseRelease(MS_LBUTTON) == true)
		{
			m_bPick = false;
		}
		else if(CInput::GetInst()->GetMousePush(MS_LBUTTON) == true)
		{
			vMouseGap.z = vMouseGap.y;
			Vector3 ObjMove;

			if (vCameraAxis.x < -0.5f)
			{
				vAxis.x *= -1.f;
			}

			if (vCameraAxis.y < -0.5f)
			{
				vAxis.y *= -1.f;
			}

			if (vCameraAxis.z < -0.5f)
			{
				vAxis.z *= -1.f;
			}
			 
			switch (m_eGizmoType)
			{
			case GT_X:
			{
				if (vCameraAxis.y >= 0.92f || vCameraAxis.y <= -0.92f)
				{
					vAxis.x = vAxisDot.y < -0.5f ? vAxis.x *= -1.f : vAxis.x;
					ObjMove.x = vAxis.x * vMouseGap.y;
					ObjMove.x /= 3.f;
					ObjMove.y = vAxis.y * vMouseGap.x;
					ObjMove.y /= 10.f;
					ObjMove.z = vAxis.z * vMouseGap.z;
					ObjMove.z /= 10.f;
				}
				else if (vCameraAxis.z >= 0.92f || vCameraAxis.z <= -0.92f)
				{
					vAxis.x = vCameraAxis.z > 0.5f ? vAxis.x *= -1.f : vAxis.x;
					ObjMove.x = vAxis.x * vMouseGap.z;
					ObjMove.x /= 3.f;
					ObjMove.y = vAxis.y * vMouseGap.y;
					ObjMove.y /= 10.f;
					ObjMove.z = vAxis.z * vMouseGap.x;
					ObjMove.z /= 10.f;
				}
				else
				{
					ObjMove.x = vAxis.x * vMouseGap.x;
					ObjMove.x /= 3.f;
					ObjMove.y = vAxis.y * vMouseGap.y;
					ObjMove.y /= 10.f;
					ObjMove.z = vAxis.z * vMouseGap.z;
					ObjMove.z /= 10.f;
					if (vAxis.x < 0.f)
					{
						ObjMove.x *= -1.f;
					}

					if (vAxis.y < 0.f)
					{
						ObjMove.y *= -1.f;
					}

					if (vAxis.z < 0.f)
					{
						ObjMove.z *= -1.f;
					}
				}
			}
				break;
			case GT_Y:
			{
				ObjMove.x = vAxis.x * vMouseGap.x;
				ObjMove.x /= 10.f;
				ObjMove.y = vAxis.y * vMouseGap.y;
				ObjMove.y /= 3.f;
				ObjMove.z = vAxis.z * vMouseGap.z;
				ObjMove.z /= 10.f;

				if (vAxis.x < 0.f)
				{
					ObjMove.x *= -1.f;
				}
				
				if (vAxis.y < 0.f)
				{
					ObjMove.y *= -1.f;
				}
				
				if (vAxis.z < 0.f)
				{
					ObjMove.z *= -1.f;
				}
			}
				break;
			case GT_Z:
			{
				float fCameraYAxisX = m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_Y].x;

				if (vCameraAxis.x >= 0.9f || vCameraAxis.x <= -0.9f)
				{
					if (vAxis.z > 0 && vAxis.y < 0)
					{
						vAxis.y *= -1.f;
						//vAxis.z *= -1.f;
					}


					ObjMove.x = vAxis.x * vMouseGap.z;
					ObjMove.x /= 10.f;
					ObjMove.y = vAxis.y * vMouseGap.y;
					ObjMove.y /= 10.f;
					vAxis.z = vCameraAxis.x > -0.5f ? vAxis.z *= -1.f : vAxis.z;
					ObjMove.z = vAxis.z * vMouseGap.x;
					ObjMove.z /= 3.f;
				}
				else if (fCameraYAxisX >= 0.9f ||
					fCameraYAxisX <= -0.9f)
				{
					ObjMove.x = vAxis.x * vMouseGap.z;
					ObjMove.x /= 10.f;
					ObjMove.y = vAxis.y * vMouseGap.y;
					ObjMove.y /= 10.f;
					vAxis.z = fCameraYAxisX < -0.5f ? vAxis.z *= -1.f : vAxis.z;
					ObjMove.z = vAxis.z * vMouseGap.x;
					ObjMove.z /= 3.f;
				}
				else
				{
					if (vAxis.z > 0 && vAxis.y < 0)
					{
						vAxis.y *= -1.f;
						vAxis.z *= -1.f;
					}
					if (vAxis.y < 0 && vAxis.z > 0)
					{
						vAxis.y *= -1.f;
					}
					if (vAxis.z < 0 && vAxis.y > 0)
					{
						vAxis.z *= -1.f;
					}
					if (vAxis.x < 0 && vAxis.z > 0)
					{
						vAxis.x *= -1.f;
					}
					if (vAxis.x < 0 && vAxis.z < 0)
					{
						vAxis.x *= -1.f;
					}

					ObjMove.x = vAxis.x * vMouseGap.x;
					ObjMove.y = vAxis.y * vMouseGap.y;
					ObjMove.z = vAxis.z * vMouseGap.z;
					ObjMove.z /= 3.f;
				}
			}
				break;
			}

			m_pTargetTr->Move(ObjMove);

			//ImGui::Text("ObjMove - X : %f Y : %f Z : %f", ObjMove.x, ObjMove.y, ObjMove.z);
		}
	}

	Vector3 vPos = m_pTargetTr->GetWorldPos();
	vPos.x += m_pTransform->GetWorldAxis()[AXIS_Y].x * 4.f;
	vPos.y += m_pTransform->GetWorldAxis()[AXIS_Y].y * 4.f;
	vPos.z += m_pTransform->GetWorldAxis()[AXIS_Y].z * 4.f;

	switch (m_eGizmoType)
	{
	case GT_X:
	{
		m_pTransform->SetWorldRot(-m_pTargetTr->GetWorldRot().y, 0.f, m_pTargetTr->GetWorldRot().z + 270.f);
		m_pTransform->SetWorldPos(vPos.x , vPos.y, vPos.z);
	}
		break;
	case GT_Y:
	{
		m_pTransform->SetWorldRot(m_pTargetTr->GetWorldRot());
		m_pTransform->SetWorldPos(vPos.x , vPos.y , vPos.z);
	}
		break;
	case GT_Z:
	{
		m_pTransform->SetWorldRot(m_pTargetTr->GetWorldRot().x + 90.f , m_pTargetTr->GetWorldRot().y ,m_pTargetTr->GetWorldRot().z);
		m_pTransform->SetWorldPos(vPos.x, vPos.y, vPos.z);
	}
		break;
	}
	
	//ImGui::Text("--------------------------------------------------------------------------");

	//ImGui::End();

	return 0;
}

int CGizmo::Update(float fTime)
{
	if (m_pTargetTr == nullptr)
	{
		return 0;
	}

	Vector3 vPos = m_pTransform->GetWorldPos();
	switch (m_eGizmoType)
	{
	case GT_X:
	{
		Vector3 vAxis = m_pTargetTr->GetWorldAxis()[AXIS_X];

		vPos += vAxis * 1.f;
		m_pTransform->SetWorldPos(vPos);
	}
	break;
	case GT_Y:
	{
		Vector3 vAxis = m_pTargetTr->GetWorldAxis()[AXIS_Y];

		vPos += vAxis * 1.f;
		m_pTransform->SetWorldPos(vPos);
	}
	break;
	case GT_Z:
	{
		Vector3 vAxis = m_pTargetTr->GetWorldAxis()[AXIS_Z];

		vPos += vAxis * 1.f;
		m_pTransform->SetWorldPos(vPos);
	}
	break;
	}




	return 0;
}

int CGizmo::LateUpdate(float fTime)
{
	return 0;
}

void CGizmo::Collision(float fTime)
{
}

void CGizmo::Render(float fTime)
{
	if (m_pTargetTr == nullptr)
	{
		return;
	}

	Matrix	matPos, matScale, matRot;

	TransformCBuffer	tCBuffer = {};
	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	Vector3 vPos = m_pTargetTr->GetWorldPos();
	Matrix	matView;
	matView = pMainCamera->GetViewMatrix();
	/////////////////////////////Cylinder///////////////////////////////////////////
	CShader* pShader = CShaderManager::GetInst()->FindShader(m_pCylinderMesh->GetShaderKey());
	pShader->SetShader();
	tCBuffer.matWorld = m_pTransform->GetWorldMatrix();
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = m_pTransform->GetPivot();
	tCBuffer.vLength = m_pCylinderMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	m_pCylinderMesh->Render();

	SAFE_RELEASE(pShader);
	matRot.Identity();

	/////////////////////////////Pyramid///////////////////////////////////////////
	pShader = CShaderManager::GetInst()->FindShader(m_pTriangleMesh->GetShaderKey());
	pShader->SetShader();
	matPos.Translation(m_pTransform->GetWorldPos());
	matRot = m_pTransform->GetWorldRotMatrix();
	matScale.Scaling(1.f, 1.f, 1.f);

	tCBuffer.matWorld = matScale * matRot * matPos;
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = Vector3(0.f, -4.f, 0.f);
	tCBuffer.vLength = m_pTriangleMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	m_pTriangleMesh->Render();

	SAFE_RELEASE(pShader);

	SAFE_RELEASE(pMainCamera);
}

CGizmo * CGizmo::Clone()
{
	return new CGizmo(*this);
}

void CGizmo::SetTargetTr(CTransform * _pTr)
{
	if (m_pTargetTr != nullptr)
		SAFE_RELEASE(m_pTargetTr);

	_pTr->AddRef();

	m_pTargetTr = _pTr;
}

void CGizmo::SetTarget(CGameObject * _pObj)
{
	if (m_pTargetTr != nullptr)
		SAFE_RELEASE(m_pTargetTr);

	m_pTargetTr = _pObj->GetTransform();
}

void CGizmo::SetGizmoType(GIZMO_TYPE _eType)
{
	if (m_eGizmoType == _eType)
	{
		return;
	}

	m_eGizmoType = _eType;

	SAFE_RELEASE(m_pCylinderMesh);
	SAFE_RELEASE(m_pTriangleMesh);

	switch (_eType)
	{
		case GT_X:
		{	
			//m_pTransform->SetLocalRot(0.f, 0.f, 270.f);
			m_pCylinderMesh = CResourcesManager::GetInst()->FindMesh("XGizmoCylinder");
			m_pTriangleMesh = CResourcesManager::GetInst()->FindMesh("Pyramid");
		}
		break;
		case GT_Y:
		{
			m_pCylinderMesh = CResourcesManager::GetInst()->FindMesh("YGizmoCylinder");
			m_pTriangleMesh = CResourcesManager::GetInst()->FindMesh("Pyramid");
		}
		break;
		case GT_Z:
		{
			//m_pTransform->SetLocalRot(90.f, 0.f, 0.f);
			m_pCylinderMesh = CResourcesManager::GetInst()->FindMesh("ZGizmoCylinder");
			m_pTriangleMesh = CResourcesManager::GetInst()->FindMesh("Pyramid");
		}
		break;
	}
}


void CGizmo::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWorld")
	{
		if (CInput::GetInst()->GetMousePress(MS_LBUTTON) == true)
		{
			m_bPick = true;
		}
	}
}