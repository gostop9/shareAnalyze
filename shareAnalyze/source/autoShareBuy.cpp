#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include "shareDef.h"
#include "dataStructure.h"
#include "autoShareBuy.h"
#include "shareSelect.h"

using namespace std;
using namespace SHAREDEF;
namespace AUTOSHAREBUY
{

void autoShareBuy(FILE *buyFp, std::vector<PROPERTY_t> &propertyAnalyVec, PROPERTY_t &programBuyProty, int &programFindFlag)
{
    programFindFlag = 0;
    //程序下单
    int buyNumber = 5;
    if (propertyAnalyVec.size() > 50)
    {
        buyNumber = 6; //涨停较多时，多取一个
    }
    vector<PROPERTY_t> buyProtyVec;
    buyProtyVec.assign(propertyAnalyVec.begin(), propertyAnalyVec.begin() + buyNumber);

    struct buyShare {
        bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zijinIdx < b.zijinIdx); }
    } cmpMethod_buyShare;
    sort(buyProtyVec.begin(), buyProtyVec.end(), cmpMethod_buyShare);
    
    float jingJiaLiangBiMargin = 74.0;
    for (int j = 0; j < buyNumber; j++)
    {
        PROPERTY_t &buyProty = buyProtyVec[j];
        string name = buyProty.name;
        std::size_t found = name.find("ST");
        if (
			(found != std::string::npos)//剔除ST股
			//|| (0 == strcmp(buyProty.limitReason, NEW_SHARE.c_str()))//剔除新股
			)
        {
            continue;
        }
        if (
            ((buyProty.zuoRiZuiGao - buyProty.zuoRiKaiPan) < FLT_MIN)//昨日一字板
            && (buyProty.zuoRiZhenFu < FLT_MIN)
            && (buyProty.zuoRiHuanShou > 2.9)
            && (buyProty.jingJiaLiangBi > jingJiaLiangBiMargin)
            )
        {
            continue;
        }
        if (
            (buyProty.zijinIdx < 31)
            && (buyProty.jingJiaLiangBi > 9.59)
            && (
            (buyProty.jingJiaLiangBi < jingJiaLiangBiMargin)
                || ((buyProty.zuoRiHuanShou < 2.4) && (buyProty.jingJiaLiangBi > jingJiaLiangBiMargin) && (buyProty.jingJiaLiangBi < 300.0))//buyProty.zuoRiHuanShou < 1.5
                )
            && (buyProty.weiBi > -83.9)
            && (buyProty.continueDay < 6)
            && (buyProty.zhangTingBan > -4900.0)//4000.0
            && (buyProty.zuoRiLiangBi < 4.2)//4.0
            && (buyProty.zhangFu > 1.0)
			&& (buyProty.zhuLiJingLiang > -0.95)
            && (buyProty.zuoRiHuanShou < 21.0)
            && (buyProty.zongJinE > (999.0 * TENTHOUSAND))
            && (buyProty.zuoRiZongJinE < (70000.0 * TENTHOUSAND))
            )
        {
            char *codeNum = (char *)(buyProty.code) + 2;
            fprintf(buyFp, "%s\n", codeNum);
            programFindFlag = 1;
            programBuyProty = buyProty;
            break;
        }
    }
    if(0 == programFindFlag)
    {
        //fprintf(buyFp, "9527\n");
    }
}


void sortByGaodu(std::vector<PROPERTY_t> &propertyAnalyVec)
{
	struct continueDay {
		bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.continueDay > b.continueDay); }
	} cmpMethodContinueDay;
	stable_sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethodContinueDay);
}

void sortByZhangfuGaodu(std::vector<PROPERTY_t> &propertyAnalyVec)
{
	struct gaoDu {
		bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhangFu > b.zhangFu); }
	} cmpMethodGaoDu;
	stable_sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethodGaoDu);
	sortByGaodu(propertyAnalyVec);
}

void sortByTimeZhangfuGaodu(std::vector<PROPERTY_t> &propertyAnalyVec)
{
	struct lastLimitTimeIndex {
		bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.lastLimitTime) < atoi(b.lastLimitTime)); }
	} cmpMethod_lastLimitTimeIndex;
	std::stable_sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpMethod_lastLimitTimeIndex);
	sortByZhangfuGaodu(propertyAnalyVec);
}

float getZuoRiJiaCha(PROPERTY_t &analyProty)
{
	float zuoRiJiaCha = abs(analyProty.zuoRiKaiPan - analyProty.zuoShou) / analyProty.preZuoShou;
	return zuoRiJiaCha * 100.0;
}

