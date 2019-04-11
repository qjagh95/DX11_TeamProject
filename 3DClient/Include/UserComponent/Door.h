#pragma once

#include "Component/UserComponent.h"

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

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AfterClone();

public:
	void Open();
	void Close();
	void Destroy();

private:
	int m_iState;
	int m_iDoorType;
	float m_fOpenTime;
	float m_fCloseTime;
	float m_fDestroyBurstTime;
	float fCurrTimer;
	Vector3 m_vRotClosed;
	Vector3 m_vRotOpened;
	ptrdiff_t m_pSndComp;
	
public:
	int GetState() const;
	void SetState(int state);

	float GetOpenTime() const;
	float GetCloseTime() const;
	float GetDestroyTime() const;

	void SetOpenTime(float time);
	void SetCloseTime(float time);
	void SetDestroyTime(float time);

	bool SetSounds(const std::string strNames[], const TCHAR** strFilePaths);
	bool SetOpenSound(const std::string strKey);
	bool SetCloseSound(const std::string strKey);
	bool SetDestroySound(const std::string strKey);

	int GetDoorType() const;
	void SetDoorType(int iType);
	
	const bool IsOpened() const;
	const bool IsOnAction() const;

public:
	void Interact(float fTime);

	CDoor* Clone();
};
