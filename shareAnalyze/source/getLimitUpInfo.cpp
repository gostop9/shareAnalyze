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

using namespace std;
using namespace SHAREDEF;
using namespace commonFun;
using namespace SHARE_FLAG;
namespace GETLIMITUPINFO
{
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
					limitUpInfo_t ztSort;
					//ztSort.code = analyProty.code;
					//ztSort.name = analyProty.name;
					strcpy(ztSort.code, analyProty.code);
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

					ztrIter->limitInfo.insert(ztSort);
				}
				break;
			}
			++ztrIter;
		}
		if (ztrIter == ztrVec.end()) //not found
		{
			limitUpInfo_t ztSort;
			//ztSort.code = analyProty.code;
			//ztSort.name = analyProty.name;
			strcpy(ztSort.code, analyProty.code);
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

				/*float maiLiang = (setIter->weiCha * 100 / setIter->weiBi - setIter->weiCha) / 2;
				float zhangTingBan = setIter->zuoShou * 1.1 * maiLiang / 10000; //千万量级显示
				if (setIter->weiBi > WEIBI_MAX)
				{
					zhangTingBan = (setIter->buyLiang * setIter->xianJia) / 1000000.0;
				}
				else if (setIter->weiBi < -100.0) // 委差为无效值时
				{
					zhangTingBan = 0.0;
				}*/
				float zhanLiuBi = 0;
				float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);
				string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));				
				fprintf(fp, "%2d  %-8s  %3s%-8s  %6.2f | %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%8.2f,  %5.2f : %d  %s: \n",
					ztrTemp.ztCount, setIter->code, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, zhangTingBan, zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());
				for (int j = 1; j < maxDisplay; j++)
				{
					++setIter;

					/*float maiLiang = (setIter->weiCha * 100 / setIter->weiBi - setIter->weiCha) / 2;
					float zhangTingBan = setIter->zuoShou * 1.1 * maiLiang / 10000; //千万量级显示
					if (setIter->weiBi > WEIBI_MAX)
					{
						zhangTingBan = (setIter->buyLiang * setIter->xianJia) / 1000000.0;
					}
					else if (setIter->weiBi < -100.0) // 委差为无效值时
					{
						zhangTingBan = 0.0;
					}*/
					float zhanLiuBi = 0;
					float zhangTingBan = getLimitUpMoney<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter), zhanLiuBi);

					string printFlag = getShareFlag<limitUpInfo_t>(const_cast<limitUpInfo_t &>(*setIter));					
					fprintf(fp, "    %-8s  %3s%-8s  %6.2f | %7.3f亿,连:%2d, %6.2f  %5.2f  %5.2f, 委:%6.2f,涨停:%8.2f\n",
						setIter->code, printFlag.c_str(), setIter->name, setIter->zhangFu, setIter->ziYouLiuTongShiZhi / DIVIDE, setIter->continueDay, setIter->jingJiaLiangBi, setIter->zuoRiHuanShou, setIter->zuoRiLiangBi, setIter->weiBi, zhangTingBan);
				}
			}
		}
	}

}
