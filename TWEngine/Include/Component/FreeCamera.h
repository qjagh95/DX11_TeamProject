#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CFreeCamera :
	public CComponent
{
	friend class CGameObject;

protected:
	CFreeCamera();
	CFreeCamera(const CFreeCamera& camera);
	~CFreeCamera();

private:
	float				m_fSpeed;
	float				m_fRotSpeed;
	class CTransform*	m_pMouseTr;

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CFreeCamera* Clone();

	void SetMoveSpeed(float fSpeed);
	void SetRotSpeed(float fSpeed);

	void YRotation(float fScale, float fTime);
	void XRotation(float fScale, float fTime);
	void ZRotation(float fScale, float fTime);

	void XMove(float fScale, float fTime);
	void YMove(float fScale, float fTime);
	void ZMove(float fScale, float fTime);

	void RotationZero(float fTime);
	void PositionZero(float fTime);

	void RotationByMouse(float fTime);
};

PUN_END