#pragma once

PUN_BEGIN

class CUAV;

class PUN_DLL CCSFilter
{
	friend class CViewManager;

protected:
	CCSFilter();
	virtual ~CCSFilter();

protected:
	vector<CUAV*>	m_vecUAV;

	//CS_FILTER_TYPE		m_eFilterType;
	bool				m_bFilterOn;

public:
	bool GetEnable() const { return m_bFilterOn; }
	void Enable();
	void Disable();

public:
	virtual bool AddUAV(const string& strUAVKey);
	virtual void Dispatch();
	virtual void SetShaderResourceTo	(int iRegister, int iIndex,  SHADER_TYPE eType = ST_PIXEL);
	virtual void ResetShaderResourceFrom(int iRegister, int iIndex,  SHADER_TYPE eType = ST_PIXEL);

protected:
	virtual void SetShaderResource(int iPass) = 0;
	virtual void ResetShaderResource(int iPass) = 0;
};

PUN_END