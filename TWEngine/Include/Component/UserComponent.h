#pragma once
#include "Component.h"

PUN_BEGIN

class CAnimation;
class PUN_DLL CUserComponent : public CComponent
{
	friend class CGameObject;

public:
	virtual void Start() override;
	virtual void AfterClone() override;
	virtual bool Init() override;
	virtual int Input(float fTime) override;
	virtual int Update(float fTime) override;
	virtual int LateUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void Render(float fTime) override;
	virtual CUserComponent* Clone() = 0;

public:
	CUserComponent();
	CUserComponent(const CUserComponent& com);
	virtual ~CUserComponent() = 0;
};

PUN_END