#include <fstream>
#include <vector>
#include <cstdio>
#include <cstring>
#include "shareDef.h"
#include "tdxCfgModify.h"

using namespace std;
using namespace SHAREDEF;

namespace TDXCFGMODIFY
{
//保存code结果到通达信文件夹
void tdxBlockAdd(std::string &date, std::vector<std::string> &resultSet)
{
	vector<std::string> strZidingyiVec;
	FILE *codeTdxFp;
	std::string pathTdx = "C:/sws2010/T0002/blocknew/";
	std::string &fileNameTdx = date;
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
		while (fgets(lineDateBuf, lineDataLen, codeTdxFp) != NULL)
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
}
void tdxBlockCfgModify(std::string &date)
{
	//修改配置文件
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
			if (0 == strncmp(cfgStruct.szName, date.substr(0, 8).c_str(), 8))
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
	memset(cfgStruct.szName, 0, sizeof(cfgStruct.szName));
	memset(cfgStruct.szAbbr, 0, sizeof(cfgStruct.szAbbr));
	strncpy(cfgStruct.szName, date.substr(0, 8).c_str(), 8);
	strncpy(cfgStruct.szAbbr, date.substr(0, 8).c_str(), 8);
	fwrite(cfgStruct.szName, sizeof(char), sizeof(cfgStruct.szName), cfgFp);
	fwrite(cfgStruct.szAbbr, sizeof(char), sizeof(cfgStruct.szAbbr), cfgFp);

	fclose(cfgFp);
}

void codeResultSave(std::vector<std::string> &resultSet)
{
	//结果code保存
	FILE *codeFp;
	codeFp = fopen(codeResult.c_str(), "wt");
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
}

void tdxBlockAppend(const std::string &blockFileName, std::vector<std::string> &resultSet)
{
	vector<std::string> strZidingyiVec;
	FILE *codeTdxFp;
	std::string pathTdx = blockFileName;
	//首先读取板块文件中的code，避免添加时重复
	codeTdxFp = fopen(pathTdx.c_str(), "rt");
	if (NULL == codeTdxFp)
	{
		printf("Open codeTdxFp file failed!\n");
	}
	else
	{
		while (fgets(lineDateBuf, lineDataLen, codeTdxFp) != NULL)
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
	codeTdxFp = fopen(pathTdx.c_str(), "wt");
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
			fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
			std::vector<std::string>::iterator it;
			it = find(strZidingyiVec.begin(), strZidingyiVec.end(), resultStr);
			if (it != strZidingyiVec.end())
			{
				strZidingyiVec.erase(it);
			}
		}
		else
		{
			std::string resultStr = "0";
			resultStr.append(iter->substr(2, 20));
			fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
			std::vector<std::string>::iterator it;
			it = find(strZidingyiVec.begin(), strZidingyiVec.end(), resultStr);
			if (it != strZidingyiVec.end())
			{
				strZidingyiVec.erase(it);
			}
		}
	}

	//write remain code to file
	for (vector<std::string>::const_iterator iter = strZidingyiVec.begin(); iter != strZidingyiVec.end(); ++iter)
	{
		fprintf(codeTdxFp, "%7s\n", iter->c_str());
	}
	fclose(codeTdxFp);
}

void tdxBlockModify(const std::string &blockFileName, std::vector<std::string> &resultSet)
{
	vector<std::string> strZidingyiVec;
	FILE *codeTdxFp;
	std::string pathTdx = blockFileName;

	codeTdxFp = fopen(pathTdx.c_str(), "wt");
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
			fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
		}
		else
		{
			std::string resultStr = "0";
			resultStr.append(iter->substr(2, 20));
			fprintf(codeTdxFp, "%7s\n", resultStr.c_str());
		}
	}
}

}
