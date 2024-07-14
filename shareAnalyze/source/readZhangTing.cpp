#include <vector>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "dataStructure.h"
#include "dataProc.h"
#include "readZhangTing.h"
#include "shareDef.h"

using namespace std;
using namespace SHAREDEF;

namespace ZHANGTING
{
bool zhangTingYiZiBanJudge(zhangTing_t& zhangTingItem)
{
	bool flag = false;
	if (
		((abs(zhangTingItem.startPrice - zhangTingItem.startPrice) < FLT_MIN)
			&& (abs(zhangTingItem.maxPrice - zhangTingItem.minPrice) < FLT_MIN)
			&& (zhangTingItem.zhenFu < FLT_MIN))
		)
	{
		flag = true;
	}
	return flag;
}

void getZhangTingCode(std::vector<zhangTing_t> &zhangTingVec, std::vector<analyseCode_t> &codeVec)
{
	//codeVec.clear();
	int zuoRiZtNum = zhangTingVec.size();
	if (0 < zuoRiZtNum)
	{
		codeVec.reserve(zuoRiZtNum);
		for (int i = 0; i < zuoRiZtNum; i++)
		{
			analyseCode_t anaCode;
			std::string codeHead = zhangTingVec[i].code;
			codeHead = codeHead.substr(0, 1);

			// 如果代码包含SH或SZ
			int cmpResult = codeHead.compare("S");
			if (0 == cmpResult)
			{
				strcpy(anaCode.code, zhangTingVec[i].code);
				codeVec.push_back(anaCode);
				continue;
			}

			cmpResult = codeHead.compare("6");
			if (0 == cmpResult) //上海股票6开头
			{
				strcpy(anaCode.code, "SH");
				strcat(anaCode.code, zhangTingVec[i].code);
			}
			else
			{
				strcpy(anaCode.code, "SZ");
				strcat(anaCode.code, zhangTingVec[i].code);
			}
			codeVec.push_back(anaCode);
		}
	}
}

bool readZhangTingFile(char *fileName, vector<zhangTing_t> &zhangTingVec)
{
	zhangTingVec.clear();
	zhangTingVec.reserve(TOTAL_SHARE_NUMBERS);

	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		printf("Open zhangTing file failed!\n");
		return false;
	}
        
	int zhangTingCount = 0;
	fscanf(fp, "%d\n", &zhangTingCount);
    //jump 1 line
    for (int i = 0; i<1; i++) fscanf(fp, "%*[^\n]%*c");

	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		string str = lineDateBuf;
		int strLen = str.length();
		str = str.substr(0, strLen - 1);// remove "\n"
		vector<string> strVec;
		strVec.reserve(40);
        float liuTong = 0;
		int iPos = str.find("\t", 0);
		while (string::npos != iPos)
		{
			string strTemp = str.substr(0, iPos);
			strTemp = strTemp.substr(0, strTemp.find_first_of(" "));//remove space
			if (strTemp.size() > 0)
			{
				strVec.push_back(strTemp);
			}
			str = str.substr(iPos + 1, str.length() - iPos);
			iPos = str.find("\t", 0);
		}
		// 最后一个字符串
		str = str.substr(0, str.find_first_of(" "));//remove space
		strVec.push_back(str);

