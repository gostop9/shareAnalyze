#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
//#include <windows.h>
#include <stdio.h>
#include <climits>
#include <unordered_set>
#include "dataStructure.h"
#include "shareSelect.h"
#include "readZhangTing.h"
#include "shareDef.h"
#include "getShareFlag.h"

using namespace std;
using namespace ZHANGTING;
using namespace SHAREDEF;
using namespace SHARE_FLAG;

int selectCondition(vector<DDE_t>::iterator ddeIter, vector<ZIJIN_t>::iterator zijinIter, vector<ZHULI_t>::iterator zhuliIter)
{
	if ((ddeIter->huanShou <= huanShouMax)
		&& (zijinIter->liangBi >= liangBiMin)
		&& (zijinIter->liangBi <= liangBiMax)
		&& (zijinIter->jingLiuRu > FLT_MIN)
		&& (zijinIter->daDanJinE > FLT_MIN)
		&& (zijinIter->zhongDanJinE > FLT_MIN)
		&& (ddeIter->zhuLiJingLiang >= zhuLiJingLiangMin)
		&& (zijinIter->daDanJingEB >= daDanJingEBMin)
		//&& (zijinIter->zhongDanJingEB >= 0.09)
		//&& (zijinIter->xiaoDanJinE > FLT_MIN)
		)
	{
		return 1;
	}
	return 0;
}

void shareSelect(FILE *fp, vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet)
{
    int ddeIndex = 1;
    vector<DDE_t>::iterator ddeIter = dde.begin();
	vector<string> selectVec;
	selectVec.reserve(zijinSelectCount);
    while(ddeIter != dde.end())
    {
        char NAME[CHAR_LEN];        
        int zijinIndex = 1;
        vector<ZIJIN_t>::iterator zijinIter = zijin.begin();
        while(zijinIter != zijin.end())
        {
            if((!strcmp(ddeIter->name, zijinIter->name)) && (zijinSelectCount > zijinIndex))
            {
                int zhuliIndex = 1;
                vector<ZHULI_t>::iterator zhuliIter = zhuli.begin();
                while(zhuliIter != zhuli.end())
                {
					if ((1 == selectCondition(ddeIter, zijinIter, zhuliIter))
						&& (!strcmp(ddeIter->name, zhuliIter->name)) 
						&& (zhuliSelectCount > zhuliIndex)
						)
                    {
                        printf("%8s, %8s, %7.3f亿, DDE:%3d, 资金:%3d, 主力: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n", 
							ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, zhuliIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
							zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);
                        fprintf(fp, "%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 主力: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n", 
							ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, zhuliIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
							zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);
						
						//保存结果code，后续输出到文件
						string rstCode = ddeIter->code;
						resultSet.insert(rstCode);

						break;
					}
                    ++zhuliIndex;
                    ++zhuliIter;
                }

				if(1 == selectCondition(ddeIter, zijinIter, zhuliIter))
				{
					char selectedChar[300];
					string selectedStr;
					sprintf(selectedChar, "%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n",
						ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
						zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);
					selectedStr = selectedChar;
					selectVec.push_back(selectedStr);

					//保存结果code，后续输出到文件
					string rstCode = ddeIter->code;
					resultSet.insert(rstCode);
					break;
				}
            }
            ++zijinIndex;
            ++zijinIter;
        }
        ++ddeIndex;
        if(ddeSelectCount < ddeIndex)
        {
            break;
        }
        ++ddeIter;
    }

	int selVecNum = selectVec.size();
	if (0 < selVecNum)
	{
		for (int i = 0; i < selVecNum; i++)
		{
			fprintf(fp, selectVec[i].c_str());
		}
	}
}

void shareSelectZiJin(FILE *fp, vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet)
{
    int zijinIndex = 1;
    vector<ZIJIN_t>::iterator zijinIter = zijin.begin();
	vector<string> selectVec;
	selectVec.reserve(zijinSelectCount);
    while(zijinIter != zijin.end())
    {
        int ddeIndex = 1;
        vector<DDE_t>::iterator ddeIter = dde.begin();
        while(ddeIter != dde.end())
        {
			if ((!strcmp(ddeIter->name, zijinIter->name)) && (ddeSelectCount > ddeIndex))
			{
				int zhuliIndex = 1;
				vector<ZHULI_t>::iterator zhuliIter = zhuli.begin();
				while (zhuliIter != zhuli.end())
				{
					if ((1 == selectCondition(ddeIter, zijinIter, zhuliIter))
						&& (!strcmp(ddeIter->name, zhuliIter->name))
						&& (zhuliSelectCount > zhuliIndex)
						)
					{
						printf("%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 主力: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n",
							ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, zhuliIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
							zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);
						fprintf(fp, "%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 主力: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n",
							ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, zhuliIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
							zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);

						//保存结果code，后续输出到文件
						string rstCode = ddeIter->code;
						resultSet.insert(rstCode);
						break;
					}
					++zhuliIndex;
					++zhuliIter;
				}

				if (1 == selectCondition(ddeIter, zijinIter, zhuliIter))
				{
					char selectedChar[300];
					string selectedStr;
					sprintf(selectedChar, "%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n",
						ddeIter->code, ddeIter->name, zijinIter->liuTongShiZhi / DIVIDE, ddeIndex, zijinIndex, ddeIter->zhangFu, ddeIter->zhuLiJingLiang, ddeIter->huanShou, zijinIter->liangBi,
						zijinIter->jingLiuRu / DIVIDE, zijinIter->daDanJinE / DIVIDE, zijinIter->daDanJingEB, zijinIter->zhongDanJinE / DIVIDE, zijinIter->zhongDanJingEB, zijinIter->xiaoDanJinE / DIVIDE, zijinIter->xiaoDanJingEB);
					selectedStr = selectedChar;
					selectVec.push_back(selectedStr);

					//保存结果code，后续输出到文件
					string rstCode = ddeIter->code;
					resultSet.insert(rstCode);
					break;
				}
			}

			++ddeIndex;
			++ddeIter;
        }
        
        ++zijinIndex;
        if(zijinSelectCount < zijinIndex)
        {
            break;
        }        
        ++zijinIter;
    }

	int selVecNum = selectVec.size();
	if (0 < selVecNum)
	{
		for (int i = 0; i < selVecNum; i++)
		{
			fprintf(fp, selectVec[i].c_str());
		}
	}
}

