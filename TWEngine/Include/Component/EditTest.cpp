#include "../EngineHeader.h"
#include "EditTest.h"

/*

 Tab ������ Scale, Position��ȯ
 ����Ű or wasd�� Rotation(X,Y Axis)
 X,Y Move
*/

PUN_USING

CEditTest::CEditTest()
{
}


CEditTest::CEditTest(const CEditTest & _EditTest)
	:CComponent(_EditTest)
{
}

CEditTest::~CEditTest()
{
}

bool CEditTest::Init()
{
	return true;
}

int CEditTest::Input(float fTime)
{
	return 0;
}

int CEditTest::Update(float fTime)
{
	return 0;
}

int CEditTest::LateUpdate(float fTime)
{
	return 0;
}

void CEditTest::Collision(float fTime)
{
}

void CEditTest::Render(float fTime)
{
}

CEditTest * CEditTest::Clone()
{
	return new CEditTest(*this);
}
