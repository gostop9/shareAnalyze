//#include <winsock2.h>
//#include <windows.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
//#include <wchar.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include "dataStructure.h"
#include "dataProc.h"
#include "shareSelect.h"
#include "shareRaisingLtdPre.h"
#include "dataLineProc.h"
#include "readGuBen.h"
#include "readZhangTing.h"
#include "getLimitUpInfo.h"
#include "shareDef.h"
#include "tdxCfgModify.h"
#include "autoShareBuy.h"

using namespace std;
using namespace DATAPROC;
using namespace GUBEN;
using namespace ZHANGTING;
using namespace GETLIMITUPINFO;
using namespace SHAREDEF;
using namespace TDXCFGMODIFY;
using namespace AUTOSHAREBUY;

char configFileName[] = "D:/share/config.txt";
char resultFileName[30] = "D:/share/result.txt";
char preFileName[] = "D:/share/searchPre.txt";
char buyFileName[] = "D:/share/buyShare.txt";

bool compZiJin(const ZIJIN_t &a, const ZIJIN_t &b) {
	     return a.jingLiuRu > b.jingLiuRu;
}

bool compZiJinFuseJingLiuRu(const PROPERTY_t &a, const PROPERTY_t &b) {
	return a.zhongDanLiuRuBiLiuTong > b.zhongDanLiuRuBiLiuTong;
}

bool compZiJinFuseDaDanJingE(const PROPERTY_t &a, const PROPERTY_t &b) {
	return a.zhuLiJingE > b.zhuLiJingE;
}
/*
void propertyVecChuBubbleSort(vector<PROPERTY_t> &propertyVec)
{
	int n = propertyVec.size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = n - 1; j >= i + 1; j--)
		{
			if (propertyVec[j - 1].zongLiuChuBiLiuTong < propertyVec[j].zongLiuChuBiLiuTong)
			{
				PROPERTY_t temp = propertyVec[j - 1];
				propertyVec[j - 1] = propertyVec[j];
				propertyVec[j] = temp;
			}
		}
	}
}
void propertyVecRuBubbleSort(vector<PROPERTY_t> &propertyVec)
{
	int n = propertyVec.size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = n - 1; j >= i + 1; j--)
		{
			if (propertyVec[j - 1].zongLiuRuBiLiuTong < propertyVec[j].zongLiuRuBiLiuTong)
			{
				PROPERTY_t temp = propertyVec[j - 1];
				propertyVec[j - 1] = propertyVec[j];
				propertyVec[j] = temp;
			}
		}
	}
}
//出现isfinite值，导致排序区域中断
void propertyVecDaDanBubbleSort(vector<PROPERTY_t> &propertyVec)
{
	int n = propertyVec.size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = n - 1; j >= i + 1; j--)
		{
			if (propertyVec[j - 1].zongLiuRuBiZuoRiZongJinE < propertyVec[j].zongLiuRuBiZuoRiZongJinE)
			{
				PROPERTY_t temp = propertyVec[j - 1];
				propertyVec[j - 1] = propertyVec[j];
				propertyVec[j] = temp;
			}
		}
	}
}
*/

float huanShouMax = 25.0F;
float liangBiMin = 1.2F;
float liangBiMax = 6.0F;
float zhuLiJingLiangMin = 0.08F;
float daDanJingEBMin = 0.08F;

