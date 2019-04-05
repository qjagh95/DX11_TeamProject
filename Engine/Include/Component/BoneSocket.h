#pragma once

PUN_BEGIN

class CGameObject;
class PUN_DLL CBoneSocket
{
	friend class CAnimation;

private:
	CBoneSocket();
	~CBoneSocket();

private:
	string	m_strName;
	CGameObject*	m_pObject;
	Vector3	 m_vOffset;
	Vector3	 m_vRotation;

public:
	bool Init();
	void Update(float fTime, const Matrix& matBone);

	string GetName() const { return m_strName; }
	void SetName(const string& strName) { m_strName = strName; }
	void SetObject(class CGameObject* pObj);
	void SetOffset(const Vector3& vOffset);
	void SetOffset(float x, float y, float z);
	void SetRotation(const Vector3& vRot);
	void SetRotation(float x, float y, float z);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);
};

PUN_END