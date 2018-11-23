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

using namespace std;
using namespace DATAPROC;
using namespace GUBEN;
using namespace ZHANGTING;
using namespace GETLIMITUPINFO;
using namespace SHAREDEF;
using namespace TDXCFGMODIFY;

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
	readGuBenFile("D:/share/������ͨ��ֵ.txt", guBenVec);

	//���code����
	std::vector<std::string> resultSet;

	// cfg file
	FILE *cfgFp;
	cfgFp = fopen(configFileName, "rt");
	if (NULL == cfgFp)
	{
		printf("Open config file failed!\n");
		return;
	}
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
	int cpyLen = strVec[strIdx].length() - 1; // -1 ȥ�����з�
	strncpy(date, strVec[strIdx++].c_str(), cpyLen);

	fileIndex = atoi(strVec[strIdx++].c_str());

	cpyLen = strVec[strIdx].length() - 1; // -1 ȥ�����з�
	strncpy(path, strVec[strIdx++].c_str(), cpyLen);

	// result file name
	memset(resultFileName, 0, sizeof(resultFileName));
	strcpy(resultFileName, path);
	cpyLen = strVec[strIdx].length() - 1; // -1 ȥ�����з�
	strncat(resultFileName, strVec[strIdx++].c_str(), cpyLen);

	ddeSelectCount = atoi(strVec[strIdx++].c_str());
	zijinSelectCount = atoi(strVec[strIdx++].c_str());
	zhuliSelectCount = atoi(strVec[strIdx++].c_str());

	huanShouMax = atof(strVec[strIdx++].c_str());
	liangBiMin = atof(strVec[strIdx++].c_str());
	liangBiMax = atof(strVec[strIdx++].c_str());
	zhuLiJingLiangMin = atof(strVec[strIdx++].c_str());
	daDanJingEBMin = atof(strVec[strIdx++].c_str());

	cpyLen = strVec[strIdx].length() - 1; // -1 ȥ�����з�
	strncpy(dataPre, strVec[strIdx++].c_str(), cpyLen);

	//��ȡ��Ҫ����������
	int analyseNum = itemNum - strIdx;
	vector<analyseCode_t> analyseVec;
	analyseVec.reserve(analyseNum);
	vector<analyseCode_t> limitTodayVec;
	limitTodayVec.reserve(analyseNum);
	for (int i = strIdx; i < itemNum; i++)
	{
		char tempBuf[10];
		memset(tempBuf, 0, sizeof(tempBuf));
		int cpyLen = strVec[strIdx].length() - 1; // -1 ȥ�����з�
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
	strcat(outFile, _itoa(fileIndex, outIdx, 10)); // 10����
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

	//��ȡǰһ������
	vector<PROPERTY_t> propertyAnalyVecPre;
	//ǰһ����ͣ����
	vector<zhangTing_t> zhangTingVecPre;
	zhangTingVecPre.clear();
	//������ͣ����
	vector<zhangTing_t> zhangTingVecToday;
	zhangTingVecToday.clear();
	//ֻ���о������ݱȽ�
	if(1 >= fileIndex)
	{
		zhuLiJingLiangMin = 2.0;

		fileNameCat(ddeFileName,
			zijinName,
			zhuliFileName,
			zhangfuFileName,
			zhangtingFileName,
			path,
			dataPre);

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
		
		//��ȡ������ͣcode
		readZhangTingFile(zhangtingFileName, zhangTingVecPre);
		getZhangTingCode(zhangTingVecPre, analyseVec);
		
		//��ȡ������ͣcode
		readZhangTingFile(zhangtingFileNameToday, zhangTingVecToday);
		getZhangTingCode(zhangTingVecToday, limitTodayVec);

		shareParaFuse(dde[0], zijin[0], zhuli[0], zhangfu[0], guBenVec, propertyV[0], analyseVec, propertyAnalyVecPre);
		chooseAnalyzeProperty(propertyV[0], analyseVec, propertyAnalyVecPre);
	}

	//����zhuLiJingLiangMin������ÿ�ν����޸�
	if ((fileIndex > 1) && (fileIndex < 5))
	{
		zhuLiJingLiangMin = float(fileIndex)/ float(10.0);
	}

	for (int i = fileIndex; i >= 0; i--)
	{
		char indexString[5];
		_itoa(i, indexString, 10);

		char dateNow[15];
		strcpy(dateNow, date);
		strcat(dateNow, indexString);

		fileNameCat(ddeFileName,
			zijinName,
			zhuliFileName,
			zhangfuFileName,
			zhangtingFileName,
			path,
			dateNow);

		int vecIndex = fileIndex - i;
		//vector<DDE_t> dde;
		//readDdeFile(ddeFileName, dde[vecIndex]);
		//vector<ZIJIN_t> zijin;
		bool zijinFileFlag = readZijinFile(zijinName, zijin[vecIndex]);
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

		//����ʵ����ͨ������������
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
		
		//print ����
		if (i == fileIndex)
		{

			resultSet.clear();

			fprintf(rstFp, "���󵥾�������\n");
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseDaDanJingE);			
			shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);
			//������������
			fprintf(rstFp, "������������\n");
			propertyVecJingLiuRu[vecIndex] = propertyV[vecIndex];
			sort(propertyVecJingLiuRu[vecIndex].begin(), propertyVecJingLiuRu[vecIndex].end(), compZiJinFuseJingLiuRu);
			shareSelectZDZ(rstFp, propertyVecJingLiuRu[vecIndex], ddeSelectCount, zijinSelectCount, zhuliSelectCount, resultSet);
		}

		//ֻ����ǰ���µ�һ������
		i = 0;

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

		//����ͣʱ������
		/*struct ztTime{
			bool operator() (const PROPERTY_t &a, const PROPERTY_t &b) { return (atoi(a.firstLimitTime) < atoi(b.firstLimitTime)); }
		} cmpZtTime;
		sort(propertyAnalyVec.begin(), propertyAnalyVec.end(), cmpZtTime);*/
					
		vector<string> newShareCodeVec;
		int analyNum = propertyAnalyVec.size();
		if (1 >= fileIndex)
		{
			limitUpReason_t limitupInfo;
			limitupInfo.limitShareOrdering(propertyAnalyVec);

			//��ȡ��ͣԭ��
			fprintf(rstFp, "��ͣ����------------------------------------------------------------------------------------------------------------------------\n");
			vector<limitUpReason_t> ztrVec;
			ztrVec.clear();
			ztrVec.reserve(analyNum);
			limitupInfo.getLimitUpReason(propertyAnalyVec, ztrVec, newShareCodeVec);

			//���򲢴�ӡ���
			limitupInfo.limitShareSort(rstFp, ztrVec);

			if (0)
			{
				//��ȡ������ҵ
				fprintf(rstFp, "��ҵ����------------------------------------------------------------------------------------------------------------------------\n");
				vector<limitUpReason_t> hyVec;
				hyVec.clear();
				hyVec.reserve(analyNum);
				limitupInfo.getLimitUpHy(propertyAnalyVec, hyVec, newShareCodeVec);

				//���򲢴�ӡ���
				limitupInfo.limitShareSort(rstFp, hyVec);
			}

			//����������ͣ����
			limitupInfo.continueLimitSave(propertyAnalyVec, limitTodayVec);
		}
		fprintf(rstFp, "------------------------------------------------------------------------------------------------------------------------\n");
		for (int i = 0; i < analyNum; i++)
		{
			PROPERTY_t &analyProty = propertyAnalyVec[i];
			if (
				(0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				&& (analyProty.weiBi > -83.9)//83.0
				//&& (analyProty.zhangTingBan > -3800.0)
				//&& (analyProty.zuoRiHuanShou < 29.9)
				//&& (analyProty.zijinIdx < 499)
				//&& ((analyProty.jingJiaLiangBi / analyProty.zuoRiHuanShou) < 300.0)
				)
			{
				if (
					(analyProty.weiBi > WEIBI_MAX) //һ�ֿ���
					)
				{
					if (
						((analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) < FLT_MIN)//����һ�ְ�
						&& (analyProty.zuoRiZhenFu < FLT_MIN)
						)
					{
						if ((analyProty.zuoRiHuanShou > 4.9))
						{
							continue;
						}
					}
				}
				if (
					(analyProty.weiBi < WEIBI_MAX) //һ�ֿ���
					&& (analyProty.jingJiaLiangBi > 300.0)
					&& ((analyProty.zhangFu > ((round)(analyProty.zuoShou * 110.0-0.5)) / 100.0))
					)
				{
					continue;
				}
				shareSelectPrint(rstFp, analyProty, propertyAnalyVecPre);
			}
		}
		fprintf(rstFp, "\n");
	}

	//��ʱ���δ󵥳�������ѡ��
	if (0)
	{
		int startIndex = 0;
		PROPERTY_t property = propertyV[0][0];
		vector<std::string> selectStrVec;
		shareFuseSelect(startIndex, property, propertyV, selectStrVec);
	}

	fprintf(rstFp, "\n");
	fclose(rstFp);

	//����ѡ�ɽ�����ļ�
	codeResultSave(resultSet);

	//ͨ���Ű���޸�
	string dateStr = dateStr;
	tdxBlockAdd(dateStr, resultSet);
	if (1 == fileIndex) // ֻ���ʼʱ����block����
	{
		tdxBlockCfgModify(dateStr);
	}
}

