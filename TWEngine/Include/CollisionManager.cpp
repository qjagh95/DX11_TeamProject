#include "../EngineHeader.h"
#include "CollisionManager.h"
#include "Input.h"
#include "Device.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/ColliderRay.h"
#include "Component/ColliderPoint.h"
#include "Rendering/RenderManager.h"
#include "Core.h"

PUN_USING

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
	m_isShow = true;
	m_pPrevMouseCollision = nullptr;
	m_iListSize = 0;
	m_iListCapacity = 128;
	m_pWorldMouseList = new CCollider*[m_iListCapacity];
	memset(m_pWorldMouseList, 0, sizeof(CCollider*) * m_iListCapacity);
}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE_ARRAY(m_pWorldMouseList);
	Safe_Delete_Map(m_mapGroup);
}

bool CCollisionManager::Init()
{
	if (CCore::GetInst()->m_bEditorMode == true)
	{
		CreateGroup("Default", Vector3(-3000.f, -3000.f, -3000.f), Vector3(3000.f, 3000.f, 3000.f), 
			1, 1, 1, CGT_3D);
	}
	else
	{
		CreateGroup("Default", Vector3(-200.f, -200.f, -200.f), Vector3(200.f, 200.f, 200.f),
			10, 10, 10, CGT_3D);
	}

	CreateGroup("BackGround", Vector3(0.f, 0.f, 0.f), Vector3(5000.f, 5000.f, 0.f),
		10, 10, 1, CGT_3D);
	CreateGroup("UI", Vector3(0.f, 0.f, 0.f),
		Vector3((float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 0.f),
		4, 4, 1, CGT_2D);

	CInput::GetInst()->AddKey("LineOnOff", VK_F5);

	return true;
}

bool CCollisionManager::CreateGroup(const string & strKey,
	const Vector3 & vMin, const Vector3 & vMax,
	int iCountX, int iCountY, int iCountZ,
	COLLISION_GROUP_TYPE eGroupType)
{
	PCollisionGroup pGroup = FindGroup(strKey);

	if (pGroup)
		return false;

	pGroup = new CollisionGroup;

	pGroup->eType = eGroupType;
	pGroup->vMin = vMin;
	pGroup->vMax = vMax;
	pGroup->iCountX = iCountX;
	pGroup->iCountY = iCountY;
	pGroup->iCountZ = iCountZ;
	pGroup->iCount = iCountX * iCountY * iCountZ;

	pGroup->pSectionList = new CollisionSection[pGroup->iCount];

	pGroup->vLength = vMax - vMin;
	pGroup->vCellLength = pGroup->vLength / Vector3((float)iCountX, (float)iCountY, (float)iCountZ);

	m_mapGroup.insert(make_pair(strKey, pGroup));

	return true;
}

void CCollisionManager::ChangeGroupType(const string & strGroup,
	COLLISION_GROUP_TYPE eType)
{
	PCollisionGroup	pGroup = FindGroup(strGroup);

	if (!pGroup)
		return;

	pGroup->eType = eType;
}

void CCollisionManager::AddCollision(CGameObject * pObj)
{
	if (!pObj->CheckComponentFromType(CT_COLLIDER))
		return;

	const list<CComponent*>*	pCollList = pObj->FindComponentsFromType(CT_COLLIDER);

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = pCollList->end();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();

	CTransform*	pTransform = pScene->GetMainCameraTransform();

	Vector3	vCameraPos = pTransform->GetWorldPos();

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pScene);

	for (iter = pCollList->begin(); iter != iterEnd; ++iter)
	{
		((CCollider*)*iter)->ClearCollisionSection();

		if ((*iter)->GetEnable() == true)
		{
			PCollisionGroup	pGroup = FindGroup(((CCollider*)*iter)->GetCollisionGroup());

			if (!pGroup)
			{
				assert(false);
				return;
			}

			/*if (pGroup->eType == CGT_2D)
			{
				Vector3	vRS = Vector3(_RESOLUTION.iWidth, _RESOLUTION.iHeight,
					0.f);
				vCameraPos += vRS / 2.f;
			}

			if (((CCollider*)*iter)->GetCollisionGroup() == "UI")
			{
				vCameraPos = Vector3::Zero;
			}*/

			// 나중에 필요하면 카메라에 충돌영역 자체를 붙여주어서
			// 충돌 영역이 카메라의 위치를 기준으로 잡힐 수 있도록
			// 한다.
			Vector3	vSectionMin = ((CCollider*)*iter)->GetSectionMin();
			Vector3	vSectionMax = ((CCollider*)*iter)->GetSectionMax();

			int	xMin = 0, xMax = 0, yMin = 0, yMax = 0, zMin = 0, zMax = 0;
			xMin = (int)(vSectionMin.x - pGroup->vMin.x) / (int)pGroup->vCellLength.x;
			xMax = (int)(vSectionMax.x - pGroup->vMin.x) / (int)pGroup->vCellLength.x;
			yMin = (int)(vSectionMin.y - pGroup->vMin.y) / (int)pGroup->vCellLength.y;
			yMax = (int)(vSectionMax.y - pGroup->vMin.y) / (int)pGroup->vCellLength.y;
			if (pGroup->iCountZ > 1)
			{
				zMin = (int)(vSectionMin.z - pGroup->vMin.z) / (int)pGroup->vCellLength.z;
				zMax = (int)(vSectionMax.z - pGroup->vMin.z) / (int)pGroup->vCellLength.z;
			}

			vector<int>	vecIndex;
			for (int i = zMin; i <= zMax; ++i)
			{
				if (i < 0 || i >= pGroup->iCountZ)
					continue;

				for (int j = yMin; j <= yMax; ++j)
				{
					if (j < 0 || j >= pGroup->iCountY)
						continue;

					for (int k = xMin; k <= xMax; ++k)
					{
						if (k < 0 || k >= pGroup->iCountX)
							continue;

						int	idx = i * (pGroup->iCountX * pGroup->iCountY) +
							j * pGroup->iCountX + k;

						vecIndex.push_back(idx);

						((CCollider*)*iter)->AddCollisionSection(idx);

						PCollisionSection	pSection = &pGroup->pSectionList[idx];

						if (pSection->iCapacity == pSection->iSize)
						{
							pSection->iCapacity *= 2;
							CCollider**	pArray = new CCollider*[pSection->iCapacity];

							memcpy(pArray, pSection->pList, sizeof(CCollider*) * pSection->iSize);

							SAFE_DELETE_ARRAY(pSection->pList);
							pSection->pList = pArray;
						}

						pSection->pList[pSection->iSize] = (CCollider*)*iter;
						++pSection->iSize;
					}
				}
			}

			//(*iter)->Release();
		}
	}
}

