#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include "getLimitUpInfo.h"
#include "shareDef.h"
#include "commonFun.h"
#include "dataStructure.h"
#include "getShareFlag.h"
#include "shareSelect.h"
#include "autoShareBuy.h"
#include "excelRdWt.h"
#include <BasicExcel.hpp>
using namespace YExcel;

using namespace std;
using namespace SHAREDEF;
using namespace commonFun;
using namespace SHARE_FLAG;
using namespace AUTOSHAREBUY;
using namespace EXCELREADWRITE;

namespace GETLIMITUPINFO
{
	void limitUpReason_t::jingBiConclude(PROPERTY_t &propertyAnaly, std::vector<float> &marginVec, std::vector<float> &jingBi)
	{
		int secNum = marginVec.size() + 1;
		float jingJiaLiangBi = propertyAnaly.jingJiaLiangBi;
		if (jingJiaLiangBi < marginVec[0])
		{
			jingBi[0] += 1.0;
		}
		if (jingJiaLiangBi >= marginVec[secNum-2])
		{
			jingBi[secNum-1] += 1.0;
		}
		for (int i = 1; i < (secNum-1); i++)
		{
			if((jingJiaLiangBi >= marginVec[i-1]) && (jingJiaLiangBi < marginVec[i]))
			{
				jingBi[i] += 1.0;
			}
		}		
	}

	void limitUpReason_t::continueLimitSave(std::vector<PROPERTY_t > propertyAnalyVec, std::vector<analyseCode_t> limitTodayVec)
	{
		vector<float> marginVec = { 7.0, 10.0, 15.0, 20.0, 30.0, 50.0 };
		int secNum = marginVec.size() + 1;
		vector<float> jingBi;
		vector<float> jingBiZt;
		jingBi.resize(secNum);
		jingBiZt.resize(secNum);
		for (int i = 0; i < secNum; i++)
		{
			jingBi[i] = 0.0;
			jingBiZt[i] = 0.0;
		}
		//continue limit result file
		FILE *continueFp;
		continueFp = fopen(continueLimitFileName.c_str(), "at+");
		if (NULL == continueFp)
		{
			printf("Open continue limit result file failed!\n");
			return;
		}

		FILE *ztProportionFp;
		ztProportionFp = fopen(ztProportionFileName.c_str(), "at+");
		if (NULL == ztProportionFp)
		{
			printf("Open ztProportionFp file failed!\n");
			return;
		}

		int analyNum = propertyAnalyVec.size();
		int limitTodayNum = limitTodayVec.size();
		PROPERTY_t proTemp;
		vector<PROPERTY_t > propertyAnalyVecPre;
		propertyAnalyVecPre.push_back(proTemp);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVec[i];
			if (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
			{
				jingBiConclude(analyProty, marginVec, jingBi);
				for (int j = 0; j < limitTodayNum; j++)
				{
					if (0 == strcmp(analyProty.code, limitTodayVec[j].code))
					{
						jingBiConclude(analyProty, marginVec, jingBiZt);
						shareSelectPrint(continueFp, analyProty, propertyAnalyVecPre);
					}
				}
			}
		}

		
		for (int i = 0; i < secNum; i++)
		{
			fprintf(ztProportionFp, "%f, %f, ", jingBi[i], jingBiZt[i]);
		}
		fprintf(ztProportionFp, "\n");
	}

