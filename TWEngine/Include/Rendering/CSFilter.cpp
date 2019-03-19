#include "../EngineHeader.h"
#include "CSFilter.h"
#include "UAV.h"
#include "ViewManager.h"

PUN_USING

CCSFilter::CCSFilter()	:
	m_bFilterOn(true)
{
	m_vecUAV.clear();
	//m_eFilterType = CFT_END;
}

CCSFilter::~CCSFilter()
{
	Safe_Delete_VecList(m_vecUAV);
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
	/*CUAV* pUAV = GET_SINGLE(CViewManager)->FindUAV(strUAVKey);

	if (!pUAV)
		return false;

	m_vecUAV.push_back(pUAV);*/

	return true;
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
	//m_vecUAV[iIndex]->SetResource(iRegister, eType);
}

void CCSFilter::ResetShaderResourceFrom(int iRegister, int iIndex, SHADER_TYPE eType)
{
	//m_vecUAV[iIndex]->ResetResource(iRegister, eType);
}