void shareSelectZDZ(FILE *fp, vector<PROPERTY_t> &propertyVec, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::vector<string> &resultSet)
{
	int zijinIndex = 1;
	vector<PROPERTY_t>::iterator ptyIter = propertyVec.begin();
	vector<string> selectVec;
	selectVec.reserve(zijinSelectCount);
	while (ptyIter != propertyVec.end())
	{
		if (zijinIndex > zijinSelectCount)
		{
			break;
		}
		//if ((ptyIter->ddeIdx > ddeSelectCount) || (ptyIter->zhuliIdx > zhuliSelectCount))
		if (ptyIter->zhuliIdx > zhuliSelectCount)
		{
			++zijinIndex;
			++ptyIter;
			continue;
		}
		if ((ptyIter->huanShou <= huanShouMax)
			&& (ptyIter->jingLiuRu > FLT_MIN)
			&& (ptyIter->daDanJinE > FLT_MIN)
			&& (ptyIter->zhongDanJinE > FLT_MIN)
			&& (ptyIter->daDanJingEB >= daDanJingEBMin)
			&& (ptyIter->liangBi >= liangBiMin)
			&& (ptyIter->liangBi <= liangBiMax)
			&& (ptyIter->zhuLiJingLiang >= zhuLiJingLiangMin)
			)
		{
			float zhangFu2 = ptyIter->erRiZhangFu - ptyIter->jinRiZhangFu;
			float zhangFu3 = ptyIter->sanRiZhangFu - ptyIter->erRiZhangFu;
			float zhangFu4 = ptyIter->wuRiZhangFu - ptyIter->jinRiZhangFu;
			float zhangFu23 = ptyIter->sanRiZhangFu - ptyIter->jinRiZhangFu;
			float zhangFu45 = ptyIter->wuRiZhangFu - ptyIter->sanRiZhangFu;
			float zhangFu2345 = ptyIter->wuRiZhangFu - ptyIter->jinRiZhangFu;

			float zhuLiJingBi = ptyIter->zhuLiJinE / ptyIter->zongJinE;//主力金额 / 总金额

			if (1
				&& (zhangFu23 < 10.0)
				&& (zhangFu2 < 3.0)
				&& (zhuLiJingBi > 0.08f)
				//&& (ptyIter->sanRiZhangFu < 10.0)
				//&& (ptyIter->wuRiZhangFu < 10.0)
				//&& (ptyIter->shiRiZhangFu < 10.0)	
				)
			{
				if (ptyIter->guXingPingFen > 0)
				{
					float neiWaiPanBi = (ptyIter->waiPan - ptyIter->neiPan) / (ptyIter->waiPan + ptyIter->neiPan);//内外盘比
					if (1
						&& (neiWaiPanBi > 0.07f)
						&& (ptyIter->jiGouDongXiang > 7.0)
						&& (ptyIter->weiBi > -80.0)
						//&& (ptyIter->jingLiZengLv > -10.0)
						&& (ptyIter->jingLiRun > 0.0)
						//&& (ptyIter->zhenFu >= 3.9)
						)
					{
						ptyIter->zijinIdx = zijinIndex;
						vector<PROPERTY_t> propertyPre;
						shareSelectPrint(fp, *ptyIter, propertyPre);
						//保存结果code，后续输出到文件
						string rstCode = ptyIter->code;
						vector<string>::iterator s = find(resultSet.begin(), resultSet.end(), rstCode);
						if (s == resultSet.end())//未找到
						{
							resultSet.push_back(rstCode);
						}
					}
				}
				else
				{
					ptyIter->zijinIdx = zijinIndex;
					vector<PROPERTY_t> propertyPre;
					shareSelectPrint(fp, *ptyIter, propertyPre);
					//保存结果code，后续输出到文件
					string rstCode = ptyIter->code;
					vector<string>::iterator s = find(resultSet.begin(), resultSet.end(), rstCode);
					if (s == resultSet.end())//未找到
					{
						resultSet.push_back(rstCode);
					}
				}
			}
		}
		++zijinIndex;
		++ptyIter;
	}

	/*int selVecNum = selectVec.size();
	if (0 < selVecNum)
	{
		for (int i = 0; i < selVecNum; i++)
		{
			fprintf(fp, selectVec[i].c_str());
		}
	}*/
}

void shareParaFuse(vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, vector<ZHANGFU_t> &zhangfu, vector<ZHANGFU_t> &preJingJiazhangfu, vector<guBen_t> &gubenVec, vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec)
{
	int zijinIndex = 1;
	int shareNum = zijin.size();
	propertyVec.clear();
	propertyVec.reserve(shareNum);
	vector<ZIJIN_t>::iterator zijinIter = zijin.begin();
	while (zijinIter != zijin.end())
	{
		PROPERTY_t property;

		//用于后续判断涨幅排名板块是否起作用
		property.guXingPingFen = -100.0;

		property.weiBi = zijinIter->weiBi;
		property.weiCha = zijinIter->weiCha;

		property.zijinIdx = zijinIndex;
		strcpy(property.code, zijinIter->code);
		strcpy(property.name, zijinIter->name);
		property.liuTongShiZhi = zijinIter->liuTongShiZhi;
		property.jingLiuRu = zijinIter->jingLiuRu;
		property.daDanLiuRu = zijinIter->daDanLiuRu;
		property.daDanLiuChu = zijinIter->daDanLiuChu;
		property.daDanJinE = zijinIter->daDanJinE;
		property.zhongDanLiuRu = zijinIter->zhongDanLiuRu;
		property.zhongDanLiuChu = zijinIter->zhongDanLiuChu;
		property.zhongDanJinE = zijinIter->zhongDanJinE;
		property.xiaoDanLiuRu = zijinIter->xiaoDanLiuRu;
		property.xiaoDanLiuChu = zijinIter->xiaoDanLiuChu;
		property.xiaoDanJinE = zijinIter->xiaoDanJinE;
		property.daDanJingEB = zijinIter->daDanJingEB;
		property.daDanZongE = zijinIter->daDanZongE;
		property.daDanZongEB = zijinIter->daDanZongEB;
		property.zhongDanJingEB = zijinIter->zhongDanJingEB;
		property.zhongDanZongE = zijinIter->zhongDanZongE;
		property.zhongDanZongEB = zijinIter->zhongDanZongEB;
		property.xiaoDanJingEB = zijinIter->xiaoDanJingEB;
		property.xiaoDanZongE = zijinIter->xiaoDanZongE;
		property.xiaoDanZongEB = zijinIter->xiaoDanZongEB;

		property.zhuLiJingLiang = zijinIter->zhuLiJingLiang;

		property.daDanBiLiuTong = zijinIter->daDanBiLiuTong;
        
        //去除DDE index
		//property.ddeIdx = zijinIter->zhuLiJingLiang;
		//property.sanHuShuLiang = zijinIter->sanHuShuLiang;
		//property.zhuLiJinE = zijinIter->zhuLiJinE;
		property.zhangFu = zijinIter->zhangFu;
		//strcpy(property.suoShuHangYe, zijinIter->suoShuHangYe);
		//strcpy(property.xingJi, zijinIter->xingJi);
		property.xianJia = zijinIter->xianJia;
		property.zhangSu = zijinIter->zhangSu;
		property.zongShou = zijinIter->zongShou;
		property.huanShou = zijinIter->huanShou;
		property.liangBi = zijinIter->liangBi;
		property.zongJinE = zijinIter->zongJinE;

        int zhuliIndex = 0;
        vector<ZHULI_t>::iterator zhuliIter = zhuli.begin();
        while (zhuliIter != zhuli.end())
        {
            zhuliIndex++;
            if (0 == strcmp(zijinIter->code, zhuliIter->code))
            {
                property.zhuliIdx = zhuliIndex;
                property.jinRiZengCangZhanBi = zhuliIter->jinRiZengCangZhanBi;
                property.jinRiPaiMing = zhuliIter->jinRiPaiMing;
                property.jinRiZhangFu = zhuliIter->jinRiZhangFu;
                property.erRiZengCangZhanBi = zhuliIter->erRiZengCangZhanBi;
                property.erRiPaiMing = zhuliIter->erRiPaiMing;
                property.erRiZhangFu = zhuliIter->erRiZhangFu;
                property.sanRiZengCangZhanBi = zhuliIter->sanRiZengCangZhanBi;
                property.sanRiPaiMing = zhuliIter->sanRiPaiMing;
                property.sanRiZhangFu = zhuliIter->sanRiZhangFu;
                property.wuRiZengCangZhanBi = zhuliIter->wuRiZengCangZhanBi;
                property.wuRiPaiMing = zhuliIter->wuRiPaiMing;
                property.wuRiZhangFu = zhuliIter->wuRiZhangFu;
                property.shiRiZengCangZhanBi = zhuliIter->shiRiZengCangZhanBi;
                property.shiRiPaiMing = zhuliIter->shiRiPaiMing;
                property.shiRiZhangFu = zhuliIter->shiRiZhangFu;

                if (0 < zhangfu.size())
                {
                    int zhangfuIndex = 0;
                    vector<ZHANGFU_t>::iterator zhangfuIter = zhangfu.begin();
                    while (zhangfuIter != zhangfu.end())
                    {
                        zhangfuIndex++;
                        if (0 == strcmp(zijinIter->code, zhangfuIter->code))
                        {
                            property.zhangfuIdx = zhangfuIndex;
                            property.zhangDie = zhangfuIter->zhangDie;
                            property.sanHuShuLiang = zhangfuIter->sanHuShuLiang;
                            property.zhuLiJingE = zhangfuIter->zhuLiJingE;
                            property.zhuLiJinE = zhangfuIter->zhuLiJingE;//这两个一样的意思

                            property.guXingPingFen = zhangfuIter->guXingPingFen;
                            //property.weiBi = zhangfuIter->weiBi;
                            property.jiGouDongXiang = zhangfuIter->jiGouDongXiang;
                            property.jingLiZengLv = zhangfuIter->jingLiZengLv;
                            property.jingLiRun = zhangfuIter->jingLiRun;
                            property.shiJingLv = zhangfuIter->shiJingLv;
                            property.waiPan = zhangfuIter->waiPan;
                            property.neiPan = zhangfuIter->neiPan;
                            property.zhenFu = zhangfuIter->zhenFu;

                            property.zongShiZhi = zhangfuIter->zongShiZhi;
                            property.xianShou = zhangfuIter->xianShou;
                            property.kaiPan = zhangfuIter->kaiPan;
                            property.zuoShou = zhangfuIter->zuoShou;
                            property.zuiGao = zhangfuIter->zuiGao;
                            property.zuiDi = zhangfuIter->zuiDi;
                            property.wuRiJunXian = zhangfuIter->wuRiJunXian;
                            property.buyLiang = zhangfuIter->buyLiang;
                            property.sellLiant = zhangfuIter->sellLiant;

                            property.jinChaGeShu = zhangfuIter->jinChaGeShu;
                            property.shouMeiBi = zhangfuIter->shouMeiBi;
                            
                            strcpy(property.suoShuHangYe, zhangfuIter->suoShuHangYe);
                            property.xingJi = zhangfuIter->xingJi;

                            property.shiYingDong = zhangfuIter->shiYingDong;
                            property.shiYingJing = zhangfuIter->shiYingJing;
                            property.shiYingTTM = zhangfuIter->shiYingTTM;
							
							if (0 < preJingJiazhangfu.size())
							{
								vector<ZHANGFU_t>::iterator prezhangfuIter = preJingJiazhangfu.begin();
								while (prezhangfuIter != preJingJiazhangfu.end())
								{
									if (0 == strcmp(zijinIter->code, prezhangfuIter->code))
									{
										property.preJingJiaZongJinE = prezhangfuIter->zongJinE;

										if (0 < gubenVec.size())
										{
											vector<guBen_t>::iterator gubenIter = gubenVec.begin();
											// 自由流通市值只有6位code码
											char codeTemp[10];
											memset(codeTemp, 0, sizeof(codeTemp));
											while (gubenIter != gubenVec.end())
											{
												char *pCode = zijinIter->code;
												strncpy(codeTemp, &pCode[2], 6);
												if (0 == strcmp(gubenIter->code, codeTemp))
												{
													property.ziYouLiuTongGu = gubenIter->ziYouLiuTongGu;
													property.guXingPingFen = gubenIter->guXingPingFen;
													break;
												}
												++gubenIter;
											}
										}
										else
										{
											property.ziYouLiuTongGu = 0.0;
											property.guXingPingFen = 0.0;
										}
										break;
									}
									++prezhangfuIter;
								}
							}
							break;
                        }
                        ++zhangfuIter;
                    }
                }

                propertyVec.push_back(property);

                //保存需要分析的item信息
                /*for (int analyIdx = 0; analyIdx < analyVec.size(); analyIdx++)
                {
                    if ((0 < strlen(zijinIter->code)) && (0 == strcmp(zijinIter->code, analyVec[analyIdx].code)))
                    {
                        propertyAnalyVec.push_back(property);
                    }
                }*/
                
                break;
            }
            //++zhuliIndex;
            ++zhuliIter;
        }
                
		++zijinIndex;
		++zijinIter;
	}
}

