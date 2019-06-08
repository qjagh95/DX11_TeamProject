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

 ************************ 추가 사항 *************************

 스테이지 문 배치는 배치하는 스테이지 기준으로 문의 Z축이 문 열고 나가는 방향으로 바라보게

 배치해야한다는 것은 위에서 언급했습니다.

 다만 조금 더 신경써야 할 부분이 메쉬를 하나로 쓰다보니까 스테이지 나갈 때는

 문 손잡이의 방향이 플레이어 기준 왼쪽에 있었는데 막상 스테이지를 이동하면

 문 손잡이가 오른쪽에 달린채로 열리는 문제가 있었습니다.

 그래서 조금 번거롭더라도 이동하는 쪽 스테이지를 구성한 팀원과 상의하여

 SetLeftRight함수를 호출해주시면 감사하겠습니다.

 SetLeftRight함수는 boolean타입의 매개변수가 들어가며,

 스테이지를 나가는 방향을 바라볼 때 문 손잡이가 왼쪽에 있으면 true

 반대편 스테이지 만드는 분은 false로 잡아주시면 되겠습니다. 

 혹시 헷갈린다 싶으시면 저에게 문의해주시면 제가 배치할 때 옆에서

 상세히 알려드리겠습니다.
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
	int			m_iLockDir;
	float		m_fOpenTime;
	float		m_fOpenSpeed;
	float		m_fFastOpenTime;
	float		m_fFastOpenSpeed;
	float		m_fDestroyBurstTime;
	float		m_fAccTime;
	float		m_fAutoCloseTime; // 몇초 후 자동으로 닫힐 건쥐?
	float		m_fAutoCloseTimeVar;
	float		m_fAccRot;
	float		m_fRot;
	bool		m_bLock;
	bool		m_bAutoClose;
	bool		m_bNeedKey;
	bool		m_bFastOpen;
	bool		m_bOpen;
	bool		m_bisPlayerColl;
	bool		m_bBroken;
	float		m_fOpenRot;
	float		m_fCloseRot;

	Vector3		m_vTeleportPos;
	DOOR_TYPE	m_eDoorType;
	string		m_strKeyName;
	string		m_strTargetDoorKey;
	string		m_strTargetSceneKey;
	ptrdiff_t	m_pSndComp;
	STAGE_SECTION_TYPE m_iSection;

	// 버모
	CRenderer*	m_Renderer;
	Vector3		m_WorldPos;
	Vector3		m_MeshLength;
	CColliderOBB3D* m_OBB;


public:
	Vector3		m_FrontCenter;
	Vector3		m_FrontCenterLeft;
	Vector3		m_FrontCenterRight;
	Vector3		m_FrontUpCenter;
	Vector3		m_FrontUpLeft;
	Vector3		m_FrontUpRight;
	Vector3		m_FrontDownCenter;
	Vector3		m_FrontDownLeft;
	Vector3		m_FrontDownRight;

	Vector3		m_CenterCenter;
	Vector3		m_CenterCenterLeft;
	Vector3		m_CenterCenterRight;
	Vector3		m_CenterUpCenter;
	Vector3		m_CenterUpLeft;
	Vector3		m_CenterUpRight;
	Vector3		m_CenterDownCenter;
	Vector3		m_CenterDownLeft;
	Vector3		m_CenterDownRight;

	Vector3		m_BackCenter;
	Vector3		m_BackCenterLeft;
	Vector3		m_BackCenterRight;
	Vector3		m_BackUpCenter;
	Vector3		m_BackUpCenterLeft;
	Vector3		m_BackUpCenterRight;
	Vector3		m_BackDownCenter;
	Vector3		m_BackDownCenterLeft;
	Vector3		m_BackDownCenterRight;

	Vector3 GetColliderCenter() const;
public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AfterClone();

public:
	bool IsLock()	const;
	bool IsBroken() const;
	void UnLock();
	void UnLock(const Vector3& vAxis);
	void Lock(bool bNeedKey = false, const string& strKeyName = "");
	void LockIn(int iDir);
	void Break();

	void SetStageSection(STAGE_SECTION_TYPE Section) { m_iSection = Section; }
	STAGE_SECTION_TYPE GetStageSection() const { return m_iSection; }
	void ChangeObjectSection(CGameObject* Object);

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

	void SetTargetDoor(const string& strSceneKey, const string& strDoorKey);
	void SetLeftRight(bool bLeft = false);

	const bool IsOpened() const;
	const bool IsOnAction() const;
	CSoundSource* Get3DSound() const { return (CSoundSource*)m_pSndComp; }

	bool GetIsPlayerColl() const { return m_bisPlayerColl; }
	bool GetIsAutoClose() const { return m_bAutoClose; };
	void SetAutoCloseTime(float Value) { m_bAutoClose = true, m_fAutoCloseTime = Value; }
	void AutoTimeClose(float fTime);

public:
	void Interact(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void InteractRelease(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);

	CDoor* Clone();

private:
	void Bumho();
};
