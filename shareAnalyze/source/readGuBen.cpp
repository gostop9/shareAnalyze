#include <vector>
#include <cstdio>
#include "dataStructure.h"
#include "dataProc.h"
#include "shareDef.h"
#include "commonFun.h"

using namespace std;
using namespace commonFun;
using namespace SHAREDEF;

namespace GUBEN
{
	bool readGuBenFile(char *fileName, vector<guBen_t> &gubenVec)
	{
		gubenVec.clear();
		gubenVec.reserve(MAX_SHARE_NUM);

		FILE *fp = fopen(fileName, "rt");
		if (NULL == fp)
		{
			printf("Open guben file failed!\n");
			return false;
		}

		while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
		{
			string str = lineDateBuf;
			int strLen = str.length();
			str = str.substr(0, strLen - 1);// remove "\n"
			vector<string> strVec;
			strVec.reserve(10);
			stringSplit(str, " ", strVec);
			
			// move the last empty line
			if (3 < strVec.size())
			{
				guBen_t gubenTemp;
				strcpy(gubenTemp.code, strVec[0].c_str());
				strcpy(gubenTemp.name, strVec[1].c_str());
				// 自由流通市值
				/*string str = strVec[2];
				iPos0 = str.find(",");
				while (iPos0 != string::npos)
				{
					str = str.erase(iPos0, 1);
					iPos0 = str.find(",");
				}
				float liuTong = atof(str.c_str());
				int strLen = str.length();
				string danWei = str.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * 10000.0;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * 10000.0 * 10000.0;
				}
				else
				{
					liuTong = liuTong;
				}
				gubenTemp.ziYouLiuTongShiZhi = liuTong;*/
				// 自由流通股本
				string str = strVec[2];
				int iPos0 = str.find(",");
				while (iPos0 != string::npos)
				{
					str = str.erase(iPos0, 1);
					iPos0 = str.find(",");
				}
				float liuTong = atof(str.c_str());
				int strLen = str.length();
				string danWei = str.substr(strLen - 2, strLen);
				if (0 == strcmp("万", danWei.c_str()))
				{
					liuTong = liuTong * 10000;
				}
				if (0 == strcmp("亿", danWei.c_str()))
				{
					liuTong = liuTong * 10000 * 10000;
				}
				else
				{
					liuTong = liuTong;
				}
				gubenTemp.ziYouLiuTongGu = liuTong;

				// 股性评分
				gubenTemp.guXingPingFen = atof(strVec[3].c_str());

				//            
				gubenVec.push_back(gubenTemp);
			}
		}
		return true;
	}
}