void shareParaFuseWithoutZhuli(vector<ZIJIN_t> &zijin, vector<ZHANGFU_t> &zhangfu, vector<ZHANGFU_t> &preJingJiazhangfu, vector<guBen_t> &gubenVec, vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec)
{
	int zijinIndex = 1;
	int shareNum = zijin.size();
	if (0 < shareNum)
	{
		propertyVec.clear();
		propertyVec.reserve(shareNum);
		vector<ZIJIN_t>::iterator zijinIter = zijin.begin();
		while (zijinIter != zijin.end())
		{
			PROPERTY_t property;

			//用于后续判断涨幅排名板块是否起作用
			property.guXingPingFen = -100.0;

			property.weiBi = zijinIter->weiBi;
			property.weiCha = zijinIter->weiCha;

			property.zijinIdx = zijinIndex;
			strcpy(property.code, zijinIter->code);
			strcpy(property.name, zijinIter->name);
			property.liuTongShiZhi = zijinIter->liuTongShiZhi;
			property.jingLiuRu = zijinIter->jingLiuRu;
			property.daDanLiuRu = zijinIter->daDanLiuRu;
			property.daDanLiuChu = zijinIter->daDanLiuChu;
			property.daDanJinE = zijinIter->daDanJinE;
			property.zhongDanLiuRu = zijinIter->zhongDanLiuRu;
			property.zhongDanLiuChu = zijinIter->zhongDanLiuChu;
			property.zhongDanJinE = zijinIter->zhongDanJinE;
			property.xiaoDanLiuRu = zijinIter->xiaoDanLiuRu;
			property.xiaoDanLiuChu = zijinIter->xiaoDanLiuChu;
			property.xiaoDanJinE = zijinIter->xiaoDanJinE;
			property.daDanJingEB = zijinIter->daDanJingEB;
			property.daDanZongE = zijinIter->daDanZongE;
			property.daDanZongEB = zijinIter->daDanZongEB;
			property.zhongDanJingEB = zijinIter->zhongDanJingEB;
			property.zhongDanZongE = zijinIter->zhongDanZongE;
			property.zhongDanZongEB = zijinIter->zhongDanZongEB;
			property.xiaoDanJingEB = zijinIter->xiaoDanJingEB;
			property.xiaoDanZongE = zijinIter->xiaoDanZongE;
			property.xiaoDanZongEB = zijinIter->xiaoDanZongEB;

			property.zhuLiJingLiang = zijinIter->zhuLiJingLiang;

			//去除DDE index
			//property.ddeIdx = zijinIter->zhuLiJingLiang;
			//property.sanHuShuLiang = zijinIter->sanHuShuLiang;
			//property.zhuLiJinE = zijinIter->zhuLiJinE;
			property.zhangFu = zijinIter->zhangFu;
			//strcpy(property.suoShuHangYe, zijinIter->suoShuHangYe);
			//strcpy(property.xingJi, zijinIter->xingJi);
			property.xianJia = zijinIter->xianJia;
			property.zhangSu = zijinIter->zhangSu;
			property.zongShou = zijinIter->zongShou;
			property.huanShou = zijinIter->huanShou;
			property.liangBi = zijinIter->liangBi;
			property.zongJinE = zijinIter->zongJinE;

			if (0 < zhangfu.size())
			{
				int zhangfuIndex = 0;
				vector<ZHANGFU_t>::iterator zhangfuIter = zhangfu.begin();
				while (zhangfuIter != zhangfu.end())
				{
					zhangfuIndex++;
					if (0 == strcmp(zijinIter->code, zhangfuIter->code))
					{
						property.zhuliIdx = 0;
						property.zhangfuIdx = zhangfuIndex;
						property.zhangDie = zhangfuIter->zhangDie;
						property.sanHuShuLiang = zhangfuIter->sanHuShuLiang;
						property.zhuLiJingE = zhangfuIter->zhuLiJingE;
						property.zhuLiJinE = zhangfuIter->zhuLiJingE;//这两个一样的意思

						property.guXingPingFen = zhangfuIter->guXingPingFen;
						//property.weiBi = zhangfuIter->weiBi;
						property.jiGouDongXiang = zhangfuIter->jiGouDongXiang;
						property.jingLiZengLv = zhangfuIter->jingLiZengLv;
						property.jingLiRun = zhangfuIter->jingLiRun;
						property.shiJingLv = zhangfuIter->shiJingLv;
						property.waiPan = zhangfuIter->waiPan;
						property.neiPan = zhangfuIter->neiPan;
						property.zhenFu = zhangfuIter->zhenFu;

						property.zongShiZhi = zhangfuIter->zongShiZhi;
						property.xianShou = zhangfuIter->xianShou;
						property.kaiPan = zhangfuIter->kaiPan;
						property.zuoShou = zhangfuIter->zuoShou;
						property.zuiGao = zhangfuIter->zuiGao;
						property.zuiDi = zhangfuIter->zuiDi;
						property.wuRiJunXian = zhangfuIter->wuRiJunXian;
						property.buyLiang = zhangfuIter->buyLiang;
						property.sellLiant = zhangfuIter->sellLiant;

						property.jinChaGeShu = zhangfuIter->jinChaGeShu;
						property.shouMeiBi = zhangfuIter->shouMeiBi;

						strcpy(property.suoShuHangYe, zhangfuIter->suoShuHangYe);
						property.xingJi = zhangfuIter->xingJi;

						property.shiYingDong = zhangfuIter->shiYingDong;
						property.shiYingJing = zhangfuIter->shiYingJing;
						property.shiYingTTM = zhangfuIter->shiYingTTM;

						if (0 < preJingJiazhangfu.size())
						{
							vector<ZHANGFU_t>::iterator prezhangfuIter = preJingJiazhangfu.begin();
							while (prezhangfuIter != preJingJiazhangfu.end())
							{
								if (0 == strcmp(zijinIter->code, prezhangfuIter->code))
								{
									property.preJingJiaZongJinE = prezhangfuIter->zongJinE;

									if (0 < gubenVec.size())
									{
										vector<guBen_t>::iterator gubenIter = gubenVec.begin();
										// 自由流通市值只有6位code码
										char codeTemp[10];
										memset(codeTemp, 0, sizeof(codeTemp));
										while (gubenIter != gubenVec.end())
										{
											char *pCode = zijinIter->code;
											strncpy(codeTemp, &pCode[2], 6);
											if (0 == strcmp(gubenIter->code, codeTemp))
											{
												property.ziYouLiuTongGu = gubenIter->ziYouLiuTongGu;
												property.guXingPingFen = gubenIter->guXingPingFen;
												break;
											}
											++gubenIter;
										}
									}
									else
									{
										property.ziYouLiuTongGu = 0.0;
										property.guXingPingFen = 0.0;
									}
									break;
								}
								++prezhangfuIter;
							}
						}

						propertyVec.push_back(property);

						//保存需要分析的item信息
						/*for (int analyIdx = 0; analyIdx < analyVec.size(); analyIdx++)
						{
							if ((0 < strlen(zijinIter->code)) && (0 == strcmp(zijinIter->code, analyVec[analyIdx].code)))
							{
								propertyAnalyVec.push_back(property);
							}
						}*/

						break;
					}
					++zhangfuIter;
				}
			}

			++zijinIndex;
			++zijinIter;
		}
	}
	else
	{
		if (0 < zhangfu.size())
		{
			int zhangfuIndex = 0;
			vector<ZHANGFU_t>::iterator zhangfuIter = zhangfu.begin();
			while (zhangfuIter != zhangfu.end())
			{
				zhangfuIndex++;
				PROPERTY_t property;

				//用于后续判断涨幅排名板块是否起作用
				property.guXingPingFen = -100.0;

				property.weiBi = zhangfuIter->weiBi;
				//property.weiCha = zhangfuIter->weiCha;

				strcpy(property.code, zhangfuIter->code);
				strcpy(property.name, zhangfuIter->name);
				property.liuTongShiZhi = zhangfuIter->liuTongShiZhi;

				property.zijinIdx = 0;
				property.zhuliIdx = 0;
				property.zhuLiJingLiang = zhangfuIter->zhuLiJingLiang;

				//去除DDE index
				property.ddeIdx = 0;
				//property.sanHuShuLiang = zijinIter->sanHuShuLiang;
				//property.zhuLiJinE = zijinIter->zhuLiJinE;
				property.zhangFu = zhangfuIter->zhangFu;
				//strcpy(property.suoShuHangYe, zijinIter->suoShuHangYe);
				//strcpy(property.xingJi, zijinIter->xingJi);
				property.xianJia = zhangfuIter->xianJia;
				property.zhangSu = zhangfuIter->zhangSu;
				property.zongShou = zhangfuIter->zongShou;
				property.huanShou = zhangfuIter->huanShou;
				property.liangBi = zhangfuIter->liangBi;
				property.zongJinE = zhangfuIter->zongJinE;


				property.zhangfuIdx = zhangfuIndex;
				property.zhangDie = zhangfuIter->zhangDie;
				property.sanHuShuLiang = zhangfuIter->sanHuShuLiang;
				property.zhuLiJingE = zhangfuIter->zhuLiJingE;
				property.zhuLiJinE = zhangfuIter->zhuLiJingE;//这两个一样的意思

				property.guXingPingFen = zhangfuIter->guXingPingFen;
				property.weiBi = zhangfuIter->weiBi;
				property.jiGouDongXiang = zhangfuIter->jiGouDongXiang;
				property.jingLiZengLv = zhangfuIter->jingLiZengLv;
				property.jingLiRun = zhangfuIter->jingLiRun;
				property.shiJingLv = zhangfuIter->shiJingLv;
				property.waiPan = zhangfuIter->waiPan;
				property.neiPan = zhangfuIter->neiPan;
				property.zhenFu = zhangfuIter->zhenFu;

				property.zongShiZhi = zhangfuIter->zongShiZhi;
				property.xianShou = zhangfuIter->xianShou;
				property.kaiPan = zhangfuIter->kaiPan;
				property.zuoShou = zhangfuIter->zuoShou;
				property.zuiGao = zhangfuIter->zuiGao;
				property.zuiDi = zhangfuIter->zuiDi;
				property.wuRiJunXian = zhangfuIter->wuRiJunXian;
				property.buyLiang = zhangfuIter->buyLiang;
				property.sellLiant = zhangfuIter->sellLiant;

				property.jinChaGeShu = zhangfuIter->jinChaGeShu;
				property.shouMeiBi = zhangfuIter->shouMeiBi;

				strcpy(property.suoShuHangYe, zhangfuIter->suoShuHangYe);
				property.xingJi = zhangfuIter->xingJi;

				property.shiYingDong = zhangfuIter->shiYingDong;
				property.shiYingJing = zhangfuIter->shiYingJing;
				property.shiYingTTM = zhangfuIter->shiYingTTM;

				if (0 < gubenVec.size())
				{
					vector<guBen_t>::iterator gubenIter = gubenVec.begin();
					// 自由流通市值只有6位code码
					char codeTemp[10];
					memset(codeTemp, 0, sizeof(codeTemp));
					while (gubenIter != gubenVec.end())
					{
						char *pCode = zhangfuIter->code;
						strncpy(codeTemp, &pCode[2], 6);
						if (0 == strcmp(gubenIter->code, codeTemp))
						{
							property.ziYouLiuTongGu = gubenIter->ziYouLiuTongGu;
							property.guXingPingFen = gubenIter->guXingPingFen;
							break;
						}
						++gubenIter;
					}
				}
				else
				{
					property.ziYouLiuTongGu = 0.0;
					property.guXingPingFen = 0.0;
				}

				propertyVec.push_back(property);
				++zhangfuIter;
			}
		}
	}
}

