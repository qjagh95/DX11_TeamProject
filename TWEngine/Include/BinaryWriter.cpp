#include "EngineHeader.h"
#include "BinaryWriter.h"
#include "PathManager.h"

PUN_USING

BinaryWrite::BinaryWrite(const string& FileName)
{
	string Temp;
	Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	
	if (Temp.empty() == true)
		return;

	Temp += FileName;

	m_WriteFile = ofstream(Temp.c_str(), ios_base::binary);
}

BinaryWrite::BinaryWrite(const wstring& FileName)
{
	wstring Temp;
	Temp = CPathManager::GetInst()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;

	m_WriteFile.open(Temp.c_str(), ios::binary);
}

BinaryWrite::~BinaryWrite()
{
	m_WriteFile.close();
}

void BinaryWrite::WriteData(const bool& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(bool));
}

void BinaryWrite::WriteData(const int& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(int));
}

void BinaryWrite::WriteData(const float& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(float));
}

void BinaryWrite::WriteData(const double& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(double));
}

void BinaryWrite::WriteData(const Vector2 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector2));
}

void BinaryWrite::WriteData(const Vector3 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector3));
}

void BinaryWrite::WriteData(const Vector4 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector4));
}

void BinaryWrite::WriteData(const string & Data)
{
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data.c_str(), Datalen);
}

void BinaryWrite::WriteData(const wstring & Data)
{
	wstring Temp = Data.c_str();
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(CW2A(Temp.c_str()), Datalen);
}

void BinaryWrite::WriteData(const char * Data)
{
	string Temp = Data;
	size_t Datalen = Temp.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data, Datalen);
}

void BinaryWrite::WriteData(const wchar_t * Data)
{
	wstring Temp = Data;
	size_t Datalen = Temp.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(CW2A(Temp.c_str()), Datalen);
}
