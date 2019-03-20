#include "stdafx.h"
#include "EngineWrapper.h"

using namespace EngineWrapper;

void CoreWrapper::FileSave(String^ _strFullPath)
{
	/* Function Create KDG */

	std::string strFullPath = marshal_as<std::string>(_strFullPath);
	//PUN::CSceneManager::GetInst()->GetScene()->SaveFile(strFullPath);
}

void CoreWrapper::FileLoad(String^ _strFullPath)
{
	/* Function Create KDG */

	std::string strFullPath = marshal_as<std::string>(_strFullPath);
	//PUN::CSceneManager::GetInst()->GetScene()->LoadFile(strFullPath);
}