	void limitUpReason_t::limitShareSummary(std::string &ztStr, PROPERTY_t &analyProty, std::vector<limitUpReason_t> &ztrVec)
	{
		vector<limitUpReason_t>::iterator ztrIter = ztrVec.begin();
		while (ztrIter != ztrVec.end())
		{
			if (ztrIter->reason == ztStr)
			{
				ztrIter->ztCount++;
				ztrIter->zhangFuZong += analyProty.zhangFu;
				ztrIter->zhangFuMax = max(analyProty.zhangFu, ztrIter->zhangFuMax);
				ztrIter->zongShiZhi += analyProty.ziYouLiuTongShiZhi;
				ztrIter->danWeiShiZhi += (analyProty.ziYouLiuTongShiZhi / analyProty.zhangFu * 100);
				ztrIter->jingJiaLiangBiZong += analyProty.jingJiaLiangBi;
				ztrIter->jingLiuRuBiLiuTongZong += analyProty.jingLiuRuBiLiuTong;
				ztrIter->zongLiuRuBiLiuTongZong += analyProty.zongLiuRuBiLiuTong;
				ztrIter->shiZhiXzhangFuZong += (analyProty.ziYouLiuTongShiZhi/(100 + analyProty.zhangFu) * analyProty.zhangFu);
				ztrIter->zongJinEXZong += analyProty.zongJinE;
				ztrIter->zongLimitVsCirculateJingJia += analyProty.limitVsCirculateJingJia;
				//if (ztrIter->zhangFu < analyProty.zhangFu)
				{
					//limitUpInfo_t &ztSort = (limitUpInfo_t &)analyProty;
					limitUpInfo_t ztSort(analyProty);
					//ztSort.code = analyProty.code;
					//ztSort.name = analyProty.name;
					/*strcpy(ztSort.code, analyProty.code);
					strcpy(ztSort.name, analyProty.name);

					ztSort.weiBi = analyProty.weiBi;
					ztSort.weiCha = analyProty.weiCha;
					ztSort.zuoShou = analyProty.zuoShou;
					ztSort.buyLiang = analyProty.buyLiang;
					ztSort.xianJia = analyProty.xianJia;

					ztSort.waiPan = analyProty.waiPan;
					ztSort.zhangFu = analyProty.zhangFu;
					ztSort.jingJiaLiangBi = analyProty.jingJiaLiangBi;
					ztSort.zuoRiHuanShou = analyProty.zuoRiHuanShou;
					ztSort.zuoRiKaiPan = analyProty.zuoRiKaiPan;
					ztSort.zuoRiZuiGao = analyProty.zuoRiZuiGao;
					ztSort.zuoRiLiangBi = analyProty.zuoRiLiangBi;
					ztSort.zuoRiZhenFu = analyProty.zuoRiZhenFu;
					ztSort.continueDay = analyProty.continueDay;
					ztSort.ziYouLiuTongShiZhi = analyProty.ziYouLiuTongShiZhi;
					ztSort.zhongDanJinBiLiuTong = analyProty.zhongDanJinBiLiuTong;
					ztSort.zhongXiaoDanJinBiLiuTong = analyProty.zhongXiaoDanJinBiLiuTong;
					//ztSort.lastLimitTime = analyProty.lastLimitTime;
					strcpy(ztSort.lastLimitTime, analyProty.lastLimitTime);
					ztSort.limitOpenCount = analyProty.limitOpenCount;
					ztSort.indexLvsC = analyProty.indexLvsC;
					ztSort.limitUpMoney = analyProty.limitUpMoney;*/

					ztrIter->limitInfo.insert(ztSort);
				}
				break;
			}
			++ztrIter;
		}
		if (ztrIter == ztrVec.end()) //not found
		{
			//limitUpInfo_t &ztSort = (limitUpInfo_t &)analyProty;
			limitUpInfo_t ztSort(analyProty);
			//ztSort.code = analyProty.code;
			//ztSort.name = analyProty.name;
			/*strcpy(ztSort.code, analyProty.code);
			strcpy(ztSort.name, analyProty.name);

			ztSort.weiBi = analyProty.weiBi;
			ztSort.weiCha = analyProty.weiCha;
			ztSort.zuoShou = analyProty.zuoShou;
			ztSort.buyLiang = analyProty.buyLiang;
			ztSort.xianJia = analyProty.xianJia;

			ztSort.waiPan = analyProty.waiPan;
			ztSort.zhangFu = analyProty.zhangFu;
			ztSort.jingJiaLiangBi = analyProty.jingJiaLiangBi;
			ztSort.zuoRiHuanShou = analyProty.zuoRiHuanShou;
			ztSort.zuoRiKaiPan = analyProty.zuoRiKaiPan;
			ztSort.zuoRiZuiGao = analyProty.zuoRiZuiGao;
			ztSort.zuoRiLiangBi = analyProty.zuoRiLiangBi;
			ztSort.zuoRiZhenFu = analyProty.zuoRiZhenFu;
			ztSort.continueDay = analyProty.continueDay;
			ztSort.ziYouLiuTongShiZhi = analyProty.ziYouLiuTongShiZhi;
			ztSort.zhongDanJinBiLiuTong = analyProty.zhongDanJinBiLiuTong;
			ztSort.zhongXiaoDanJinBiLiuTong = analyProty.zhongXiaoDanJinBiLiuTong;
			//ztSort.lastLimitTime = analyProty.lastLimitTime;
			strcpy(ztSort.lastLimitTime, analyProty.lastLimitTime);
			ztSort.limitOpenCount = analyProty.limitOpenCount;
			ztSort.indexLvsC = analyProty.indexLvsC;
			ztSort.limitUpMoney = analyProty.limitUpMoney;*/

			limitUpReason_t ztrTemp;
			ztrTemp.limitInfo.insert(ztSort);
			ztrTemp.ztCount = 1;
			ztrTemp.reason = ztStr;
			ztrTemp.zhangFuZong = analyProty.zhangFu;
			ztrTemp.zhangFuMax = analyProty.zhangFu;
			ztrTemp.zongShiZhi = analyProty.ziYouLiuTongShiZhi;
			ztrTemp.danWeiShiZhi = analyProty.ziYouLiuTongShiZhi / analyProty.zhangFu;
			ztrTemp.jingJiaLiangBiZong = analyProty.jingJiaLiangBi;
			ztrTemp.jingLiuRuBiLiuTongZong = analyProty.jingLiuRuBiLiuTong;
			ztrTemp.zongLiuRuBiLiuTongZong = analyProty.zongLiuRuBiLiuTong;
			ztrTemp.shiZhiXzhangFuZong = (analyProty.ziYouLiuTongShiZhi / (100 + analyProty.zhangFu) * analyProty.zhangFu);
			ztrTemp.zongJinEXZong = analyProty.zongJinE;
			ztrTemp.zongLimitVsCirculateJingJia = analyProty.limitVsCirculateJingJia;
			ztrVec.push_back(ztrTemp);
		}
	}


