#pragma once

#include "Component/UserComponent.h"
#include "Component/Collider.h"

enum DOOR_STATE {

	DOOR_CLOSE = 1,
	DOOR_ONACT = 2,
	DOOR_OPEN = 4,
	DOOR_DESTROYED = 8

	//¹®ÀÌ ´ÝÇûÀ» °æ¿ì : DOOR_CLOSE
	//¹®À» ´Ý´Â Áß : DOOR_CLOSE | DOOR_ONACT
};

class CDoor : public PUN::CUserComponent
{
	friend class PUN::CGameObject;

protected:
	CDoor();
	CDoor(const CDoor& door);
	~CDoor();

private:
	int			m_iState;
	int			m_iDir;
	float		m_fOpenTime;
	float		m_fOpenSpeed;
	float		m_fFastOpenTime;
	float		m_fFastOpenSpeed;
	float		m_fDestroyBurstTime;
	float		m_fAccTime;
	float		m_fAccRot;
	float		m_fRot;
	bool		m_bLock;
	bool		m_bFastOpen;
	float		m_fOpenRot;
	float		m_fCloseRot;
	DOOR_TYPE	m_eDoorType;


	ptrdiff_t	m_pSndComp;

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AfterClone();

public:
	bool IsLock() const;
	void UnLock();

	int  GetState() const;
	void SetState(int state);

	float GetOpenTime() const;
	float GetDestroyTime() const;

	void SetOpenRot(float fRot = 90.0f);
	void SetOpenTime(float time);
	void SetCloseTime(float fTime);
	void SetFastOpenTime(float fTime);
	void SetDestroyTime(float time);

	bool SetSounds(const std::string strNames[], const TCHAR** strFilePaths);
	bool SetOpenSound(const std::string strKey);
	bool SetCloseSound(const std::string strKey);
	bool SetDestroySound(const std::string strKey);

	void OnAct(float fTime);
	void OnActNormal(float fTime);
	void OnActStage(float fTime);
	void OnActLocker(float fTime);
	void OnActHeavy(float fTime);

	void Open(const Vector3& vDir);
	void Close();

	void OpenNormal(const Vector3& vDir);
	void OpenStage(const Vector3& vDir);
	void OpenLocker(const Vector3& vDir);

	void SetMesh(const string& strMeshKey, const TCHAR* pFileName);

	DOOR_TYPE  GetDoorType() const;
	void SetDoorType(int iType);

	const bool IsOpened() const;
	const bool IsOnAction() const;

public:
	void Interact(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);

	CDoor* Clone();
};
