//#include <winsock2.h>
//#include <windows.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
//#include <wchar.h>
#include <math.h>
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

using namespace std;
using namespace GUBEN;
using namespace ZHANGTING;
using namespace getLimitUpInfo;

char nonChar[CHAR_LEN] = "--";
float divide = 100000000;

char configFileName[] = "D:/share/config.txt";
char resultFileName[30] = "D:/share/result.txt";
char preFileName[] = "D/share/searchPre.txt";

bool compZiJin(const ZIJIN_t &a, const ZIJIN_t &b) {
	     return a.jingLiuRu > b.jingLiuRu;
}

bool compZiJinFuseJingLiuRu(const PROPERTY_t &a, const PROPERTY_t &b) {
	return a.zhongDanLiuRuBiLiuTong > b.zhongDanLiuRuBiLiuTong;
}

bool compZiJinFuseDaDanJingE(const PROPERTY_t &a, const PROPERTY_t &b) {
	return a.zhuLiJingE > b.zhuLiJingE;
}

float huanShouMax = 25.0F;
float liangBiMin = 1.2F;
float liangBiMax = 6.0F;
float zhuLiJingLiangMin = 0.08F;
float daDanJingEBMin = 0.08F;

void main()
{
	vector<guBen_t> guBenVec;
	readGuBenFile("D:/share/自由流通市值.txt", guBenVec);

	/*
	int lineCountInFile = 0;
	vector<RIXIAN_t> riXianVec;
	char dataLineFileName[100] = "C:/sws2010/T0002/export/SH600000.txt";
	readDataLineFile(dataLineFileName, lineCountInFile, riXianVec);
	*/
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
	char ddeFileName[100], zijinName[100], zhuliFileName[100], zhangfuFileName[100], zhangtingFileName[100];
	char date[20], path[100];
	char dataPre[20];
	memset(date, 0, size(date));
	memset(path, 0, size(path));
	memset(dataPre, 0, size(date));
	int fileIndex, ddeSelectCount, zijinSelectCount, zhuliSelectCount;
	/*fscanf(cfgFp, "%s\n%d\n%s\n%d %d %d",
		&date,
		&fileIndex,
		&path,
		&ddeSelectCount,
		&zijinSelectCount,
		&zhuliSelectCount);
	fclose(cfgFp);*/

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
	memset(resultFileName, 0, size(resultFileName));
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
	for (int i = strIdx; i < itemNum; i++)
	{
		char tempBuf[10];
		memset(tempBuf, 0, size(tempBuf));
		int cpyLen = strVec[strIdx].length() - 1; // -1 去掉换行符
		strncpy(tempBuf, strVec[strIdx++].c_str(), cpyLen);
		analyseCode_t anaCode;
		strcpy(anaCode.code, tempBuf);
		analyseVec.push_back(anaCode);
	}

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
	//只进行竞价数据比较
	if(1 >= fileIndex)
	{
		zhuLiJingLiangMin = 1.0;

		memset(ddeFileName, 0, size(date));
		memset(zijinName, 0, size(date));
		memset(zhuliFileName, 0, size(date));
		memset(zhangfuFileName, 0, size(date));
		memset(zhangtingFileName, 0, size(date));

		strcpy(ddeFileName, path);
		strcat(ddeFileName, "DDE_");
		strcat(ddeFileName, dataPre);
		strcat(ddeFileName, ".txt");

		strcpy(zijinName, path);
		strcat(zijinName, "zijin_");
		strcat(zijinName, dataPre);
		strcat(zijinName, ".txt");

		strcpy(zhuliFileName, path);
		strcat(zhuliFileName, "zhuli_");
		strcat(zhuliFileName, dataPre);
		strcat(zhuliFileName, ".txt");

		strcpy(zhangfuFileName, path);
		strcat(zhangfuFileName, "zhangfu_");
		strcat(zhangfuFileName, dataPre);
		strcat(zhangfuFileName, ".txt");

		strcpy(zhangtingFileName, path);
		strcat(zhangtingFileName, "zhangting_");
		strncat(zhangtingFileName, dataPre, 8);
		strcat(zhangtingFileName, ".txt");

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
		
		readZhangTingFile(zhangtingFileName, zhangTingVecPre);
		int zuoRiZtNum = zhangTingVecPre.size();
		if (0 < zuoRiZtNum)
		{
			analyseVec.clear();
			analyseVec.reserve(zuoRiZtNum);
			for (int i = 0; i < zuoRiZtNum; i++)
			{
				analyseCode_t anaCode;
				std::string codeHead = zhangTingVecPre[i].code;
				codeHead = codeHead.substr(0, 1);
				int cmpResult = codeHead.compare("6");
				if (0 == cmpResult) //上海股票6开头
				{
					strcpy(anaCode.code, "SH");
					strcat(anaCode.code, zhangTingVecPre[i].code);
				}
				else
				{
					strcpy(anaCode.code, "SZ");
					strcat(anaCode.code, zhangTingVecPre[i].code);
				}
				analyseVec.push_back(anaCode);
			}
		}

		shareParaFuse(dde[0], zijin[0], zhuli[0], zhangfu[0], guBenVec, propertyV[0], analyseVec, propertyAnalyVecPre);
		chooseAnalyzeProperty(propertyV[0], analyseVec, propertyAnalyVecPre);
	}

	//用于zhuLiJingLiangMin，不用每次进行修改
	if ((fileIndex > 1) && (fileIndex < 5))
	{
		zhuLiJingLiangMin = float(fileIndex)/ float(10.0);
	}

	for (int i = fileIndex; i >= 0; i--)
	{
		char indexString[5];
		_itoa(i, indexString, 10);

		memset(ddeFileName, 0, size(date));
		memset(zijinName, 0, size(date));
		memset(zhuliFileName, 0, size(date));
		memset(zhangfuFileName, 0, size(date));

		strcpy(ddeFileName, path);
		strcat(ddeFileName, "DDE_");
		strcat(ddeFileName, date);
		strcat(ddeFileName, indexString);
		strcat(ddeFileName, ".txt");

		strcpy(zijinName, path);
		strcat(zijinName, "zijin_");
		strcat(zijinName, date);
		strcat(zijinName, indexString);
		strcat(zijinName, ".txt");

		strcpy(zhuliFileName, path);
		strcat(zhuliFileName, "zhuli_");
		strcat(zhuliFileName, date);
		strcat(zhuliFileName, indexString);
		strcat(zhuliFileName, ".txt");

		strcpy(zhangfuFileName, path);
		strcat(zhangfuFileName, "zhangfu_");
		strcat(zhangfuFileName, date);
		strcat(zhangfuFileName, indexString);
		strcat(zhangfuFileName, ".txt");

		int vecIndex = fileIndex - i;
		//vector<DDE_t> dde;
		//readDdeFile(ddeFileName, dde[vecIndex]);
		//vector<ZIJIN_t> zijin;
		readZijinFile(zijinName, zijin[vecIndex]);
		vector<ZIJIN_t > ziinDaDanJingE = zijin[vecIndex];
		//sort(ziinDaDanJingE.begin(), ziinDaDanJingE.end(), compZiJin);
		//vector<ZHULI_t> zhuli;
		bool zhuliFileFlag = readZhuliFile(zhuliFileName, zhuli[vecIndex]);

		int cnt_int = atoi(date);
		if (cnt_int > 20180525)
		{
			//vector<ZHANGFU_t> zhangfu;
			readZhangfuFile(zhangfuFileName, zhangfu[vecIndex]);
		}

		vector<PROPERTY_t > propertyAnalyVec;
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

		// sort for zijinIndex
		{
			struct myclass {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zongLiuRuBiLiuTong > b.zongLiuRuBiLiuTong); }
			} neiWaiPanBiLiuTongGu;

			sort(propertyV[vecIndex].begin(), propertyV[vecIndex].end(), neiWaiPanBiLiuTongGu);
			vector<PROPERTY_t>::iterator proIter = propertyV[vecIndex].begin();
			int ziJinIndex = 1;
			while (proIter != propertyV[vecIndex].end())
			{
				proIter->zijinIdx = ziJinIndex++;
				++proIter;
			}
			//shareSelectPrint(rstFp, propertyV[vecIndex][0], propertyAnalyVecPre);
		}

		//print 最新
		if (i == fileIndex)
		{

			resultSet.clear();

			fprintf(rstFp, "按大单净额排序\n");
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseDaDanJingE);			
			shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);
			//按净流入排序
			fprintf(rstFp, "按净流入排序\n");
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseJingLiuRu);
			shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);
		}

		i = 0;

		{
			time_t timep;
			time(&timep);
			char tmp[64];
			strftime(tmp, sizeof(tmp), "%H%M%S", localtime(&timep));
			int localTime = atoi(tmp);
			struct myclass {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.zhongXiaoDanJinBiLiuTong > b.zhongXiaoDanJinBiLiuTong); }
			} cmpMethod;

			struct jingJia {
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (a.jingJiaLiangBi > b.jingJiaLiangBi); }
			} cmpMethodJingJia;

			vector<PROPERTY_t> propertyAnalyVecSort = propertyV[vecIndex];
			if ((localTime < 92500) && (localTime > 91500)) //  befor time 9:25
			{
				sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), cmpMethodJingJia);
			}
			else
			{
				sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), cmpMethod);
			}
			chooseAnalyzeProperty(propertyAnalyVecSort, analyseVec, propertyAnalyVec);
			//sort(propertyAnalyVecSort.begin(), propertyAnalyVecSort.end(), compZiJinFuseDaDanJingE);

			//按涨停时间排序
			/*struct ztTime{
				bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.firstLimitTime) < atoi(b.firstLimitTime)); }
			} cmpZtTime;
			sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpZtTime);*/
			
			vector<string> newShareCodeVec;
			int analyNum = propertyAnalyVec.size();
			if (1 >= fileIndex)
			{
				//提取涨停原因
				fprintf(rstFp, "涨停种类------------------------------------------------------------------------------------------------------------------------\n");
				vector<limitUpReason_t> ztrVec;
				ztrVec.clear();
				ztrVec.reserve(analyNum);
				getLimitUpReason(propertyAnalyVec, ztrVec, newShareCodeVec);

				//排序并打印结果
				limitShareSort(ztrVec);
				int ztrNum = ztrVec.size();
				for (int i = 0; i < ztrNum; i++)
				{
					limitUpReason_t &ztrTemp = ztrVec[i];
					float zhangFuAvg = ztrTemp.zhangFuZong / float(ztrTemp.ztCount);
					fprintf(rstFp, "%d  %-8s  %-8s  %5.2f  %5.2f : %d  %s: \n", ztrTemp.ztCount, ztrTemp.code.c_str(), ztrTemp.name.c_str(), ztrTemp.zhangFu, zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());
				}

				if (0)
				{
					int newCodeNum = newShareCodeVec.size();
					//提取所属行业
					fprintf(rstFp, "行业分类------------------------------------------------------------------------------------------------------------------------\n");
					vector<limitUpReason_t> hyVec;
					hyVec.clear();
					hyVec.reserve(analyNum);
					getLimitUpHy(propertyAnalyVec, hyVec, newShareCodeVec);

					int ztrNum = hyVec.size();
					for (int i = 0; i < ztrNum; i++)
					{
						limitUpReason_t &ztrTemp = hyVec[i];
						float zhangFuAvg = ztrTemp.zhangFuZong / float(ztrTemp.ztCount);
						fprintf(rstFp, "%d  %-8s  %-8s  %5.2f  %5.2f : %d  %s: \n", ztrTemp.ztCount, ztrTemp.code.c_str(), ztrTemp.name.c_str(), ztrTemp.zhangFu, zhangFuAvg, ztrTemp.zfAvgOder, ztrTemp.reason.c_str());
					}
				}
			}
			fprintf(rstFp, "------------------------------------------------------------------------------------------------------------------------\n");
			for (int i = 0; i < analyNum; i++)
			{
				PROPERTY_t &analyProty = propertyAnalyVec[i];
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
			fprintf(rstFp, "\n");
		}
	}

	//暂时屏蔽大单持续流入选项
	if (0)
	{
		int startIndex = 0;
		PROPERTY_t property = propertyV[0][0];
		vector<std::string> selectStrVec;
		shareFuseSelect(startIndex, property, propertyV, selectStrVec);
	}

	/*for (int i = 0; i < fileIndex; i++)
	{
		char indexString[5];
		itoa(fileIndex, indexString, 10);

		strcpy(ddeFileName, path);
		strcat(ddeFileName, "DDE_");
		strcat(ddeFileName, date);
		strcat(ddeFileName, indexString);
		strcat(ddeFileName, ".txt");

		strcpy(zijinName, path);
		strcat(zijinName, "zijin_");
		strcat(zijinName, date);
		strcat(zijinName, indexString);
		strcat(zijinName, ".txt");

		strcpy(zhuliFileName, path);
		strcat(zhuliFileName, "zhuli_");
		strcat(zhuliFileName, date);
		strcat(zhuliFileName, indexString);
		strcat(zhuliFileName, ".txt");

		int vecIndex = fileIndex - i;
		//vector<DDE_t> dde;
		readDdeFile(ddeFileName, dde[vecIndex]);
		//vector<ZIJIN_t> zijin;
		readZijinFile(zijinName, zijin[vecIndex]);
		//vector<ZHULI_t> zhuli;
		readZhuliFile(zhuliFileName, zhuli[vecIndex]);

		//print 最新
		if (i == fileIndex)
		{
			shareSelect(rstFp, dde[vecIndex], zijin[vecIndex], zhuli[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount);
			shareSelectZiJin(rstFp, dde[vecIndex], zijin[vecIndex], zhuli[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount);
		}
	}*/

	fprintf(rstFp, "\n");
	fclose(rstFp);

	//结果code保存
	FILE *codeFp;
	codeFp = fopen("D:/share/Analyze/codeResult.txt", "wt");
	if (NULL == codeFp)
	{
		printf("Open codeResult file failed!\n");
		return;
	}
	for (vector<std::string>::const_iterator iter = resultSet.begin(); iter != resultSet.end(); ++iter)
	{
		fprintf(codeFp, "%8s\n", iter->c_str());
	}
	fclose(codeFp);

	//保存code结果到通达信文件夹
	{
		vector<std::string> strZidingyiVec;
		FILE *codeTdxFp;
		std::string pathTdx = "C:/sws2010/T0002/blocknew/";
		std::string fileNameTdx = date;
		pathTdx.append(fileNameTdx.substr(0, 8));
		pathTdx.append(".blk");
		//首先读取板块文件中的code，避免添加时重复
		codeTdxFp = fopen(pathTdx.c_str(), "rt");
		if (NULL == codeTdxFp)
		{
			printf("Open codeTdxFp file failed!\n");
		}
		else
		{
			while (fgets(lineDateBuf, lineDataLen, cfgFp) != NULL)
			{
				std::string str = lineDateBuf;
				if (5 < str.length())
				{
					strZidingyiVec.push_back(str.substr(0, 7));
				}
				memset(lineDateBuf, 0, lineDataLen);
			}
			fclose(codeTdxFp);
		}
		codeTdxFp = fopen(pathTdx.c_str(), "a");
		if (NULL == codeTdxFp)
		{
			printf("Open codeTdxFp file failed!\n");
			return;
		}
		for (vector<std::string>::const_iterator iter = resultSet.begin(); iter != resultSet.end(); ++iter)
		{
			std::string codeHead = iter->substr(0, 2);
			int cmpResult = codeHead.compare("SH");
			if (0 == cmpResult) //上海股票SH开头
			{
				std::string resultStr = "1";
				resultStr.append(iter->substr(2, 20));
				std::vector<std::string>::iterator it;
				it = find(strZidingyiVec.begin(), strZidingyiVec.end(), resultStr);
				if (it != strZidingyiVec.end())
				{
				}
				else
				{
					fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
				}
			}
			else
			{
				std::string resultStr = "0";
				resultStr.append(iter->substr(2, 20));
				std::vector<std::string>::iterator it;
				it = find(strZidingyiVec.begin(), strZidingyiVec.end(), resultStr);
				if (it != strZidingyiVec.end())
				{
				}
				else
				{
					fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
				}
			}
		}
		fclose(codeTdxFp);

		//修改配置文件
		if (1 == fileIndex)
		{
			FILE *cfgFp;
			std::string cfgFileName = "C:/sws2010/T0002/blocknew/blocknew.cfg";
			// 自定义板块概述文件格式，T0002\blocknew\blocknew.cfg
			struct TTDXUserBlockRecord
			{
				char        szName[50];
				char        szAbbr[70];        // 也是文件名称前缀 T0002\blocknew\xxxx.blk
			};
			TTDXUserBlockRecord cfgStruct;
			//首先读取板块文件中的板块名称，避免添加时重复
			cfgFp = fopen(cfgFileName.c_str(), "rb");
			if (NULL == cfgFp)
			{
				printf("Open codeTdxFp file failed!\n");
			}
			else
			{
				while (!feof(cfgFp))
				{
					fread(&cfgStruct, sizeof(cfgStruct), 1, cfgFp);
					if (0 == strncmp(cfgStruct.szName, fileNameTdx.substr(0, 8).c_str(), 8))
					{
						fclose(cfgFp);
						return;
					}
				}
				fclose(cfgFp);
			}
			cfgFp = fopen(cfgFileName.c_str(), "a");
			if (NULL == cfgFp)
			{
				printf("Open cfgFp file failed!\n");
				return;
			}
			memset(cfgStruct.szName, 0, size(cfgStruct.szName));
			memset(cfgStruct.szAbbr, 0, size(cfgStruct.szAbbr));
			strncpy(cfgStruct.szName, fileNameTdx.substr(0, 8).c_str(), 8);
			strncpy(cfgStruct.szAbbr, fileNameTdx.substr(0, 8).c_str(), 8);
			fwrite(cfgStruct.szName, sizeof(char), sizeof(cfgStruct.szName), cfgFp);
			fwrite(cfgStruct.szAbbr, sizeof(char), sizeof(cfgStruct.szAbbr), cfgFp);

			fclose(cfgFp);
		}
	}
}

