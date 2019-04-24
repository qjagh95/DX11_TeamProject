#include "ClientHeader.h"
#include "CommonSoundLoader.h"
#include <PathManager.h>
#include <SoundManager.h>

CCommonSoundLoader * CCommonSoundLoader::m_pInst = nullptr;

CCommonSoundLoader::CCommonSoundLoader()
{
}

CCommonSoundLoader::~CCommonSoundLoader()
{
}

CCommonSoundLoader * CCommonSoundLoader::GetInst()
{
	if (!m_pInst)
		m_pInst = new CCommonSoundLoader;
	return m_pInst;
}

void CCommonSoundLoader::DestroyInst()
{
	delete m_pInst;
}

bool CCommonSoundLoader::LoadSoundCSVList(const TCHAR * pFilePath, std::string strPathKey)
{
	FILE *pFile = nullptr;
	std::wstring strPath = PUN::CPathManager::GetInst()->FindPath(DATA_PATH);
	strPath += pFilePath;

	_wfopen_s(&pFile, strPath.c_str(), TEXT("rt"));

	if (!pFile)
		return false;

	PUN::CSoundManager* _SMgr = PUN::CSoundManager::GetInst();

	char strBuf[1024] = {};
	char strDataBuf[512] = {};
	char strDataBuf_Key[512] = {};
	TCHAR strDataBuf_wchar[512] = {};

	int iBufIdx = 0;

	while (fgets(strBuf, 1024, pFile))
	{
		int iSteps = 0;
		iBufIdx = 0; //0번부터 쉼표 번호 이전까지
		int iLast = (int)strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if (strBuf[iLast + 1] == 0)
			strBuf[iLast + 1] = ',';

		for (int i = 0; i < strnlen_s(strBuf, 1024); ++i)
		{

			if (strBuf[i] == ',')
			{
				
				int iDataSize = i - iBufIdx; // +1 하면 쉼표까지 붙어버림
				if (iSteps == 0)
				{
					memcpy_s(strDataBuf_Key, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);
				}
					
				else
				{
					memcpy_s(strDataBuf, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);
					MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), strDataBuf_wchar, (int)strlen(strDataBuf));
					
					_SMgr->CreateSoundEffect(strDataBuf_Key, strDataBuf_wchar);

					memset(strDataBuf, 0, sizeof(char) * 512);
					memset(strDataBuf_Key, 0, sizeof(char) * 512);
					memset(strDataBuf_wchar, 0, sizeof(char) * 512);
					break;
				}

				++iSteps;
				iBufIdx = i + 1;
			}
		}
	}

	fclose(pFile);
	return true;
}

bool CCommonSoundLoader::LoadSoundRandomSeedCnt(const TCHAR * pFilePath, std::string strPathKey)
{
	FILE *pFile = nullptr;
	std::wstring strPath = PUN::CPathManager::GetInst()->FindPath(DATA_PATH);
	strPath += pFilePath;

	_wfopen_s(&pFile, strPath.c_str(), TEXT("rt"));

	if (!pFile)
		return false;

	PUN::CSoundManager* _SMgr = PUN::CSoundManager::GetInst();

	char strBuf[1024] = {};
	char strDataBuf[512] = {};
	char strDataBuf_Key[512] = {};
	
	int iBufIdx = 0;

	while (fgets(strBuf, 1024, pFile))
	{
		int iSteps = 0;
		iBufIdx = 0; //0번부터 쉼표 번호 이전까지
		int iLast = (int)strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if (strBuf[iLast + 1] == 0)
			strBuf[iLast + 1] = ',';

		for (int i = 0; i < strnlen_s(strBuf, 1024); ++i)
		{
			if (strBuf[i] == ',')
			{

				int iDataSize = i - iBufIdx; // +1 하면 쉼표까지 붙어버림
				if (iSteps == 0)
				{
					memcpy_s(strDataBuf_Key, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);
				}
				else
				{
					memcpy_s(strDataBuf, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);
					int iCnt = std::stoi(strDataBuf);

					if (iCnt > 1)
					{
						std::unordered_map<std::string, int>::const_iterator itr = m_mapSoundRandomCnt.find(strDataBuf_Key);
						std::unordered_map<std::string, int>::const_iterator itrEnd = m_mapSoundRandomCnt.end();
						if (itr == itrEnd)
							m_mapSoundRandomCnt.insert(std::make_pair(strDataBuf_Key, iCnt));
					}

					memset(strDataBuf, 0, sizeof(char) * 512);
					memset(strDataBuf_Key, 0, sizeof(char) * 512);
					break;
				}

				++iSteps;
				iBufIdx = i + 1;
			}
			
		}
	}

	fclose(pFile);

	return true;
}

int CCommonSoundLoader::GetSoundRandomCnt(const std::string & strTag) const
{
	std::unordered_map<std::string, int>::const_iterator itr = m_mapSoundRandomCnt.find(strTag);
	std::unordered_map<std::string, int>::const_iterator itrEnd = m_mapSoundRandomCnt.end();
	if (itr == itrEnd)
		return 1;


	return itr->second;
}
