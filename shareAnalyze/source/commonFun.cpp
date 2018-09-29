#include <vector>
#include "commonFun.h"

using namespace std;
namespace commonFun
{
void stringSplit(std::string inputStr, std::string splitSymbol, std::vector<std::string> &outStrVec)
{
	outStrVec.clear();
	int iPos = inputStr.find(splitSymbol, 0);
	while (string::npos != iPos)
	{
		string strTemp = inputStr.substr(0, iPos);
		if (strTemp.size() > 0)
		{
			outStrVec.push_back(strTemp);
		}
		inputStr = inputStr.substr(iPos + 1, inputStr.length() - iPos);
		iPos = inputStr.find(splitSymbol, 0);
	}
	// 最后一个字符串
	outStrVec.push_back(inputStr);
}
}
