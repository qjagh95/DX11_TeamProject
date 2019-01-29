#pragma once
#include "Engine.h"

PUN_BEGIN

class PUN_DLL CUIManager
{
private:
	std::vector<class CNumber*> m_vecNumber;
	std::vector<int> m_vecNumberValue;
	int  m_iNumber;
	bool m_bChangeNumber;
	int  m_iDigitCount;
public:
	void SetNumber(int _iNumber)
	{
		m_vecNumberValue.clear();
		m_bChangeNumber = true;
		m_iNumber = _iNumber;
	}

	void AddNumber()
	{
		m_bChangeNumber = true;
		++m_iNumber;
	}

	void ClearNumber()
	{
		m_bChangeNumber = true;
		m_iNumber = 0;
	}
public:
	bool Init();
	void Input(float _fTime);
	void ClearVecNumber();
	void AddNumber(int iNumber);

	DECLARE_SINGLE(CUIManager)
};

PUN_END