		int guBenColumn = 26; // before 20201207 28; after 26
		// move the last empty line
		if (guBenColumn < strVec.size())
		{
            int vecSize = strVec.size();                
            int i = 0;
            zhangTing_t zhangTingTemp;
            strcpy(zhangTingTemp.code, strVec[i++].c_str());
            strcpy(zhangTingTemp.name, strVec[i++].c_str());
            //liangBi
            string liangBi = strVec[i++];
            if (0 == strcmp(liangBi.c_str(), nonChar))
            {
                zhangTingTemp.liangBi = FLT_MIN;
            }
            else zhangTingTemp.liangBi = atof(liangBi.c_str());
            //ziYouHuanShouLv
            string ziYouHuanShouLv = strVec[i++];
            if (0 == strcmp(ziYouHuanShouLv.c_str(), nonChar))
            {
                zhangTingTemp.ziYouHuanShouLv = FLT_MIN;
            }
            else zhangTingTemp.ziYouHuanShouLv = atof(ziYouHuanShouLv.c_str());
            //ziYouLiuTongShiZhi
            string ziYouLiuTongShiZhi = strVec[i++];
            if (0 == strcmp(ziYouLiuTongShiZhi.c_str(), nonChar))
            {
                zhangTingTemp.ziYouLiuTongShiZhi = FLT_MIN;
            }
            else
            {
                iPos = ziYouLiuTongShiZhi.find(",");
                while (iPos != string::npos)
                {
                    ziYouLiuTongShiZhi = ziYouLiuTongShiZhi.erase(iPos, 1);
                    iPos = ziYouLiuTongShiZhi.find(",");
                }
                liuTong = atof(ziYouLiuTongShiZhi.c_str());
                int strLen = ziYouLiuTongShiZhi.length();
                string danWei = ziYouLiuTongShiZhi.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.ziYouLiuTongShiZhi = liuTong;
            }
            //continueDay
            string continueDay = strVec[i++];
            if (0 == strcmp(continueDay.c_str(), nonChar))
            {
                zhangTingTemp.continueDay = 0;
            }
            else zhangTingTemp.continueDay = atoi(continueDay.c_str());
            //firstLimitTime
            string firstLimitTime = strVec[i++];
            if (0 == strcmp(firstLimitTime.c_str(), nonChar))
            {
				memset(zhangTingTemp.firstLimitTime, 0, sizeof(zhangTingTemp.firstLimitTime));
            }
            else 
            {
                iPos = firstLimitTime.find(":");
                while (iPos != string::npos)
                {
                    firstLimitTime = firstLimitTime.erase(iPos, 1);
                    iPos = firstLimitTime.find(":");
                }
				strcpy(zhangTingTemp.firstLimitTime, firstLimitTime.c_str());
            }
            //lastLimitTime
            string lastLimitTime = strVec[i++];
            if (0 == strcmp(lastLimitTime.c_str(), nonChar))
            {
                memset(zhangTingTemp.lastLimitTime, 0, sizeof(zhangTingTemp.lastLimitTime));
            }
            else 
            {
                iPos = lastLimitTime.find(":");
                while (iPos != string::npos)
                {
                    lastLimitTime = lastLimitTime.erase(iPos, 1);
                    iPos = lastLimitTime.find(":");
                }
				strcpy(zhangTingTemp.lastLimitTime, lastLimitTime.c_str());
            }
            //limitOpenCount
            string limitOpenCount = strVec[i++];
            if (0 == strcmp(limitOpenCount.c_str(), nonChar))
            {
                zhangTingTemp.limitOpenCount = 0;
            }
            else zhangTingTemp.limitOpenCount = atoi(limitOpenCount.c_str());
            //limitVsDeal
            string limitVsDeal = strVec[i++];
            if (0 == strcmp(limitVsDeal.c_str(), nonChar))
            {
                zhangTingTemp.limitVsDeal = FLT_MIN;
            }
            else
            {
                iPos = limitVsDeal.find(",");
                while (iPos != string::npos)
                {
                    limitVsDeal = limitVsDeal.erase(iPos, 1);
                    iPos = limitVsDeal.find(",");
                }
                liuTong = atof(limitVsDeal.c_str());
                int strLen = limitVsDeal.length();
                string danWei = limitVsDeal.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.limitVsDeal = liuTong;
            }
            //limitVsCirculate
            string limitVsCirculate = strVec[i++];
            if (0 == strcmp(limitVsCirculate.c_str(), nonChar))
            {
                zhangTingTemp.limitVsCirculate = FLT_MIN;
            }
            else
            {
                iPos = limitVsCirculate.find(",");
                while (iPos != string::npos)
                {
                    limitVsCirculate = limitVsCirculate.erase(iPos, 1);
                    iPos = limitVsCirculate.find(",");
                }
                liuTong = atof(limitVsCirculate.c_str());
                int strLen = limitVsCirculate.length();
                string danWei = limitVsCirculate.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.limitVsCirculate = liuTong;
            }
            //limitUpMoney
            string limitUpMoney = strVec[i++];
            if (0 == strcmp(limitUpMoney.c_str(), nonChar))
            {
                zhangTingTemp.limitUpMoney = FLT_MIN;
            }
            else
            {
                iPos = limitUpMoney.find(",");
                while (iPos != string::npos)
                {
                    limitUpMoney = limitUpMoney.erase(iPos, 1);
                    iPos = limitUpMoney.find(",");
                }
                liuTong = atof(limitUpMoney.c_str());
                int strLen = limitUpMoney.length();
                string danWei = limitUpMoney.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.limitUpMoney = liuTong;
            }
            //limitReason
			memset(zhangTingTemp.limitReason, 0, sizeof(zhangTingTemp.limitReason));
            strcpy(zhangTingTemp.limitReason, strVec[i++].c_str());
            //guXingPingFen
            string guXingPingFen = strVec[i++];
            if (0 == strcmp(guXingPingFen.c_str(), nonChar))
            {
                zhangTingTemp.guXingPingFen = FLT_MIN;
            }
            else zhangTingTemp.guXingPingFen = atof(guXingPingFen.c_str());
            //limitShareNumber
            string limitShareNumber = strVec[i++];
            if (0 == strcmp(limitShareNumber.c_str(), nonChar))
            {
                zhangTingTemp.limitShareNumber = FLT_MIN;
            }
            else
            {
                iPos = limitShareNumber.find(",");
                while (iPos != string::npos)
                {
                    limitShareNumber = limitShareNumber.erase(iPos, 1);
                    iPos = limitShareNumber.find(",");
                }
                liuTong = atof(limitShareNumber.c_str());
                int strLen = limitShareNumber.length();
                string danWei = limitShareNumber.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.limitShareNumber = liuTong;
            }
            //startPrice
            string startPrice = strVec[i++];
            if (0 == strcmp(startPrice.c_str(), nonChar))
            {
                zhangTingTemp.startPrice = FLT_MIN;
            }
            else zhangTingTemp.startPrice = atof(startPrice.c_str());
            //endPrice
            string endPrice = strVec[i++];
            if (0 == strcmp(endPrice.c_str(), nonChar))
            {
                zhangTingTemp.endPrice = FLT_MIN;
            }
            else zhangTingTemp.endPrice = atof(endPrice.c_str());
            //maxPrice
            string maxPrice = strVec[i++];
            if (0 == strcmp(maxPrice.c_str(), nonChar))
            {
                zhangTingTemp.maxPrice = FLT_MIN;
            }
            else zhangTingTemp.maxPrice = atof(maxPrice.c_str());
            //minPrice
            string minPrice = strVec[i++];
            if (0 == strcmp(minPrice.c_str(), nonChar))
            {
                zhangTingTemp.minPrice = FLT_MIN;
            }
            else zhangTingTemp.minPrice = atof(minPrice.c_str());
            //zhenFu
            string zhenFu = strVec[i++];
            if (0 == strcmp(zhenFu.c_str(), nonChar))
            {
                zhangTingTemp.zhenFu = FLT_MIN;
            }
            else zhangTingTemp.zhenFu = atof(zhenFu.c_str());
            //chengJiaoE
            string chengJiaoE = strVec[i++];
            if (0 == strcmp(chengJiaoE.c_str(), nonChar))
            {
                zhangTingTemp.chengJiaoE = FLT_MIN;
            }
            else
            {
                iPos = chengJiaoE.find(",");
                while (iPos != string::npos)
                {
                    chengJiaoE = chengJiaoE.erase(iPos, 1);
                    iPos = chengJiaoE.find(",");
                }
                liuTong = atof(chengJiaoE.c_str());
                int strLen = chengJiaoE.length();
                string danWei = chengJiaoE.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.chengJiaoE = liuTong;
            }
            //chengJiaoLiang
            string chengJiaoLiang = strVec[i++];
            if (0 == strcmp(chengJiaoLiang.c_str(), nonChar))
            {
                zhangTingTemp.chengJiaoLiang = FLT_MIN;
            }
            else
            {
                iPos = chengJiaoLiang.find(",");
                while (iPos != string::npos)
                {
                    chengJiaoLiang = chengJiaoLiang.erase(iPos, 1);
                    iPos = chengJiaoLiang.find(",");
                }
                liuTong = atof(chengJiaoLiang.c_str());
                int strLen = chengJiaoLiang.length();
                string danWei = chengJiaoLiang.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.chengJiaoLiang = liuTong;
            }
            //huanShouLv
            string huanShouLv = strVec[i++];
            if (0 == strcmp(huanShouLv.c_str(), nonChar))
            {
                zhangTingTemp.huanShouLv = FLT_MIN;
            }
            else zhangTingTemp.huanShouLv = atof(huanShouLv.c_str());
            //ziYouLiuTongGu
            string ziYouLiuTongGu = strVec[i++];
            if (0 == strcmp(ziYouLiuTongGu.c_str(), nonChar))
            {
                zhangTingTemp.ziYouLiuTongGu = FLT_MIN;
            }
            else
            {
                iPos = ziYouLiuTongGu.find(",");
                while (iPos != string::npos)
                {
                    ziYouLiuTongGu = ziYouLiuTongGu.erase(iPos, 1);
                    iPos = ziYouLiuTongGu.find(",");
                }
                liuTong = atof(ziYouLiuTongGu.c_str());
                int strLen = ziYouLiuTongGu.length();
                string danWei = ziYouLiuTongGu.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.ziYouLiuTongGu = liuTong;
            }
            //liuTongGu
            string liuTongGu = strVec[i++];
            if (0 == strcmp(liuTongGu.c_str(), nonChar))
            {
                zhangTingTemp.liuTongGu = FLT_MIN;
            }
            else
            {
                iPos = liuTongGu.find(",");
                while (iPos != string::npos)
                {
                    liuTongGu = liuTongGu.erase(iPos, 1);
                    iPos = liuTongGu.find(",");
                }
                liuTong = atof(liuTongGu.c_str());
                int strLen = liuTongGu.length();
                string danWei = liuTongGu.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.liuTongGu = liuTong;
            }
            //liuTongShiZhi
            string liuTongShiZhi = strVec[i++];
            if (0 == strcmp(liuTongShiZhi.c_str(), nonChar))
            {
                zhangTingTemp.liuTongShiZhi = FLT_MIN;
            }
            else
            {
                iPos = liuTongShiZhi.find(",");
                while (iPos != string::npos)
                {
                    liuTongShiZhi = liuTongShiZhi.erase(iPos, 1);
                    iPos = liuTongShiZhi.find(",");
                }
                liuTong = atof(liuTongShiZhi.c_str());
                int strLen = liuTongShiZhi.length();
                string danWei = liuTongShiZhi.substr(strLen - 2, strLen);
                if (0 == strcmp("万", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_WAN;
                }
                if (0 == strcmp("亿", danWei.c_str()))
                {
                    liuTong = liuTong * DANWEI_YI;
                }
                else
                {
                    liuTong = liuTong;
                }
                zhangTingTemp.liuTongShiZhi = liuTong;
            }

			if (0) // after 20201207, set to 0
			{
				//ipoDays
				string ipoDays = strVec[i++];
				if (0 == strcmp(ipoDays.c_str(), nonChar))
				{
					zhangTingTemp.ipoDays = 0;
				}
				else
				{
					iPos = ipoDays.find(",");
					while (iPos != string::npos)
					{
						ipoDays = ipoDays.erase(iPos, 1);
						iPos = ipoDays.find(",");
					}
					zhangTingTemp.ipoDays = atoi(ipoDays.c_str());;
				}
				//zongGuBen
				string zongGuBen = strVec[i++];
				if (0 == strcmp(zongGuBen.c_str(), nonChar))
				{
					zhangTingTemp.zongGuBen = FLT_MIN;
				}
				else
				{
					iPos = zongGuBen.find(",");
					while (iPos != string::npos)
					{
						zongGuBen = zongGuBen.erase(iPos, 1);
						iPos = zongGuBen.find(",");
					}
					liuTong = atof(zongGuBen.c_str());
					int strLen = zongGuBen.length();
					string danWei = zongGuBen.substr(strLen - 2, strLen);
					if (0 == strcmp("万", danWei.c_str()))
					{
						liuTong = liuTong * DANWEI_WAN;
					}
					if (0 == strcmp("亿", danWei.c_str()))
					{
						liuTong = liuTong * DANWEI_YI;
					}
					else
					{
						liuTong = liuTong;
					}
					zhangTingTemp.zongGuBen = liuTong;
				}
			}

			//recaculate with ziYouLiuTongGu
			//zhangTingTemp.limitVsCirculate = zhangTingTemp.limitShareNumber / zhangTingTemp.ziYouLiuTongGu;

			//            
			zhangTingVec.push_back(zhangTingTemp);
		}
	}