int shareFuseSelect(int &fileIndex, PROPERTY_t &property, vector<vector<PROPERTY_t>> &propertyVecV, vector<string> &selectVec)
{
	int flag = 0;
	int fileCount = propertyVecV.size();
	if (fileIndex >= fileCount)
	{
		return flag;
	}

	vector<PROPERTY_t>::iterator proIter = propertyVecV[fileIndex].begin();
	while (proIter != propertyVecV[fileIndex].end())
	{
		if (0 == fileIndex)
		{
			property = (*proIter);
		}
		if (property.daDanJinE <= FLT_MIN)
		{
			++proIter;
			continue;
		}
		if (0 == strcmp(property.code, proIter->code))
		{
			if ((0 < proIter->daDanJinE) 
				&& (property.daDanJinE >= proIter->daDanJinE)
				&& (property.zhongDanJinE >= proIter->zhongDanJinE)
				&& (property.daDanJingEB >= proIter->daDanJingEB)
				&& (property.zhongDanJingEB >= proIter->zhongDanJingEB)
				//&& (property.zhuLiJingLiang >= proIter->zhuLiJingLiang)
				)
			{
				fileIndex++;
				flag = shareFuseSelect(fileIndex, *proIter, propertyVecV, selectVec);
				fileIndex--;
				if (0 == flag)
				{
					//string str = property.name;
					//selectVec.push_back(str);
					flag = 0;

					if (0 != fileIndex)
					{
						break;
					}
				}
			}
			else
			{
				flag = 1;
				if (0 != fileIndex)
				{
					break;
				}
			}
		}
		++proIter;

		if (0 == fileIndex)
		{
			if (0 == flag)
			{
				string str = property.name;
				selectVec.push_back(str);
			}
		}
	}
	return flag;

	fileIndex++;
}

