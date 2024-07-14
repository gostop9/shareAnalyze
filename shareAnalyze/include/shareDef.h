#pragma once

#include <cstdio>
#include <cstring>
#include <string>

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

	const float DIVIDE = 100000000.0; //“亿”为单位

	const float TENTHOUSAND = 10000.0; //“万”为单位

	const float ONE_HUNDRED = 100.0; //“百”为单位

	const float ONE_MILLION = 1000000; //“百万”为单位

	const int MAX_SHARE_NUM = 6000;

	const std::string codeResult = "D:/share/shareAnalyze/codeResult.txt";

	const std::string continueLimitFileName = "D:/share/continueLimit.txt";

	const std::string ztProportionFileName = "D:/share/ztProportion.txt";

	const std::string zxgTdxFileName = "F:/new_tdx/T0002/blocknew/ZXG.blk";
	const std::string tjxgTdxFileName = "F:/new_tdx/T0002/blocknew/TJXG.blk";
	const std::string bkjeTdxFileName = "F:/new_tdx/T0002/blocknew/BKJE.blk";
	const std::string zpjjTdxFileName = "F:/new_tdx/T0002/blocknew/ZPJJ.blk";
	const std::string wbgkTdxFileName = "F:/new_tdx/T0002/blocknew/WBGK.blk";
	const std::string cybzpjjTdxFileName = "F:/new_tdx/T0002/blocknew/CYBZPJJ.blk";
	const std::string ztpremeetTdxFileName = "F:/new_tdx/T0002/blocknew/ZTPREMEET.blk";

	const std::string zxgTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/ZXG.blk";
	const std::string tjxgTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/TJXG.blk";
	const std::string bkjeTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/BKJE.blk";
	const std::string zpjjTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/ZPJJ.blk";
	const std::string wbgkTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/WBGK.blk";
	const std::string cybzpjjTdxFileName_multi = "D:/zd_cjzq/T0002/blocknew/CYBZPJJ.blk";

	const std::string zxgTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/ZXG.blk";
	const std::string tjxgTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/TJXG.blk";
	const std::string bkjeTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/BKJE.blk";
	const std::string zpjjTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/ZPJJ.blk";
	const std::string wbgkTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/WBGK.blk";
	const std::string cybzpjjTdxFileName_hongta = "D:/Doc/Stock/TDX_KXG/T0002/blocknew/CYBZPJJ.blk";

	const float kaiPanZfMax = 7.99; //5.99; // 1.2, //3.99
	const float kaiPanZfMin = -6.9;
	const float zhangFuRoundMin = 0.0019;
	const int MAX_ZIJIN_IDX = 257;//57; //37
	const float zuoRiKaiPanZfMinCeil = 8.99;

	const int CHAR_LEN = (30);
	static char nonChar[CHAR_LEN];

	const float weiBiThreshold = -79.0; //-68.0; // -78.0;
	const float dianDanThreshold = 1450.0; //1500.0; //520.0;//580.0
	const float dianDanBigThreshold = 7000.0;
	const float yiZiBanDianDanBigThreshold = 1400.0;
	const float zhangTingBanThreshold = -700; //-1000.0;
	const float zongLiuRuThreshold0 = 0.001754;//0.001754;
	const float zongLiuRuThreshold1 = 0.002417;//0.002794;
	const float zongLiuRuThreshold_for_yizi = 0.0024;//0.002794;
	const float danRiJingJiaThreshould = 2999.99; //2999.99
	const float jingJiaJinEChaThreshould = 1000.0;
	const float jingJiaJinEThreshould = 9099.0 * TENTHOUSAND; //9707.0  9999.0
	const float zuoRiLimitUpMoney = 1000.0 * TENTHOUSAND;
	const float jinRiJingJiaLimitUpMoney = 100.0;
	const float jinRiJingJiaLimitUpMoneyWithLiuTong = 100.0;
	const float jingJiaChaThreshould = -8.39;//2.39;
	const float zhenFuChaThreshould = 5.8;
	const float zuoRiKaiPanZfMax = 4.0;
	const float kaiPanZfBigDealThreshould = -2;//0.5;
	const int LIMIT_REASON_SIZE = 250;
	const int limitOpenThreshould = 20;
	const int lastLimitTimeThreshould = 155555;//144444;
	//const std::string ZTFPfileName = "D:/share/ZTFP.xlsx";
}