void CCollisionManager::ClearCollisionGroup()
{
	Safe_Delete_Map(m_mapGroup);
}

void CCollisionManager::Collision(float fTime)
{
	// 마우스와 오보젝트간 충돌처리
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();

	CColliderPoint*	pWindowPoint = pMouseObj->FindComponentFromTag<CColliderPoint>("MouseWindow");

	// 섹션 정보를 비워준다.
	pWindowPoint->ClearCollisionSection();

	PCollisionGroup	pGroup = FindGroup("UI");

	Vector3	vMousePoint = pWindowPoint->GetInfo();

	int	iMouseSectionX = (int)vMousePoint.x / (int)pGroup->vCellLength.x;
	int	iMouseSectionY = (int)vMousePoint.y / (int)pGroup->vCellLength.y;

	bool	bUICollision = false;

	if (iMouseSectionX >= 0 && iMouseSectionX < pGroup->iCountX &&
		iMouseSectionY >= 0 && iMouseSectionY < pGroup->iCountY)
	{
		int	iPointSection = iMouseSectionY * pGroup->iCountX + iMouseSectionX;

		PCollisionSection	pSection = &pGroup->pSectionList[iPointSection];

		pWindowPoint->AddCollisionSection(iPointSection);

		if (pSection->iSize > 0)
		{
			for (int i = 0; i < pSection->iSize; ++i)
			{
				CCollider*	pCollSrc = pSection->pList[i];
				CCollider*	pCollDest = pWindowPoint;

				bool Pair = false;
				for (size_t a = 0; a < pCollSrc->GetContinueSize(); a++)
				{
					if (pCollDest->GetMyTypeName() == pCollSrc->GetContinueName(a))
					{
						Pair = true;
						break;
					}
				}

				if (pCollSrc->Collision(pCollDest, fTime) && Pair == false)
				{
					// 처음 충돌될 경우
					if (!pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->AddPrevCollision(pCollDest);
						pCollDest->AddPrevCollision(pCollSrc);

						pCollSrc->OnCollisionEnter(pCollDest, fTime);
						pCollDest->OnCollisionEnter(pCollSrc, fTime);

						bUICollision = true;

						break;
					}

					// 이전에 충돌되고 있었을 경우
					else
					{
						pCollSrc->OnCollision(pCollDest, fTime);
						pCollDest->OnCollision(pCollSrc, fTime);

						bUICollision = true;

						break;
					}
				}

				else
				{
					// 충돌되고 있다가 떨어질 경우
					if (pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->ErasePrevCollision(pCollDest);
						pCollDest->ErasePrevCollision(pCollSrc);

						pCollSrc->OnCollisionLeave(pCollDest, fTime);
						pCollDest->OnCollisionLeave(pCollSrc, fTime);
					}
				}
			}
		}
	}

	SAFE_RELEASE(pWindowPoint);

	if (!bUICollision)
	{
		if (GET_SINGLE(CRenderManager)->GetGameMode() == GM_2D)
			CollisionMouse2D(pMouseObj, fTime);

		else
			CollisionMouse3D(pMouseObj, fTime);
	}

	//SAFE_RELEASE(pMouseObj);

	unordered_map<string, PCollisionGroup>::iterator iter;
	unordered_map<string, PCollisionGroup>::iterator iterEnd = m_mapGroup.end();

	for (iter = m_mapGroup.begin(); iter != iterEnd; ++iter)
	{
		// 영역 수만큼 반복한다.
		for (int i = 0; i < iter->second->iCount; ++i)
		{
			PCollisionSection	pSection = &iter->second->pSectionList[i];

			if (pSection->iSize < 2)
			{
				for (int j = 0; j < pSection->iSize; ++j)
				{
					pSection->pList[j]->CheckPrevCollisionInSection(fTime);
				}
				continue;
			}

			for (int j = 0; j < pSection->iSize; ++j)
			{
				pSection->pList[j]->CheckPrevCollisionInSection(fTime);
			}

			// 각 영역별 충돌체 수만큼 반복한다.
			for (int j = 0; j < pSection->iSize - 1; ++j)
			{
				for (int k = j + 1; k < pSection->iSize; ++k)
				{
					CGameObject*	pSrc = pSection->pList[j]->GetGameObject();
					CGameObject*	pDest = pSection->pList[k]->GetGameObject();

					if (pSrc == pDest)
					{
						SAFE_RELEASE(pSrc);
						SAFE_RELEASE(pDest);
						continue;
					}

					CCollider*	pCollSrc = pSection->pList[j];
					CCollider*	pCollDest = pSection->pList[k];

					bool Pair = false;
					for (size_t a = 0; a < pCollSrc->GetContinueSize(); a++)
					{
						if (pCollDest->GetMyTypeName() == pCollSrc->GetContinueName(a))
						{
							Pair = true;
							break;
						}
					}

					if (pCollSrc->Collision(pCollDest, fTime) && Pair == false)
					{
						// 처음 충돌될 경우
						if (!pCollSrc->CheckPrevCollision(pCollDest))
						{
							pCollSrc->AddPrevCollision(pCollDest);
							pCollDest->AddPrevCollision(pCollSrc);

							pCollSrc->OnCollisionEnter(pCollDest, fTime);
							pCollDest->OnCollisionEnter(pCollSrc, fTime);
						}

						// 이전에 충돌되고 있었을 경우
						else
						{
							if (pCollSrc->GetCallBackFunc() == false && pCollDest->GetCallBackFunc() == false)
							{
								pCollSrc->OnCollision(pCollDest, fTime);
								pCollDest->OnCollision(pCollSrc, fTime);
								pCollSrc->SetCallBackFunc(true);
								pCollDest->SetCallBackFunc(true);
							}
						}
					}

					else
					{
						// 충돌되고 있다가 떨어질 경우
						if (pCollSrc->CheckPrevCollision(pCollDest))
						{
							pCollSrc->ErasePrevCollision(pCollDest);
							pCollDest->ErasePrevCollision(pCollSrc);

							pCollSrc->OnCollisionLeave(pCollDest, fTime);
							pCollDest->OnCollisionLeave(pCollSrc, fTime);
						}
					}

					SAFE_RELEASE(pSrc);
					SAFE_RELEASE(pDest);
				}
			}
		}
	}
}