void shareSelectPrint(FILE *fp, PROPERTY_t &property, vector<PROPERTY_t> &propertyPre)
{
	float daDanJinBiLiuTong = property.daDanJinBiLiuTong * BI_LIUTONG;
	float zhongDanJinBiLiuTong = property.zhongDanJinBiLiuTong * BI_LIUTONG;
	float zhongXiaoDanJinBiLiuTong = property.zhongXiaoDanJinBiLiuTong * BI_LIUTONG;
	/*if (propertyPre.empty())
	{
		//printf("%8s, %8s, %7.3f亿, DDE: %3d, 资金: %3d, 主力: %3d, 涨幅: %5.2f, 三日涨幅: %5.2f, 五日涨幅: %5.2f, 十日涨幅: %5.2f, 主力净量：%5.2f, 换手：%5.2f,   量比：%5.2f, 净流入：%7.4f, 大单净额：%7.4f, 占比：%5.2f, 中单净额：%7.4f, 占比：%5.2f, 小单净额：%7.4f, 占比：%5.2f\n",
		//	ptyIter->code, ptyIter->name, ptyIter->liuTongShiZhi / DIVIDE, ptyIter->ddeIdx, zijinIndex, ptyIter->zhuliIdx, ptyIter->zhangFu, ptyIter->sanRiZhangFu, ptyIter->wuRiZhangFu, ptyIter->shiRiZhangFu, ptyIter->zhuLiJingLiang, ptyIter->huanShou, ptyIter->liangBi,
		//	ptyIter->jingLiuRu / DIVIDE, ptyIter->daDanJinE / DIVIDE, ptyIter->daDanJingEB, ptyIter->zhongDanJinE / DIVIDE, ptyIter->zhongDanJingEB, ptyIter->xiaoDanJinE / DIVIDE, ptyIter->xiaoDanJingEB);
		float neiWaiPanBi = (property.waiPan - property.neiPan) / (property.waiPan + property.neiPan);//内外盘比
		float zhuLiJingBi = property.zhuLiJinE / property.zongJinE;//主力金额 / 总金额
		fprintf(fp, "%8s,%8s,%7.3f亿,资:%3d,主:%3d,涨:%5.2f,主净:%5.2f,换:%5.2f,量:%5.2f,\
资:%4d,大:%8.2f,中:%8.2f,中小:%8.2f, | 股:%5.2f,机:%6.2f,委:%6.2f,盘比:%5.2f,主总:%5.2f,净增:%7.2f,净利:%7.4f,\
净流:%7.4f,大:%7.4f,比:%5.2f,中:%7.4f,比:%5.2f,小:%7.4f,比:%5.2f,\
三:%5.2f,五:%5.2f,十:%5.2f\n",
property.code, property.name, property.ziYouLiuTongShiZhi / DIVIDE, property.zijinIdx, property.zhuliIdx, property.zhangFu, property.zhuLiJingLiang, property.huanShou, property.liangBi,
property.zijinIdx, daDanJinBiLiuTong, zhongDanJinBiLiuTong, zhongXiaoDanJinBiLiuTong, property.guXingPingFen, property.jiGouDongXiang, property.weiBi, neiWaiPanBi, zhuLiJingBi, property.jingLiZengLv, property.jingLiRun / DIVIDE,
property.jingLiuRu / DIVIDE, property.daDanJinE / DIVIDE, property.daDanJingEB, property.zhongDanJinE / DIVIDE, property.zhongDanJingEB, property.xiaoDanJinE / DIVIDE, property.xiaoDanJingEB,
property.sanRiZhangFu, property.wuRiZhangFu, property.shiRiZhangFu);
	}
	else*/
	{
		//for(vector<PROPERTY_t>::iterator it = propertyPre.begin(); it != propertyPre.end(); it++)
		{
			//if (0 == strcmp(property.code, it->code))
			{
				float jingJiaLiangBi = property.jingJiaLiangBi;
				float zuoRiHuanShou = property.zuoRiHuanShou;
				float zuoRiLiangBi = property.zuoRiLiangBi;
				float neiWaiPanBi = (property.waiPan - property.neiPan) / (property.waiPan + property.neiPan);//内外盘比
				float zhuLiJingBi = property.zhuLiJinE / property.zongJinE;//主力金额 / 总金额

				/*float maiLiang = (property.weiCha * 100 / property.weiBi - property.weiCha) / 2;
				float zhangTingBan = property.zuoShou * 1.1 * maiLiang / 10000; //千万量级显示

				float zhanLiuBi = zhangTingBan * 1000000 / property.ziYouLiuTongShiZhi;
				if (property.weiBi > WEIBI_MAX)
				if (true == yiZiBanJudge(property))// 一字开盘
				{
					zhangTingBan = (property.buyLiang * property.xianJia) / 1000000.0;
					zhanLiuBi = zhangTingBan * 100000000.0 / property.ziYouLiuTongShiZhi;
				}
				else if (property.weiBi < -100.0) // 委差为无效值时
				{
					zhangTingBan = 0.0;
					zhanLiuBi = 0.0;
				}*/
				float zhanLiuBi = 0;
				float dianDanJinE = 0;
				float zhangTingBan = getLimitUpMoney<PROPERTY_t>(property, zhanLiuBi, dianDanJinE);
				/*if ((zuoRiHuanShou > 1.9)
				&& (zuoRiHuanShou < 15.0)
				&& (zuoRiLiangBi > 1.4)
				&& (zuoRiLiangBi < 7.0)
				&& (zuoRiHuanShou > 1.2 * zuoRiLiangBi)
				&& (jingJiaLiangBi > 5.0)
				&& (property.guXingPingFen > 40.0)
				)*/
				/*if ((zuoRiHuanShou > 1.2 * zuoRiLiangBi)
				&& (jingJiaLiangBi > 1.2 * zuoRiHuanShou)
				&& (jingJiaLiangBi > 5.0)
				&& (zuoRiLiangBi > 1.4)
				)*/
				/*if (
					(property.zuoRiHuanShou < 15.0)
					&& (jingJiaLiangBi > 10.0)
					&& (zuoRiLiangBi > 0.9)
					&& (property.zijinIdx < 999)
					)
				{
					//fprintf(fp, "#");
				}
				if (
					(
					(property.zhongXiaoDanJinBiLiuTong > 0.0)
						&& (property.zhongDanJinBiLiuTong > 0.0)
						)
					&& (property.zijinIdx < 199)
					&& (property.zuoRiHuanShou < 25.0)
					&& ((jingJiaLiangBi / zuoRiHuanShou) < 5.9)
					&& (property.zhangFu > -0.88)
					&& (zuoRiLiangBi < 4.1)
					&& (zuoRiLiangBi > 0.8)
					)
				{
					fprintf(fp, "#");
				}
				if (
					(property.weiBi < WEIBI_MIN)
					)
				{
					fprintf(fp, "*");
				}
				if (
					(property.weiBi > 30)
					&& (property.zhangFu > 5.0)
					&& (property.zuoRiZhenFu > 0.0001)
					&& ((zuoRiLiangBi < 4.1) && ((jingJiaLiangBi / zuoRiHuanShou) < 5.9))
					)
				{
					fprintf(fp, "*");
				}
				if ((property.zuoRiZuiGao - property.zuoRiKaiPan) < 0.0001)
				{
					fprintf(fp, "-"); //T字板
					if (property.zuoRiZhenFu < 0.0001) //昨日一字板
					{
						fprintf(fp, "-");
						if ((property.waiPan < 0.0001) && (property.weiBi > WEIBI_MAX)) //一字开盘，连续一字板
						if (true == yiZiBanJudge(property))// 一字开盘
						{
							fprintf(fp, "-");
						}
					}
				}
				int ztTime = atoi(property.lastLimitTime);
				if (
					(ztTime < 103001)
					&& (property.limitOpenCount < 2)
					&& (property.zuoRiZhenFu > 3.0001)
					&& (zuoRiLiangBi < 4.1)
					&& ((jingJiaLiangBi / zuoRiHuanShou) < 5.9)
					)
				{
					fprintf(fp, "=");
				}
				//弱转强
				if (
					(property.zhangFu > 1.8)
					&& (property.limitOpenCount > 3)
					&& (property.zuoRiZhenFu > 3.0001)
					&& (zuoRiLiangBi < 4.1)
					&& ((jingJiaLiangBi / zuoRiHuanShou) < 5.9)
					)
				{
					fprintf(fp, "^");
				}*/
				string printFlag = getShareFlag<PROPERTY_t>(property);
				//float yuanMeiShou = ((float)property.shouMeiBi * property.xianJia) / 10000;
				/*fprintf(fp, "%-8s, %6.2f, %2s%-8s,昨:%6.2f,涨:%6.2f,原:%-16s, 比:%5.2f, __成:%5.2f,资:%4d,出:%4d,成交:%5.0f,昨竞:%5.0f,连:%2d,  竞比:%6.2f,委:%7.2f,涨停:%9.2f,垫:%9.2f, 昨成:%6.0f,昨停:%5.0f, 开:%2d, \
| %7.3f亿,股:%5.2f, 主净:%5.2f,昨换:%5.2f,昨量:%5.2f,HY:%-14s, | \
大:%8.2f,中:%8.2f,中小:%8.2f, | 全:%5.2f,净流:%4d,总入:%4d,总出:%4d,成交:%4d,主:%3d,换:%5.2f,量:%5.2f,\
始:%6s,末:%6s,封:%3d,机:%6.2f,盘比:%5.2f,主总:%5.2f,净增:%7.2f,净利:%7.4f,\
净流:%7.4f,大:%7.4f,比:%5.2f,中:%7.4f,比:%5.2f,小:%7.4f,比:%5.2f\n",
//三:%5.2f,五:%5.2f,十:%5.2f\n", 
property.code, property.xianJia, printFlag.c_str(), property.name, property.zuoRiKaiPanZhangFu, property.zhangFu, property.limitReason, property.zongLiuRuBiZuoRiZongJinE*100.0, property.zongLiuRuBiLiuTong*100.0, property.zijinIdx, property.zijinChuIdx, property.zongJinE/TENTHOUSAND, property.preJingJiaZongJinE/TENTHOUSAND, property.continueDay, jingJiaLiangBi, property.weiBi, zhangTingBan, dianDanJinE, property.zuoRiZongJinE/TENTHOUSAND, property.limitUpMoney/TENTHOUSAND, property.limitOpenCount,
property.ziYouLiuTongShiZhi / DIVIDE, property.guXingPingFen, property.zhuLiJingLiang, zuoRiHuanShou, zuoRiLiangBi, property.suoShuHangYe,
daDanJinBiLiuTong, zhongDanJinBiLiuTong, zhongXiaoDanJinBiLiuTong, property.jingLiuRu / TENTHOUSAND, property.jingLiuRuBiLiuTongIndex, property.zongLiuRuBiLiuTongIndex, property.zongLiuChuBiLiuTongIndex, property.chengJiaoBiLiuTongIndex, property.ddeIdx, property.huanShou, property.liangBi,
property.firstLimitTime, property.lastLimitTime, property.indexLvsC, property.jiGouDongXiang, property.zongLiuRuBiLiuTong*100.0, zhuLiJingBi, property.jingLiZengLv, property.jingLiRun / DIVIDE,
property.jingLiuRu / DIVIDE, property.daDanJinE / DIVIDE, property.daDanJingEB, property.zhongDanJinE / DIVIDE, property.zhongDanJingEB, property.xiaoDanJinE / DIVIDE, property.xiaoDanJingEB
//property.sanRiZhangFu, property.wuRiZhangFu, property.shiRiZhangFu
);*/
				fprintf(fp, "%-8s, %6.2f, %2s%-8s,昨:%6.2f,涨:%6.2f,涨:%6.2f,原:%-16s, 比:%5.2f, __成:%5.2f,资:%4d,成交:%6.0f,昨:%5.0f,连:%2d,  竞比:%6.2f,委:%7.2f,涨停:%9.2f,垫:%9.2f, 昨成:%6.0f,昨停:%5.0f, 开:%2d, \
| %7.3f亿,股:%5.2f,HY:%-14s, | \
换:%5.2f,量:%5.2f,\
始:%6s,末:%6s\n",
//三:%5.2f,五:%5.2f,十:%5.2f\n", 
property.code, property.xianJia, printFlag.c_str(), property.name, property.zuoRiKaiPanZhangFu, property.zhangFu, property.zuoRiZhangFu, property.limitReason, property.zongLiuRuBiZuoRiZongJinE*100.0, property.zongLiuRuBiLiuTong*100.0, property.zijinIdx, property.zongJinE / TENTHOUSAND, property.preJingJiaZongJinE / TENTHOUSAND, property.continueDay, jingJiaLiangBi, property.weiBi, zhangTingBan, dianDanJinE, property.zuoRiZongJinE / TENTHOUSAND, property.limitUpMoney / TENTHOUSAND, property.limitOpenCount,
property.ziYouLiuTongShiZhi / DIVIDE, property.guXingPingFen, property.suoShuHangYe,
property.huanShou, property.liangBi,
property.firstLimitTime, property.lastLimitTime
);
				/*fprintf(fp, "%-8s, %6.2f, %-8s,昨:%6.2f,涨:%6.2f,原:%-16s, 成交:%6.0f,昨竞:%5.0f,连:%2d, 涨停:%9.2f, 开:%2d, \
| %7.3f亿,股:%5.2f, HY:%-14s, | \
始:%6s,末:%6s\n",
property.code, property.xianJia, property.name, property.zuoRiKaiPanZhangFu, property.zhangFu, property.limitReason, property.zongJinE / TENTHOUSAND, property.preJingJiaZongJinE / TENTHOUSAND, property.continueDay, zhangTingBan, property.limitOpenCount,
property.ziYouLiuTongShiZhi / DIVIDE, property.guXingPingFen, property.suoShuHangYe,
property.firstLimitTime, property.lastLimitTime
);*/
			}
		}
	}
}

