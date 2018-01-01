// 这是主 DLL 文件。

#include "CP16Library1.h"

#include <msclr\marshal_cppstd.h>  
using namespace System;

array<float>^ CP16Library1::CP16Library1::getSound(String ^ str)
{
	std::string tmpStr = msclr::interop::marshal_as<std::string>(str);
	cliext::vector<float> tmpVec = cp16(tmpStr);
	return tmpVec.to_array();
}
