#include <vector>
#include "commonFun.h"
#include "shareDef.h"

using namespace std;
using namespace SHAREDEF;

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

void stringSplitMultiSymbol(std::string inputStr, std::string splitSymbol, std::vector<std::string> &outStrVec)
{
	int tableCount = 0;
	int iPos = inputStr.find(splitSymbol, 0);
	while (string::npos != iPos)
	{
		tableCount++;
		string strTemp = inputStr.substr(0, iPos);
		if (strTemp.size() > 0)
		{
			outStrVec.push_back(strTemp);
			tableCount = 0;
		}
		else if (tableCount >= 3)
		{
			strTemp = nonChar;
			outStrVec.push_back(strTemp);
			tableCount = 0;
		}
		inputStr = inputStr.substr(iPos + 1, inputStr.length() - iPos);

		iPos = inputStr.find("\t", 0);
	}
	// 最后一个字符串  
	outStrVec.push_back(inputStr);
}
}