void calculateOtherPara(FILE *fp, vector<PROPERTY_t> &propertyVec, vector<PROPERTY_t> &propertyAnalyVec, vector<zhangTing_t> &zhangTingVec)
{
	if (propertyVec.empty())
	{
		return;
	}
	float averageIncrease = 0.0;
	int   codeCount = 0;
	int   zhangCount = 0;
	int   dieCount = 0;
	int   zuoRiYiZiCount = 0;
	int   stCount = 0;
	int   yiZiBanCount = 0;
	int   xinGuCount = 0;
	vector<PROPERTY_t>::iterator proIter = propertyVec.begin();
	while (proIter != propertyVec.end())
	{		
		//set default zhang ting para
		{
			float zhanLiuBi = 0;
			float dianDanJinE = 0;
			float zhangTingBan = getLimitUpMoney<PROPERTY_t>((*proIter), zhanLiuBi, dianDanJinE);

			//proIter->indexLvsC = ztIter->indexLvsC;
			//proIter->ziYouLiuTongGu = ztIter->ziYouLiuTongGu;
			//proIter->guXingPingFen = ztIter->guXingPingFen;
			proIter->continueDay = 0;
			proIter->limitOpenCount = 0;
			proIter->limitVsDeal = zhangTingBan / proIter->zongJinE;
			proIter->limitVsCirculate = zhangTingBan / proIter->ziYouLiuTongShiZhi;
			proIter->limitUpMoney = zhangTingBan;
			strcpy(proIter->firstLimitTime, "150000");
			strcpy(proIter->lastLimitTime, "150000");
			strcpy(proIter->limitReason, "未涨停");
		}
		vector<zhangTing_t>::iterator ztIter = zhangTingVec.begin();
		while (ztIter != zhangTingVec.end())
		{
			char codeTemp[10];
			memset(codeTemp, 0, sizeof(codeTemp));
			char *pCode = proIter->code;
			strncpy(codeTemp, &pCode[2], 6);

			if (0 == strcmp(codeTemp, ztIter->code))
			{
				proIter->indexLvsC = ztIter->indexLvsC;
				proIter->ziYouLiuTongGu = ztIter->ziYouLiuTongGu;
				proIter->guXingPingFen = ztIter->guXingPingFen;
				proIter->continueDay = ztIter->continueDay;
				proIter->limitOpenCount = ztIter->limitOpenCount;
				proIter->limitVsDeal = ztIter->limitVsDeal;
				proIter->limitVsCirculate = ztIter->limitVsCirculate;
				proIter->limitUpMoney = ztIter->limitUpMoney;
				strcpy(proIter->firstLimitTime, ztIter->firstLimitTime);
				strcpy(proIter->lastLimitTime, ztIter->lastLimitTime);
				strcpy(proIter->limitReason, ztIter->limitReason);
				break;
			}
			++ztIter;
		}

		float zuiXinLiuTongShiZhi = proIter->ziYouLiuTongGu * proIter->xianJia;
		//float zuiXinLiuTongShiZhi = proIter->liuTongShiZhi;

		proIter->ziYouLiuTongShiZhi = zuiXinLiuTongShiZhi;

		proIter->jingLiuRuBiLiuTong = (proIter->jingLiuRu) / zuiXinLiuTongShiZhi;

		proIter->neiWaiPanBiLiuTongGu = (proIter->waiPan - proIter->neiPan) * 100.0 / proIter->ziYouLiuTongGu;

		proIter->zhuLiJingBiLiuTong = proIter->zhuLiJingE / zuiXinLiuTongShiZhi;

		proIter->daDanLiuRuBiLiuTong = (proIter->daDanLiuRu) / zuiXinLiuTongShiZhi;
		proIter->daDanLiuChuBiLiuTong = (proIter->daDanLiuChu) / zuiXinLiuTongShiZhi;
		proIter->zhongXiaoLiuRuBiLiuTong = (proIter->zhongDanLiuRu + proIter->xiaoDanLiuRu) / zuiXinLiuTongShiZhi;
		proIter->daXiaoLiuRuBiLiuTong = (proIter->daDanLiuRu + proIter->xiaoDanLiuRu) / zuiXinLiuTongShiZhi;
		proIter->xiaoDanLiuRuBiLiuTong = (proIter->xiaoDanLiuRu) / zuiXinLiuTongShiZhi;
		proIter->zhongDanLiuRuBiLiuTong = (proIter->zhongDanLiuRu) / zuiXinLiuTongShiZhi;
		proIter->daZhongLiuRuBiLiuTong = (proIter->daDanLiuRu + proIter->zhongDanLiuRu) / zuiXinLiuTongShiZhi;
		if (!isfinite(proIter->daZhongLiuRuBiLiuTong)) { proIter->daZhongLiuRuBiLiuTong = 0.0; }
		proIter->zongLiuRuBiLiuTong = (proIter->daDanLiuRu + proIter->zhongDanLiuRu + proIter->xiaoDanLiuRu) / zuiXinLiuTongShiZhi;
		if (!isfinite(proIter->zongLiuRuBiLiuTong)) { proIter->zongLiuRuBiLiuTong = 0.0; }
		proIter->zongLiuChuBiLiuTong = (proIter->daDanLiuChu + proIter->zhongDanLiuChu + proIter->xiaoDanLiuChu) / zuiXinLiuTongShiZhi;
		if (!isfinite(proIter->zongLiuChuBiLiuTong)) { proIter->zongLiuChuBiLiuTong = 0.0; }

		//proIter->chengJiaoBiLiuTong = (proIter->zongShou*proIter->xianJia) / zuiXinLiuTongShiZhi;
		proIter->chengJiaoBiLiuTong = (proIter->zongJinE) / zuiXinLiuTongShiZhi;

		//proIter->chengJiaoJinE = (proIter->zongShou*proIter->xianJia);

		proIter->zhongXiaoBiJingLiuRu = (proIter->zhongDanJinE + proIter->xiaoDanJinE) / proIter->jingLiuRu;

		proIter->daDanJinBiLiuTong = proIter->daDanJinE / zuiXinLiuTongShiZhi;
		proIter->zhongDanJinBiLiuTong = proIter->zhongDanJinE / zuiXinLiuTongShiZhi;
		proIter->xiaoDanJinBiLiuTong = proIter->xiaoDanJinE / zuiXinLiuTongShiZhi;
		proIter->zhongXiaoDanJinBiLiuTong = (proIter->zhongDanJinE + proIter->xiaoDanJinE) / zuiXinLiuTongShiZhi;
		proIter->zhongXiaoDanZongBiLiuTong = (proIter->zhongDanZongE + proIter->xiaoDanZongE) / zuiXinLiuTongShiZhi;

		float zhanLiuBi = 0;
		float dianDanJinE = 0;
		float zhangTingBan = getLimitUpMoney<PROPERTY_t>((*proIter), zhanLiuBi, dianDanJinE);
		proIter->zhangTingBan = zhangTingBan;
		proIter->dianDanJinE = dianDanJinE;
		if (0.0 < zhangTingBan)
		{
			proIter->limitVsDealJingJia = zhangTingBan / proIter->zongJinE;
			proIter->limitVsCirculateJingJia = zhangTingBan / proIter->ziYouLiuTongShiZhi;
		}

		vector<PROPERTY_t>::iterator proPreIter = propertyAnalyVec.begin();
		while (proPreIter != propertyAnalyVec.end())
		{
			if (0 == strcmp(proIter->code, proPreIter->code))
			{
				float jinRiZongShou = proIter->zongShou;
				if (proIter->zongShou < 0.1)
				{
					jinRiZongShou = proIter->buyLiang;
				}
				proIter->jingJiaLiangBi = jinRiZongShou / proPreIter->zongShou * 100.0;
				//proIter->jingJiaLiangBi = jinRiZongShou / proPreIter->ziYouLiuTongGu * 10000.0;
				proIter->zuoRiHuanShou = proPreIter->huanShou;
				proIter->zuoRiKaiPan = proPreIter->kaiPan;
				proIter->zuoRiZuiGao = proPreIter->zuiGao;
				proIter->zuoRiLiangBi = proPreIter->liangBi;
				proIter->zuoRiZhenFu = proPreIter->zhenFu;
				proIter->zuoRiZhangFu = proPreIter->zhangFu;
				proIter->zuoRiZhuLiJingLiang = proPreIter->zhuLiJingLiang;
				proIter->zuoRiDaDanJingEB = proPreIter->daDanJingEB;
				proIter->zuoRiJiGouDongXiang = proPreIter->jiGouDongXiang;
				proIter->zuoRiSanHuShuLiang = proPreIter->sanHuShuLiang;
				proIter->zuoRiZongJinE = proPreIter->zongJinE;
				//proIter->zongLiuRuBiZuoRiZongJinE = (proIter->daDanLiuRu + proIter->zhongDanLiuRu + proIter->xiaoDanLiuRu) / proIter->zuoRiZongJinE;
				//proIter->zongLiuRuBiZuoRiZongJinE = (proIter->waiPan) / proPreIter->zongShou;
				//proIter->zongLiuRuBiZuoRiZongJinE = proIter->liangBi;//(proIter->waiPan - proIter->neiPan) / proIter->ziYouLiuTongGu;
				//proIter->zongLiuRuBiZuoRiZongJinE = proIter->waiPan * proIter->xianJia / proIter->liuTongShiZhi;
				proIter->zongLiuRuBiZuoRiZongJinE = proIter->zhuLiJingE / zuiXinLiuTongShiZhi;
				if (!isfinite(proIter->zongLiuRuBiZuoRiZongJinE)) { proIter->zongLiuRuBiZuoRiZongJinE = -100.0; }
				//proIter->zongLiuRuBiZuoRiZongJinE = proIter->zhuLiJingLiang;
				proIter->zuoRiKaiPanZhangFu = (proPreIter->kaiPan - proPreIter->zuoShou) * 100.0 / proPreIter->zuoShou;
				proIter->preZuoShou = proPreIter->zuoShou;

				float zuoRiLiuTongShiZhi = proIter->ziYouLiuTongGu * proIter->zuoShou;
				proIter->zuoRiJingLiuRuBiLiuTong = (proPreIter->jingLiuRu) / zuoRiLiuTongShiZhi;
				proIter->zuoRidaDanJinBiLiuTong = proPreIter->daDanJinE / zuoRiLiuTongShiZhi;
				proIter->zuoRidaDanLiuRuBiLiuTong = proPreIter->daDanLiuRu / zuoRiLiuTongShiZhi;
				proIter->zuoRiDaDanLiuChuBiLiuTong = proPreIter->daDanLiuChu / zuoRiLiuTongShiZhi;
				proIter->zuoRiChengJiaoBiLiuTong = proPreIter->zongJinE / zuoRiLiuTongShiZhi;

				//计算平均涨幅
				zhangFuSummarize(*proIter, zhangCount, dieCount, yiZiBanCount, xinGuCount, stCount, averageIncrease, codeCount);
				if(0)
				{
					string name = proIter->name;
					std::size_t found = name.find("ST");
					if (found != std::string::npos)//剔除ST股的统计
					{
						stCount++;
						//break;
					}
					//else
					{
						if (0 == strcmp(proIter->limitReason, NEW_SHARE.c_str()))
						{
							xinGuCount++;
							break;
						}
						if (
							(proPreIter->zhenFu < 0.001) 
							&& (true == yiZiBanJudge(*proIter))// 一字开盘
							)//不统计连续一字板
						{							
							zuoRiYiZiCount++;
							//break;
						}
						averageIncrease += proIter->zhangFu;
						codeCount++;

						if (proIter->zhangFu >= 0)
						{
							zhangCount++;
						}
						else
						{
							dieCount++;
						}
						if (true == yiZiBanJudge(*proIter))// 一字开盘
						{
							yiZiBanCount++;
						}
					}
				}
				break;
			}
			++proPreIter;
		}
		++proIter;
	}
	float shangZhangLv = (float)zhangCount / (float)(zhangCount + dieCount);
	float avgZhang = averageIncrease / (float)codeCount;
	fprintf(fp, "今日一字开盘:%d, 上涨率:%5.2f, 平均涨幅:%5.2f, 上涨数:%d, 下跌数:%d, 新股:%d, 连续一字板:%d, ST股数:%d\n",
		yiZiBanCount, shangZhangLv, avgZhang, zhangCount, dieCount, xinGuCount, zuoRiYiZiCount, stCount);
}

