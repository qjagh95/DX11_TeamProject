#include "EngineHeader.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Device.h"
#include "Input.h"
#include "Component/ColliderPoint.h"

PUN_USING

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
	Safe_Delete_Map(m_mapGroup);
}

bool CCollisionManager::Init()
{
	CreateGroup("Default", Vector3(0.f, 0.f, 0.f), Vector3(10000.f, 10000.f, 0.f),
		10, 10, 1, CGT_2D);
	CreateGroup("BackGround", Vector3(0.f, 0.f, 0.f), Vector3(10000.f, 10000.f, 0.f),
		10, 10, 1, CGT_2D);
	CreateGroup("UI", Vector3(0.f, 0.f, 0.f),
		Vector3((float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 0.f),
		4, 4, 1, CGT_2D);

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
	PCollisionGroup pGroup = FindGroup(strGroup);

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

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CTransform* pTransform = pScene->GetMainCameraTransform();

	Vector3 vCameraPos = pTransform->GetWorldPos();

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pScene);

	for (iter = pCollList->begin(); iter != iterEnd; ++iter)
	{
		((CCollider*)*iter)->ClearCollisionSection();

		PCollisionGroup pGroup = FindGroup(((CCollider*)*iter)->GetCollisionGroup());

		if (!pGroup)
		{
			assert(false);
			return;
		}

		/*if (pGroup->eType == CGT_2D)
		{
			Vector3 vRS = Vector3(_RESOLUTION.iWidth, _RESOLUTION.iHeight,
				0.f);
			vCameraPos += vRS / 2.f;
		}

		if (((CCollider*)*iter)->GetCollisionGroup() == "UI")
		{
			vCameraPos = Vector3::Zero;
		}*/

		// ���߿� �ʿ��ϸ� ī�޶� �浹���� ��ü�� �ٿ��־
		// �浹 ������ ī�޶��� ��ġ�� �������� ���� �� �ֵ��� �Ѵ�


		Vector3 vSectionMin = ((CCollider*)*iter)->GetSectionMin();
		Vector3 vSectionMax = ((CCollider*)*iter)->GetSectionMax();

		int xMin = 0, xMax = 0, yMin = 0, yMax = 0, zMin = 0, zMax = 0;
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

					int idx = i * (pGroup->iCountX * pGroup->iCountY) +
						j * pGroup->iCountX + k;

					vecIndex.push_back(idx);

					((CCollider*)*iter)->AddCollisionSection(idx);

					PCollisionSection	pSection = &pGroup->pSectionList[idx];

					if (pSection->iCapacity == pSection->iSize)
					{
						pSection->iCapacity *= 2;
						CCollider** pArray = new CCollider*[pSection->iCapacity];

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

void CCollisionManager::ClearCollisionGroup()
{
	Safe_Delete_Map(m_mapGroup);
}

void CCollisionManager::Collision(float fTime)
{
	// ���콺�� ������Ʈ�� �浹ó��
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();

	CColliderPoint*	pWindowPoint = pMouseObj->FindComponentFromTag<CColliderPoint>("MouseWindow");

	// ���� ������ ����ش�
	pWindowPoint->ClearCollisionSection();

	PCollisionGroup pGroup = FindGroup("UI");

	Vector3 vMousePoint = pWindowPoint->GetInfo();

	int iMouseSectionX = (int)vMousePoint.x / (int)pGroup->vCellLength.x;
	int iMouseSectionY = (int)vMousePoint.y / (int)pGroup->vCellLength.y;

	bool	bUICollision = false;

	if (iMouseSectionX >= 0 && iMouseSectionX < pGroup->iCountX &&
		iMouseSectionY >= 0 && iMouseSectionY < pGroup->iCountY)
	{
		int iPointSection = iMouseSectionY * pGroup->iCountX + iMouseSectionX;

		PCollisionSection pSection = &pGroup->pSectionList[iPointSection];

		pWindowPoint->AddCollisionSection(iPointSection);

		if (pSection->iSize > 0)
		{
			// UI�� ZOrder�� ���� �����Ѵ�
			/*sort(pSection->pColliderArray,
			&pSection->pColliderArray[pSection->iSize - 1],
			CCollisionManager::SortZOrder);
			*/

			for (int i = 0; i < pSection->iSize; ++i)
			{
				CCollider*	pCollSrc = pSection->pList[i];
				CCollider*	pCollDest = pWindowPoint;

				if (pCollSrc->Collision(pCollDest, fTime))
				{
					// ó�� �浹�� ���
					if (!pCollSrc->CheckPrevCollision(pCollDest))
					{
						pCollSrc->AddPrevCollision(pCollDest);
						pCollDest->AddPrevCollision(pCollSrc);

						pCollSrc->OnCollisionEnter(pCollDest, fTime);
						pCollDest->OnCollisionEnter(pCollSrc, fTime);

						bUICollision = true;

						break;
					}

					// ������ �浹�ǰ� �־��� ���
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
					// �浹�ǰ� �ִٰ� ������ ���
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
		CColliderPoint* pWorldPoint = pMouseObj->FindComponentFromTag<CColliderPoint>("MouseWorld");

		// ���� ������ ����ش�
		pWorldPoint->ClearCollisionSection();

		unordered_map<string, PCollisionGroup>::iterator	iterG;
		unordered_map<string, PCollisionGroup>::iterator	iterGEnd = m_mapGroup.end();

		bool	bMouseCollision = false;

		for (iterG = m_mapGroup.begin(); iterG != iterGEnd; ++iterG)
		{
			if (iterG->first == "UI")
				continue;

			vMousePoint = pWorldPoint->GetInfo();

			iMouseSectionX = (int)vMousePoint.x / (int)iterG->second->vCellLength.x;
			iMouseSectionY = (int)vMousePoint.y / (int)iterG->second->vCellLength.y;

			if (iMouseSectionX >= 0 && iMouseSectionX < iterG->second->iCountX &&
				iMouseSectionY >= 0 && iMouseSectionY < iterG->second->iCountY)
			{
				int iPointSection = iMouseSectionY * iterG->second->iCountX + iMouseSectionX;

				PCollisionSection	pSection = &iterG->second->pSectionList[iPointSection];

				pWorldPoint->AddCollisionSection(iPointSection);

				/*
				if(iterG->second->pSectionList[i]->iSize >= 2)
				{
				sort(iterG->second->pSection[i]->pColliderArray,
					&iterG->second->pSection[i]->pColliderArray[iterG->second->pSection[i]->iSize - 1],
					CCollisionManager::SortZ);

				}
				*/

				for (int j = 0; j < pSection->iSize; ++j)
				{
					// �浹ó���� �Ѵ�.
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
						// ó�� �浹�� ���
						if (!pCollSrc->CheckPrevCollision(pCollDest))
						{
							pCollSrc->AddPrevCollision(pCollDest);
							pCollDest->AddPrevCollision(pCollSrc);

							pCollSrc->OnCollisionEnter(pCollDest, fTime);
							pCollDest->OnCollisionEnter(pCollSrc, fTime);

							bMouseCollision = true;
						}

						// ������ �浹�ǰ� �־��� ���
						else
						{
							pCollSrc->OnCollision(pCollDest, fTime);
							pCollDest->OnCollision(pCollSrc, fTime);

							bMouseCollision = true;
						}
					}

					else
					{
						// �浹�ǰ� �ִٰ� ������ ���
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

	unordered_map<string, PCollisionGroup>::iterator iter;
	unordered_map<string, PCollisionGroup>::iterator iterEnd = m_mapGroup.end();

	for (iter = m_mapGroup.begin(); iter != iterEnd; ++iter)
	{
		// ���� ����ŭ �ݺ��Ѵ�.
		for (int i = 0; i < iter->second->iCount; ++i)
		{
			PCollisionSection	pSection = &iter->second->pSectionList[i];

			if (pSection->iSize < 2)
			{
				for (int j = 0; j < pSection->iSize; ++j)
				{
					pSection->pList[j]->CheckPrevCollisionInSection(fTime);
				}
				pSection->iSize = 0;
				continue;
			}

			for (int j = 0; j < pSection->iSize; ++j)
			{
				pSection->pList[j]->CheckPrevCollisionInSection(fTime);
			}

			// �� ������ �浹ü ����ŭ �ݺ��Ѵ�.
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

					if (pCollSrc->Collision(pCollDest, fTime))
					{
						// ó�� �浹�� ���
						if (!pCollSrc->CheckPrevCollision(pCollDest))
						{
							pCollSrc->AddPrevCollision(pCollDest);
							pCollDest->AddPrevCollision(pCollSrc);

							pCollSrc->OnCollisionEnter(pCollDest, fTime);
							pCollDest->OnCollisionEnter(pCollSrc, fTime);
						}

						// ������ �浹�ǰ� �־��� ���
						else
						{
							pCollSrc->OnCollision(pCollDest, fTime);
							pCollDest->OnCollision(pCollSrc, fTime);
						}
					}

					else
					{
						// �浹�ǰ� �ִٰ� ������ ���
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

			pSection->iSize = 0;
		}
	}
}

CCollisionManager::PCollisionGroup CCollisionManager::FindGroup(const string & strGroup)
{
	unordered_map<string, PCollisionGroup>::iterator iter = m_mapGroup.find(strGroup);

	if (iter == m_mapGroup.end())
		return nullptr;

	return iter->second;
}