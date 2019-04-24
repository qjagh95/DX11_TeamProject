#pragma once


class CCommonSoundLoader {

public:
	static CCommonSoundLoader* m_pInst;

private:
	CCommonSoundLoader();
	~CCommonSoundLoader();

private:
	std::unordered_map<std::string, int> m_mapSoundRandomCnt;

public:
	static CCommonSoundLoader *GetInst();
	static void DestroyInst();

	bool LoadSoundCSVList(const TCHAR* pFilePath, std::string strPathKey = DATA_PATH);
	bool LoadSoundRandomSeedCnt(const TCHAR* pFilePath, std::string strPathKey = DATA_PATH);

	int GetSoundRandomCnt(const std::string& strTag) const;
};