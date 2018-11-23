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

using namespace std;
using namespace SHAREDEF;
using namespace commonFun;
using namespace SHARE_FLAG;
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
				ztrIter->zongShiZhi += analyProty.ziYouLiuTongShiZhi;
				ztrIter->danWeiShiZhi += (analyProty.ziYouLiuTongShiZhi / analyProty.zhangFu * 100);
				ztrIter->jingJiaLiangBiZong += analyProty.jingJiaLiangBi;
				ztrIter->jingLiuRuBiLiuTongZong += analyProty.jingLiuRuBiLiuTong;
				ztrIter->zongLiuRuBiLiuTongZong += analyProty.zongLiuRuBiLiuTong;
				ztrIter->shiZhiXzhangFuZong += (analyProty.ziYouLiuTongShiZhi/(100 + analyProty.zhangFu) * analyProty.zhangFu);
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
			ztrTemp.zongShiZhi = analyProty.ziYouLiuTongShiZhi;
			ztrTemp.danWeiShiZhi = analyProty.ziYouLiuTongShiZhi / analyProty.zhangFu;
			ztrTemp.jingJiaLiangBiZong = analyProty.jingJiaLiangBi;
			ztrTemp.jingLiuRuBiLiuTongZong = analyProty.jingLiuRuBiLiuTong;
			ztrTemp.zongLiuRuBiLiuTongZong = analyProty.zongLiuRuBiLiuTong;
			ztrTemp.shiZhiXzhangFuZong = (analyProty.ziYouLiuTongShiZhi / (100 + analyProty.zhangFu) * analyProty.zhangFu);
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

	void limitUpReason_t::limitShareSort(FILE *fp, std::vector<limitUpReason_t> &ztrVec)
	{
		int ztrNum = ztrVec.size();
		//按平均涨幅排序，用于查看板块强度
		{
			zfAvg_t zfAvg;
			sort(ztrVec.begin(), ztrVec.end(), zfAvg);
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
		ztrCount_t ztrCount;
		sort(ztrVec.begin(), ztrVec.end(), ztrCount);

		for (int i = 0; i < ztrNum; i++)
		{
			limitUpReason_t &ztrTemp = ztrVec[i];
			if (NEW_SHARE != ztrTemp.reason)
			{
				float zhangFuAvg = ztrTemp.zhangFuZong / float(ztrTemp.ztCount);
				set<limitUpInfo_t>::iterator setIter = ztrTemp.limitInfo.begin();
				int shareCount = ztrTemp.limitInfo.size();
				int maxDisplay = 5;
				maxDisplay = shareCount < maxDisplay ? shareCount : maxDisplay;

				//float zhanLiuBi = 0;
				//float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);
				string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));				
				fprintf(fp, "%2d  %-8s  %6.2f, %4s%-8s  %6.2f | 昨停:%5.0f,封:%2d, %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%8.2f,  %5.2f : %d  %s: \n",
					ztrTemp.ztCount, setIter->code, setIter->xianJia, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->limitUpMoney/TENTHOUSAND, setIter->indexLvsC, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, setIter->zhangTingBan, zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());
				for (int j = 1; j < maxDisplay; j++)
				{
					++setIter;

					//float zhanLiuBi = 0;
					//float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);

					string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));					
					fprintf(fp, "    %-8s  %6.2f, %4s%-8s  %6.2f | 昨停:%5.0f,封:%2d, %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%8.2f\n",
						setIter->code, setIter->xianJia, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->limitUpMoney/TENTHOUSAND, setIter->indexLvsC, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, setIter->zhangTingBan);
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
		// sort for zhongDanJinBiLiuTongIndex
		struct zhongDanJinBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongDanJinBiLiuTong > b.zhongDanJinBiLiuTong); }
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
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongXiaoDanJinBiLiuTong > b.zhongXiaoDanJinBiLiuTong); }
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
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoRiZhuLiJingLiang > b.zuoRiZhuLiJingLiang); }
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
		// sort for zuoJingLiuRuBiLiuTongIndex
		struct zuoJingLiuRuBiLiuTongIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zuoJingLiuRuBiLiuTong < b.zuoJingLiuRuBiLiuTong); }
		} cmpMethod_zuoJingLiuRuBiLiuTongIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_zuoJingLiuRuBiLiuTongIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->zuoJingLiuRuBiLiuTongIndex = index++;
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
	/*{
		// sort for firstLimitTimeIndex
		struct firstLimitTimeIndex {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.firstLimitTime) < atoi(b.firstLimitTime)); }
		} cmpMethod_firstLimitTimeIndex;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_firstLimitTimeIndex);
		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
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
	}*/
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
			{ return (
				(a.ddeIdx
					+ a.jingLiuRuBiLiuTongIndex
					+ a.chengJiaoBiLiuTongIndex
					+ a.zongLiuRuBiLiuTongIndex
					+ a.zongLiuChuBiLiuTongIndex
					+ a.jingJiaLiangBiIndex
					+ a.liangBiIndex
					+ a.zhangTingBanIndex
					+ a.zhongXiaoLiuRuBiLiuTongIndex
					+ a.zhongDanJinBiLiuTongIndex
					+ a.daDanLiuChuBiLiuTongIndex
					//+ a.zhongXiaoDanJinBiLiuTongIndex
					//+ a.xianJiaIndex
					//+ a.zuoRiZhuLiJingLiangIndex
					//+ a.zuoRiDaDanJingEBIndex
					//+ a.zuoJingLiuRuBiLiuTongIndex
					//+ a.zuoRiHuanShouIndex
					//+ a.weiBiIndex
					+ a.zhangFuIndex
					+ a.indexLvsC
					//+ a.limitUpMoneyIndex
					//+ a.firstLimitTimeIndex
					//+ a.lastLimitTimeIndex
					+ a.guXingPingFenIndex
					+ a.ziYouLiuTongShiZhiIndex
					) <
					(b.ddeIdx
						+ b.jingLiuRuBiLiuTong
						+ b.chengJiaoBiLiuTongIndex
						+ b.zongLiuRuBiLiuTongIndex
						+ b.zongLiuChuBiLiuTongIndex 
						+ b.jingJiaLiangBiIndex
						+ b.liangBiIndex
						+ b.zhangTingBanIndex
						+ b.zhongXiaoLiuRuBiLiuTongIndex
						+ b.zhongDanJinBiLiuTongIndex
						+ b.daDanLiuChuBiLiuTongIndex
						//+ b.zhongXiaoDanJinBiLiuTongIndex
						//+ b.xianJiaIndex
						//+ b.zuoRiZhuLiJingLiangIndex
						//+ b.zuoRiDaDanJingEBIndex
						//+ b.zuoJingLiuRuBiLiuTongIndex
						//+ b.zuoRiHuanShouIndex
						//+ b.weiBiIndex
						+ b.zhangFuIndex
						+ b.indexLvsC
						//+ b.limitUpMoneyIndex
						//+ b.firstLimitTimeIndex
						//+ b.lastLimitTimeIndex
						+ b.guXingPingFenIndex
						+ b.ziYouLiuTongShiZhiIndex
						)
				);
			}
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.ddeIdx + a.jingLiuRuBiLiuTongIndex + a.chengJiaoBiLiuTongIndex + a.zongLiuRuBiLiuTongIndex) < (b.ddeIdx + b.jingLiuRuBiLiuTong + b.chengJiaoBiLiuTongIndex + b.zongLiuRuBiLiuTongIndex)); }
		} cmpMethodTotal;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethodTotal);

		vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
		int index = 1;
		while (proIter != propertyAnalyVec.end())
		{
			proIter->totalIndex = index++;
			++proIter;
		}
	}

	}
}