void CCollisionManager::Render(float fTime)
{
	OnOff();

	unordered_map<string, PCollisionGroup>::iterator iter;
	unordered_map<string, PCollisionGroup>::iterator iterEnd = m_mapGroup.end();

	for (iter = m_mapGroup.begin(); iter != iterEnd; ++iter)
	{
		if (iter->first == "UI")
			continue;

		// 영역 수만큼 반복한다.
		for (int i = 0; i < iter->second->iCount; ++i)
		{
			PCollisionSection	pSection = &iter->second->pSectionList[i];

			for (int j = 0; j < pSection->iSize; ++j)
			{
				pSection->pList[j]->Render(fTime);
			}

			pSection->iSize = 0;
		}
	}

	iter = m_mapGroup.find("UI");

	// 영역 수만큼 반복한다.
	for (int i = 0; i < iter->second->iCount; ++i)
	{
		PCollisionSection	pSection = &iter->second->pSectionList[i];

		for (int j = 0; j < pSection->iSize; ++j)
			pSection->pList[j]->Render(fTime);

		pSection->iSize = 0;
	}
}

void CCollisionManager::OnOff()
{
	if (CInput::GetInst()->KeyPress("LineOnOff"))
		m_isShow ^= true;
}

void CCollisionManager::CollisionMouse2D(CGameObject* pMouseObj,
	float fTime)
{
	CColliderPoint*	pWorldPoint = pMouseObj->FindComponentFromTag<CColliderPoint>("MouseWorld");

	// 섹션 정보를 비워준다.
	pWorldPoint->ClearCollisionSection();

	unordered_map<string, PCollisionGroup>::iterator	iterG;
	unordered_map<string, PCollisionGroup>::iterator	iterGEnd = m_mapGroup.end();

	bool	bMouseCollision = false;
	for (iterG = m_mapGroup.begin(); iterG != iterGEnd; ++iterG)
	{
		if (iterG->first == "UI")
			continue;

		Vector3 vMousePoint = pWorldPoint->GetInfo();

		int iMouseSectionX = (int)vMousePoint.x / (int)iterG->second->vCellLength.x;
		int iMouseSectionY = (int)vMousePoint.y / (int)iterG->second->vCellLength.y;

		if (iMouseSectionX >= 0 && iMouseSectionX < iterG->second->iCountX &&
			iMouseSectionY >= 0 && iMouseSectionY < iterG->second->iCountY)
		{
			int iPointSection = iMouseSectionY * iterG->second->iCountX + iMouseSectionX;

			PCollisionSection	pSection = &iterG->second->pSectionList[iPointSection];

			pWorldPoint->AddCollisionSection(iPointSection);

			// 영역 수만큼 반복한다.
			/*if (iterG->second->pSectionList[i]->iSize >= 2)
			{
			sort(iterG->second->pSection[i]->pColliderArray,
			&iterG->second->pSection[i]->pColliderArray[iterG->second->pSection[i]->iSize - 1],
			CCollisionManager::SortZ);
			}*/

			for (int j = 0; j < pSection->iSize; ++j)
			{
				// 충돌처리를 한다.
				CCollider*	pCollSrc = pSection->pList[j];
				CCollider*	pCollDest = pWorldPoint;

				if (bMouseCollision)
				{
					if (pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->ErasePrevCollision(pCollDest);
						pCollDest->ErasePrevCollision(pCollSrc);

						pCollSrc->OnCollisionLeave(pCollDest, fTime);
						pCollDest->OnCollisionLeave(pCollSrc, fTime);
					}

					continue;
				}

				if (pCollSrc->Collision(pCollDest, fTime))
				{
					// 처음 충돌될 경우
					if (!pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->AddPrevCollision(pCollDest);
						pCollDest->AddPrevCollision(pCollSrc);

						pCollSrc->OnCollisionEnter(pCollDest, fTime);
						pCollDest->OnCollisionEnter(pCollSrc, fTime);

						bMouseCollision = true;
					}

					// 이전에 충돌되고 있었을 경우
					else
					{
						pCollSrc->OnCollision(pCollDest, fTime);
						pCollDest->OnCollision(pCollSrc, fTime);

						bMouseCollision = true;
					}
				}

				else
				{
					// 충돌되고 있다가 떨어질 경우
					if (pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->ErasePrevCollision(pCollDest);
						pCollDest->ErasePrevCollision(pCollSrc);

						pCollSrc->OnCollisionLeave(pCollDest, fTime);
						pCollDest->OnCollisionLeave(pCollSrc, fTime);
					}
				}
			}
		}
	}

	SAFE_RELEASE(pWorldPoint);
}

