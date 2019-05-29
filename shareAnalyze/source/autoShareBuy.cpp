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

void selectFirstShare(FILE *rstFp,
	FILE *buyFp,
	PROPERTY_t programBuyProty,
	int programFindFlag,
	std::vector<PROPERTY_t> &propertyAnalyVec,
	std::vector<PROPERTY_t> &propertyAnalyVecSort,
	std::vector<PROPERTY_t> &propertyAnalyVecPre) 
{
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
		int findFlag = 0;
		int ztFindFlag = 0;
		for (int i = 0; i < 15; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (1
				//&& (analyProty.jingLiuRu < (analyProty.daDanJinE))
				//&& (analyProty.zhangFu > -2.0)
				//&& (analyProty.zhangTingBan > -6900.0)
				//&& (analyProty.weiBi > 0.0)
				)
			{

				if (analyProty.zhangFu > 4.0)
				{
					fprintf(rstFp, "\n");
				}
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVecSort[i];
			if (
				(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.weiBi > -80.0)
				&& (analyProty.zijinIdx < 6)
				&& (analyProty.zhangFu > 0.8)
				&& (analyProty.xianJia > 1.2)
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