	void limitUpReason_t::getLimitUpReason(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec)
	{
		int analyNum = propertyAnalyVec.size();
		newShareCodeVec.clear();
		newShareCodeVec.reserve(analyNum);
		//涨停原因
		ztrVec.clear();
		ztrVec.reserve(analyNum);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVec[i];

			string name = analyProty.name;
			std::size_t found = name.find("ST");
			if (
				(found != std::string::npos)//剔除ST股
				)
			{
				continue;
			}

			string ztReason = analyProty.limitReason;
			vector<string> strVec;
			stringSplit(ztReason, "+", strVec);
			int strNum = strVec.size();
			for (int j = 0; j < strNum; j++)
			{
				string &strTemp = strVec[j];
				limitShareSummary(strTemp, analyProty, ztrVec);

				//保存新股信息
				if (strTemp == NEW_SHARE)
				{
					string newCode = analyProty.code;
					newShareCodeVec.push_back(newCode);
				}
			}
		}
	}

	void limitUpReason_t::getLimitUpHy(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec)
	{
		int analyNum = propertyAnalyVec.size();
		int newCodeNum = newShareCodeVec.size();
		//行业
		vector<limitUpReason_t> hyVec;
		hyVec.clear();
		hyVec.reserve(analyNum);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVec[i];
			string suoShuHangYe = analyProty.suoShuHangYe;

			int newCodeFlag = 0;
			string shareCode = analyProty.code;
			//不统计新股
			for (int j = 0; j < newCodeNum; j++)
			{
				if (shareCode == newShareCodeVec[j])//新股
				{
					newCodeFlag = 1;
					break;
				}
			}
			if (1 == newCodeFlag)
			{
				continue;
			}

			limitShareSummary(suoShuHangYe, analyProty, ztrVec);
		}
	}

	void limitUpReason_t::sortByZhangFuZongLimitVsCirculateJingJiaZong(std::vector<limitUpReason_t> &ztrVec)
	{
		// sort for zhangFuAvg
		struct zhangFuAvg {
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zhangFuZong / a.ztCount > b.zhangFuZong / b.ztCount); }
		} cmpMethod_zhangFuAvg;
		std::stable_sort(ztrVec.begin(), ztrVec.end(), cmpMethod_zhangFuAvg);

		// sort for zhangFuZong
		struct zhangFuZong {
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zhangFuMax > b.zhangFuMax); }
		} cmpMethod_zhangFuZong;
		std::stable_sort(ztrVec.begin(), ztrVec.end(), cmpMethod_zhangFuZong);

		// sort for LimitVsCirculateJingJiaZong
		struct LimitVsCirculateJingJiaZong {
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zongLimitVsCirculateJingJia > b.zongLimitVsCirculateJingJia); }
		} cmpMethod_LimitVsCirculateJingJiaZong;
		std::stable_sort(ztrVec.begin(), ztrVec.end(), cmpMethod_LimitVsCirculateJingJiaZong);
	}

	void limitUpReason_t::limitShareSort(FILE *fp, ExcelRwC &excelReadWrite, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &resultSetBlock, int fileIndex, std::vector<PROPERTY_t> &propertyAnalyVecBlock)
	{
		resultSetBlock.clear();
		int ztrNum = ztrVec.size();
		//按平均涨幅排序，用于查看板块强度
		{
			//zfAvg_t zfAvg;
			//sort(ztrVec.begin(), ztrVec.end(), zfAvg);

			sortByZhangFuZongLimitVsCirculateJingJiaZong(ztrVec);
			int zfAvgOder = 1;
			for (int i = 0; i < ztrNum; i++)
			{
				limitUpReason_t &ztrTemp = ztrVec[i];
				ztrTemp.zfAvgOder = 0;
				if ((ztrTemp.ztCount > 1) && (ztrTemp.reason != NEW_SHARE))
				{
					ztrTemp.zfAvgOder = zfAvgOder++;
				}
			}
		}
		//按涨停原因出现次数排序
		//ztrCount_t ztrCount;
		//sort(ztrVec.begin(), ztrVec.end(), ztrCount);

		for (int i = 0; i < ztrNum; i++)
		{
			limitUpReason_t &ztrTemp = ztrVec[i];
			int bkShareNum = ztrTemp.limitInfo.size();
			std::vector<PROPERTY_t> propertyAnalyVec;
			propertyAnalyVec.reserve(bkShareNum);

			set<limitUpInfo_t>::iterator setIter = ztrTemp.limitInfo.begin();
			for (int j = 0; j < bkShareNum; j++)
			{
				propertyAnalyVec.push_back(*setIter++);
			}

			if (1 == fileIndex)
			{
				sortByZhangfuLimitVsDealJingJiaGaodu(propertyAnalyVec);
				excelReadWrite.writeExcelSheet(propertyAnalyVec);
			}
			else
			{
				sortByFirstLimitTimeGaodu(propertyAnalyVec);
			}
			//sortByLimitVsCirculateGaodu(propertyAnalyVec);

			if (NEW_SHARE != ztrTemp.reason)
			{
				//sortByZhangfuLimitVsDealJingJiaGaodu((vector<PROPERTY_t>)ztrTemp);

				float zhangFuAvg = ztrTemp.zhangFuZong / float(ztrTemp.ztCount);
				set<limitUpInfo_t>::iterator setIter = ztrTemp.limitInfo.begin();
				int shareCount = ztrTemp.limitInfo.size();
				int maxDisplay = 5;
				maxDisplay = shareCount < maxDisplay ? shareCount : maxDisplay;

				maxDisplay = shareCount;

				//float zhanLiuBi = 0;
				//float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);
				/*string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));				
				fprintf(fp, "%2d  %-8s  %6.2f, %4s%-8s  %6.2f | 昨停:%5.0f,封:%3d, %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%9.2f, 资金:%4d,  | %5.2f : %d  %s: \n",
					ztrTemp.ztCount, setIter->code, setIter->xianJia, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->limitUpMoney/TENTHOUSAND, setIter->indexLvsC, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, setIter->zhangTingBan, setIter->zijinIdx, zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());
				*/
				//fprintf(fp, "| %5.2f : %d  %s: \n", zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());

				fprintf(fp, "|%-16s: , 涨幅：%6.2f， 板块总成交金额：%6.2f亿\n", ztrTemp.reason.c_str(), ztrTemp.zhangFuZong, (ztrTemp.zongJinEXZong / (DIVIDE)));

				for (int j = 0; j < maxDisplay; j++)
				{
					/*++setIter;

					//float zhanLiuBi = 0;
					//float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);

					string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));					
					fprintf(fp, "    %-8s  %6.2f, %4s%-8s  %6.2f | 昨停:%5.0f,封:%3d, %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%9.2f, 资金:%4d\n",
						setIter->code, setIter->xianJia, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->limitUpMoney/TENTHOUSAND, setIter->indexLvsC, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, setIter->zhangTingBan, setIter->zijinIdx);
					*/
					PROPERTY_t propertyAnaly = propertyAnalyVec[j];
					string name = propertyAnaly.name;
					std::size_t found = name.find("ST");
					if (
						(found != std::string::npos)//剔除ST股
						//|| (propertyAnaly.guXingPingFen < 45.0)
						)
					{
						continue;
					}

					vector<PROPERTY_t> propertyAnalyVecPre;
					shareSelectPrint(fp, propertyAnaly, propertyAnalyVecPre);
					propertyAnalyVecBlock.push_back(propertyAnaly);

					resultSetBlock.push_back(propertyAnaly.code);
				}
			}
		}
	}

	void limitUpReason_t::limitShareOrdering(std::vector<PROPERTY_t> &propertyAnalyVec)
	{
	/*{
		// sort for xianJiaIndex
		struct xianJiaIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.xianJia <= b.xianJia); }
		} cmpMethod_xianJiaIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_xianJiaIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->xianJiaIndex = index++;
			++proIter;
		}
	}*/
	/*{
		// sort for continueDayIndex
		struct continueDayIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.continueDay < b.continueDay); }
		} cmpMethod_continueDayIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_continueDayIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->continueDayIndex = index++;
			++proIter;
		}
	}*/
	{
		// sort for limitUpMoneyIndex
		struct limitUpMoneyIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.limitUpMoney > b.limitUpMoney); }
		} cmpMethod_limitUpMoneyIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_limitUpMoneyIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->limitUpMoneyIndex = index++;
			++proIter;
		}
	}
	{
		// sort for xiaoDanLiuRuBiLiuTongIndex
		struct xiaoDanLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.xiaoDanLiuRuBiLiuTong > b.xiaoDanLiuRuBiLiuTong); }
		} cmpMethod_xiaoDanLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_xiaoDanLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->xiaoDanLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhongDanLiuRuBiLiuTongIndex
		struct zhongDanLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongDanLiuRuBiLiuTong > b.zhongDanLiuRuBiLiuTong); }
		} cmpMethod_zhongDanLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhongDanLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhongDanLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhongDanLiuChuBiLiuTongIndex
		struct zhongDanLiuChuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongDanLiuChuBiLiuTong > b.zhongDanLiuChuBiLiuTong); }
		} cmpMethod_zhongDanLiuChuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhongDanLiuChuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhongDanLiuChuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhongXiaoLiuRuBiLiuTongIndex
		struct zhongXiaoLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongXiaoLiuRuBiLiuTong > b.zhongXiaoLiuRuBiLiuTong); }
		} cmpMethod_zhongXiaoLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhongXiaoLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhongXiaoLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for xiaoDanJinBiLiuTongIndex
		struct xiaoDanJinBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.xiaoDanJinBiLiuTong < b.xiaoDanJinBiLiuTong); }
		} cmpMethod_xiaoDanJinBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_xiaoDanJinBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->xiaoDanJinBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhongDanJinBiLiuTongIndex
		struct zhongDanJinBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongDanJinBiLiuTong < b.zhongDanJinBiLiuTong); }
		} cmpMethod_zhongDanJinBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhongDanJinBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhongDanJinBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhongXiaoDanJinBiLiuTongIndex
		struct zhongXiaoDanJinBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongXiaoDanJinBiLiuTong < b.zhongXiaoDanJinBiLiuTong); }
		} cmpMethod_zhongXiaoDanJinBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhongXiaoDanJinBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhongXiaoDanJinBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for daDanLiuRuBiLiuTongIndex
		struct daDanLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.daDanLiuRuBiLiuTong < b.daDanLiuRuBiLiuTong); }
		} cmpMethod_daDanLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_daDanLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->daDanLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for daDanLiuChuBiLiuTongIndex
		struct daDanLiuChuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.daDanLiuChuBiLiuTong > b.daDanLiuChuBiLiuTong); }
		} cmpMethod_daDanLiuChuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_daDanLiuChuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->daDanLiuChuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zhangTingBanIndex
		struct zhangTingBanIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.zhangTingBan / a.ziYouLiuTongShiZhi) > (b.zhangTingBan / b.ziYouLiuTongShiZhi)); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.zhangTingBan) > (b.zhangTingBan)); }
		} cmpMethod_zhangTingBanIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhangTingBanIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhangTingBanIndex = index++;
			++proIter;
		}
	}
	{
		// sort for jingLiuRuBiLiuTongIndex
		struct jingLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.jingLiuRuBiLiuTong < b.jingLiuRuBiLiuTong); }
		} cmpMethod_jingLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_jingLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->jingLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zongLiuRuBiLiuTongIndex
		struct zongLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuRuBiLiuTong > b.zongLiuRuBiLiuTong); }
		} cmpMethod_zongLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zongLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zongLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zongLiuChuBiLiuTongIndex
		struct zongLiuChuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuChuBiLiuTong > b.zongLiuChuBiLiuTong); }
		} cmpMethod_zongLiuChuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zongLiuChuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zongLiuChuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for chengJiaoBiLiuTongIndex
		struct chengJiaoBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.chengJiaoBiLiuTong > b.chengJiaoBiLiuTong); }
		} cmpMethod_chengJiaoBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_chengJiaoBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->chengJiaoBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for jingJiaLiangBiIndex
		struct jingJiaLiangBiIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.jingJiaLiangBi > b.jingJiaLiangBi); }
		} cmpMethod_jingJiaLiangBiIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_jingJiaLiangBiIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->jingJiaLiangBiIndex = index++;
			++proIter;
		}
	}
	{
		// sort for liangBiIndex
		struct liangBiIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.liangBi > b.liangBi); }
		} cmpMethod_liangBiIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_liangBiIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->liangBiIndex = index++;
			++proIter;
		}
	}
	/*{
		// sort for weiBiIndex
		struct weiBiIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.weiBi > b.weiBi); }
		} cmpMethod_weiBiIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_weiBiIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->weiBiIndex = index++;
			++proIter;
		}
	}*/
	{
		// sort for zhangFuIndex
		struct zhangFuIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhangFu > b.zhangFu); }
		} cmpMethod_zhangFuIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zhangFuIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zhangFuIndex = index++;
			++proIter;
		}
	}
	{
		// sort for ziYouLiuTongShiZhiIndex
		struct ziYouLiuTongShiZhiIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.ziYouLiuTongShiZhi < b.ziYouLiuTongShiZhi); }
		} cmpMethod_ziYouLiuTongShiZhiIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_ziYouLiuTongShiZhiIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->ziYouLiuTongShiZhiIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiZhuLiJingLiangIndex
		struct zuoRiZhuLiJingLiangIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiZhuLiJingLiang < b.zuoRiZhuLiJingLiang); }
		} cmpMethod_zuoRiZhuLiJingLiangIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiZhuLiJingLiangIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiZhuLiJingLiangIndex = index++;
			++proIter;
		}
	}
	/*{
		// sort for zuoRiHuanShouIndex
		struct zuoRiHuanShouIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiHuanShou < b.zuoRiHuanShou); }
		} cmpMethod_zuoRiHuanShouIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiHuanShouIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiHuanShouIndex = index++;
			++proIter;
		}
	}*/
	{
		// sort for zuoRiJingLiuRuBiLiuTongIndex
		struct zuoRiJingLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiJingLiuRuBiLiuTong < b.zuoRiJingLiuRuBiLiuTong); }
		} cmpMethod_zuoRiJingLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiJingLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiJingLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiDaDanJingEBIndex
		struct zuoRiDaDanJingEBIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiDaDanJingEB < b.zuoRiDaDanJingEB); }
		} cmpMethod_zuoRiDaDanJingEBIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiDaDanJingEBIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiDaDanJingEBIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRidaDanJinBiLiuTongIndex
		struct zuoRidaDanJinBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRidaDanJinBiLiuTong > b.zuoRidaDanJinBiLiuTong); }
		} cmpMethod_zuoRidaDanJinBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRidaDanJinBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRidaDanJinBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRidaDanLiuRuBiLiuTongIndex
		struct zuoRidaDanLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRidaDanLiuRuBiLiuTong > b.zuoRidaDanLiuRuBiLiuTong); }
		} cmpMethod_zuoRidaDanLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRidaDanLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRidaDanLiuRuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiDaDanLiuChuBiLiuTongIndex
		struct zuoRiDaDanLiuChuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiDaDanLiuChuBiLiuTong > b.zuoRiDaDanLiuChuBiLiuTong); }
		} cmpMethod_zuoRiDaDanLiuChuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiDaDanLiuChuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiDaDanLiuChuBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiChengJiaoBiLiuTongIndex
		struct zuoRiChengJiaoBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiChengJiaoBiLiuTong < b.zuoRiChengJiaoBiLiuTong); }
		} cmpMethod_zuoRiChengJiaoBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiChengJiaoBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiChengJiaoBiLiuTongIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiJiGouDongXiangIndex
		struct zuoRiJiGouDongXiangIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiJiGouDongXiang > b.zuoRiJiGouDongXiang); }
		} cmpMethod_zuoRiJiGouDongXiangIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiJiGouDongXiangIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiJiGouDongXiangIndex = index++;
			++proIter;
		}
	}
	{
		// sort for zuoRiSanHuShuLiangIndex
		struct zuoRiSanHuShuLiangIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiSanHuShuLiang > b.zuoRiSanHuShuLiang); }
		} cmpMethod_zuoRiSanHuShuLiangIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoRiSanHuShuLiangIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoRiSanHuShuLiangIndex = index++;
			++proIter;
		}
	}
	{
		// sort for limitVsDealIndex
		struct limitVsDealIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.limitVsDeal > b.limitVsDeal); }
		} cmpMethod_limitVsDealIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_limitVsDealIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->limitVsDealIndex = index++;
			++proIter;
		}
	}
	/*{
		// sort for limitVsCirculateIndex
		struct limitVsCirculateIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.limitVsCirculate > b.limitVsCirculate); }
		} cmpMethod_limitVsCirculateIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_limitVsCirculateIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->limitVsCirculateIndex = index++;
			++proIter;
		}
	}*/
	{
		// sort for indexLvsC
		struct indexLvsC {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.limitVsCirculate > b.limitVsCirculate); }
		} cmpMethod_indexLvsC;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_indexLvsC);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->indexLvsC = index++;
			++proIter;
		}
	}
	{
		// sort for guXingPingFenIndex
		struct guXingPingFenIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.guXingPingFen > b.guXingPingFen); }
		} cmpMethod_guXingPingFenIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_guXingPingFenIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->guXingPingFenIndex = index++;
			++proIter;
		}
	}
	{
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		while (proIter != propertyAnalyVec.end())
		{
			if (
				((proIter->zuoRiZuiGao - proIter->zuoRiKaiPan) < FLT_MIN)//昨日一字板
				&& (proIter->zuoRiZhenFu < FLT_MIN)
				)
			{
				//proIter->firstLimitTime = "150000";
				strcpy(proIter->firstLimitTime, "150000");
			}
			++proIter;
		}
		// sort for firstLimitTimeIndex
		struct firstLimitTimeIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.firstLimitTime) < atoi(b.firstLimitTime)); }
		} cmpMethod_firstLimitTimeIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_firstLimitTimeIndex);
		//vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->firstLimitTimeIndex = index++;
			++proIter;
		}
	}
	{
		// sort for lastLimitTimeIndex
		struct lastLimitTimeIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.lastLimitTime) <atoi(b.lastLimitTime)); }
		} cmpMethod_lastLimitTimeIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_lastLimitTimeIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->lastLimitTimeIndex = index++;
			++proIter;
		}
	}
	{
		// sort for sanHuShuLiangIndex
		struct sanHuShuLiangIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.sanHuShuLiang > b.sanHuShuLiang); }
		} cmpMethod_sanHuShuLiangIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_sanHuShuLiangIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->sanHuShuLiangIndex = index++;
			++proIter;
		}
	}
	{
		// sort for ddeIndex
		struct ddeIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhuLiJingLiang < b.zhuLiJingLiang); }
		} cmpMethodDde;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethodDde);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->ddeIdx = index++;
			++proIter;
		}
	}

	{
		// sort for total Index
		struct totalIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b)
				/*{ return (
					(a.ddeIdx
					+ a.jingLiuRuBiLiuTongIndex
					+ a.chengJiaoBiLiuTongIndex
					+ a.zongLiuRuBiLiuTongIndex
					+ a.zongLiuChuBiLiuTongIndex) <
					(b.ddeIdx
					+ b.jingLiuRuBiLiuTong
					+ b.chengJiaoBiLiuTongIndex
					+ b.zongLiuRuBiLiuTongIndex
					+ b.zongLiuChuBiLiuTongIndex)
					);
				}*/
			{
				return (
					(a.ddeIdx
						//+ a.sanHuShuLiangIndex
						+ a.jingLiuRuBiLiuTongIndex
						+ a.chengJiaoBiLiuTongIndex
						+ a.zongLiuRuBiLiuTongIndex
						+ a.zongLiuChuBiLiuTongIndex
						+ a.jingJiaLiangBiIndex
						//+ a.liangBiIndex
						+ a.zhangTingBanIndex
						//+ a.zhongDanLiuRuBiLiuTongIndex
						//+ a.zhongDanLiuChuBiLiuTongIndex
						//+ a.xiaoDanLiuRuBiLiuTongIndex
						//+ a.zhongXiaoLiuRuBiLiuTongIndex
						//+ a.xiaoDanJinBiLiuTongIndex
						//+ a.zhongDanJinBiLiuTongIndex
						//+ a.daDanLiuRuBiLiuTongIndex
						+ a.daDanLiuChuBiLiuTongIndex
						//+ a.zhongXiaoDanJinBiLiuTongIndex
						//+ a.xianJiaIndex
						+ a.zuoRiZhuLiJingLiangIndex
						+ a.zuoRiDaDanJingEBIndex
						+ a.zuoRiDaDanLiuChuBiLiuTongIndex
						//+ a.zuoRiChengJiaoBiLiuTongIndex
						//+ a.zuoRiJiGouDongXiangIndex
						//+ a.zuoRiSanHuShuLiangIndex
						//+ a.zuoRiJingLiuRuBiLiuTongIndex
						//+ a.zuoRidaDanJinBiLiuTong
						//+ a.zuoRidaDanLiuRuBiLiuTongIndex
						//+ a.zuoRiHuanShouIndex
						//+ a.weiBiIndex
						+ a.zhangFuIndex
						+ a.limitVsDealIndex
						+ a.indexLvsC
						//+ a.limitUpMoneyIndex
						//+ a.firstLimitTimeIndex
						//+ a.lastLimitTimeIndex
						+ a.guXingPingFenIndex
						//+ a.continueDayIndex
						//+ a.ziYouLiuTongShiZhiIndex
					) <
					(b.ddeIdx
						//+ b.sanHuShuLiangIndex
						+ b.jingLiuRuBiLiuTongIndex
						+ b.chengJiaoBiLiuTongIndex
						+ b.zongLiuRuBiLiuTongIndex
						+ b.zongLiuChuBiLiuTongIndex 
						+ b.jingJiaLiangBiIndex
						//+ b.liangBiIndex
						+ b.zhangTingBanIndex 
						//+ b.zhongDanLiuRuBiLiuTongIndex
						//+ b.zhongDanLiuChuBiLiuTongIndex
						//+ b.xiaoDanLiuRuBiLiuTongIndex
						//+ b.zhongXiaoLiuRuBiLiuTongIndex
						//+ b.xiaoDanJinBiLiuTongIndex
						//+ b.zhongDanJinBiLiuTongIndex
						//+ b.daDanLiuRuBiLiuTongIndex
						+ b.daDanLiuChuBiLiuTongIndex
						//+ b.zhongXiaoDanJinBiLiuTongIndex
						//+ b.xianJiaIndex
						+ b.zuoRiZhuLiJingLiangIndex
						+ b.zuoRiDaDanJingEBIndex
						+ b.zuoRiDaDanLiuChuBiLiuTongIndex
						//+ b.zuoRiChengJiaoBiLiuTongIndex
						//+ b.zuoRiJiGouDongXiangIndex
						//+ b.zuoRiSanHuShuLiangIndex
						//+ b.zuoRiJingLiuRuBiLiuTongIndex
						//+ b.zuoRidaDanJinBiLiuTong
						//+ b.zuoRidaDanLiuRuBiLiuTongIndex
						//+ b.zuoRiHuanShouIndex
						//+ b.weiBiIndex
						+ b.zhangFuIndex
						+ b.limitVsDealIndex
						+ b.indexLvsC
						//+ b.limitUpMoneyIndex
						//+ b.firstLimitTimeIndex
						//+ b.lastLimitTimeIndex
						+ b.guXingPingFenIndex
						//+ b.continueDayIndex
						//+ b.ziYouLiuTongShiZhiIndex
						)
				);
			}
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.ddeIdx + a.jingLiuRuBiLiuTongIndex + a.chengJiaoBiLiuTongIndex + a.zongLiuRuBiLiuTongIndex) < (b.ddeIdx + b.jingLiuRuBiLiuTong + b.chengJiaoBiLiuTongIndex + b.zongLiuRuBiLiuTongIndex)); }
		} cmpMethodTotal;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethodTotal);

		/*{
			vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
			vector<PROPERTY_t> propertyAnalySelectVec;
			propertyAnalySelectVec.clear();
			int count = 0;
			while (proIter != propertyAnalyVec.end())
			{
				if (proIter->zijinIdx < 1500)
				{
					count++;
					propertyAnalySelectVec.push_back(*proIter);
				}
				if (10 < count)
				{
					break;
				}
				++proIter;
			}
			{
				// sort for ddeIndex
				struct totalIndex {
					bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) {
						return (
							a.ddeIdx
							+ a.chengJiaoBiLiuTongIndex
							+ a.zongLiuChuBiLiuTongIndex
							+ a.jingJiaLiangBiIndex
							+ a.daDanLiuChuBiLiuTongIndex
							+ a.zhangFuIndex
							)
							<
							(
								b.ddeIdx
								+ b.chengJiaoBiLiuTongIndex
								+ b.zongLiuChuBiLiuTongIndex
								+ b.jingJiaLiangBiIndex
								+ b.daDanLiuChuBiLiuTongIndex
								+ b.zhangFuIndex
								);
					}
				} cmpMethodTotal;
				std::sort(propertyAnalySelectVec.begin(), propertyAnalySelectVec.end(), cmpMethodTotal);
				propertyAnalyVec = propertyAnalySelectVec;
			}
		}*/

		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			PROPERTY_t &a = (*proIter);
			//proIter->totalIndex = index++;
			proIter->totalIndex = a.ddeIdx
				+ a.jingLiuRuBiLiuTongIndex
				+ a.chengJiaoBiLiuTongIndex
				+ a.zongLiuRuBiLiuTongIndex
				+ a.zongLiuChuBiLiuTongIndex
				+ a.jingJiaLiangBiIndex
				//+ a.liangBiIndex
				+ a.zhangTingBanIndex
				//+ a.zhongDanLiuRuBiLiuTongIndex
				//+ a.zhongDanLiuChuBiLiuTongIndex
				//+ a.xiaoDanLiuRuBiLiuTongIndex
				//+ a.zhongXiaoLiuRuBiLiuTongIndex
				//+ a.xiaoDanJinBiLiuTongIndex
				//+ a.zhongDanJinBiLiuTongIndex
				//+ a.daDanLiuRuBiLiuTongIndex
				+a.daDanLiuChuBiLiuTongIndex
				//+ a.zhongXiaoDanJinBiLiuTongIndex
				//+ a.xianJiaIndex
				+ a.zuoRiZhuLiJingLiangIndex
				+ a.zuoRiDaDanJingEBIndex
				+ a.zuoRiDaDanLiuChuBiLiuTongIndex
				+ a.zuoRiChengJiaoBiLiuTongIndex
				//+ a.zuoRiJiGouDongXiangIndex
				+ a.zuoRiSanHuShuLiangIndex
				//+ a.zuoRiJingLiuRuBiLiuTongIndex
				//+ a.zuoRidaDanJinBiLiuTong
				//+ a.zuoRidaDanLiuRuBiLiuTongIndex
				//+ a.zuoRiHuanShouIndex
				//+ a.weiBiIndex
				+a.zhangFuIndex
				+ a.limitVsDealIndex
				+ a.indexLvsC
				//+ a.limitUpMoneyIndex
				//+ a.firstLimitTimeIndex
				//+ a.lastLimitTimeIndex
				+a.guXingPingFenIndex
				//+ a.continueDayIndex
				//+ a.ziYouLiuTongShiZhiIndex
				;
			++proIter;
		}
	}

	}
}
