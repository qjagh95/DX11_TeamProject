#pragma once

#include <Component/UserComponent.h>
#include <Component/ColliderSphere.h>
#include <Component/ColliderRay.h>

class CInteractiveObj: public PUN::CUserComponent
{
	friend class PUN::CGameObject;
	
	protected:
		CInteractiveObj();
		CInteractiveObj(const CInteractiveObj& obj);
		virtual ~CInteractiveObj();
		
	public:
		virtual bool Init();
		virtual int Update(float fTime);
		virtual int LateUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void Render(float fTime);
		virtual void AfterClone();
		virtual CInteractiveObj *Clone() = 0;
	
	protected:
		Vector3 m_vInteractOffset;
		float m_fInteractAreaSize;
		bool m_bActivated;
		PUN::CCollider *m_pInteractiveCol;
		

	public:
		virtual bool Activate(bool bSwitch);
		bool IsActivated() const;
		
		const Vector3& GetInteractOffset() const;
		void SetInteractOffset(const Vector3& offset);
};