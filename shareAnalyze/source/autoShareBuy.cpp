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
            (abs(buyProty.zuoRiZuiGao - buyProty.zuoRiKaiPan) < FLT_MIN)//昨日一字板
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
	//if ((analyProty.zijinIdx < MAX_ZIJIN_IDX)
	//	|| (analyProty.continueDay < 1))
	{
		//if (analyProty.zuoRiZhenFu < 0.1)
		float zuoRiJiaCha = getZuoRiJiaCha(analyProty);
		//大幅高开板
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
			//昨日开一字
			else if ((abs(analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) < FLT_MIN)
				&& (abs(analyProty.zuoShou - analyProty.zuoRiKaiPan) < FLT_MIN))
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
	float zhangTingDvalue = 0.009;
	float weiBiThreshold = -78.0;
	float weiBiMaxThreshold = 45.0;//50.0;
	int zhuLiIndexThreshold = 2500;
	float zhuLiJingLiangThreshold = -0.001209;//-0.001209;
	float zongLiuRuThreshold0 = 0.001754;//0.001754;
	float zongLiuRuThreshold1 = 0.001754;//0.002794;
	int buyNum = MAX_ZIJIN_IDX - 1;
	int weiBanNum = 60;
	vector<PROPERTY_t> analyVecBuy;
	analyVecBuy.reserve(weiBanNum);
	vector<PROPERTY_t> analyVecWei;
	analyVecWei.reserve(weiBanNum);
	//委比
	vector<PROPERTY_t> analyVecWeiBi;
	analyVecWeiBi.reserve(weiBanNum);
	int buyFlag = 0;

	//判断高度板是否高开
	PROPERTY_t gaoduProperty;
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
		gaoduProperty = analyProty;
		//shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
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
		float zhangFuMargin = 1.2;
		PROPERTY_t finalBuy = propertyAnalyVecSort[analyNum - 1];
		finalBuy.zhangFu = -10.0;
		finalBuy.weiBi = -100.0;
		finalBuy.zhangTingBan = -10000.0;

		int findFlag = 0;
		int ztFindFlag = 0;
		for (int i = 0; i < buyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (1
				//&& (analyProty.jingLiuRu < (analyProty.daDanJinE))
				//&& (analyProty.zhangFu > 1.5)
				//&& (analyProty.zongJinE > 1300 * TENTHOUSAND)
				//&& (analyProty.zhangTingBan > -6900.0)
				//&& (analyProty.weiBi > 0.0)
				&& (analyProty.weiBi > weiBiThreshold)
				&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				)
			{
				//跳过昨日涨停原因为超跌反弹
				string limitR = analyProty.limitReason;
				string::size_type idx;
				idx = limitR.find("超跌反弹");//在a中查找b.
				if (1
					&& (idx != string::npos)
					&& (analyProty.continueDay > 0)
					)
				{
					continue;
				}
				if (1
					&& ((analyProty.zhangFu > kaiPanZfMax)
						|| (analyProty.zhangFu < kaiPanZfMin))
					//&& (i < 12)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 30)
					//&& (analyProty.zijinIdx < MAX_ZIJIN_IDX)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 3000)
					&& (analyProty.continueDay > 0)
					&& (
					((analyProty.zijinIdx < MAX_ZIJIN_IDX) && (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1))
						|| (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + 0.003))
					&& (analyProty.zongJinE > (998.0 * TENTHOUSAND))
					)
				{
					//昨日非一字或T字的，成交需大于2500万，避免庄股
					if (
						((abs(analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) > FLT_MIN)
							|| (abs(analyProty.zuoShou - analyProty.zuoRiKaiPan) > FLT_MIN))
						)
					{
						if (analyProty.zuoRiZongJinE > (4000.0 * TENTHOUSAND))
						{
							analyVecBuy.push_back(analyProty);
						}
					}
					else
					{
						analyVecBuy.push_back(analyProty);
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
		//选择昨日未涨停板高开股
		for (int i = 0; i < weiBanNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (1
				&& (analyProty.weiBi > weiBiThreshold)
				&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.zongJinE > (970.0 * TENTHOUSAND))
				)
			{
				if (1
					//&& (analyProty.zhangFu > kaiPanZfMax)
					&& (analyProty.continueDay < 1)
					&& (analyProty.zhangFu > kaiPanZfMax + 2.0)
					&& ((analyProty.liuRuBiZuoRiChengJiaoIdx < zhuLiIndexThreshold) 
						|| (analyProty.zongLiuRuBiZuoRiZongJinE > zhuLiJingLiangThreshold))
					//&& (analyProty.zongLiuRuBiZuoRiZongJinE > zhuLiJingLiangThreshold)
					)
				{
					analyVecWei.push_back(analyProty);
					analyVecBuy.push_back(analyProty);
				}
				if (1
					&& (analyProty.zhangFu > kaiPanZfMax)
					&& (analyProty.weiBi > 0.0)
					)
				{
					analyVecWeiBi.push_back(analyProty);
				}
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
		//打印大幅低开股
		/*fprintf(rstFp, "低开股\n");
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecBuy[i];
			if (analyProty.zhangFu < kaiPanZfMin)
			{
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}*/
		//打印高开股
		fprintf(rstFp, "高开股\n");
		shareSelectPrint(rstFp, gaoduProperty, propertyAnalyVecPre);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecBuy[i];
			if ((analyProty.zhangFu > kaiPanZfMax)
				//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < zhuLiIndexThreshold)
				//&& ((analyProty.zuoRiKaiPanZhangFu - zhangFuMargin) < analyProty.zhangFu)
				&& (analyProty.continueDay > 0)
				//&& (analyProty.continueDay != 2)
				&& ((analyProty.zijinIdx < MAX_ZIJIN_IDX) || (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1))
				)
			{
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
			//选择开盘涨幅最高代码
			float zuoRiJiaCha = getZuoRiJiaCha(analyProty);
			if (1
				&& (analyProty.zhangFu > kaiPanZfMax)
				&& (analyProty.zhangTingBan < 17.0)				
				//&& ((zuoRiJiaCha < 0.12)
				//	|| ((analyProty.zuoRiKaiPanZhangFu + zhangFuMargin) < analyProty.zhangFu))
				//&& (analyProty.continueDay < 5)
				//&& (analyProty.zijinIdx < MAX_ZIJIN_IDX)
				&& ((analyProty.liuRuBiZuoRiChengJiaoIdx < zhuLiIndexThreshold) 
					|| (analyProty.zongLiuRuBiZuoRiZongJinE > zhuLiJingLiangThreshold))
				//&& (analyProty.zongLiuRuBiZuoRiZongJinE > zhuLiJingLiangThreshold)
				)
			{
				float finalBuyDvalue = abs(round(round(finalBuy.zuoShou * 100.0) * 1.1) / 100.0 - finalBuy.xianJia);
				float analyProtyDvalue = abs(round(round(analyProty.zuoShou * 100.0) * 1.1) / 100.0 - analyProty.xianJia);
				//abs(round(analyProty.zuoShou * 110.0) / 100.0 - analyProty.xianJia);
				//跳过涨停价开盘，委比较小股
				if (
					(analyProty.weiBi < -30.0) 
					&& (analyProtyDvalue < zhangTingDvalue)
					&& (analyProty.continueDay > 0)
					)
				{
					continue;
				}
				//跳过委买市值较小，委比较小股
				if (
					(analyProty.weiBi < 10.0)
					&& (analyProty.zhangTingBan > -700.0)
					)
				{
					continue;
				}
				//跳过昨日涨停原因为超跌反弹
				string limitR = analyProty.limitReason;
				string::size_type idx;
				idx = limitR.find("超跌反弹");//在a中查找b.
				if (1
					&& (idx != string::npos)
					&& (analyProty.continueDay > 0)
					)
				{
					continue;
				}
				//跳过涨停价开盘，成交金额较小股
				if (
					//(analyProty.zongJinE < (2000.0 * TENTHOUSAND))
					((analyProty.jingJiaLiangBi < 30.0) 
						&& (analyProty.weiBi < weiBiMaxThreshold) 
						//&& (analyProty.zongJinE < (4000.0 * TENTHOUSAND))
						&& ((analyProty.zongJinE / analyProty.ziYouLiuTongShiZhi) < 0.03)
						)
					&& (analyProtyDvalue < zhangTingDvalue)
					)
				{
					continue;
				}
				//昨日非一字涨停开，今天开盘涨幅没拉出高度
				if (1
					//(analyProty.continueDay > 0)
					&& ((analyProty.zuoRiKaiPanZhangFu + zhangFuMargin) > analyProty.zhangFu)
					&& (analyProtyDvalue > zhangTingDvalue)
					)
				{
					//昨日一字或T字的暂时跳过
					if ((abs(analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) < FLT_MIN)
						&& (abs(analyProty.zuoShou - analyProty.zuoRiKaiPan) < FLT_MIN))
					{
					}
					else
					{
						continue;
					}
				}
				if (
					(analyProty.continueDay < 1)
					&& ((analyProty.zijinIdx > 60)
						|| (analyProty.zongLiuRuBiLiuTong < zongLiuRuThreshold0))
					)
				{
					continue;
				}
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
		}
		//打印高开未板股
		fprintf(rstFp, "未板高开股\n");
		analyNum = analyVecWei.size();
		sortByZhangfuGaodu(analyVecWei);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecWei[i];
			if ((analyProty.zhangFu > kaiPanZfMax + 2.0)
				&& ((analyProty.liuRuBiZuoRiChengJiaoIdx < zhuLiIndexThreshold) || (analyProty.zongLiuRuBiZuoRiZongJinE > zhuLiJingLiangThreshold))
				//&& ((analyProty.zuoRiKaiPanZhangFu - zhangFuMargin) < analyProty.zhangFu)
				)
			{
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}
		//委比股
		fprintf(rstFp, "委比高股---------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		analyNum = analyVecWeiBi.size();
		sortByZhangfuGaodu(analyVecWeiBi);
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = analyVecWeiBi[i];
			if ((analyProty.zhangFu > kaiPanZfMax + 2.0)
				//&& ((analyProty.zuoRiKaiPanZhangFu - zhangFuMargin) < analyProty.zhangFu)
				)
			{
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}

		/*// 买入力度
		{
			fprintf(rstFp, "买入力度---------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			for (int i = 0; i < 10; i++)
			{
				PROPERTY_t &analyProty = propertyAnalyVecSort[i];
				{
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
				}
			}
		}*/
		//打印最终选股
		PROPERTY_t &analyProty = finalBuy;
		if (1 == buyFlag)
		{
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
