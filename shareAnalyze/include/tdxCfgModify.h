#pragma once
#include <vector>
#include <cstdio>
#include <cstring>

namespace TDXCFGMODIFY
{
	void tdxBlockAdd(std::string &date, std::vector<std::string> &resultSet);

	void tdxBlockCfgModify(std::string &date);

	void codeResultSave(std::vector<std::string> &resultSet);
}