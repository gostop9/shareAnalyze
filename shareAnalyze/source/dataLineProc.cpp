#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include "dataStructure.h"
#include "dataLineProc.h"
#include "dataProc.h"
#include "shareDef.h"

using namespace std;
using namespace SHAREDEF;

bool readDataLineFile(char *fileName, int &lineCountInFile, vector<RIXIAN_t> &riXianVec)
{
	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		//printf("Open DDE file failed!");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int lineCount = 0;
	
	if(0 == lineCountInFile)
	{
		while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
		{
			lineCountInFile++;
		}
	}

	fseek(fp, 0, SEEK_SET);
	for(int i=0; i<lineCountInFile-1; i++)
	{
		fgets(lineDateBuf, lineDataLen, fp);
	}
		
	vector<vector<string>> hisStrVec;
	string str = lineDateBuf;
	vector<string> strVec;
	int iPos0 = 0;
	while (string::npos != str.find("\t", 0))
	{
		iPos0 = str.find("\t", 0);
		string strTemp = str.substr(0, iPos0);
		if (strTemp.size() > 0)
		{
			strVec.push_back(strTemp);
		}
		str = str.substr(iPos0 + 1, str.length() - iPos0);
	}
	// 最后一个字符串  
	strVec.push_back(str);
	// move the last empty line
	if (2 < strVec.size())
	{
		hisStrVec.push_back(strVec);
	}

	vector<vector<string>>::iterator hisVecIter = hisStrVec.begin();
	while (hisVecIter != hisStrVec.end())
	{
		int i = 0;
		RIXIAN_t tempRiXian;
		vector<string> &strVec = *hisVecIter++;
		strcpy(tempRiXian.date, strVec[i++].c_str());
		//kaiPan
		char kaiPan[CHAR_LEN];
		strcpy(kaiPan, strVec[i++].c_str());
		if (0 == strcmp(kaiPan, nonChar))
		{
			tempRiXian.kaiPan = FLT_MIN;
		}
		else tempRiXian.kaiPan = atof(kaiPan);
		//zuiGao
		char zuiGao[CHAR_LEN];
		strcpy(zuiGao, strVec[i++].c_str());
		if (0 == strcmp(zuiGao, nonChar))
		{
			tempRiXian.zuiGao = FLT_MIN;
		}
		else tempRiXian.zuiGao = atof(zuiGao);
		//zuiDi
		char zuiDi[CHAR_LEN];
		strcpy(zuiDi, strVec[i++].c_str());
		if (0 == strcmp(zuiDi, nonChar))
		{
			tempRiXian.zuiDi = FLT_MIN;
		}
		else tempRiXian.zuiDi = atof(zuiDi);
		//shouPan
		char shouPan[CHAR_LEN];
		strcpy(shouPan, strVec[i++].c_str());
		if (0 == strcmp(shouPan, nonChar))
		{
			tempRiXian.shouPan = FLT_MIN;
		}
		else tempRiXian.shouPan = atof(shouPan);
		//chengJiaoLiang
		char chengJiaoLiang[CHAR_LEN];
		strcpy(chengJiaoLiang, strVec[i++].c_str());
		if (0 == strcmp(chengJiaoLiang, nonChar))
		{
			tempRiXian.chengJiaoLiang = FLT_MIN;
		}
		else tempRiXian.chengJiaoLiang = atof(chengJiaoLiang);
		//chengJiaoE
		char chengJiaoE[CHAR_LEN];
		strcpy(chengJiaoE, strVec[i++].c_str());
		if (0 == strcmp(chengJiaoE, nonChar))
		{
			tempRiXian.chengJiaoE = FLT_MIN;
		}
		else tempRiXian.chengJiaoE = atof(chengJiaoE);

		riXianVec.push_back(tempRiXian);
	}
	
	return true;
}
