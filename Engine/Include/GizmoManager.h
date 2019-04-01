#pragma once
PUN_BEGIN

class PUN_DLL CGizmoManager
{
private:
	static CGizmoManager* m_pInst;
private:
	class CGizmo* m_pXGizmo;
	class CGizmo* m_pYGizmo;
	class CGizmo* m_pZGizmo;
public:
	void GizmoRender(float fTime);
	void SetXGizmo(class CGizmo* _pGizmo);
	void SetYGizmo(class CGizmo* _pGizmo);
	void SetZGizmo(class CGizmo* _pGizmo);
public:
	static CGizmoManager* GetInst()
	{
		if (m_pInst == nullptr)
		{
			m_pInst = new CGizmoManager;
		}

		return m_pInst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}
private:
	CGizmoManager();
	~CGizmoManager();
};

PUN_END