void main()
{
	vector<guBen_t> guBenVec;

	//结果code保存
	std::vector<std::string> resultSet;

	// cfg file
	FILE *cfgFp;
	cfgFp = fopen(configFileName, "rt");
	if (NULL == cfgFp)
	{
		printf("Open config file failed!\n");
		return;
	}
	char guBenFileName[100];
	char ddeFileName[100], zijinName[100], zhuliFileName[100], zhangfuFileName[100], zhangtingFileName[100], zhangtingFileNameToday[100];
	char date[20], path[100];
	char dataPre[20];
	memset(date, 0, sizeof(date));
	memset(path, 0, sizeof(path));
	memset(dataPre, 0, sizeof(date));
	int fileIndex, ddeSelectCount, zijinSelectCount, zhuliSelectCount;

	int itemNum = 0;
	vector<std::string> strVec;
	while (fgets(lineDateBuf, lineDataLen, cfgFp) != NULL)
	{
		std::string str = lineDateBuf;
		strVec.push_back(str);

		memset(lineDateBuf, 0, lineDataLen);

		itemNum++;
	}
	int strIdx = 0;
	int cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
	strncpy(date, strVec[strIdx++].c_str(), cpyLen);

	fileIndex = atoi(strVec[strIdx++].c_str());

	cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
	strncpy(path, strVec[strIdx++].c_str(), cpyLen);

	// result file name
	memset(resultFileName, 0, sizeof(resultFileName));
	strcpy(resultFileName, path);
	cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
	strncat(resultFileName, strVec[strIdx++].c_str(), cpyLen);

	ddeSelectCount = atoi(strVec[strIdx++].c_str());
	zijinSelectCount = atoi(strVec[strIdx++].c_str());
	zhuliSelectCount = atoi(strVec[strIdx++].c_str());

	huanShouMax = atof(strVec[strIdx++].c_str());
	liangBiMin = atof(strVec[strIdx++].c_str());
	liangBiMax = atof(strVec[strIdx++].c_str());
	zhuLiJingLiangMin = atof(strVec[strIdx++].c_str());
	daDanJingEBMin = atof(strVec[strIdx++].c_str());

	cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
	strncpy(dataPre, strVec[strIdx++].c_str(), cpyLen);

	//提取需要分析的名称
	int analyseNum = itemNum - strIdx;
	vector<analyseCode_t> analyseVec;
	analyseVec.reserve(analyseNum);
	vector<analyseCode_t> limitTodayVec;
	limitTodayVec.reserve(analyseNum);
	for (int i = strIdx; i < itemNum; i++)
	{
		char tempBuf[10];
		memset(tempBuf, 0, sizeof(tempBuf));
		int cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
		strncpy(tempBuf, strVec[strIdx++].c_str(), cpyLen);
		analyseCode_t anaCode;
		strcpy(anaCode.code, tempBuf);
		analyseVec.push_back(anaCode);
	}
	vector<analyseCode_t> independentAnalyVec = analyseVec;

	fclose(cfgFp);

	//result file
	FILE *rstFp;
	rstFp = fopen(resultFileName, "at+");
	if (NULL == rstFp)
	{
		printf("Open result file failed!\n");
		return;
	}

	// out file name
	char outFile[20], outIdx[20];
	strcpy(outFile, date);
	strcat(outFile, _itoa(fileIndex, outIdx, 10)); // 10进制
	fprintf(rstFp, "%s\n", outFile);

	//shareRaisingLtdPre(preFileName, date, path, 3);

	vector<vector<DDE_t>> dde;
	vector<vector<ZIJIN_t>> zijin;
	vector<vector<ZHULI_t>> zhuli;
	vector<vector<ZHANGFU_t>> zhangfu;
	vector<vector<PROPERTY_t>> propertyV;
	vector<vector<ZIJIN_t>> zijinJingLiuRu;
	vector<vector<PROPERTY_t>> propertyVecJingLiuRu;

	int fileSize = (fileIndex > 1) ? fileIndex : 1;
	dde.resize(fileSize);
	zijin.resize(fileSize);
	zhuli.resize(fileSize);
	zhangfu.resize(fileSize);
	propertyV.resize(fileSize);
	zijinJingLiuRu.resize(fileSize);
	propertyVecJingLiuRu.resize(fileSize);

	//提取前一日数据
	vector<PROPERTY_t> propertyAnalyVecPre;
	//前一日涨停数据
	vector<zhangTing_t> zhangTingVecPre;
	zhangTingVecPre.clear();
	//今日涨停数据
	vector<zhangTing_t> zhangTingVecToday;
	zhangTingVecToday.clear();
	//只进行竞价数据比较
	//if(1 >= fileIndex)
	{
		zhuLiJingLiangMin = 2.0;

		fileNameCat(ddeFileName,
			zijinName,
			zhuliFileName,
			zhangfuFileName,
			zhangtingFileName,
			guBenFileName,
			path,
			dataPre);

		readGuBenFile(guBenFileName, guBenVec);

		memset(zhangtingFileNameToday, 0, sizeof(zhangtingFileNameToday));
		strcpy(zhangtingFileNameToday, path);
		strcat(zhangtingFileNameToday, "zhangting_");
		strncat(zhangtingFileNameToday, date, 8);
		strcat(zhangtingFileNameToday, ".txt");

		//vector<DDE_t> dde;
		//readDdeFile(ddeFileName, dde[0]);
		//vector<ZIJIN_t> zijin;
		readZijinFile(zijinName, zijin[0]);
		//vector<ZHULI_t> zhuli;
		readZhuliFile(zhuliFileName, zhuli[0]);

		int cnt_int = atoi(date);
		if (cnt_int > 20180525)
		{
			//vector<ZHANGFU_t> zhangfu;
			readZhangfuFile(zhangfuFileName, zhangfu[0]);
		}
		
		//提取昨日涨停code
		readZhangTingFile(zhangtingFileName, zhangTingVecPre);
		getZhangTingCode(zhangTingVecPre, analyseVec);
		
		//提取今日涨停code
		readZhangTingFile(zhangtingFileNameToday, zhangTingVecToday);
		getZhangTingCode(zhangTingVecToday, limitTodayVec);

		shareParaFuse(dde[0], zijin[0], zhuli[0], zhangfu[0], guBenVec, propertyV[0], analyseVec, propertyAnalyVecPre);
		//chooseAnalyzeProperty(propertyV[0], analyseVec, propertyAnalyVecPre);
		propertyAnalyVecPre = propertyV[0];
	}

	//用于zhuLiJingLiangMin，不用每次进行修改
	if ((fileIndex > 1) && (fileIndex < 5))
	{
		zhuLiJingLiangMin = float(fileIndex)/ float(10.0);
	}

	for (int iFile = fileIndex; iFile >= 0; iFile--)
	{
		char indexString[5];
		_itoa(iFile, indexString, 10);

		char dateNow[15];
		strcpy(dateNow, date);
		strcat(dateNow, indexString);

		fileNameCat(ddeFileName,
			zijinName,
			zhuliFileName,
			zhangfuFileName,
			zhangtingFileName,
			guBenFileName,
			path,
			dateNow);

		int vecIndex = fileIndex - iFile;
		//vector<DDE_t> dde;
		//readDdeFile(ddeFileName, dde[vecIndex]);
		//vector<ZIJIN_t> zijin;
		bool zijinFileFlag = readZijinFile(zijinName, zijin[vecIndex]);
		vector<ZIJIN_t > ziinDaDanJingE = zijin[vecIndex];
		//std::sort(ziinDaDanJingE.begin(), ziinDaDanJingE.end(), compZiJin);
		//vector<ZHULI_t> zhuli;
		bool zhuliFileFlag = readZhuliFile(zhuliFileName, zhuli[vecIndex]);

		int cnt_int = atoi(date);
		if (cnt_int > 20180525)
		{
			//vector<ZHANGFU_t> zhangfu;
			readZhangfuFile(zhangfuFileName, zhangfu[vecIndex]);
		}

		vector<PROPERTY_t> propertyAnalyVec;
		if (true == zhuliFileFlag)
		{
			shareParaFuse(dde[vecIndex], ziinDaDanJingE, zhuli[vecIndex], zhangfu[vecIndex], guBenVec, propertyV[vecIndex], analyseVec, propertyAnalyVec);
		}
		else
		{
			shareParaFuseWithoutZhuli(ziinDaDanJingE, zhangfu[vecIndex], guBenVec, propertyV[vecIndex], analyseVec, propertyAnalyVec);
		}

		//根据实际流通计算其他参数
		calculateOtherPara(rstFp, propertyV[vecIndex], propertyAnalyVecPre, zhangTingVecPre);

		vector<PROPERTY_t> &propertyAnalyVecForSort = propertyV[vecIndex];
		// sort for zijinChuIdx
		{
			struct zongLiuChuBiLiuTong_t {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuChuBiLiuTong > b.zongLiuChuBiLiuTong); }
			} zongLiuChuBiLiuTong_c;

			std::sort(propertyAnalyVecForSort.begin(), propertyAnalyVecForSort.end(), zongLiuChuBiLiuTong_c);
			//propertyVecChuBubbleSort(propertyAnalyVecForSort);
			vector<PROPERTY_t>::iterator proIter = propertyAnalyVecForSort.begin();
			int ziJinIndex = 1;
			while (proIter != propertyAnalyVecForSort.end())
			{
				proIter->zijinChuIdx = ziJinIndex++;
				++proIter;
			}
		}
		// sort for liuRuBiZuoRiChengJiaoIdx
		{
			struct myclass {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuRuBiZuoRiZongJinE > b.zongLiuRuBiZuoRiZongJinE); }
			} liuRuBiZuoRiChengJiao;
			//propertyVecDaDanBubbleSort(propertyAnalyVecForSort);
			std::sort(propertyAnalyVecForSort.begin(), propertyAnalyVecForSort.end(), liuRuBiZuoRiChengJiao);
			vector<PROPERTY_t>::iterator proIter = propertyAnalyVecForSort.begin();
			int ziJinIndex = 1;
			while (proIter != propertyAnalyVecForSort.end())
			{
				proIter->liuRuBiZuoRiChengJiaoIdx = ziJinIndex++;
				++proIter;
			}
		}
		// sort for zijinIndex
		{
			struct myclass {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuRuBiLiuTong > b.zongLiuRuBiLiuTong); }
			} neiWaiPanBiLiuTongGu;

			std::sort(propertyAnalyVecForSort.begin(), propertyAnalyVecForSort.end(), neiWaiPanBiLiuTongGu);
			//propertyVecRuBubbleSort(propertyAnalyVecForSort);
			vector<PROPERTY_t>::iterator proIter = propertyAnalyVecForSort.begin();
			int ziJinIndex = 1;
			while (proIter != propertyAnalyVecForSort.end())
			{
				proIter->zijinIdx = ziJinIndex++;
				++proIter;
			}
		}

		time_t timep;
		time(&timep);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%H%M%S", localtime(&timep));
		int localTime = atoi(tmp);
		struct myclass {//zhongXiaoDanJinBiLiuTong jingLiuRuBiLiuTong
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhuLiJingLiang < b.zhuLiJingLiang); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.jingLiuRuBiLiuTong < b.jingLiuRuBiLiuTong); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.chengJiaoBiLiuTong > b.chengJiaoBiLiuTong); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongXiaoDanJinBiLiuTong > b.zhongXiaoDanJinBiLiuTong); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.zhongDanJingEB + a.xiaoDanJingEB - a.zhuLiJingLiang) > (b.zhongDanJingEB + b.xiaoDanJingEB - b.zhuLiJingLiang)); }
			//bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return ((a.zhongDanJingEB + a.xiaoDanJingEB  - a.zhuLiJingE / a.ziYouLiuTongShiZhi) > (b.zhongDanJingEB + b.xiaoDanJingEB- b.zhuLiJingE / b.ziYouLiuTongShiZhi)); }
		} cmpMethod;

		struct jingJia {
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.jingJiaLiangBi > b.jingJiaLiangBi); }
		} cmpMethodJingJia;

		vector<PROPERTY_t> &propertyAnalyVecSort = propertyV[vecIndex];
		//if ((localTime < 92500) && (localTime > 91500)) //  befor time 9:25
		if (0 == fileIndex)
		{
			std::sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), cmpMethodJingJia);
		}
		else
		{
			//std::sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), cmpMethod);
		}

		// sort all item
		/*limitUpReason_t limitupInfo;
		if (1 == fileIndex)
		{
			limitupInfo.limitShareOrdering(propertyAnalyVecSort);
		}*/

		chooseAnalyzeProperty(propertyAnalyVecSort, analyseVec, propertyAnalyVec);
		//std::sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), compZiJinFuseDaDanJingE);

		//按涨停时间排序
		/*struct ztTime{
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.firstLimitTime) < atoi(b.firstLimitTime)); }
		} cmpZtTime;
		std::sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpZtTime);*/

		vector<string> newShareCodeVec;
		PROPERTY_t programBuyProty;
		int programFindFlag = 0;
		FILE *buyFp;
		buyFp = fopen(buyFileName, "at+");
		if (NULL == buyFp)
		{
			printf("Open buy file failed!\n");
			return;
		}
		int analyNum = propertyAnalyVec.size();
		if (1 >= fileIndex)
		{
			// sort all item
			limitUpReason_t limitupInfo;
			if (1 == fileIndex)
			{
				limitupInfo.limitShareOrdering(propertyAnalyVec);

				//程序下单
				//autoShareBuy(buyFp, propertyAnalyVec, programBuyProty, programFindFlag);
			}

			//提取涨停原因
			fprintf(rstFp, "涨停种类------------------------------------------------------------------------------------------------------------------------\n");
			vector<limitUpReason_t> ztrVec;
			ztrVec.clear();
			ztrVec.reserve(analyNum);
			limitupInfo.getLimitUpReason(propertyAnalyVec, ztrVec, newShareCodeVec);

			//排序并打印结果
			limitupInfo.limitShareSort(rstFp, ztrVec);

			if (0)
			{
				//提取所属行业
				fprintf(rstFp, "行业分类------------------------------------------------------------------------------------------------------------------------\n");
				vector<limitUpReason_t> hyVec;
				hyVec.clear();
				hyVec.reserve(analyNum);
				limitupInfo.getLimitUpHy(propertyAnalyVec, hyVec, newShareCodeVec);

				//排序并打印结果
				limitupInfo.limitShareSort(rstFp, hyVec);
			}

			//保存连续涨停数据
			limitupInfo.continueLimitSave(propertyAnalyVec, limitTodayVec);
		}
		fprintf(rstFp, "------------------------------------------------------------------------------------------------------------------------\n");
		//print 最新
		if (iFile == fileIndex)
		{

			resultSet.clear();

			fprintf(rstFp, "%s 按大单净额排序\n", outFile);
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			//std::sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseDaDanJingE);	
			//shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);

			//shareSelectFinal(rstFp, propertyVecJingLiuRu[vecIndex]);
			//按净流入排序
			/*fprintf(rstFp, "按净流入排序\n");
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			std::sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseJingLiuRu);
			shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);*/
		}

		{
			//昨日涨停统计
			fprintf(rstFp, "\n%s 昨日涨停开盘统计----------------------------------------------------------\n", outFile);
			vector<PROPERTY_t> propertyAnalyVecTongJi = propertyAnalyVec;
			struct fengBanJinE {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhangTingBan > b.zhangTingBan); }
			} cmpMethodFengBanJinE;
			stable_sort(propertyAnalyVecTongJi.begin(), propertyAnalyVecTongJi.end(), cmpMethodFengBanJinE);
			int num = propertyAnalyVecTongJi.size();
			float averageIncrease = 0.0;
			int   codeCount = 0;
			int   zhangCount = 0;
			int   dieCount = 0;
			int   zuoRiYiZiCount = 0;
			int   stCount = 0;
			int   yiZiBanCount = 0;
			int   xinGuCount = 0;

			//连板统计
			float conAvgIncrease = 0.0;
			int   conZhangCount = 0;
			for (int i = 0; i < num; i++)
			{
				PROPERTY_t &analyProty = propertyAnalyVecTongJi[i];
				if (analyProty.continueDay > 1
					&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
					)
				{
					string name = analyProty.name;
					std::size_t found = name.find("ST");
					if (found != std::string::npos) {}//剔除ST股的统计
					else
					{
						conAvgIncrease += analyProty.zhangFu;
						++conZhangCount;
					}
				}
				if (
					(true == yiZiBanJudge(analyProty))
					&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
					&& ((yiZiBanCount - xinGuCount) < 6)
					)
				{
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
				}
				zhangFuSummarize(analyProty, zhangCount, dieCount, yiZiBanCount, xinGuCount, stCount, averageIncrease, codeCount);
			}
			float shangZhangLv = (float)zhangCount / (float)(zhangCount + dieCount);
			float avgZhang = averageIncrease / (float)codeCount;
			fprintf(rstFp, "\n今日一字开盘:%d, 上涨率:%5.2f, 平均涨幅:%5.2f, 上涨数:%d, 下跌数:%d, 新股:%d, 连续一字板:%d, ST股数:%d\n\n",
				yiZiBanCount, shangZhangLv, avgZhang, zhangCount, dieCount, xinGuCount, zuoRiYiZiCount, stCount);

			fprintf(rstFp, "连板涨幅::%5.2f, 昨日连板个数:%d\n", conAvgIncrease / float(conZhangCount), conZhangCount);

			/*if (avgZhang > 1.5)
			{
				fprintf(rstFp, "可操作\n");
			}
			else
			{
				fprintf(rstFp, "………………………………………………………………………………………………谨慎！！！！！\n");
			}*/
		}

		{
			/*fprintf(rstFp, "总流出排序------------------------------------------------------------------------------------------------------------------------\n");

			struct jingJiaLiuChu {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuChuBiLiuTong > b.zongLiuChuBiLiuTong); }
			} compZiJinFuseDaDanJingE;
			std::sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseDaDanJingE);
			//propertyVecChuBubbleSort(propertyVecJingLiuRu[vecIndex]);
			for (int i = 0; i < 15; i++)
			{
				PROPERTY_t &analyProty = propertyVecJingLiuRu[vecIndex][i];
				if (
					(analyProty.zhangFu > 0)
					&& (analyProty.zijinIdx < 30)
					)
				{
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
				}
			}*/
			//shareSelectFinal(rstFp, propertyVecJingLiuRu[vecIndex]);
			
			fprintf(rstFp, "总流入排序-----------------\n");
			//打印排第一的票
			selectFirstShare(rstFp,
				buyFp,
				programBuyProty,
				programFindFlag,
				propertyAnalyVec,
				propertyAnalyVecSort,
				propertyAnalyVecPre);

			fclose(buyFp);
		}
		{
			//内外盘差比流通
			fprintf(rstFp, "\n%s 内外盘差比流通----------------------------------------------------------\n", outFile);
			struct neiWaiPanBiLiuTongGu {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.neiWaiPanBiLiuTongGu > b.neiWaiPanBiLiuTongGu); }
			} compNeiWaiPanBiLiuTongGu;
			std::sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compNeiWaiPanBiLiuTongGu);
			int codeCount = 0;
			int num = propertyVecJingLiuRu[vecIndex].size();
			for (int i = 0; i < num; i++)
			{
				PROPERTY_t &analyProty = propertyVecJingLiuRu[vecIndex][i];
				if (
					(analyProty.zhangFu > 0)
					&& (analyProty.zijinIdx < 30)
					&& (analyProty.neiWaiPanBiLiuTongGu > 0.3)
					//&& (analyProty.zongLiuRuBiLiuTong > 0.002)
					)
				{
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
					if (codeCount > 10)
					{
						break;
					}
					++codeCount;
				}
			}
		}
		if (1 >= fileIndex)
		{
			fprintf(rstFp, "高度板排序------------------------------------------------------------------------------------------------------------------------\n");
			vector<PROPERTY_t> propertyAnalyVecJingJia = propertyAnalyVec;
			sortByTimeZhangfuGaodu(propertyAnalyVecJingJia);
			int num = propertyAnalyVecJingJia.size();
			int countOne = 0;
			for (int i = 0; i < num; i++)
			{
				PROPERTY_t &analyProty = propertyAnalyVecJingJia[i];
				//不同梯队打印换行符
				if ((i > 1) && (propertyAnalyVecJingJia[i - 1].continueDay != propertyAnalyVecJingJia[i].continueDay))
				{
					fprintf(rstFp, "\n");
				}
				if ((0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
					//&&(analyProty.zongLiuRuBiZuoRiZongJinE > 0.0)
					)
				{
					shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
				}
				if(1 == analyProty.continueDay)
				{
					countOne++;
				}
				//if ((analyProty.continueDay < 2) && (i>20))
				if (20 < countOne)
				{
					//break;
				}
			}
		}
		else
		{
			fprintf(rstFp, "持仓股------------------------------------------------------------------------------------------------------------------------\n");
			int analySize = independentAnalyVec.size();
			for (int analyIdx = 0; analyIdx < analySize; analyIdx++)				
			{
				vector<PROPERTY_t>::iterator proIter = propertyAnalyVec.begin();
				while (proIter != propertyAnalyVec.end())
				{
					if (0 == strcmp(proIter->code, independentAnalyVec[analyIdx].code))
					{
						shareSelectPrint(rstFp, *proIter, propertyAnalyVecPre);
						break;
					}
					++proIter;
				}
			}
		}
		//只处理当前最新的一组数据
		iFile = 0;
		fprintf(rstFp, "------------------------------------------------------------------------------------------------------------------------\n");
		analyNum = propertyAnalyVec.size();
		int count = 0;
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVec[i];
			//shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			if (
				(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.weiBi > -88.9)
				&& (analyProty.zhangTingBan > -7000.0)
				&& (analyProty.zuoRiLiangBi < 4.67)
				//&& (analyProty.daDanJinBiLiuTong < 0)
				//&& (analyProty.jingJiaLiangBi > 9.90)
				&& (
				(analyProty.zijinIdx <= 1500)
					|| (
					(analyProty.zijinIdx > 1500) 
						&& (analyProty.weiBi > WEIBI_MAX)
						)
					)
				&& (analyProty.limitUpMoney > 980)
				//&& (analyProty.zijinIdx < 20)
				//&& (analyProty.zongJinE > (800 * TENTHOUSAND))
				)
			{
				if ((analyProty.zhangFu < 7.0)
					&& (analyProty.zhangTingBan > -900.0)
					)
				{
					//continue;
				}
				count++;
				//shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
			if (10 < count)
			{
				//break;
			}
			if (
				(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.weiBi > -85.9)//83.0
				//&& (analyProty.zhangTingBan > -3800.0)
				//&& (analyProty.zuoRiHuanShou < 29.9)
				//&& (analyProty.zijinIdx < 499)
				//&& ((analyProty.jingJiaLiangBi / analyProty.zuoRiHuanShou) < 300.0)
				)
			{
				if (true == yiZiBanJudge(analyProty))// 一字开盘
				{
					if (
						((analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) < FLT_MIN)//昨日一字板
						&& (analyProty.zuoRiZhenFu < FLT_MIN)
						)
					{
						if (
							(analyProty.zuoRiHuanShou > 2.99)
							|| (analyProty.jingJiaLiangBi > 200.0)
							)
						{
							continue;
						}
					}
					if (
						(analyProty.zuoRiHuanShou > 7.9)
						&& (analyProty.zhangTingBan < 1000.0)
						)
					{
						continue;
					}
				}
				/*if (
					(analyProty.weiBi < WEIBI_MAX) //基本一字开盘
					&& (analyProty.jingJiaLiangBi > 300.0)
					&& ((analyProty.zhangFu > ((round)(analyProty.zuoShou * 110.0-0.5)) / 100.0))
					)
				{
					continue;
				}*/
				if (
					((analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) < FLT_MIN)//昨日一字板
					&& (analyProty.zuoRiZhenFu < FLT_MIN)
					)
				{
					if (
						(analyProty.zuoRiHuanShou > 6.99)
						|| (analyProty.jingJiaLiangBi > 400.0)
						|| (analyProty.zhangTingBan < -7000.0)
						)
					{
						continue;
					}
					if (
						(analyProty.zuoRiHuanShou > 1.99)
						&& (analyProty.jingJiaLiangBi > 80.0)
						)
					{
						continue;
					}
				}
				//shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}
		fprintf(rstFp, "\n");
	}

	//暂时屏蔽大单持续流入选项
	if (0)
	{
		int startIndex = 0;
		PROPERTY_t property = propertyV[0][0];
		vector<std::string> selectStrVec;
		shareFuseSelect(startIndex, property, propertyV, selectStrVec);
	}

	fprintf(rstFp, "\n");
	fclose(rstFp);

	//保存选股结果到文件
	codeResultSave(resultSet);

	//通达信板块修改
	string dateStr = dateStr;
	tdxBlockAdd(dateStr, resultSet);
	if (1 == fileIndex) // 只在最开始时增加block名称
	{
		tdxBlockCfgModify(dateStr);
	}
}