void CCollisionManager::CollisionMouse3D(CGameObject* pMouseObj,
	float fTime)
{
	CColliderRay*	pWorldRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseWorld");

	// 섹션 정보를 비워준다.
	pWorldRay->ClearCollisionSection();

	unordered_map<string, PCollisionGroup>::iterator	iterG;
	unordered_map<string, PCollisionGroup>::iterator	iterGEnd = m_mapGroup.end();

	for (iterG = m_mapGroup.begin(); iterG != iterGEnd; ++iterG)
	{
		if (iterG->first == "UI" || iterG->first == "BackGround")
			continue;

		Vector3 vSectionMin = pWorldRay->GetSectionMin();
		Vector3 vSectionMax = pWorldRay->GetSectionMax();

		int iMinX = (int)(vSectionMin.x - iterG->second->vMin.x) / (int)iterG->second->vCellLength.x;
		int iMinY = (int)(vSectionMin.y - iterG->second->vMin.y) / (int)iterG->second->vCellLength.y;
		int iMinZ = (int)(vSectionMin.z - iterG->second->vMin.z) / (int)iterG->second->vCellLength.z;

		iMinX = iMinX < 0 ? 0 : iMinX;
		iMinY = iMinY < 0 ? 0 : iMinY;
		iMinZ = iMinZ < 0 ? 0 : iMinZ;

		int iMaxX = (int)(vSectionMax.x - iterG->second->vMin.x) / (int)iterG->second->vCellLength.x;
		int iMaxY = (int)(vSectionMax.y - iterG->second->vMin.y) / (int)iterG->second->vCellLength.y;
		int iMaxZ = (int)(vSectionMax.z - iterG->second->vMin.z) / (int)iterG->second->vCellLength.z;

		iMaxX = iMaxX >= iterG->second->iCountX ? iterG->second->iCountX - 1 : iMaxX;
		iMaxY = iMaxY >= iterG->second->iCountY ? iterG->second->iCountY - 1 : iMaxY;
		iMaxZ = iMaxZ >= iterG->second->iCountZ ? iterG->second->iCountZ - 1 : iMaxZ;

		for (int z = iMinZ; z <= iMaxZ; ++z)
		{
			for (int y = iMinY; y <= iMaxY; ++y)
			{
				for (int x = iMinX; x <= iMaxX; ++x)
				{
					int	idx = z * (iterG->second->iCountX * iterG->second->iCountY) +
						y * iterG->second->iCountX + x;

					PCollisionSection	pSection = &iterG->second->pSectionList[idx];

					pWorldRay->AddCollisionSection(idx);

					for (int j = 0; j < pSection->iSize; ++j)
					{
						if (m_iListSize == m_iListCapacity)
						{
							m_iListCapacity *= 2;
							CCollider**	pList = new CCollider*[m_iListCapacity];
							memset(pList, 0, sizeof(CCollider*) * m_iListCapacity);
							memcpy(pList, m_pWorldMouseList, sizeof(CCollider*) * m_iListSize);

							SAFE_DELETE_ARRAY(m_pWorldMouseList);
							m_pWorldMouseList = pList;
						}

						m_pWorldMouseList[m_iListSize] = pSection->pList[j];
						++m_iListSize;
					}
				}
			}
		}
	}

	bool	bMouseCollision = false;
	class CCollider*	pCollision = nullptr;

	// 영역 수만큼 반복한다.
	if (m_iListSize >= 2)
	{
		qsort(m_pWorldMouseList, m_iListSize,
			sizeof(CCollider*), CCollisionManager::SortZ);
	}

	for (int i = 0; i < m_iListSize; ++i)
	{
		// 충돌처리를 한다.
		CCollider*	pCollSrc = m_pWorldMouseList[i];
		CCollider*	pCollDest = pWorldRay;

		if (pCollSrc->Collision(pCollDest, fTime))
		{
			// 처음 충돌될 경우
			if (!pCollSrc->CheckPrevCollision(pCollDest))
			{
				pCollSrc->AddPrevCollision(pCollDest);
				pCollDest->AddPrevCollision(pCollSrc);

				pCollSrc->OnCollisionEnter(pCollDest, fTime);
				pCollDest->OnCollisionEnter(pCollSrc, fTime);

				bMouseCollision = true;
			}

			// 이전에 충돌되고 있었을 경우
			else
			{
				pCollSrc->OnCollision(pCollDest, fTime);
				pCollDest->OnCollision(pCollSrc, fTime);

				bMouseCollision = true;
			}

			pCollision = pCollSrc;
			break;
		}

		else
		{
			// 충돌되고 있다가 떨어질 경우
			if (pCollSrc->CheckPrevCollision(pCollDest))
			{
				pCollSrc->ErasePrevCollision(pCollDest);
				pCollDest->ErasePrevCollision(pCollSrc);

				pCollSrc->OnCollisionLeave(pCollDest, fTime);
				pCollDest->OnCollisionLeave(pCollSrc, fTime);

				if (m_pPrevMouseCollision == pCollSrc)
				{
					m_pPrevMouseCollision = nullptr;
				}
			}
		}
	}

	if (bMouseCollision)
	{
		if (m_pPrevMouseCollision != pCollision)
		{
			if (m_pPrevMouseCollision)
			{
				m_pPrevMouseCollision->ErasePrevCollision(pWorldRay);
				pWorldRay->ErasePrevCollision(m_pPrevMouseCollision);

				m_pPrevMouseCollision->OnCollisionLeave(pWorldRay, fTime);
				pWorldRay->OnCollisionLeave(m_pPrevMouseCollision, fTime);
			}
			m_pPrevMouseCollision = pCollision;
		}
	}

	m_iListSize = 0;

	SAFE_RELEASE(pWorldRay);
}

CCollisionManager::PCollisionGroup CCollisionManager::FindGroup(const string & strGroup)
{
	unordered_map<string, PCollisionGroup>::iterator iter = m_mapGroup.find(strGroup);

	if (iter == m_mapGroup.end())
		return nullptr;

	return iter->second;
}

int CCollisionManager::SortZ(const void * pSrc, const void * pDest)
{
	CCollider*	pCollSrc = *((CCollider**)pSrc);
	CCollider*	pCollDest = *((CCollider**)pDest);

	CTransform*	pSrcTr = pCollSrc->GetTransform();
	CTransform*	pDestTr = pCollDest->GetTransform();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();

	CTransform*	pCameraTr = pScene->GetMainCameraTransform();

	Vector3	vSrcPos = pSrcTr->GetWorldPos();
	Vector3	vDestPos = pDestTr->GetWorldPos();
	Vector3	vCameraPos = pCameraTr->GetWorldPos();

	float	fSrcDist = vSrcPos.Distance(vCameraPos);
	float	fDestDist = vDestPos.Distance(vCameraPos);

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pScene);

	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);

	if (fSrcDist > fDestDist)
		return 1;

	else if (fSrcDist < fDestDist)
		return -1;

	return 0;
}
