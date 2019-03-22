#include "../EngineHeader.h"
#include "CSFilter.h"
#include "UAV.h"
#include "ViewManager.h"

PUN_USING

CCSFilter::CCSFilter()	:
	m_bFilterOn(true),
	m_pSrcSRV(nullptr)
{
	m_vecUAV.clear();
	m_eFilterType = CFT_END;
}

CCSFilter::~CCSFilter()
{
}

void CCSFilter::Enable()
{
	m_bFilterOn = true;
}

void CCSFilter::Disable()
{
	m_bFilterOn = false;
}

bool CCSFilter::AddUAV(const string & strUAVKey)
{
	CUAV* pUAV = GET_SINGLE(CViewManager)->FindUAV(strUAVKey);

	if (!pUAV)
		return false;

	m_vecUAV.push_back(pUAV);

	return true;
}

void CCSFilter::SetSourceSRV(ID3D11ShaderResourceView * pSrcSRV)
{
	m_pSrcSRV = pSrcSRV;
}

void CCSFilter::Dispatch()
{
	for (int i = 0;i < m_vecUAV.size(); ++i)
	{
		SetShaderResource(i);
		m_vecUAV[i]->Dispatch();
		ResetShaderResource(i);
	}
}

void CCSFilter::SetShaderResourceTo(int iRegister, int iIndex, SHADER_TYPE eType)
{
	m_vecUAV[iIndex]->SetSRV(iRegister, eType);
}

void CCSFilter::ResetShaderResourceFrom(int iRegister, int iIndex, SHADER_TYPE eType)
{
	m_vecUAV[iIndex]->ResetSRV(iRegister, eType);
}
