#include "EngineHeader.h"
#include "BinaryReader.h"
#include "PathManager.h"
#include <atlbase.h>

PUN_USING

BinaryRead::BinaryRead(const string& FileName)
{
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;

	m_ReadFile.open(Temp.c_str(), ios::binary);
}

BinaryRead::BinaryRead(const wstring& FileName)
{
	wstring Temp;
	Temp = CPathManager::GetInst()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;
	m_ReadFile.open(Temp.c_str());
}

BinaryRead::~BinaryRead()
{
	m_ReadFile.close();
}

bool BinaryRead::ReadBool()
{
	bool Return;

	m_ReadFile.read((char*)&Return, sizeof(bool));

	return Return;
}

int BinaryRead::ReadInt()
{
	int Temp = INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(int));

	if (Temp == INT_MIN)
		TrueAssert(true);

	return Temp;
}

float BinaryRead::ReadFloat()
{
	float Temp = (float)INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(float));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

double BinaryRead::ReadDouble()
{
	double Temp = (double)INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(double));

	if (Temp == (double)INT_MIN)
		TrueAssert(true);

	return Temp;
}
 
Vector2 BinaryRead::ReadVector2()
{
	Vector2 Temp = Vector2((float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector2));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

Vector3 BinaryRead::ReadVector3()
{
	Vector3 Temp = Vector3((float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector3));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

Vector4 BinaryRead::ReadVector4()
{
	Vector4 Temp = Vector4((float)INT_MIN, (float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector4));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

string BinaryRead::ReadString()
{
	char getString[255] = {};
	size_t Datalen;

	m_ReadFile.read((char*)&Datalen, sizeof(size_t));
	m_ReadFile.read(getString, Datalen);

	return string(getString);
}

wstring BinaryRead::ReadWString()
{
	char getString[255];
	size_t Datalen;

	m_ReadFile.read((char*)&Datalen, sizeof(size_t));
	m_ReadFile.read(getString, Datalen);

	return wstring(CA2W(getString));
}

void BinaryRead::ReadData(bool & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(bool));
}

void BinaryRead::ReadData(int & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(int));
}

void BinaryRead::ReadData(float & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(float));
}

void BinaryRead::ReadData(double & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(double));
}

void BinaryRead::ReadData(Vector2 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector2));
}

void BinaryRead::ReadData(Vector3 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector3));
}

void BinaryRead::ReadData(Vector4 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector4));
}

void BinaryRead::ReadData(string & Data)
{
	size_t DataLen;
	char getData[255] = {};

	m_ReadFile.read((char*)&DataLen, sizeof(size_t));
	m_ReadFile.read(getData, DataLen);

	Data = getData;
}

void BinaryRead::ReadData(wstring & Data)
{
	size_t DataLen;
	char getData[255] = {};

	m_ReadFile.read((char*)&DataLen, sizeof(size_t));
	m_ReadFile.read(getData, DataLen);

	Data = CA2W(getData);
}
