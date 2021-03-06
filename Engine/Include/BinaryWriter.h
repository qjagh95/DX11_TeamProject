#pragma once
PUN_BEGIN
class PUN_DLL BinaryWrite
{
public:
	void WriteData(const bool& Data);
	void WriteData(const int& Data);
	void WriteData(const float& Data);
	void WriteData(const double& Data);
	void WriteData(const Vector2& Data);
	void WriteData(const Vector3& Data);
	void WriteData(const Vector4& Data);
	void WriteData(const string& Data);
	void WriteData(const std::string& _Data, int _iSize);
	void WriteData(const wstring& Data);
	void WriteData(const char* Data);
	void WriteData(const wchar_t* Data);
private:
	ofstream m_WriteFile;

public:
	BinaryWrite(const string& FileName);
	BinaryWrite(const wstring& FileName);
	BinaryWrite(const wchar_t* FullPath);
	BinaryWrite(const char* FullPath);
	~BinaryWrite();
};

PUN_END
