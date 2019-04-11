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
	float		m_fSpeed;

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CFreeCamera* Clone();
	void Move(float fScale, float fTime);
	void Rotation(float fScale, float fTime);
	void XRotation(float fScale, float fTime);
	void ZRotation(float fScale, float fTime);

	void XMove(float fScale, float fTime);
	void YMove(float fScale, float fTime);
	void ZMove(float fScale, float fTime);

	void RotationZero(float fScale, float fTime);
	void PositionZero(float fScale, float fTime);
};

PUN_END