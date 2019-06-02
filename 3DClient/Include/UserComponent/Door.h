#pragma once

#include "Component/UserComponent.h"
#include "Component/Collider.h"

/*
Door 클래스를 사용하려는 자들은 들으라

 - 일반 문을 달아 놓기 위해서는 로컬 Y를 90도 회전시켜놓아야 한다

툴에서 배치하는 자들이여, 월드 포지션과 로테이션 스케일만을 조정하라.

피봇은 자동으로 맞춰질지니,

기본으로 문 메쉬는 Wood_Door_Right_01.msh를 사용하고 있으니

수정하고자 하는 자, 바꿔도 좋다. 단, Right로 되어있는 메쉬를 사용해야 할 것이다.

 - 스테이지 문을 배치하려는 자들이여

SetDoorType(DOOR_STAGE) 함수를 호출하라. 그리고 현재 스테이지에서 밖으로 나가는 방향을 Z방향으로 맞추라

스테이지를 바꾸기 위해서는 SetTargetDoor함수를 호출해야 할 것이다.

넣어줘야할 인자로는 Scene의 이름, 대상 Door의 이름, 그리고 해당 문앞에서 등장할 위치가 있다.

해당 스테이지 작업자와 협의하여 작성해야할 것이라.

 - 잠긴 문을 배치하려는 자들이여

 Lock함수를 호출하라

 인자로는 열쇠가 있어야 열릴 수 있는 지에 대한 boolean값과 열쇠 이름이 있다.

 만약 반대편에서 잠겨있는 문의 경우 첫번째 인자를 false로 두고 string은 넣지 않아도 된다.
*/

enum DOOR_STATE {

	DOOR_CLOSE = 1,
	DOOR_ONACT = 2,
	DOOR_OPEN = 4,
	DOOR_DESTROYED = 8

	//문이 닫혔을 경우 : DOOR_CLOSE
	//문을 닫는 중 : DOOR_CLOSE | DOOR_ONACT
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

	void OpenFast(const Vector3& vDir);

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
