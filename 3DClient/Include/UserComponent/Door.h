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
	bool		m_bNeedKey;
	bool		m_bFastOpen;
	bool		m_bOpen;
	float		m_fOpenRot;
	float		m_fCloseRot;
	Vector3		m_vTeleportPos;
	DOOR_TYPE	m_eDoorType;
	string		m_strKeyName;
	string		m_strTargetDoorKey;
	string		m_strTargetSceneKey;


	ptrdiff_t	m_pSndComp;

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AfterClone();

public:
	bool IsLock() const;
	void UnLock();
	void UnLock(const Vector3& vAxis);
	void Lock(bool bNeedKey = false, const string& strKeyName = "");

	bool IsOpenFinished() const;

	void SetFastOpen(bool bFastOpen);

	int  GetState() const;
	void SetState(int state);

	float GetOpenTime() const;
	float GetDestroyTime() const;

	void SetOpenRot(float fRot = 90.0f);
	void SetOpenTime(float time);
	void SetCloseTime(float fTime);
	void SetFastOpenTime(float fTime);
	void SetDestroyTime(float time);

	bool SetSounds(const string strNames[], const TCHAR** strFilePaths);
	bool SetOpenSound		(const string& strKey, const TCHAR* pFileName);
	bool SetCloseSound		(const string& strKey, const TCHAR* pFileName);
	bool SetFastOpenSound	(const string& strKey, const TCHAR* pFileName);
	bool SetLockSound		(const string& strKey, const TCHAR* pFileName);
	bool SetUnLockSound		(const string& strKey, const TCHAR* pFileName);

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
	void SetDoorTypeLocker();

	void SetTargetDoor(const string& strSceneKey, const string& strDoorKey, const Vector3& vPos);

	const bool IsOpened() const;
	const bool IsOnAction() const;

public:
	void Interact(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);

	CDoor* Clone();
};
