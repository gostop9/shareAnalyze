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

	const std::string NEW_SHARE = "新股";
	const std::string NEW_SHARE_ON = "新股上市";
	const std::string PRE_NEW_SHARE = "次新";
	const std::string PRO_NEW_SHARE = "次新股";

	const float DIVIDE = 100000000; //“亿”为单位

	const float TENTHOUSAND = 10000; //“万”为单位

	const int MAX_SHARE_NUM = 4000;

	const std::string codeResult = "D:/share/shareAnalyze/codeResult.txt";

	const std::string continueLimitFileName = "D:/share/continueLimit.txt";

	const std::string ztProportionFileName = "D:/share/ztProportion.txt";

	const std::string zxgTdxFileName = "D:/new_jyplug/T0002/blocknew/ZXG.blk";
	const std::string tjxgTdxFileName = "D:/new_jyplug/T0002/blocknew/TJXG.blk";

	const float kaiPanZfMax = 1.2;
	const float kaiPanZfMin = -6.9;
	const float zhangFuRoundMin = 0.0019;
	const int MAX_ZIJIN_IDX = 37;

	const int CHAR_LEN = (30);
	static char nonChar[CHAR_LEN];
}