	// sort by limitVsCirculate
	limitVsCirculate_t limitVsCirculate;
	sort(zhangTingVec.begin(), zhangTingVec.end(), limitVsCirculate);
	vector<zhangTing_t>::iterator ztIter = zhangTingVec.begin();
	int index = 1;
	while (ztIter != zhangTingVec.end())
	{
		if (0 != strcmp(ztIter->limitReason, NEW_SHARE.c_str()))
		{
			ztIter->indexLvsC = index++;
		}
		else
		{
			ztIter->indexLvsC = 0;
		}
		++ztIter;
	}

	// 只保留封单大于900万的
	/*{
		vector<zhangTing_t> zhangTingSelectVec;
		zhangTingSelectVec.reserve(zhangTingVec.size());
		vector<zhangTing_t>::iterator ztIter = zhangTingVec.begin();
		int index = 1;
		while (ztIter != zhangTingVec.end())
		{
			if (
				(0 != strcmp(ztIter->limitReason, NEW_SHARE.c_str()))
				&& (ztIter->limitUpMoney > 899)
				)
			{
				zhangTingSelectVec.push_back(*ztIter);
			}
			++ztIter;
		}
		zhangTingVec = zhangTingSelectVec;
	}*/
	return true;
}

bool readZhangTingFile_THS(char *fileName, vector<zhangTing_t> &zhangTingVec)
{
	int codeNumber = 0;
	zhangTingVec.clear();
	zhangTingVec.reserve(TOTAL_SHARE_NUMBERS);

	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		printf("Open zhangTing file failed!\n");
		return false;
	}

	//jump 2 line
	for (int i = 0; i<2; i++) fscanf(fp, "%*[^\n]%*c");

	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		codeNumber++;
		if (300 < codeNumber)
		{
			break;
		}
		string str = lineDateBuf;
		int strLen = str.length();
		str = str.substr(0, strLen - 1);// remove "\n"
		vector<string> strVec;
		strVec.reserve(40);
		float liuTong = 0;
		int iPos = str.find("\t", 0);
		while (string::npos != iPos)
		{
			string strTemp = str.substr(0, iPos);
			strTemp = strTemp.substr(0, strTemp.find_first_of(" "));//remove space
			if (strTemp.size() > 0)
			{
				strVec.push_back(strTemp);
			}
			str = str.substr(iPos + 1, str.length() - iPos);
			iPos = str.find("\t", 0);
		}
		// 最后一个字符串
		str = str.substr(0, str.find_first_of(" "));//remove space
		strVec.push_back(str);

		int guBenColumn = 26; // before 20201207 28; after 26
							  // move the last empty line
		if (guBenColumn < strVec.size())
		{
			int vecSize = strVec.size();
			int i = 0;
			zhangTing_t zhangTingTemp;
			strcpy(zhangTingTemp.code, strVec[i++].c_str());
			strcpy(zhangTingTemp.name, strVec[i++].c_str());
			//liangBi
			string liangBi = strVec[i++];
			if (0 == strcmp(liangBi.c_str(), nonChar))
			{
				zhangTingTemp.liangBi = FLT_MIN;
			}
			else zhangTingTemp.liangBi = atof(liangBi.c_str());
			//ziYouHuanShouLv
			string ziYouHuanShouLv = strVec[i++];
			if (0 == strcmp(ziYouHuanShouLv.c_str(), nonChar))
			{
				zhangTingTemp.ziYouHuanShouLv = FLT_MIN;
			}
			else zhangTingTemp.ziYouHuanShouLv = atof(ziYouHuanShouLv.c_str());
			//ziYouLiuTongShiZhi
			string ziYouLiuTongShiZhi = strVec[i++];
			if (0 == strcmp(ziYouLiuTongShiZhi.c_str(), nonChar))
			{
				zhangTingTemp.ziYouLiuTongShiZhi = FLT_MIN;
			}
			else
			{
				iPos = ziYouLiuTongShiZhi.find(",");
				while (iPos != string::npos)
				{
					ziYouLiuTongShiZhi = ziYouLiuTongShiZhi.erase(iPos, 1);
					iPos = ziYouLiuTongShiZhi.find(",");
				}
				liuTong = atof(ziYouLiuTongShiZhi.c_str());
				int strLen = ziYouLiuTongShiZhi.length();
				string danWei = ziYouLiuTongShiZhi.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.ziYouLiuTongShiZhi = liuTong;
			}
			//continueDay
			string continueDay = strVec[i++];
			if (0 == strcmp(continueDay.c_str(), nonChar))
			{
				zhangTingTemp.continueDay = 0;
			}
			else zhangTingTemp.continueDay = atoi(continueDay.c_str());
			//firstLimitTime
			string firstLimitTime = strVec[i++];
			if (0 == strcmp(firstLimitTime.c_str(), nonChar))
			{
				memset(zhangTingTemp.firstLimitTime, 0, sizeof(zhangTingTemp.firstLimitTime));
			}
			else
			{
				iPos = firstLimitTime.find(":");
				while (iPos != string::npos)
				{
					firstLimitTime = firstLimitTime.erase(iPos, 1);
					iPos = firstLimitTime.find(":");
				}
				strcpy(zhangTingTemp.firstLimitTime, firstLimitTime.c_str());
			}
			//lastLimitTime
			string lastLimitTime = strVec[i++];
			if (0 == strcmp(lastLimitTime.c_str(), nonChar))
			{
				memset(zhangTingTemp.lastLimitTime, 0, sizeof(zhangTingTemp.lastLimitTime));
			}
			else
			{
				iPos = lastLimitTime.find(":");
				while (iPos != string::npos)
				{
					lastLimitTime = lastLimitTime.erase(iPos, 1);
					iPos = lastLimitTime.find(":");
				}
				strcpy(zhangTingTemp.lastLimitTime, lastLimitTime.c_str());
			}
			//limitOpenCount
			string limitOpenCount = strVec[i++];
			if (0 == strcmp(limitOpenCount.c_str(), nonChar))
			{
				zhangTingTemp.limitOpenCount = 0;
				continue;
			}
			else zhangTingTemp.limitOpenCount = atoi(limitOpenCount.c_str());
			//limitVsDeal
			string limitVsDeal = strVec[i++];
			if ((0 == strcmp(limitVsDeal.c_str(), nonChar)) || (0 == strcmp(limitVsDeal.c_str(), "0")))
			{
				zhangTingTemp.limitVsDeal = FLT_MIN;
			}
			else
			{
				iPos = limitVsDeal.find(",");
				while (iPos != string::npos)
				{
					limitVsDeal = limitVsDeal.erase(iPos, 1);
					iPos = limitVsDeal.find(",");
				}
				liuTong = atof(limitVsDeal.c_str());
				int strLen = limitVsDeal.length();
				string danWei = limitVsDeal.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.limitVsDeal = liuTong;
			}
			//limitVsCirculate
			string limitVsCirculate = strVec[i++];
			if ((0 == strcmp(limitVsCirculate.c_str(), nonChar)) || (0 == strcmp(limitVsCirculate.c_str(), "0")))
			{
				zhangTingTemp.limitVsCirculate = FLT_MIN;
			}
			else
			{
				iPos = limitVsCirculate.find(",");
				while (iPos != string::npos)
				{
					limitVsCirculate = limitVsCirculate.erase(iPos, 1);
					iPos = limitVsCirculate.find(",");
				}
				liuTong = atof(limitVsCirculate.c_str());
				int strLen = limitVsCirculate.length();
				string danWei = limitVsCirculate.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.limitVsCirculate = liuTong;
			}
			//limitUpMoney
			string limitUpMoney = strVec[i++];
			if ((0 == strcmp(limitUpMoney.c_str(), nonChar)) || (0 == strcmp(limitUpMoney.c_str(), "0")))
			{
				zhangTingTemp.limitUpMoney = FLT_MIN;
			}
			else
			{
				iPos = limitUpMoney.find(",");
				while (iPos != string::npos)
				{
					limitUpMoney = limitUpMoney.erase(iPos, 1);
					iPos = limitUpMoney.find(",");
				}
				liuTong = atof(limitUpMoney.c_str());
				int strLen = limitUpMoney.length();
				string danWei = limitUpMoney.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.limitUpMoney = liuTong;
			}
			//limitReason
			memset(zhangTingTemp.limitReason, 0, sizeof(zhangTingTemp.limitReason));
			strcpy(zhangTingTemp.limitReason, strVec[i++].c_str());
			//guXingPingFen
			string guXingPingFen = strVec[i++];
			if (0 == strcmp(guXingPingFen.c_str(), nonChar))
			{
				zhangTingTemp.guXingPingFen = FLT_MIN;
			}
			else zhangTingTemp.guXingPingFen = atof(guXingPingFen.c_str());
			//limitShareNumber
			string limitShareNumber = strVec[i++];
			if ((0 == strcmp(limitShareNumber.c_str(), nonChar)) || (0 == strcmp(limitShareNumber.c_str(), "0")))
			{
				zhangTingTemp.limitShareNumber = FLT_MIN;
			}
			else
			{
				iPos = limitShareNumber.find(",");
				while (iPos != string::npos)
				{
					limitShareNumber = limitShareNumber.erase(iPos, 1);
					iPos = limitShareNumber.find(",");
				}
				liuTong = atof(limitShareNumber.c_str());
				int strLen = limitShareNumber.length();
				string danWei = limitShareNumber.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.limitShareNumber = liuTong;
			}
			//startPrice
			string startPrice = strVec[i++];
			if (0 == strcmp(startPrice.c_str(), nonChar))
			{
				zhangTingTemp.startPrice = FLT_MIN;
			}
			else zhangTingTemp.startPrice = atof(startPrice.c_str());
			//endPrice
			string endPrice = strVec[i++];
			if (0 == strcmp(endPrice.c_str(), nonChar))
			{
				zhangTingTemp.endPrice = FLT_MIN;
			}
			else zhangTingTemp.endPrice = atof(endPrice.c_str());
			//maxPrice
			string maxPrice = strVec[i++];
			if (0 == strcmp(maxPrice.c_str(), nonChar))
			{
				zhangTingTemp.maxPrice = FLT_MIN;
			}
			else zhangTingTemp.maxPrice = atof(maxPrice.c_str());
			//minPrice
			string minPrice = strVec[i++];
			if (0 == strcmp(minPrice.c_str(), nonChar))
			{
				zhangTingTemp.minPrice = FLT_MIN;
			}
			else zhangTingTemp.minPrice = atof(minPrice.c_str());
			//zhenFu
			string zhenFu = strVec[i++];
			if (0 == strcmp(zhenFu.c_str(), nonChar))
			{
				zhangTingTemp.zhenFu = FLT_MIN;
			}
			else zhangTingTemp.zhenFu = atof(zhenFu.c_str());
			//chengJiaoE
			string chengJiaoE = strVec[i++];
			if (0 == strcmp(chengJiaoE.c_str(), nonChar))
			{
				zhangTingTemp.chengJiaoE = FLT_MIN;
			}
			else
			{
				iPos = chengJiaoE.find(",");
				while (iPos != string::npos)
				{
					chengJiaoE = chengJiaoE.erase(iPos, 1);
					iPos = chengJiaoE.find(",");
				}
				liuTong = atof(chengJiaoE.c_str());
				int strLen = chengJiaoE.length();
				string danWei = chengJiaoE.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.chengJiaoE = liuTong;
			}
			//chengJiaoLiang
			string chengJiaoLiang = strVec[i++];
			if (0 == strcmp(chengJiaoLiang.c_str(), nonChar))
			{
				zhangTingTemp.chengJiaoLiang = FLT_MIN;
			}
			else
			{
				iPos = chengJiaoLiang.find(",");
				while (iPos != string::npos)
				{
					chengJiaoLiang = chengJiaoLiang.erase(iPos, 1);
					iPos = chengJiaoLiang.find(",");
				}
				liuTong = atof(chengJiaoLiang.c_str());
				int strLen = chengJiaoLiang.length();
				string danWei = chengJiaoLiang.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.chengJiaoLiang = liuTong;
			}
			//huanShouLv
			string huanShouLv = strVec[i++];
			if (0 == strcmp(huanShouLv.c_str(), nonChar))
			{
				zhangTingTemp.huanShouLv = FLT_MIN;
			}
			else zhangTingTemp.huanShouLv = atof(huanShouLv.c_str());
			//ziYouLiuTongGu
			string ziYouLiuTongGu = strVec[i++];
			if (0 == strcmp(ziYouLiuTongGu.c_str(), nonChar))
			{
				zhangTingTemp.ziYouLiuTongGu = FLT_MIN;
			}
			else
			{
				iPos = ziYouLiuTongGu.find(",");
				while (iPos != string::npos)
				{
					ziYouLiuTongGu = ziYouLiuTongGu.erase(iPos, 1);
					iPos = ziYouLiuTongGu.find(",");
				}
				liuTong = atof(ziYouLiuTongGu.c_str());
				int strLen = ziYouLiuTongGu.length();
				string danWei = ziYouLiuTongGu.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.ziYouLiuTongGu = liuTong;
			}
			//liuTongGu
			string liuTongGu = strVec[i++];
			if (0 == strcmp(liuTongGu.c_str(), nonChar))
			{
				zhangTingTemp.liuTongGu = FLT_MIN;
			}
			else
			{
				iPos = liuTongGu.find(",");
				while (iPos != string::npos)
				{
					liuTongGu = liuTongGu.erase(iPos, 1);
					iPos = liuTongGu.find(",");
				}
				liuTong = atof(liuTongGu.c_str());
				int strLen = liuTongGu.length();
				string danWei = liuTongGu.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.liuTongGu = liuTong;
			}
			//liuTongShiZhi
			string liuTongShiZhi = strVec[i++];
			if (0 == strcmp(liuTongShiZhi.c_str(), nonChar))
			{
				zhangTingTemp.liuTongShiZhi = FLT_MIN;
			}
			else
			{
				iPos = liuTongShiZhi.find(",");
				while (iPos != string::npos)
				{
					liuTongShiZhi = liuTongShiZhi.erase(iPos, 1);
					iPos = liuTongShiZhi.find(",");
				}
				liuTong = atof(liuTongShiZhi.c_str());
				int strLen = liuTongShiZhi.length();
				string danWei = liuTongShiZhi.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_WAN;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * DANWEI_YI;
				}
				else
				{
					liuTong = liuTong;
				}
				zhangTingTemp.liuTongShiZhi = liuTong;
			}

			if (0) // after 20201207, set to 0
			{
				//ipoDays
				string ipoDays = strVec[i++];
				if (0 == strcmp(ipoDays.c_str(), nonChar))
				{
					zhangTingTemp.ipoDays = 0;
				}
				else
				{
					iPos = ipoDays.find(",");
					while (iPos != string::npos)
					{
						ipoDays = ipoDays.erase(iPos, 1);
						iPos = ipoDays.find(",");
					}
					zhangTingTemp.ipoDays = atoi(ipoDays.c_str());;
				}
				//zongGuBen
				string zongGuBen = strVec[i++];
				if (0 == strcmp(zongGuBen.c_str(), nonChar))
				{
					zhangTingTemp.zongGuBen = FLT_MIN;
				}
				else
				{
					iPos = zongGuBen.find(",");
					while (iPos != string::npos)
					{
						zongGuBen = zongGuBen.erase(iPos, 1);
						iPos = zongGuBen.find(",");
					}
					liuTong = atof(zongGuBen.c_str());
					int strLen = zongGuBen.length();
					string danWei = zongGuBen.substr(strLen - 2, strLen);
					if (0 == strcmp("万", danWei.c_str()))
					{
						liuTong = liuTong * DANWEI_WAN;
					}
					if (0 == strcmp("亿", danWei.c_str()))
					{
						liuTong = liuTong * DANWEI_YI;
					}
					else
					{
						liuTong = liuTong;
					}
					zhangTingTemp.zongGuBen = liuTong;
				}
			}

			//recaculate with ziYouLiuTongGu
			//zhangTingTemp.limitVsCirculate = zhangTingTemp.limitShareNumber / zhangTingTemp.ziYouLiuTongGu;

			//
			if (0 < zhangTingTemp.continueDay)
			{
				zhangTingVec.push_back(zhangTingTemp);
			}
		}
	}

	// sort by continueDay_t
	continueDay_t continueDay;
	sort(zhangTingVec.begin(), zhangTingVec.end(), continueDay);
	vector<zhangTing_t>::iterator ztIter = zhangTingVec.begin();
	int index = 1;
	while (ztIter != zhangTingVec.end())
	{
		if (0 != strcmp(ztIter->limitReason, NEW_SHARE.c_str()))
		{
			ztIter->indexLvsC = index++;
		}
		else
		{
			ztIter->indexLvsC = 0;
		}
		++ztIter;
	}

	// 只保留封单大于900万的
	/*{
	vector<zhangTing_t> zhangTingSelectVec;
	zhangTingSelectVec.reserve(zhangTingVec.size());
	vector<zhangTing_t>::iterator ztIter = zhangTingVec.begin();
	int index = 1;
	while (ztIter != zhangTingVec.end())
	{
	if (
	(0 != strcmp(ztIter->limitReason, NEW_SHARE.c_str()))
	&& (ztIter->limitUpMoney > 899)
	)
	{
	zhangTingSelectVec.push_back(*ztIter);
	}
	++ztIter;
	}
	zhangTingVec = zhangTingSelectVec;
	}*/
	return true;
}

}
