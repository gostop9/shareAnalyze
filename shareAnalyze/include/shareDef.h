#pragma once

#include <cstdio>

namespace SHAREDEF
{
	const int lineDataLen = 1000;
	static char lineDateBuf[lineDataLen];

    const float DANWEI_WAN = 10000.0;
    const float DANWEI_YI  = 100000000.0;

	const float WEIBI_MAX =  99.9999;
	const float WEIBI_MIN = -99.9999;

	const std::string NEW_SHARE = "新股";

	const float DIVIDE = 100000000; //“亿”为单位

	const std::string codeResult = "D:/share/shareAnalyze/codeResult.txt";
}