#include <iostream>
#include <fstream>
#include <vector>
#include <list>
//#include <windows.h>
#include <stdio.h>
#include "dataStructure.h"
#include "dataProc.h"
#include "shareRaisingLtdPre.h"
#include "shareDef.h"

using namespace std;
using namespace SHAREDEF;
using namespace DATAPROC;

void shareRaisingLtdPre(char *preFileName, char *date, char *path, int searhDays)
{
	// cfg file
    FILE *preFp;
    preFp = fopen(preFileName, "at+");
    if(NULL == preFp)
    {
        //printf("Open pre file failed!\n");
        return;
    }
    fprintf(preFp, "%s\n", date);
	
	list<vector<DDE_t>> ddeList;
	list<vector<ZIJIN_t>> zijinList;
	list<vector<ZHULI_t>> zhuliList;
	vector<DDE_t> ddeFirst;
	
	char ddeFileName[100], zijinName[100], zhuliFileName[100];
	for (int searhIndex = 0; searhIndex < searhDays; searhIndex++)
	{
		int dataInt = atoi(date);
		strcpy(ddeFileName, path);
		strcat(ddeFileName, "DDE_");
		strcat(ddeFileName, date);
		strcat(ddeFileName, ".txt");

		strcpy(zijinName, path);
		strcat(zijinName, "zijin_");
		strcat(zijinName, date);
		strcat(zijinName, ".txt");

		strcpy(zhuliFileName, path);
		strcat(zhuliFileName, "zhuli_");
		strcat(zhuliFileName, date);
		strcat(zhuliFileName, ".txt");

        dataInt--;
        _itoa (dataInt, date, 10);

		vector<DDE_t> dde;
		if(false == readDdeFile(ddeFileName, dde))
		{
			searhIndex--;
			continue;
		}
		vector<ZIJIN_t> zijin;
		readZijinFile(zijinName, zijin);
		vector<ZHULI_t> zhuli;
		readZhuliFile(zhuliFileName, zhuli);
		
		if(0 == searhIndex)
		{
			ddeFirst = dde;
		}
		
		if(searhIndex > 0)
		{
			ddeList.push_back(dde);
			zijinList.push_back(zijin);
			zhuliList.push_back(zhuli);
		}
	}
				
				
				
	vector<DDE_t>::iterator ddeFirstIter = ddeFirst.begin();
	while(ddeFirstIter != ddeFirst.end())
	{
		if(ddeFirstIter->zhangFu > 9.68)
		{
			fprintf(preFp, "%8s\n", ddeFirstIter->name);
			list<vector<DDE_t>>::iterator ddeListIter = ddeList.begin();
			list<vector<ZIJIN_t>>::iterator zijinListIter = zijinList.begin();
			list<vector<ZHULI_t>>::iterator zhuliListIter = zhuliList.begin();
			while(ddeListIter != ddeList.end())
			{
				int ddeIndex = 0;
				vector<DDE_t>::iterator ddeIter = ddeListIter->begin();
				while(ddeIter != ddeListIter->end())
				{
					if(!strcmp(ddeFirstIter->name, ddeIter->name))
					{
						fprintf(preFp, "DDE: %2d, ", ddeIndex);
						break;
					}
					++ddeIter;
					++ddeIndex;
				}
				
				int zijinIndex = 0;
				vector<ZIJIN_t>::iterator zijinIter = zijinListIter->begin();
				while(zijinIter != zijinListIter->end())
				{
					if(!strcmp(ddeFirstIter->name, zijinIter->name))
					{
						fprintf(preFp, "%.3f亿, 资金: %2d, ", zijinIter->liuTongShiZhi / DIVIDE, zijinIndex);
						break;
					}
					++zijinIter;
					++zijinIndex;
				}
				
				int zhuliIndex = 0;
				vector<ZHULI_t>::iterator zhuliIter = zhuliListIter->begin();
				while(zhuliIter != zhuliListIter->end())
				{
					if(!strcmp(ddeFirstIter->name, zhuliIter->name))
					{
						fprintf(preFp, "主力: %2d\n", zhuliIndex);
						break;
					}
					++zhuliIter;
					++zhuliIndex;
				}
				++ddeListIter;
				++zijinListIter;
				++zhuliListIter;
			}
		}
		++ddeFirstIter;
	}
	
	
    fclose(preFp);
}