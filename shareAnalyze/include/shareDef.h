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

	const float BI_LIUTONG = 100000.0;

	const std::string NEW_SHARE = "�¹�";

	const float DIVIDE = 100000000; //���ڡ�Ϊ��λ

	const float TENTHOUSAND = 10000; //����Ϊ��λ

	const std::string codeResult = "D:/share/shareAnalyze/codeResult.txt";

	const std::string continueLimitFileName = "D:/share/continueLimit.txt";

	const std::string ztProportionFileName = "D:/share/ztProportion.txt";

	const int CHAR_LEN = (30);
	static char nonChar[CHAR_LEN];
}