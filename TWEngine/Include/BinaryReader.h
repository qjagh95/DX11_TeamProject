#pragma once

PUN_BEGIN

class PUN_DLL BinaryRead
{
public:
	bool ReadBool();
	int ReadInt();
	float ReadFloat();
	double ReadDouble();
	Vector2 ReadVector2();
	Vector3 ReadVector3();
	Vector4 ReadVector4();
	string ReadString();
	wstring ReadWString();

	void ReadData(bool& Data);
	void ReadData(int& Data);
	void ReadData(float& Data);
	void ReadData(double& Data);
	void ReadData(Vector2& Data);
	void ReadData(Vector3& Data);
	void ReadData(Vector4& Data);
	void ReadData(string& Data);
	void ReadData(std::string& _Data, int _iSize);
	void ReadData(wstring& Data);

	ifstream* BinaryRead::GetIfStream();

private:
	ifstream m_ReadFile;

public:
	BinaryRead(const string& FileName);
	BinaryRead(const wstring& FileName);
	BinaryRead(const wchar_t* FullPath);
	BinaryRead(const char* FullPath);
	~BinaryRead();
};

PUN_END

