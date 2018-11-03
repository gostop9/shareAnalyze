#pragma once

#include <vector>

namespace commonFun
{

void stringSplit(std::string inputStr, std::string splitSymbol, std::vector<std::string> &outStrVec);
void stringSplitMultiSymbol(std::string inputStr, std::string splitSymbol, std::vector<std::string> &outStrVec);

}