#pragma once
#include "../Ref.h"

PUN_BEGIN

class PUN_DLL CRenderState :
	public CRef
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState() = 0;

protected:
	ID3D11DeviceChild*	m_pState;
	ID3D11DeviceChild*	m_pOldState;
	RENDER_STATE		m_eState;

public:
	RENDER_STATE GetRenderState()	const;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

PUN_END