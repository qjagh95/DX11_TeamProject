#include "../EngineHeader.h"
#include "Gizmo.h"
#include "../Resource/Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "../Rendering/Shader.h"
#include "ColliderOBB3D.h"
#include "../Input.h"
#include "../Rendering/ViewManager.h"
#include "../Rendering/RenderState.h"
#include "../Rendering/DepthState.h"
#include "../Device.h"
#include "../EditManager.h"

PUN_USING

CGizmo::CGizmo()
{
	m_eComType = CT_GIZMO;
	m_pCylinderMesh = nullptr;
	m_pTriangleMesh = nullptr;
	m_pTargetTr = nullptr;
	m_pDepthDisable = nullptr;
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
	//SAFE_RELEASE(m_pTargetTr);
	SAFE_RELEASE(m_pDepthDisable);
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

	CColliderOBB3D* pOBB = m_pObject->AddComponent<CColliderOBB3D>("GizmoCollider");

	float	fLength[3] = { 4.f , 20.f, 4.f };

	pOBB->SetInfo(Vector3::Zero, Vector3::Axis, fLength);
	pOBB->SetCollisionCallback(CCT_STAY, this, &CGizmo::Hit);
	SAFE_RELEASE(pOBB);

	m_pTransform->SetWorldScale(4.f, 12.f, 4.f);
	m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);

	m_pDepthDisable = CViewManager::GetInst()->FindRenderState(DEPTH_DISABLE);

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

	Vector3 vAxisDot = vAxis.Dot(vCameraAxis);

	if (m_bPick == true)
	{
		if (CInput::GetInst()->KeyRelease("LButton") == true)
		{
			m_bPick = false;
			CEditManager::GetInst()->SetGizmoClick(m_bPick);
		}
		else if (CInput::GetInst()->KeyPush("LButton") == true)
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
				if (fabsf(vAxis.x) < 0.2f)
				{
					vAxis.x = 0.f;
				}

				if (fabsf(vAxis.z) < 0.2f)
				{
					vAxis.z = 0.f;
				}

				if (vCameraAxis.y >= 0.92f || vCameraAxis.y <= -0.92f)
				{
					vAxis.x = vAxisDot.y < -0.5f ? vAxis.x *= -1.f : vAxis.x;
					ObjMove.x = vAxis.x * vMouseGap.y;
					ObjMove.x /= 3.f;
					ObjMove.y = vAxis.y * vMouseGap.x;
					ObjMove.y /= 10.f;
					ObjMove.z = vAxis.z * vMouseGap.z;
					ObjMove.z /= 10.f;

					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
					{
						ObjMove.x *= -1.f;
					}
					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_Z].z < 0.f)
					{
						ObjMove.z *= -1.f;
					}
				}
				//else if (vCameraAxis.z >= 0.92f || vCameraAxis.z <= -0.92f)
				//{
				//	vAxis.x = vCameraAxis.z > 0.5f ? vAxis.x *= -1.f : vAxis.x;
				//	ObjMove.x = vAxis.x * vMouseGap.z;
				//	ObjMove.x /= 3.f;
				//	ObjMove.y = vAxis.y * vMouseGap.y;
				//	ObjMove.y /= 10.f;
				//	ObjMove.z = vAxis.z * vMouseGap.x;
				//	ObjMove.z /= 10.f;
				//}
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

					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
					{
						ObjMove.x *= -1.f;
					}
					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_Z].z < 0.f)
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

				if (fabsf(vAxis.x) < 0.2f)
				{
					vAxis.x = 0.f;
				}

				if (fabsf(vAxis.z) < 0.2f)
				{
					vAxis.z = 0.f;
				}

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

				if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
				{
					ObjMove.x *= -1.f;
				}
				if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_Z].z < 0.f)
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

					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
					{
						ObjMove.x *= -1.f;
					}
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

					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
					{
						ObjMove.x *= -1.f;
					}
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

					if (m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X].x < 0.f)
					{
						ObjMove.x *= -1.f;
					}
				}
			}
			break;
			}

			m_pTargetTr->Move(ObjMove);

			//ImGui::Text("ObjMove - X : %f Y : %f Z : %f", ObjMove.x, ObjMove.y, ObjMove.z);
		}
	}

	Vector3 vPos = m_pTargetTr->GetWorldPos();
	vPos.x += m_pTargetTr->GetWorldAxis()[m_eGizmoType].x * 25.f;
	vPos.y += m_pTargetTr->GetWorldAxis()[m_eGizmoType].y * 25.f;
	vPos.z += m_pTargetTr->GetWorldAxis()[m_eGizmoType].z * 25.f;

	switch (m_eGizmoType)
	{
	case GT_X:
	{
		m_pTransform->SetWorldRot(-m_pTargetTr->GetWorldRot().y, 0.f, m_pTargetTr->GetWorldRot().z + 270.f);
		m_pTransform->SetWorldPos(vPos.x, vPos.y, vPos.z);
	}
	break;
	case GT_Y:
	{
		m_pTransform->SetWorldRot(m_pTargetTr->GetWorldRot());
		m_pTransform->SetWorldPos(vPos.x, vPos.y, vPos.z);
	}
	break;
	case GT_Z:
	{
		m_pTransform->SetWorldRot(m_pTargetTr->GetWorldRot().x + 90.f, m_pTargetTr->GetWorldRot().y, m_pTargetTr->GetWorldRot().z);
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

	m_pDepthDisable->SetState();


	ID3D11InputLayout* pLayout = CShaderManager::GetInst()->FindInputLayout(m_pCylinderMesh->GetInputLayoutKey());

	Matrix	matPos, matScale, matRot;

	TransformCBuffer	tCBuffer = {};
	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	Vector3 vPos = m_pTargetTr->GetWorldPos();
	Matrix	matView;
	matView = pMainCamera->GetViewMatrix();
	/////////////////////////////Cylinder///////////////////////////////////////////
	CShader* pShader = CShaderManager::GetInst()->FindShader(m_pCylinderMesh->GetShaderKey());
	pShader->SetShader();
	CONTEXT->IASetInputLayout(pLayout);
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
	pLayout = CShaderManager::GetInst()->FindInputLayout(m_pTriangleMesh->GetInputLayoutKey());
	pShader = CShaderManager::GetInst()->FindShader(m_pTriangleMesh->GetShaderKey());
	pShader->SetShader();
	CONTEXT->IASetInputLayout(pLayout);
	matPos.Translation(m_pTransform->GetWorldPos());
	matRot = m_pTransform->GetWorldRotMatrix();
	matScale.Scaling(6.f, 5.f, 6.f);

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
	m_pDepthDisable->ResetState();
}

CGizmo * CGizmo::Clone()
{
	return new CGizmo(*this);
}

void CGizmo::SetTargetTr(CTransform * _pTr)
{
	//if (m_pTargetTr != nullptr)
	//	SAFE_RELEASE(m_pTargetTr);

	//_pTr->AddRef();

	m_pTargetTr = _pTr;
}

void CGizmo::SetTarget(CGameObject * _pObj)
{
	//if (m_pTargetTr != nullptr)
	//	SAFE_RELEASE(m_pTargetTr);

	m_pTargetTr = _pObj->GetTransformNonCount();
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
		if (CInput::GetInst()->KeyPress("LButton") == true)
		{
			m_bPick = true;
			CEditManager::GetInst()->SetGizmoClick(m_bPick);
		}
	}
}
