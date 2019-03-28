#pragma once

PUN_BEGIN

enum PLANE_DIR
{
	PD_LEFT,
	PD_RIGHT,
	PD_TOP,
	PD_BOTTOM,
	PD_NEAR,
	PD_FAR,
	PD_END
};

class PUN_DLL CFrustum
{
	friend class CCamera;

private:
	CFrustum();
	~CFrustum();

private:
	Vector3	m_vOriginPos[8];
	Vector3	m_vPos[8];
	Vector4	m_vPlane[PD_END];

public:
	void Update(Matrix matVP);
	bool FrustumInPoint(Vector3& vPos);
	bool FrustumInSphere(Vector3& vCenter, float fRadius);
};

PUN_END