#pragma once

#include "Component/UserComponent.h"
#include "Component/Collider.h"

/*
Door Ŭ������ ����Ϸ��� �ڵ��� ������

 - �Ϲ� ���� �޾� ���� ���ؼ��� ���� Y�� 90�� ȸ�����ѳ��ƾ� �Ѵ�

������ ��ġ�ϴ� �ڵ��̿�, ���� �����ǰ� �����̼� �����ϸ��� �����϶�.

�Ǻ��� �ڵ����� ����������,

�⺻���� �� �޽��� Wood_Door_Right_01.msh�� ����ϰ� ������

�����ϰ��� �ϴ� ��, �ٲ㵵 ����. ��, Right�� �Ǿ��ִ� �޽��� ����ؾ� �� ���̴�.

 - �������� ���� ��ġ�Ϸ��� �ڵ��̿�

SetDoorType(DOOR_STAGE) �Լ��� ȣ���϶�. �׸��� ���� ������������ ������ ������ ������ Z�������� ���߶�

���������� �ٲٱ� ���ؼ��� SetTargetDoor�Լ��� ȣ���ؾ� �� ���̴�.

�־������ ���ڷδ� Scene�� �̸�, ��� Door�� �̸�, �׸��� �ش� ���տ��� ������ ��ġ�� �ִ�.

�ش� �������� �۾��ڿ� �����Ͽ� �ۼ��ؾ��� ���̶�.

 - ��� ���� ��ġ�Ϸ��� �ڵ��̿�

 Lock�Լ��� ȣ���϶�

 ���ڷδ� ���谡 �־�� ���� �� �ִ� ���� ���� boolean���� ���� �̸��� �ִ�.

 ���� �ݴ����� ����ִ� ���� ��� ù��° ���ڸ� false�� �ΰ� string�� ���� �ʾƵ� �ȴ�.

 ************************ �߰� ���� *************************

 �������� �� ��ġ�� ��ġ�ϴ� �������� �������� ���� Z���� �� ���� ������ �������� �ٶ󺸰�

 ��ġ�ؾ��Ѵٴ� ���� ������ ����߽��ϴ�.

 �ٸ� ���� �� �Ű��� �� �κ��� �޽��� �ϳ��� ���ٺ��ϱ� �������� ���� ����

 �� �������� ������ �÷��̾� ���� ���ʿ� �־��µ� ���� ���������� �̵��ϸ�

 �� �����̰� �����ʿ� �޸�ä�� ������ ������ �־����ϴ�.

 �׷��� ���� ���ŷӴ��� �̵��ϴ� �� ���������� ������ ������ �����Ͽ�

 SetLeftRight�Լ��� ȣ�����ֽø� �����ϰڽ��ϴ�.

 SetLeftRight�Լ��� booleanŸ���� �Ű������� ����,

 ���������� ������ ������ �ٶ� �� �� �����̰� ���ʿ� ������ true

 �ݴ��� �������� ����� ���� false�� ����ֽø� �ǰڽ��ϴ�. 

 Ȥ�� �򰥸��� �����ø� ������ �������ֽø� ���� ��ġ�� �� ������

 ���� �˷��帮�ڽ��ϴ�.
 */

enum DOOR_STATE {

	DOOR_CLOSE = 1,
	DOOR_ONACT = 2,
	DOOR_OPEN = 4,
	DOOR_DESTROYED = 8

	//���� ������ ��� : DOOR_CLOSE
	//���� �ݴ� �� : DOOR_CLOSE | DOOR_ONACT
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
	float		m_fAutoCloseTime; // ���� �� �ڵ����� ���� ����?
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

	// ����
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