void chooseAnalyzeProperty(vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec)
{
	if ((propertyVec.empty()) || (analyVec.empty()))
	{
		return;
	}
	int analySize = analyVec.size();
	//printf("analySize:%d\n", analySize);
	propertyAnalyVec.clear();
	propertyAnalyVec.reserve(analySize);
	vector<PROPERTY_t>::iterator proIter = propertyVec.begin();
	while (proIter != propertyVec.end())
	{
		//保存需要分析的item信息
		for (int analyIdx = 0; analyIdx < analySize; analyIdx++)
		{
			if (0 == strcmp(proIter->code, analyVec[analyIdx].code))
			{
				propertyAnalyVec.push_back(*proIter);
				break;
			}
		}
		++proIter;
	}
}

bool yiZiBanJudge(PROPERTY_t &property)
{
	bool flag = false;
	if (
		(property.sellLiant < 1.0)
		&& (property.weiBi > WEIBI_MAX)
		)// 一字开盘
	{
		flag = true;
	}
	return flag;
}

void zhangFuSummarize(PROPERTY_t &property, int &zhangCount, int &dieCount, int &yiZiBanCount, int &xinGuCount, int &stCount, float &averageIncrease, int &codeCount)
{
	//计算平均涨幅
	{
		string name = property.name;
		std::size_t found = name.find("ST");
		if (found != std::string::npos)//剔除ST股的统计
		{
			stCount++;
			//break;
		}
		//else
		{
			if (0 == strcmp(property.limitReason, NEW_SHARE.c_str()))
			{
				xinGuCount++;
				return;
			}
			averageIncrease += property.zhangFu;
			codeCount++;

			if (property.zhangFu >= 0)
			{
				zhangCount++;
			}
			else
			{
				dieCount++;
			}
			if (true == yiZiBanJudge(property))// 一字开盘
			{
				yiZiBanCount++;
			}
		}
	}
}
