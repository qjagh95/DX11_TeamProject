#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CDecal :
	public CComponent
{
	friend class CGameObject;

protected:
	CDecal();
	CDecal(const CDecal& decal);
	~CDecal();
protected:
	std::string m_strDiffuseTex;
	std::string m_strNormalTex;
	std::string m_strSpecTex;
public:
	void SetDiffuseTexTag(const std::string& _strTag)
	{
		m_strDiffuseTex = _strTag;
	}
	void SetNormalTexTag(const std::string& _strTag)
	{
		m_strNormalTex = _strTag;
	}
	void SetSpecTexTag(const std::string& _strTag)
	{
		m_strSpecTex = _strTag;
	}
	const std::string& GetDiffuseTexTag() const
	{
		return m_strDiffuseTex;
	}
	const std::string& GetNormalTexTag() const
	{
		return m_strNormalTex;
	}
	const std::string& GetSpecTexTag() const
	{
		return m_strSpecTex;
	}


public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CDecal* Clone();
public:
	virtual void Save(BinaryWrite* _pInstBW);
	virtual void Load(BinaryRead*  _pInstBR);
};

PUN_END