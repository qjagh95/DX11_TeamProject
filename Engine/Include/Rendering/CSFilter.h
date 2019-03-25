#pragma once

PUN_BEGIN

class CUAV;

class PUN_DLL CCSFilter
{
	friend class CRenderManager;

	friend class CViewManager;

protected:
	CCSFilter();

public:
	virtual ~CCSFilter();

protected:
	vector<CUAV*>				m_vecUAV;
	ID3D11ShaderResourceView*	m_pSrcSRV;
	CS_FILTER_TYPE				m_eFilterType;
	int							m_iFinalPassRegister;
	bool						m_bFilterOn;

public:
	bool GetEnable() const { return m_bFilterOn; }
	virtual void Enable();
	virtual void Disable();

public:
	virtual bool Init() = 0;
	virtual bool AddUAV(const string& strUAVKey);
	virtual void SetSourceSRV(ID3D11ShaderResourceView* pSrcSRV);
	virtual void Dispatch();
	virtual void SetShaderResourceTo	(int iRegister, int iIndex,  SHADER_TYPE eType = ST_PIXEL);
	virtual void ResetShaderResourceFrom(int iRegister, int iIndex,  SHADER_TYPE eType = ST_PIXEL);	
	virtual void SetShaderResourceTo() = 0;
	virtual void ResetShaderResourceFrom() = 0;

protected:	
	virtual void SetShaderResource(int iPass) {};
	virtual void ResetShaderResource(int iPass) {};
};

PUN_END