#pragma once
#include <string>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace std;
using namespace msclr::interop;

namespace EditWrapper {
	public ref class WrapperManager
	{
	public:
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
		void TestFunction(String^ _str)
		{
			string strMarshal = marshal_as<string>(_str);
			if (strMarshal == "Button 2")
			{
				int breakPoint = 0;
			}
		}
	};
}