#pragma once

using namespace std;

PUN_BEGIN

class CScene;
class CGameObject;
class PUN_DLL CEditManager
{
private:
	CEditManager();
	~CEditManager();

private:
	static CEditManager* m_pInst;

public:
	static CEditManager* GetInst()
	{
		if (m_pInst == nullptr)
		{
			m_pInst = new CEditManager;
		}

		return m_pInst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}
private:
	CScene*		 m_pScene;
	CGameObject* m_pActiveObject;
	std::vector<std::string> m_vecstrObjList;
	class CAnimation*		 m_pAnimation;
public:
	bool Init();
	void CreateObject(const std::string& _strTag, const std::string& _strLayerTag = "Default");
	void ObjectAddComponent(std::vector<int>& _vecComType);
	void ObjectAddComponent(std::string& _strCompTag);
	void ActiveObjectSetTransform(Vector3 _vPos, Vector3 _vScale, Vector3 _vRot);
	void ActiveObjectSetPosition(double _dX, double _dY,  double _dZ);
	void ActiveObjectSetScale(double _dX, double _dY, double _dZ);
	void ActiveObjectSetRotation(double _dX, double _dY, double _dZ);

	void SetActiveObject(const std::string& _strObjTag , const std::string& _strLayerTag);
	void GetLayerListObjTag(const std::string& _strLayerTag);
	void LoadClipFromFullPath(const std::wstring& _strFullPath);
	int GetVecListObjSize() const
	{
		return (int)m_vecstrObjList.size();
	}
	std::string GetIndexFromObjTag(int _idx);
	void SetIndexFromSetObject(int _idx , const std::string& _strLayerTag);
	void AddRenderer(const std::string& _strTag);
	void ActiveObjectFromSetMesh(const std::string& _strMeshTag);
	void ActiveObjectFromSetTag(const std::string& _strObjTag);
	void GetClipNameList(std::vector<std::string>* _vecstrClipList);
// Renderer
public:
	vector<string>* GetMeshNameList();
	void AddComponentRenderer(string _strName);

};

PUN_END