void changeFinalBuy(PROPERTY_t &finalBuy, PROPERTY_t &analyProty, int &buyFlag)
{
	if ((analyProty.zijinIdx < MAX_ZIJIN_IDX)
		|| (analyProty.continueDay < 1))
	{
		//if (analyProty.zuoRiZhenFu < 0.1)
		float zuoRiJiaCha = getZuoRiJiaCha(analyProty);
		if (zuoRiJiaCha < 0.12)
		{
			finalBuy = analyProty;
			buyFlag = 1;
		}
		else
		{
			if ((analyProty.zhangFu - analyProty.zuoRiKaiPanZhangFu) > 0.4)
			{
				finalBuy = analyProty;
				buyFlag = 1;
			}
		}
	}
}

void selectFirstShare(FILE *rstFp,
	FILE *buyFp,
	PROPERTY_t programBuyProty,
	int programFindFlag,
	std::vector<PROPERTY_t> &propertyAnalyVec,
	std::vector<PROPERTY_t> &propertyAnalyVecSort,
	std::vector<PROPERTY_t> &propertyAnalyVecPre)
{
	vector<PROPERTY_t> analyVecBuy;
	analyVecBuy.reserve(15);
	int buyFlag = 0;

	//判断高度板是否高开
	vector<PROPERTY_t> propertyAnalyVecGaodu = propertyAnalyVec;
	sortByZhangfuGaodu(propertyAnalyVecGaodu);
	int analyNum = propertyAnalyVecGaodu.size();
	for (int i = 0; i < analyNum; i++)
	{
		PROPERTY_t &analyProty = propertyAnalyVecGaodu[i];
		string name = analyProty.name;
		std::size_t found = name.find("ST");
		if (
			(found != std::string::npos)//剔除ST股
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
			)
		{
			continue;
		}
		//if (analyProty.zhangFu > 0.0)
		{
			//buyFlag = 1;
		}
		shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
		//fprintf(rstFp, "**********\n");
		break;
	}

	//打印排第一的票
	FILE *firstFp;
	firstFp = fopen("d:/share/firstLiuRu.txt", "at+");
	if (NULL == firstFp)
	{
		printf("Open firstLiuRu file failed!\n");
	}
	else
	{
		int analyNum = propertyAnalyVecSort.size();
		float zhangFuMargin = 0.4;
		PROPERTY_t finalBuy = propertyAnalyVecSort[analyNum - 1];
		finalBuy.zhangFu = -10.0;
		finalBuy.weiBi = -100.0;
		finalBuy.zhangTingBan = -10000.0;

		int findFlag = 0;
		int ztFindFlag = 0;
		for (int i = 0; i < 30; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (1
				//&& (analyProty.jingLiuRu < (analyProty.daDanJinE))
				//&& (analyProty.zhangFu > 1.5)
				//&& (analyProty.zongJinE > 1300 * TENTHOUSAND)
				//&& (analyProty.zhangTingBan > -6900.0)
				//&& (analyProty.weiBi > 0.0)
				&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				)
			{
				if (1
					&& ((analyProty.zhangFu > kaiPanZfMax)
						|| (analyProty.zhangFu < kaiPanZfMin))
					//&& (i < 12)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 30)
					&& (analyProty.zijinIdx < MAX_ZIJIN_IDX)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 20)
					)
				{
					analyVecBuy.push_back(analyProty);
				}

				//选择开盘涨幅最高代码
				float zuoRiJiaCha = getZuoRiJiaCha(analyProty);
				if (1
					&& (analyProty.zhangFu > kaiPanZfMax)
					&& (analyProty.zhangTingBan < 20)
					&& ((analyProty.zuoRiKaiPanZhangFu - zhangFuMargin) < analyProty.zhangFu)
					//&& ((zuoRiJiaCha < 0.12)
					//	|| ((analyProty.zuoRiKaiPanZhangFu + zhangFuMargin) < analyProty.zhangFu))
					&& (analyProty.continueDay < 5))
				{
					float finalBuyDvalue   = abs(round(finalBuy.zuoShou * 110.0) / 100.0 - finalBuy.xianJia);
					float analyProtyDvalue = abs(round(analyProty.zuoShou * 110.0) / 100.0 - analyProty.xianJia);
					if ((finalBuyDvalue < zhangFuRoundMin)
						&& (analyProtyDvalue < zhangFuRoundMin)
						)
					{
						if (abs(analyProty.weiBi - finalBuy.weiBi) < FLT_MIN)
						{
							if (analyProty.zhangTingBan > finalBuy.zhangTingBan)
							{
								changeFinalBuy(finalBuy, analyProty, buyFlag);
							}
						}
						else
						{
							if (analyProty.weiBi > finalBuy.weiBi)
							{
								changeFinalBuy(finalBuy, analyProty, buyFlag);
							}
						}
					}
					else
					{
						if (analyProty.zhangFu > finalBuy.zhangFu)
						{
							changeFinalBuy(finalBuy, analyProty, buyFlag);
						}
					}
				}
				/*if (analyProty.zhangFu > 4.0)
				{
					fprintf(rstFp, "\n");
				}*/
				//shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
			if (11 == i)
			{
				//fprintf(rstFp, "\n");
			}
		}
		/*for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (
				(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.weiBi > -80.0)
				&& (analyProty.zijinIdx < 6)
				&& (analyProty.zhangFu > 0.8)
				&& (analyProty.xianJia > kaiPanZfMax)
				//&& (analyProty.zuoRiLiangBi < 4.9)
				//&& (analyProty.jingLiuRu > (analyProty.daDanJinE))
				&& (analyProty.zhangTingBan > -6900.0)
				)
			{
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
				shareSelectPrint(firstFp, analyProty, propertyAnalyVecPre);
				findFlag = 1;
				if (0 == programFindFlag)
				{
					int ztPropertyAnalyVecNum = propertyAnalyVec.size();
					for (int j = 0; j < ztPropertyAnalyVecNum; j++)
					{
						if (0 == strcmp(analyProty.code, propertyAnalyVec[j].code))
						{
							ztFindFlag = 1;
							break;
						}
					}
					if (0 == ztFindFlag)
					{
						char *codeNum = (char *)(analyProty.code) + 2;
						fprintf(buyFp, "%s\n", codeNum);
					}
				}
				break;
			}
		}*/
		analyNum = analyVecBuy.size();
		sortByZhangfuGaodu(analyVecBuy);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecBuy[i];
			if ((analyProty.zhangFu > kaiPanZfMax)
				//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 90)
				&& ((analyProty.zuoRiKaiPanZhangFu - zhangFuMargin) < analyProty.zhangFu)
				)
			{
				shareSelectPrint(rstFp, analyProty, analyVecBuy);
			}
		}
		fprintf(rstFp, "\n");
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecBuy[i];
			if (analyProty.zhangFu < kaiPanZfMin)
			{
				shareSelectPrint(rstFp, analyProty, analyVecBuy);
			}
		}
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecBuy[i];
			string name = analyProty.name;
			std::size_t found = name.find("ST");
			if (
				(found != std::string::npos)//剔除ST股
				|| (0 == strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				)
			{
				continue;
			}

			if (
				//(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				//&& (0 != strcmp(analyProty.limitReason, PRE_NEW_SHARE.c_str()))
				//&& (0 != strcmp(analyProty.limitReason, PRO_NEW_SHARE.c_str()))
				(analyProty.weiBi > -90.0)
				//&& (analyProty.zhangFu < 9.0)
				//&& (analyProty.zhangTingBan < 100.0)
				&& (analyProty.xianJia > kaiPanZfMax)
				)
			{

				if (1 == buyFlag)
				{
					analyProty = finalBuy;
					fprintf(rstFp, "++++++++++\n");
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
					shareSelectPrint(firstFp, analyProty, propertyAnalyVecPre);
					findFlag = 1;
					if (
						(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
						&& (0 != strcmp(analyProty.limitReason, PRE_NEW_SHARE.c_str()))
						&& (0 != strcmp(analyProty.limitReason, PRO_NEW_SHARE.c_str()))
						)
					{
						char *codeNum = (char *)(analyProty.code) + 2;
						fprintf(buyFp, "%s\n", codeNum);
					}
					else
					{
						fprintf(buyFp, "9527\n");
					}
				}
				else
				{
					fprintf(rstFp, "NOTBUY++++++++++NOTBUY\n");
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
					shareSelectPrint(firstFp, analyProty, propertyAnalyVecPre);
					findFlag = 1;
					fprintf(buyFp, "9527\n");
				}
				break;
			}
		}
		if (1 == programFindFlag)
		{
			fprintf(rstFp, "++++++++++\n");
			shareSelectPrint(rstFp, programBuyProty, propertyAnalyVecPre);
		}
		if (0 == findFlag)
		{
			fprintf(firstFp, "9527\n");
		}
		if ((0 == programFindFlag) && ((1 == ztFindFlag) || (0 == findFlag)))
		{
			fprintf(buyFp, "9527\n");
		}
		fclose(firstFp);
	}
}
}
