#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include "dataStructure.h"
#include "dataProc.h"
#include "shareDef.h"
#include "commonFun.h"

using namespace std;
using namespace commonFun;
using namespace SHAREDEF;

//char lineDateBuf[lineDataLen];

namespace DATAPROC
{
bool readDdeFile(char *fileName, vector<DDE_t> &ddeVec)
{
    ddeVec.clear();
	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		//printf("Open DDE file failed!");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int fileOffset = 0;
	for (int i = 0; i<1; i++) fscanf(fp, "%*[^\n]%*c");  //jump 1 line

	fileOffset = ftell(fp);
	
	vector<vector<string>> ddeStrVec;
	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		string str = lineDateBuf;
		vector<string> strVec;
		strVec.reserve(50);
		stringSplitMultiSymbol(str, "\t", strVec);
		/*for (const auto &i : strVec)
		{
			cout << i << endl;
		}*/
		// move the last empty line
		if (2 < strVec.size())
		{
			ddeStrVec.push_back(strVec);
		}
	}

	int itemNum = 0;
	vector<vector<string>>::iterator ddeStrVecIter = ddeStrVec.begin();
	while (ddeStrVecIter != ddeStrVec.end())
	{
		int i = 0;
		itemNum++;
		DDE_t tempDde;
		vector<string> &strVec = *ddeStrVecIter++;
		strcpy(tempDde.code, strVec[i++].c_str());
		strcpy(tempDde.name, strVec[i++].c_str());
		//zhuLiJingLiang
		char zhuLiJingLiang[CHAR_LEN];
		strcpy(zhuLiJingLiang, strVec[i++].c_str());
		if (0 == strcmp(zhuLiJingLiang, nonChar))
		{
			tempDde.zhuLiJingLiang = FLT_MIN;
		}
		else tempDde.zhuLiJingLiang = atof(zhuLiJingLiang);
		//sanHuShuLiang
		char sanHuShuLiang[CHAR_LEN];
		strcpy(sanHuShuLiang, strVec[i++].c_str());
		if (0 == strcmp(sanHuShuLiang, nonChar))
		{
			tempDde.sanHuShuLiang = FLT_MIN;
		}
		else tempDde.sanHuShuLiang = atof(sanHuShuLiang);
		//zhuLiJinE
		char zhuLiJinE[CHAR_LEN];
		strcpy(zhuLiJinE, strVec[i++].c_str());
		if (0 == strcmp(zhuLiJinE, nonChar))
		{
			tempDde.zhuLiJinE = FLT_MIN;
		}
		else tempDde.zhuLiJinE = atof(zhuLiJinE);
		//zhangFu
		char zhangFu[CHAR_LEN];
		strcpy(zhangFu, strVec[i++].c_str());
		if (0 == strcmp(zhangFu, nonChar))
		{
			tempDde.zhangFu = FLT_MIN;
		}
		else tempDde.zhangFu = atof(zhangFu);

		strcpy(tempDde.suoShuHangYe, strVec[i++].c_str());

		//xingJi
		char xingJi[CHAR_LEN];
		strcpy(xingJi, strVec[i++].c_str());
		if (0 == strcmp(xingJi, nonChar))
		{
			tempDde.xingJi = FLT_MIN;
		}
		else tempDde.xingJi = atof(xingJi);
		//xianJia
		char xianJia[CHAR_LEN];
		strcpy(xianJia, strVec[i++].c_str());
		if (0 == strcmp(xianJia, nonChar))
		{
			tempDde.xianJia = FLT_MIN;
		}
		else tempDde.xianJia = atof(xianJia);
		//zhangSu
		char zhangSu[CHAR_LEN];
		strcpy(zhangSu, strVec[i++].c_str());
		if (0 == strcmp(zhangSu, nonChar))
		{
			tempDde.zhangSu = FLT_MIN;
		}
		else tempDde.zhangSu = atof(zhangSu);
		//zongShou
		char zongShou[CHAR_LEN];
		strcpy(zongShou, strVec[i++].c_str());
		if (0 == strcmp(zongShou, nonChar))
		{
			tempDde.zongShou = FLT_MIN;
		}
		else tempDde.zongShou = atof(zongShou);
		//huanShou
		char huanShou[CHAR_LEN];
		strcpy(huanShou, strVec[i++].c_str());
		if (0 == strcmp(huanShou, nonChar))
		{
			tempDde.huanShou = FLT_MIN;
		}
		else tempDde.huanShou = atof(huanShou);
		//liangBi
		char liangBi[CHAR_LEN];
		strcpy(liangBi, strVec[i++].c_str());
		if (0 == strcmp(liangBi, nonChar))
		{
			tempDde.liangBi = FLT_MIN;
		}
		else tempDde.liangBi = atof(liangBi);

		//zongJinE
		char zongJinE[CHAR_LEN];
		strcpy(zongJinE, strVec[i++].c_str());
		if (0 == strcmp(zongJinE, nonChar))
		{
			tempDde.zongJinE = FLT_MIN;
		}
		else tempDde.zongJinE = atof(zongJinE);

		ddeVec.push_back(tempDde);
	}

	fclose(fp);
	return true;
}

bool readZijinFile(char *fileName, vector<ZIJIN_t> &zijinVec)
{
    zijinVec.clear();
	int cnt_index = 0, cnt_int = 0, cnt_num = 0;
	//cnt_int 用于存放字符串中的数字.
	//cnt_index 作为字符串b的下标.
	int flag_20180522 = 0;
	int flag_20180525 = 0;
	int flag_20180828 = 0;
	for (int i = 0;fileName[i] != '\0';++i) //当a数组元素不为结束符时.遍历字符串a.
	{
		if (fileName[i] >= '0'&& fileName[i] <= '9') //如果是数字.
		{
			cnt_num++;
			cnt_int *= 10;
			cnt_int += fileName[i] - '0'; //数字字符的ascii-字符'0'的ascii码就等于该数字.
			if (cnt_num >= 8) //取8位数字进行判断20180522
			{
				break;
			}
		}
	}
	if (cnt_int > 20180522)
	{
		flag_20180522 = 1;
	}
	if (cnt_int > 20180525)
	{
		flag_20180525 = 1;
	}
	if (cnt_int > 20180828)
	{
		flag_20180828 = 1;
	}

    FILE *fp = fopen(fileName, "rt");
    if(NULL == fp)
    {
        //printf("Open file failed!");
        return false;
    }
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    int fileOffset = 0;
    for (int i=0; i<2; i++) fileOffset = fscanf(fp,"%*[^\n]%*c");  //junm 2 lines

    fileOffset = ftell(fp);

	vector<vector<string>> zijinStrVec;
	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		string str = lineDateBuf;
		vector<string> strVec;
		strVec.reserve(50);
		stringSplitMultiSymbol(str, "\t", strVec);
		/*for (const auto &i : strVec)
		{
		cout << i << endl;
		}*/
		// move the last empty line
		if (2 < strVec.size())
		{
			zijinStrVec.push_back(strVec);
		}
	}

	vector<vector<string>>::iterator zijinStrVecIter = zijinStrVec.begin();
	while (zijinStrVecIter != zijinStrVec.end())
	{
		int i = 0;
		ZIJIN_t tempZijin;
		vector<string> &strVec = *zijinStrVecIter++;
		strcpy(tempZijin.code, strVec[i++].c_str());
		strcpy(tempZijin.name, strVec[i++].c_str());

		//liuTongShiZhi
		char liuTongShiZhi[CHAR_LEN];
		strcpy(liuTongShiZhi, strVec[i++].c_str());
		if (0 == strcmp(liuTongShiZhi, nonChar))
		{
			tempZijin.liuTongShiZhi = FLT_MIN;
		}
		else tempZijin.liuTongShiZhi = atof(liuTongShiZhi);
		//huanShou
		char huanShou[CHAR_LEN];
		strcpy(huanShou, strVec[i++].c_str());
		if (0 == strcmp(huanShou, nonChar))
		{
			tempZijin.huanShou = FLT_MIN;
		}
		else tempZijin.huanShou = atof(huanShou);
		//xianJia
		char xianJia[CHAR_LEN];
		strcpy(xianJia, strVec[i++].c_str());
		if (0 == strcmp(xianJia, nonChar))
		{
			tempZijin.xianJia = FLT_MIN;
		}
		else tempZijin.xianJia = atof(xianJia);
		//zhangFu
		char zhangFu[CHAR_LEN];
		strcpy(zhangFu, strVec[i++].c_str());
		if (0 == strcmp(zhangFu, nonChar))
		{
			tempZijin.zhangFu = FLT_MIN;
		}
		else tempZijin.zhangFu = atof(zhangFu);

		if (1 == flag_20180522) //增加新标签的判断
		{
			//zhuLiJingLiang
			char zhuLiJingLiang[CHAR_LEN];
			strcpy(zhuLiJingLiang, strVec[i++].c_str());
			if (0 == strcmp(zhuLiJingLiang, nonChar))
			{
				tempZijin.zhuLiJingLiang = FLT_MIN;
			}
			else tempZijin.zhuLiJingLiang = atof(zhuLiJingLiang);
		}
		if (1 == flag_20180525) //增加新标签的判断
		{
			//liangBi
			char liangBi[CHAR_LEN];
			strcpy(liangBi, strVec[i++].c_str());
			if (0 == strcmp(liangBi, nonChar))
			{
				tempZijin.liangBi = FLT_MIN;
			}
			else tempZijin.liangBi = atof(liangBi);
			//weiBi
			char weiBi[CHAR_LEN];
			strcpy(weiBi, strVec[i++].c_str());
			if (0 == strcmp(weiBi, nonChar))
			{
				tempZijin.weiBi = FLT_MIN;
			}
			else tempZijin.weiBi = atof(weiBi);
			if (1 == flag_20180828) //增加“委差”标签的判断
			{
				//weiCha
				char weiCha[CHAR_LEN];
				strcpy(weiCha, strVec[i++].c_str());
				if (0 == strcmp(weiCha, nonChar))
				{
					tempZijin.weiCha = FLT_MIN;
				}
				else tempZijin.weiCha = atof(weiCha);
			}
			else
			{
				tempZijin.weiCha = -200.0; //委差最大为100，设置此数用于后续判断委差值是否有效
			}
			//jiGouDongXiang
			char jiGouDongXiang[CHAR_LEN];
			strcpy(jiGouDongXiang, strVec[i++].c_str());
			if (0 == strcmp(jiGouDongXiang, nonChar))
			{
				tempZijin.jiGouDongXiang = FLT_MIN;
			}
			else tempZijin.jiGouDongXiang = atof(jiGouDongXiang);
			//jingLiZengLv
			char jingLiZengLv[CHAR_LEN];
			strcpy(jingLiZengLv, strVec[i++].c_str());
			if (0 == strcmp(jingLiZengLv, nonChar))
			{
				tempZijin.jingLiZengLv = FLT_MIN;
			}
			else tempZijin.jingLiZengLv = atof(jingLiZengLv);
		}
		//jingLiuRu
		char jingLiuRu[CHAR_LEN];
		strcpy(jingLiuRu, strVec[i++].c_str());
		if (0 == strcmp(jingLiuRu, nonChar))
		{
			tempZijin.jingLiuRu = FLT_MIN;
		}
		else tempZijin.jingLiuRu = atof(jingLiuRu);
		//daDanLiuRu
		char daDanLiuRu[CHAR_LEN];
		strcpy(daDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(daDanLiuRu, nonChar))
		{
			tempZijin.daDanLiuRu = FLT_MIN;
		}
		else tempZijin.daDanLiuRu = atof(daDanLiuRu);
		//daDanLiuChu
		char daDanLiuChu[CHAR_LEN];
		strcpy(daDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(daDanLiuChu, nonChar))
		{
			tempZijin.daDanLiuChu = FLT_MIN;
		}
		else tempZijin.daDanLiuChu = atof(daDanLiuChu);
		//daDanJinE
		char daDanJinE[CHAR_LEN];
		strcpy(daDanJinE, strVec[i++].c_str());
		if (0 == strcmp(daDanJinE, nonChar))
		{
			tempZijin.daDanJinE = FLT_MIN;
		}
		else tempZijin.daDanJinE = atof(daDanJinE);
		//daDanJingEB
		char daDanJingEB[CHAR_LEN];
		strcpy(daDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(daDanJingEB, nonChar))
		{
			tempZijin.daDanJingEB = FLT_MIN;
		}
		else tempZijin.daDanJingEB = atof(daDanJingEB);
		//daDanZongE
		char daDanZongE[CHAR_LEN];
		strcpy(daDanZongE, strVec[i++].c_str());
		if (0 == strcmp(daDanZongE, nonChar))
		{
			tempZijin.daDanZongE = FLT_MIN;
		}
		else tempZijin.daDanZongE = atof(daDanZongE);
		//daDanZongEB
		char daDanZongEB[CHAR_LEN];
		strcpy(daDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(daDanZongEB, nonChar))
		{
			tempZijin.daDanZongEB = FLT_MIN;
		}
		else tempZijin.daDanZongEB = atof(daDanZongEB);

		//zhongDanLiuRu
		char zhongDanLiuRu[CHAR_LEN];
		strcpy(zhongDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(zhongDanLiuRu, nonChar))
		{
			tempZijin.zhongDanLiuRu = FLT_MIN;
		}
		else tempZijin.zhongDanLiuRu = atof(zhongDanLiuRu);
		//zhongDanLiuChu
		char zhongDanLiuChu[CHAR_LEN];
		strcpy(zhongDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(zhongDanLiuChu, nonChar))
		{
			tempZijin.zhongDanLiuChu = FLT_MIN;
		}
		else tempZijin.zhongDanLiuChu = atof(zhongDanLiuChu);
		//zhongDanJinE
		char zhongDanJinE[CHAR_LEN];
		strcpy(zhongDanJinE, strVec[i++].c_str());
		if (0 == strcmp(zhongDanJinE, nonChar))
		{
			tempZijin.zhongDanJinE = FLT_MIN;
		}
		else tempZijin.zhongDanJinE = atof(zhongDanJinE);
		//zhongDanJingEB
		char zhongDanJingEB[CHAR_LEN];
		strcpy(zhongDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(zhongDanJingEB, nonChar))
		{
			tempZijin.zhongDanJingEB = FLT_MIN;
		}
		else tempZijin.zhongDanJingEB = atof(zhongDanJingEB);
		//zhongDanZongE
		char zhongDanZongE[CHAR_LEN];
		strcpy(zhongDanZongE, strVec[i++].c_str());
		if (0 == strcmp(zhongDanZongE, nonChar))
		{
			tempZijin.zhongDanZongE = FLT_MIN;
		}
		else tempZijin.zhongDanZongE = atof(zhongDanZongE);
		//zhongDanZongEB
		char zhongDanZongEB[CHAR_LEN];
		strcpy(zhongDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(zhongDanZongEB, nonChar))
		{
			tempZijin.zhongDanZongEB = FLT_MIN;
		}
		else tempZijin.zhongDanZongEB = atof(zhongDanZongEB);
		
		//xiaoDanLiuRu
		char xiaoDanLiuRu[CHAR_LEN];
		strcpy(xiaoDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanLiuRu, nonChar))
		{
			tempZijin.xiaoDanLiuRu = FLT_MIN;
		}
		else tempZijin.xiaoDanLiuRu = atof(xiaoDanLiuRu);
		//xiaoDanLiuChu
		char xiaoDanLiuChu[CHAR_LEN];
		strcpy(xiaoDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanLiuChu, nonChar))
		{
			tempZijin.xiaoDanLiuChu = FLT_MIN;
		}
		else tempZijin.xiaoDanLiuChu = atof(xiaoDanLiuChu);
		//xiaoDanJinE
		char xiaoDanJinE[CHAR_LEN];
		strcpy(xiaoDanJinE, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanJinE, nonChar))
		{
			tempZijin.xiaoDanJinE = FLT_MIN;
		}
		else tempZijin.xiaoDanJinE = atof(xiaoDanJinE);
		//xiaoDanJingEB
		char xiaoDanJingEB[CHAR_LEN];
		strcpy(xiaoDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanJingEB, nonChar))
		{
			tempZijin.xiaoDanJingEB = FLT_MIN;
		}
		else tempZijin.xiaoDanJingEB = atof(xiaoDanJingEB);
		//xiaoDanZongE
		char xiaoDanZongE[CHAR_LEN];
		strcpy(xiaoDanZongE, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanZongE, nonChar))
		{
			tempZijin.xiaoDanZongE = FLT_MIN;
		}
		else tempZijin.xiaoDanZongE = atof(xiaoDanZongE);
		//xiaoDanZongEB
		char xiaoDanZongEB[CHAR_LEN];
		strcpy(xiaoDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanZongEB, nonChar))
		{
			tempZijin.xiaoDanZongEB = FLT_MIN;
		}
		else tempZijin.xiaoDanZongEB = atof(xiaoDanZongEB);
		
		//zhangSu
		char zhangSu[CHAR_LEN];
		strcpy(zhangSu, strVec[i++].c_str());
		if (0 == strcmp(zhangSu, nonChar))
		{
			tempZijin.zhangSu = FLT_MIN;
		}
		else tempZijin.zhangSu = atof(zhangSu);

		if (0 == flag_20180525) //量比标签前移
		{
			//liangBi
			char liangBi[CHAR_LEN];
			strcpy(liangBi, strVec[i++].c_str());
			if (0 == strcmp(liangBi, nonChar))
			{
				tempZijin.liangBi = FLT_MIN;
			}
			else tempZijin.liangBi = atof(liangBi);
		}
		//zongShou
		char zongShou[CHAR_LEN];
		strcpy(zongShou, strVec[i++].c_str());
		if (0 == strcmp(zongShou, nonChar))
		{
			tempZijin.zongShou = FLT_MIN;
		}
		else tempZijin.zongShou = atof(zongShou);
		//zongJinE
		char zongJinE[CHAR_LEN];
		strcpy(zongJinE, strVec[i++].c_str());
		if (0 == strcmp(zongJinE, nonChar))
		{
			tempZijin.zongJinE = FLT_MIN;
		}
		else tempZijin.zongJinE = atof(zongJinE);

		tempZijin.jingLiuBiLiuTong = tempZijin.jingLiuRu / tempZijin.liuTongShiZhi * 1000;
		tempZijin.daDanBiLiuTong = tempZijin.daDanJinE / tempZijin.liuTongShiZhi * 1000;
		
		zijinVec.push_back(tempZijin);
	}
    fclose(fp);
    return true;
}
/*
bool readZijinFile(char *fileName, vector<ZIJIN_t> &zijinVec)
{
	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		//printf("Open file failed!");
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int fileOffset = 0;
	for (int i = 0; i<2; i++) fileOffset = fscanf(fp, "%*[^\n]%*c");  //junm 2 lines

	fileOffset = ftell(fp);

	vector<vector<string>> zijinStrVec;
	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
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
		/*for (const auto &i : strVec)
		{
		cout << i << endl;
		}*/
		// move the last empty line
/*		if (2 < strVec.size())
		{
			zijinStrVec.push_back(strVec);
		}
	}

	vector<vector<string>>::iterator zijinStrVecIter = zijinStrVec.begin();
	while (zijinStrVecIter != zijinStrVec.end())
	{
		int i = 0;
		ZIJIN_t tempZijin;
		vector<string> &strVec = *zijinStrVecIter++;
		strcpy(tempZijin.code, strVec[i++].c_str());
		strcpy(tempZijin.name, strVec[i++].c_str());
		//liuTongShiZhi
		char liuTongShiZhi[CHAR_LEN];
		strcpy(liuTongShiZhi, strVec[i++].c_str());
		if (0 == strcmp(liuTongShiZhi, nonChar))
		{
			tempZijin.liuTongShiZhi = FLT_MIN;
		}
		else tempZijin.liuTongShiZhi = atof(liuTongShiZhi);
		//huanShou
		char huanShou[CHAR_LEN];
		strcpy(huanShou, strVec[i++].c_str());
		if (0 == strcmp(huanShou, nonChar))
		{
			tempZijin.huanShou = FLT_MIN;
		}
		else tempZijin.huanShou = atof(huanShou);
		//zhangFu
		char zhangFu[CHAR_LEN];
		strcpy(zhangFu, strVec[i++].c_str());
		if (0 == strcmp(zhangFu, nonChar))
		{
			tempZijin.zhangFu = FLT_MIN;
		}
		else tempZijin.zhangFu = atof(zhangFu);
		//xianJia
		char xianJia[CHAR_LEN];
		strcpy(xianJia, strVec[i++].c_str());
		if (0 == strcmp(xianJia, nonChar))
		{
			tempZijin.xianJia = FLT_MIN;
		}
		else tempZijin.xianJia = atof(xianJia);
		//jingLiuRu
		char jingLiuRu[CHAR_LEN];
		strcpy(jingLiuRu, strVec[i++].c_str());
		if (0 == strcmp(jingLiuRu, nonChar))
		{
			tempZijin.jingLiuRu = FLT_MIN;
		}
		else tempZijin.jingLiuRu = atof(jingLiuRu);
		//daDanLiuRu
		char daDanLiuRu[CHAR_LEN];
		strcpy(daDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(daDanLiuRu, nonChar))
		{
			tempZijin.daDanLiuRu = FLT_MIN;
		}
		else tempZijin.daDanLiuRu = atof(daDanLiuRu);
		//daDanLiuChu
		char daDanLiuChu[CHAR_LEN];
		strcpy(daDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(daDanLiuChu, nonChar))
		{
			tempZijin.daDanLiuChu = FLT_MIN;
		}
		else tempZijin.daDanLiuChu = atof(daDanLiuChu);

		//daDanJinE
		char daDanJinE[CHAR_LEN];
		strcpy(daDanJinE, strVec[i++].c_str());
		if (0 == strcmp(daDanJinE, nonChar))
		{
			tempZijin.daDanJinE = FLT_MIN;
		}
		else tempZijin.daDanJinE = atof(daDanJinE);
		//zhongDanLiuRu
		char zhongDanLiuRu[CHAR_LEN];
		strcpy(zhongDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(zhongDanLiuRu, nonChar))
		{
			tempZijin.zhongDanLiuRu = FLT_MIN;
		}
		else tempZijin.zhongDanLiuRu = atof(zhongDanLiuRu);
		//zhongDanLiuChu
		char zhongDanLiuChu[CHAR_LEN];
		strcpy(zhongDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(zhongDanLiuChu, nonChar))
		{
			tempZijin.zhongDanLiuChu = FLT_MIN;
		}
		else tempZijin.zhongDanLiuChu = atof(zhongDanLiuChu);
		//zhongDanJinE
		char zhongDanJinE[CHAR_LEN];
		strcpy(zhongDanJinE, strVec[i++].c_str());
		if (0 == strcmp(zhongDanJinE, nonChar))
		{
			tempZijin.zhongDanJinE = FLT_MIN;
		}
		else tempZijin.zhongDanJinE = atof(zhongDanJinE);
		//xiaoDanLiuRu
		char xiaoDanLiuRu[CHAR_LEN];
		strcpy(xiaoDanLiuRu, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanLiuRu, nonChar))
		{
			tempZijin.xiaoDanLiuRu = FLT_MIN;
		}
		else tempZijin.xiaoDanLiuRu = atof(xiaoDanLiuRu);
		//xiaoDanLiuChu
		char xiaoDanLiuChu[CHAR_LEN];
		strcpy(xiaoDanLiuChu, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanLiuChu, nonChar))
		{
			tempZijin.xiaoDanLiuChu = FLT_MIN;
		}
		else tempZijin.xiaoDanLiuChu = atof(xiaoDanLiuChu);
		//xiaoDanJinE
		char xiaoDanJinE[CHAR_LEN];
		strcpy(xiaoDanJinE, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanJinE, nonChar))
		{
			tempZijin.xiaoDanJinE = FLT_MIN;
		}
		else tempZijin.xiaoDanJinE = atof(xiaoDanJinE);
		//zhangSu
		char zhangSu[CHAR_LEN];
		strcpy(zhangSu, strVec[i++].c_str());
		if (0 == strcmp(zhangSu, nonChar))
		{
			tempZijin.zhangSu = FLT_MIN;
		}
		else tempZijin.zhangSu = atof(zhangSu);
		//liangBi
		char liangBi[CHAR_LEN];
		strcpy(liangBi, strVec[i++].c_str());
		if (0 == strcmp(liangBi, nonChar))
		{
			tempZijin.liangBi = FLT_MIN;
		}
		else tempZijin.liangBi = atof(liangBi);
		//zongShou
		char zongShou[CHAR_LEN];
		strcpy(zongShou, strVec[i++].c_str());
		if (0 == strcmp(zongShou, nonChar))
		{
			tempZijin.zongShou = FLT_MIN;
		}
		else tempZijin.zongShou = atof(zongShou);
		//zongJinE
		char zongJinE[CHAR_LEN];
		strcpy(zongJinE, strVec[i++].c_str());
		if (0 == strcmp(zongJinE, nonChar))
		{
			tempZijin.zongJinE = FLT_MIN;
		}
		else tempZijin.zongJinE = atof(zongJinE);
		//daDanJingEB
		char daDanJingEB[CHAR_LEN];
		strcpy(daDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(daDanJingEB, nonChar))
		{
			tempZijin.daDanJingEB = FLT_MIN;
		}
		else tempZijin.daDanJingEB = atof(daDanJingEB);
		//daDanZongE
		char daDanZongE[CHAR_LEN];
		strcpy(daDanZongE, strVec[i++].c_str());
		if (0 == strcmp(daDanZongE, nonChar))
		{
			tempZijin.daDanZongE = FLT_MIN;
		}
		else tempZijin.daDanZongE = atof(daDanZongE);
		//daDanZongEB
		char daDanZongEB[CHAR_LEN];
		strcpy(daDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(daDanZongEB, nonChar))
		{
			tempZijin.daDanZongEB = FLT_MIN;
		}
		else tempZijin.daDanZongEB = atof(daDanZongEB);
		//zhongDanJingEB
		char zhongDanJingEB[CHAR_LEN];
		strcpy(zhongDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(zhongDanJingEB, nonChar))
		{
			tempZijin.zhongDanJingEB = FLT_MIN;
		}
		else tempZijin.zhongDanJingEB = atof(zhongDanJingEB);
		//zhongDanZongE
		char zhongDanZongE[CHAR_LEN];
		strcpy(zhongDanZongE, strVec[i++].c_str());
		if (0 == strcmp(zhongDanZongE, nonChar))
		{
			tempZijin.zhongDanZongE = FLT_MIN;
		}
		else tempZijin.zhongDanZongE = atof(zhongDanZongE);
		//zhongDanZongEB
		char zhongDanZongEB[CHAR_LEN];
		strcpy(zhongDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(zhongDanZongEB, nonChar))
		{
			tempZijin.zhongDanZongEB = FLT_MIN;
		}
		else tempZijin.zhongDanZongEB = atof(zhongDanZongEB);

		//xiaoDanJingEB
		char xiaoDanJingEB[CHAR_LEN];
		strcpy(xiaoDanJingEB, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanJingEB, nonChar))
		{
			tempZijin.xiaoDanJingEB = FLT_MIN;
		}
		else tempZijin.xiaoDanJingEB = atof(xiaoDanJingEB);
		//xiaoDanZongE
		char xiaoDanZongE[CHAR_LEN];
		strcpy(xiaoDanZongE, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanZongE, nonChar))
		{
			tempZijin.xiaoDanZongE = FLT_MIN;
		}
		else tempZijin.xiaoDanZongE = atof(xiaoDanZongE);
		//xiaoDanZongEB
		char xiaoDanZongEB[CHAR_LEN];
		strcpy(xiaoDanZongEB, strVec[i++].c_str());
		if (0 == strcmp(xiaoDanZongEB, nonChar))
		{
			tempZijin.xiaoDanZongEB = FLT_MIN;
		}
		else tempZijin.xiaoDanZongEB = atof(xiaoDanZongEB);

		zijinVec.push_back(tempZijin);
	}
	fclose(fp);
	return true;
}*/

bool readZhuliFile(char *fileName, vector<ZHULI_t> &zhuliVec)
{
    zhuliVec.clear();
	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		//printf("Open file failed!");
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int fileOffset = 0;
	//for (int i=0; i<1; i++) fscanf(fp,"%*[^\n]%*c");  //junm 1 line

	fileOffset = ftell(fp);

	vector<vector<string>> zhuliStrVec;
	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		string str = lineDateBuf;
		vector<string> strVec;
		strVec.reserve(50);
		stringSplitMultiSymbol(str, "\t", strVec);
		/*for (const auto &i : strVec)
		{
		cout << i << endl;
		}*/
		// move the last empty line
		if (2 < strVec.size())
		{
			zhuliStrVec.push_back(strVec);
		}
	}

	vector<vector<string>>::iterator zhuliStrVecIter = zhuliStrVec.begin();
	//get first line for liutongshizhi
	vector<string> &strVec = *zhuliStrVecIter++;
	std::vector<string>::iterator it;

	bool liutongshizhiFlag = true;

	it = find(strVec.begin(), strVec.end(), "流通市值");
	if (it != strVec.end())
		std::cout << "Element found in myvector: " << *it << '\n';
	else
	{
		std::cout << "Element not found in myvector 流通市值\n";
		liutongshizhiFlag = false;
	}

	while (zhuliStrVecIter != zhuliStrVec.end())
	{
		int i = 0;
		ZHULI_t tempZhuli;
		vector<string> &strVec = *zhuliStrVecIter++;
		strcpy(tempZhuli.code, strVec[i++].c_str());
		strcpy(tempZhuli.name, strVec[i++].c_str());
		//流通市值选项判断
		if (true == liutongshizhiFlag)
		{
			//liuTongShiZhi
			char liuTongShiZhi[CHAR_LEN];
			strcpy(liuTongShiZhi, strVec[i++].c_str());
			if (0 == strcmp(liuTongShiZhi, nonChar))
			{
				tempZhuli.liuTongShiZhi = FLT_MIN;
			}
			else tempZhuli.liuTongShiZhi = atof(liuTongShiZhi);
		}
		//xianJia
		char xianJia[CHAR_LEN];
		strcpy(xianJia, strVec[i++].c_str());
		if (0 == strcmp(xianJia, nonChar))
		{
			tempZhuli.xianJia = FLT_MIN;
		}
		else tempZhuli.xianJia = atof(xianJia);
		//jinRiZengCangZhanBi
		char jinRiZengCangZhanBi[CHAR_LEN];
		strcpy(jinRiZengCangZhanBi, strVec[i++].c_str());
		if (0 == strcmp(jinRiZengCangZhanBi, nonChar))
		{
			tempZhuli.jinRiZengCangZhanBi = FLT_MIN;
		}
		else tempZhuli.jinRiZengCangZhanBi = atof(jinRiZengCangZhanBi);
		//jinRiPaiMing
		char jinRiPaiMing[CHAR_LEN];
		strcpy(jinRiPaiMing, strVec[i++].c_str());
		if (0 == strcmp(jinRiPaiMing, nonChar))
		{
			tempZhuli.jinRiPaiMing = FLT_MIN;
		}
		else tempZhuli.jinRiPaiMing = atof(jinRiPaiMing);
		//jinRiZhangFu
		char jinRiZhangFu[CHAR_LEN];
		strcpy(jinRiZhangFu, strVec[i++].c_str());
		if (0 == strcmp(jinRiZhangFu, nonChar))
		{
			tempZhuli.jinRiZhangFu = FLT_MIN;
		}
		else tempZhuli.jinRiZhangFu = atof(jinRiZhangFu);
		//erRiZengCangZhanBi
		char erRiZengCangZhanBi[CHAR_LEN];
		strcpy(erRiZengCangZhanBi, strVec[i++].c_str());
		if (0 == strcmp(erRiZengCangZhanBi, nonChar))
		{
			tempZhuli.erRiZengCangZhanBi = FLT_MIN;
		}
		else tempZhuli.erRiZengCangZhanBi = atof(erRiZengCangZhanBi);
		//erRiPaiMing
		char erRiPaiMing[CHAR_LEN];
		strcpy(erRiPaiMing, strVec[i++].c_str());
		if (0 == strcmp(erRiPaiMing, nonChar))
		{
			tempZhuli.erRiPaiMing = FLT_MIN;
		}
		else tempZhuli.erRiPaiMing = atof(erRiPaiMing);
		//erRiZhangFu
		char erRiZhangFu[CHAR_LEN];
		strcpy(erRiZhangFu, strVec[i++].c_str());
		if (0 == strcmp(erRiZhangFu, nonChar))
		{
			tempZhuli.erRiZhangFu = FLT_MIN;
		}
		else tempZhuli.erRiZhangFu = atof(erRiZhangFu);
		//sanRiZengCangZhanBi
		char sanRiZengCangZhanBi[CHAR_LEN];
		strcpy(sanRiZengCangZhanBi, strVec[i++].c_str());
		if (0 == strcmp(sanRiZengCangZhanBi, nonChar))
		{
			tempZhuli.sanRiZengCangZhanBi = FLT_MIN;
		}
		else tempZhuli.sanRiZengCangZhanBi = atof(sanRiZengCangZhanBi);
		//sanRiPaiMing
		char sanRiPaiMing[CHAR_LEN];
		strcpy(sanRiPaiMing, strVec[i++].c_str());
		if (0 == strcmp(sanRiPaiMing, nonChar))
		{
			tempZhuli.sanRiPaiMing = FLT_MIN;
		}
		else tempZhuli.sanRiPaiMing = atof(sanRiPaiMing);
		//sanRiZhangFu
		char sanRiZhangFu[CHAR_LEN];
		strcpy(sanRiZhangFu, strVec[i++].c_str());
		if (0 == strcmp(sanRiZhangFu, nonChar))
		{
			tempZhuli.sanRiZhangFu = FLT_MIN;
		}
		else tempZhuli.sanRiZhangFu = atof(sanRiZhangFu);
		//wuRiZengCangZhanBi
		char wuRiZengCangZhanBi[CHAR_LEN];
		strcpy(wuRiZengCangZhanBi, strVec[i++].c_str());
		if (0 == strcmp(wuRiZengCangZhanBi, nonChar))
		{
			tempZhuli.wuRiZengCangZhanBi = FLT_MIN;
		}
		else tempZhuli.wuRiZengCangZhanBi = atof(wuRiZengCangZhanBi);
		//wuRiPaiMing
		char wuRiPaiMing[CHAR_LEN];
		strcpy(wuRiPaiMing, strVec[i++].c_str());
		if (0 == strcmp(wuRiPaiMing, nonChar))
		{
			tempZhuli.wuRiPaiMing = FLT_MIN;
		}
		else tempZhuli.wuRiPaiMing = atof(wuRiPaiMing);
		//wuRiZhangFu
		char wuRiZhangFu[CHAR_LEN];
		strcpy(wuRiZhangFu, strVec[i++].c_str());
		if (0 == strcmp(wuRiZhangFu, nonChar))
		{
			tempZhuli.wuRiZhangFu = FLT_MIN;
		}
		else tempZhuli.wuRiZhangFu = atof(wuRiZhangFu);
		//shiRiZengCangZhanBi
		char shiRiZengCangZhanBi[CHAR_LEN];
		strcpy(shiRiZengCangZhanBi, strVec[i++].c_str());
		if (0 == strcmp(shiRiZengCangZhanBi, nonChar))
		{
			tempZhuli.shiRiZengCangZhanBi = FLT_MIN;
		}
		else tempZhuli.shiRiZengCangZhanBi = atof(shiRiZengCangZhanBi);
		//shiRiPaiMing
		char shiRiPaiMing[CHAR_LEN];
		strcpy(shiRiPaiMing, strVec[i++].c_str());
		if (0 == strcmp(shiRiPaiMing, nonChar))
		{
			tempZhuli.shiRiPaiMing = FLT_MIN;
		}
		else tempZhuli.shiRiPaiMing = atof(shiRiPaiMing);
		//shiRiZhangFu
		char shiRiZhangFu[CHAR_LEN];
		strcpy(shiRiZhangFu, strVec[i++].c_str());
		if (0 == strcmp(shiRiZhangFu, nonChar))
		{
			tempZhuli.shiRiZhangFu = FLT_MIN;
		}
		else tempZhuli.shiRiZhangFu = atof(shiRiZhangFu);
		//zhangSu
		char zhangSu[CHAR_LEN];
		strcpy(zhangSu, strVec[i++].c_str());
		if (0 == strcmp(zhangSu, nonChar))
		{
			tempZhuli.zhangSu = FLT_MIN;
		}
		else tempZhuli.zhangSu = atof(zhangSu);
		//liangBi
		char liangBi[CHAR_LEN];
		strcpy(liangBi, strVec[i++].c_str());
		if (0 == strcmp(liangBi, nonChar))
		{
			tempZhuli.liangBi = FLT_MIN;
		}
		else tempZhuli.liangBi = atof(liangBi);
		//huanShou
		char huanShou[CHAR_LEN];
		strcpy(huanShou, strVec[i++].c_str());
		if (0 == strcmp(huanShou, nonChar))
		{
			tempZhuli.huanShou = FLT_MIN;
		}
		else tempZhuli.huanShou = atof(huanShou);
		//zongShou
		char zongShou[CHAR_LEN];
		strcpy(zongShou, strVec[i++].c_str());
		if (0 == strcmp(zongShou, nonChar))
		{
			tempZhuli.zongShou = FLT_MIN;
		}
		else tempZhuli.zongShou = atof(zongShou);
		//zongJinE
		char zongJinE[CHAR_LEN];
		strcpy(zongJinE, strVec[i++].c_str());
		if (0 == strcmp(zongJinE, nonChar))
		{
			tempZhuli.zongJinE = FLT_MIN;
		}
		else tempZhuli.zongJinE = atof(zongJinE);

		zhuliVec.push_back(tempZhuli);
	}
    fclose(fp);
    return true;
}

bool readZhangfuFile(char *fileName, vector<ZHANGFU_t> &zhangfuVec)
{
    zhangfuVec.clear();
	int cnt_index = 0, cnt_int = 0, cnt_num = 0;
	//cnt_int 用于存放字符串中的数字.
	//cnt_index 作为字符串b的下标.
	int flag_20180901 = 0;
	for (int i = 0;fileName[i] != '\0';++i) //当a数组元素不为结束符时.遍历字符串a.
	{
		if (fileName[i] >= '0'&& fileName[i] <= '9') //如果是数字.
		{
			cnt_num++;
			cnt_int *= 10;
			cnt_int += fileName[i] - '0'; //数字字符的ascii-字符'0'的ascii码就等于该数字.
			if (cnt_num >= 8) //取8位数字进行判断20180522
			{
				break;
			}
		}
	}
	if (cnt_int > 20180901)
	{
		flag_20180901 = 1;
	}

	FILE *fp = fopen(fileName, "rt");
	if (NULL == fp)
	{
		//printf("Open DDE file failed!");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int fileOffset = 0;
	for (int i = 0; i<1; i++) fscanf(fp, "%*[^\n]%*c");  //jump 1 line

	fileOffset = ftell(fp);

	vector<vector<string>> zhangfuStrVec;
	while (fgets(lineDateBuf, lineDataLen, fp) != NULL)
	{
		string str = lineDateBuf;
		vector<string> strVec;
		strVec.reserve(50);
		stringSplitMultiSymbol(str, "\t", strVec);
		/*for (const auto &i : strVec)
		{
		cout << i << endl;
		}*/
		// move the last empty line
		if (2 < strVec.size())
		{
			zhangfuStrVec.push_back(strVec);
		}
	}

	int itemNum = 0;
	vector<vector<string>>::iterator zhangfuStrVecIter = zhangfuStrVec.begin();
	while (zhangfuStrVecIter != zhangfuStrVec.end())
	{
		int i = 0;
		itemNum++;
		ZHANGFU_t tempZhangfu;
		vector<string> &strVec = *zhangfuStrVecIter++;
		strcpy(tempZhangfu.code, strVec[i++].c_str());
		strcpy(tempZhangfu.name, strVec[i++].c_str());
		//liuTongShiZhi
		char liuTongShiZhi[CHAR_LEN];
		strcpy(liuTongShiZhi, strVec[i++].c_str());
		if (0 == strcmp(liuTongShiZhi, nonChar))
		{
			tempZhangfu.liuTongShiZhi = FLT_MIN;
		}
		else tempZhangfu.liuTongShiZhi = atof(liuTongShiZhi);
		/*if (1 == flag_20180901)
		{
			//ziYouLiuTongShiZhi
			char ziYouLiuTongShiZhi[CHAR_LEN];
			strcpy(ziYouLiuTongShiZhi, strVec[i++].c_str());
			if (0 == strcmp(ziYouLiuTongShiZhi, nonChar))
			{
				tempZhangfu.ziYouLiuTongShiZhi = FLT_MIN;
			}
			else tempZhangfu.ziYouLiuTongShiZhi = atof(ziYouLiuTongShiZhi);
		}
		else
		{
			tempZhangfu.ziYouLiuTongShiZhi = 0;
		}*/
		//huanShou
		char huanShou[CHAR_LEN];
		strcpy(huanShou, strVec[i++].c_str());
		if (0 == strcmp(huanShou, nonChar))
		{
			tempZhangfu.huanShou = FLT_MIN;
		}
		else tempZhangfu.huanShou = atof(huanShou);
		//xianJia
		char xianJia[CHAR_LEN];
		strcpy(xianJia, strVec[i++].c_str());
		if (0 == strcmp(xianJia, nonChar))
		{
			tempZhangfu.xianJia = FLT_MIN;
		}
		else tempZhangfu.xianJia = atof(xianJia);
		//zhangFu
		char zhangFu[CHAR_LEN];
		strcpy(zhangFu, strVec[i++].c_str());
		if (0 == strcmp(zhangFu, nonChar))
		{
			tempZhangfu.zhangFu = FLT_MIN;
		}
		else tempZhangfu.zhangFu = atof(zhangFu);
		//zhangDie
		char zhangDie[CHAR_LEN];
		strcpy(zhangDie, strVec[i++].c_str());
		if (0 == strcmp(zhangDie, nonChar))
		{
			tempZhangfu.zhangDie = FLT_MIN;
		}
		else tempZhangfu.zhangDie = atof(zhangDie);
		//zhuLiJingLiang
		char zhuLiJingLiang[CHAR_LEN];
		strcpy(zhuLiJingLiang, strVec[i++].c_str());
		if (0 == strcmp(zhuLiJingLiang, nonChar))
		{
			tempZhangfu.zhuLiJingLiang = FLT_MIN;
		}
		else tempZhangfu.zhuLiJingLiang = atof(zhuLiJingLiang);
		//sanHuShuLiang
		char sanHuShuLiang[CHAR_LEN];
		strcpy(sanHuShuLiang, strVec[i++].c_str());
		if (0 == strcmp(sanHuShuLiang, nonChar))
		{
			tempZhangfu.sanHuShuLiang = FLT_MIN;
		}
		else tempZhangfu.sanHuShuLiang = atof(sanHuShuLiang);
		//zhuLiJingE
		char zhuLiJingE[CHAR_LEN];
		strcpy(zhuLiJingE, strVec[i++].c_str());
		if (0 == strcmp(zhuLiJingE, nonChar))
		{
			tempZhangfu.zhuLiJingE = FLT_MIN;
		}
		else tempZhangfu.zhuLiJingE = atof(zhuLiJingE);
		//guXingPingFen
		char guXingPingFen[CHAR_LEN];
		strcpy(guXingPingFen, strVec[i++].c_str());
		if (0 == strcmp(guXingPingFen, nonChar))
		{
			tempZhangfu.guXingPingFen = FLT_MIN;
		}
		else tempZhangfu.guXingPingFen = atof(guXingPingFen);
		//liangBi
		char liangBi[CHAR_LEN];
		strcpy(liangBi, strVec[i++].c_str());
		if (0 == strcmp(liangBi, nonChar))
		{
			tempZhangfu.liangBi = FLT_MIN;
		}
		else tempZhangfu.liangBi = atof(guXingPingFen);
		//weiBi
		char weiBi[CHAR_LEN];
		strcpy(weiBi, strVec[i++].c_str());
		if (0 == strcmp(weiBi, nonChar))
		{
			tempZhangfu.weiBi = FLT_MIN;
		}
		else tempZhangfu.weiBi = atof(weiBi);
		//jiGouDongXiang
		char jiGouDongXiang[CHAR_LEN];
		strcpy(jiGouDongXiang, strVec[i++].c_str());
		if (0 == strcmp(jiGouDongXiang, nonChar))
		{
			tempZhangfu.jiGouDongXiang = FLT_MIN;
		}
		else tempZhangfu.jiGouDongXiang = atof(jiGouDongXiang);
		//jingLiZengLv
		char jingLiZengLv[CHAR_LEN];
		strcpy(jingLiZengLv, strVec[i++].c_str());
		if (0 == strcmp(jingLiZengLv, nonChar))
		{
			tempZhangfu.jingLiZengLv = FLT_MIN;
		}
		else tempZhangfu.jingLiZengLv = atof(jingLiZengLv);
		//jingLiRun
		char jingLiRun[CHAR_LEN];
		strcpy(jingLiRun, strVec[i++].c_str());
		if (0 == strcmp(jingLiRun, nonChar))
		{
			tempZhangfu.jingLiRun = FLT_MIN;
		}
		else tempZhangfu.jingLiRun = atof(jingLiRun);
		//shiJingLv
		char shiJingLv[CHAR_LEN];
		strcpy(shiJingLv, strVec[i++].c_str());
		if (0 == strcmp(shiJingLv, nonChar))
		{
			tempZhangfu.shiJingLv = FLT_MIN;
		}
		else tempZhangfu.shiJingLv = atof(shiJingLv);
		//waiPan
		char waiPan[CHAR_LEN];
		strcpy(waiPan, strVec[i++].c_str());
		if (0 == strcmp(waiPan, nonChar))
		{
			tempZhangfu.waiPan = FLT_MIN;
		}
		else tempZhangfu.waiPan = atof(waiPan);
		//neiPan
		char neiPan[CHAR_LEN];
		strcpy(neiPan, strVec[i++].c_str());
		if (0 == strcmp(neiPan, nonChar))
		{
			tempZhangfu.neiPan = FLT_MIN;
		}
		else tempZhangfu.neiPan = atof(neiPan);
		//zhenFu
		char zhenFu[CHAR_LEN];
		strcpy(zhenFu, strVec[i++].c_str());
		if (0 == strcmp(zhenFu, nonChar))
		{
			tempZhangfu.zhenFu = FLT_MIN;
		}
		else tempZhangfu.zhenFu = atof(zhenFu);
		//zhangSu
		char zhangSu[CHAR_LEN];
		strcpy(zhangSu, strVec[i++].c_str());
		if (0 == strcmp(zhangSu, nonChar))
		{
			tempZhangfu.zhangSu = FLT_MIN;
		}
		else tempZhangfu.zhangSu = atof(zhangSu);
		//zongJinE
		char zongJinE[CHAR_LEN];
		strcpy(zongJinE, strVec[i++].c_str());
		if (0 == strcmp(zongJinE, nonChar))
		{
			tempZhangfu.zongJinE = FLT_MIN;
		}
		else tempZhangfu.zongJinE = atof(zongJinE);
		//zongShiZhi
		char zongShiZhi[CHAR_LEN];
		strcpy(zongShiZhi, strVec[i++].c_str());
		if (0 == strcmp(zongShiZhi, nonChar))
		{
			tempZhangfu.zongShiZhi = FLT_MIN;
		}
		else tempZhangfu.zongShiZhi = atof(zongShiZhi);
		//zongShou
		char zongShou[CHAR_LEN];
		strcpy(zongShou, strVec[i++].c_str());
		if (0 == strcmp(zongShou, nonChar))
		{
			tempZhangfu.zongShou = FLT_MIN;
		}
		else tempZhangfu.zongShou = atof(zongShou);
		//xianShou
		char xianShou[CHAR_LEN];
		strcpy(xianShou, strVec[i++].c_str());
		if (0 == strcmp(xianShou, nonChar))
		{
			tempZhangfu.xianShou = FLT_MIN;
		}
		else tempZhangfu.xianShou = atof(xianShou);
		//kaiPan
		char kaiPan[CHAR_LEN];
		strcpy(kaiPan, strVec[i++].c_str());
		if (0 == strcmp(kaiPan, nonChar))
		{
			tempZhangfu.kaiPan = FLT_MIN;
		}
		else tempZhangfu.kaiPan = atof(kaiPan);
		//zuoShou
		char zuoShou[CHAR_LEN];
		strcpy(zuoShou, strVec[i++].c_str());
		if (0 == strcmp(zuoShou, nonChar))
		{
			tempZhangfu.zuoShou = FLT_MIN;
		}
		else tempZhangfu.zuoShou = atof(zuoShou);
		//zuiGao
		char zuiGao[CHAR_LEN];
		strcpy(zuiGao, strVec[i++].c_str());
		if (0 == strcmp(zuiGao, nonChar))
		{
			tempZhangfu.zuiGao = FLT_MIN;
		}
		else tempZhangfu.zuiGao = atof(zuiGao);
		//zuiDi
		char zuiDi[CHAR_LEN];
		strcpy(zuiDi, strVec[i++].c_str());
		if (0 == strcmp(zuiDi, nonChar))
		{
			tempZhangfu.zuiDi = FLT_MIN;
		}
		else tempZhangfu.zuiDi = atof(zuiDi);
		//wuRiJunXian
		char wuRiJunXian[CHAR_LEN];
		strcpy(wuRiJunXian, strVec[i++].c_str());
		if (0 == strcmp(wuRiJunXian, nonChar))
		{
			tempZhangfu.wuRiJunXian = FLT_MIN;
		}
		else tempZhangfu.wuRiJunXian = atof(wuRiJunXian);
		//buyLiang
		char buyLiang[CHAR_LEN];
		strcpy(buyLiang, strVec[i++].c_str());
		if (0 == strcmp(buyLiang, nonChar))
		{
			tempZhangfu.buyLiang = FLT_MIN;
		}
		else tempZhangfu.buyLiang = atof(buyLiang);
		//sellLiant
		char sellLiant[CHAR_LEN];
		strcpy(sellLiant, strVec[i++].c_str());
		if (0 == strcmp(sellLiant, nonChar))
		{
			tempZhangfu.sellLiant = FLT_MIN;
		}
		else tempZhangfu.sellLiant = atof(sellLiant);
		//jinChaGeShu
		char jinChaGeShu[CHAR_LEN];
		strcpy(jinChaGeShu, strVec[i++].c_str());
		if (0 == strcmp(jinChaGeShu, nonChar))
		{
			tempZhangfu.jinChaGeShu = FLT_MIN;
		}
		else tempZhangfu.jinChaGeShu = atoi(jinChaGeShu);
		//shouMeiBi
		char shouMeiBi[CHAR_LEN];
		strcpy(shouMeiBi, strVec[i++].c_str());
		if (0 == strcmp(shouMeiBi, nonChar))
		{
			tempZhangfu.shouMeiBi = FLT_MIN;
		}
		else tempZhangfu.shouMeiBi = atoi(shouMeiBi);

		/*if (1 == flag_20180901)
		{
			//zongGuBen
			char zongGuBen[CHAR_LEN];
			strcpy(zongGuBen, strVec[i++].c_str());
			if (0 == strcmp(zongGuBen, nonChar))
			{
				tempZhangfu.zongGuBen = FLT_MIN;
			}
			else tempZhangfu.zongGuBen = atof(zongGuBen);
			//liuTongGuBen
			char liuTongGuBen[CHAR_LEN];
			strcpy(liuTongGuBen, strVec[i++].c_str());
			if (0 == strcmp(liuTongGuBen, nonChar))
			{
				tempZhangfu.liuTongGuBen = FLT_MIN;
			}
			else tempZhangfu.liuTongGuBen = atof(liuTongGuBen);
			//liuTongAGu
			char liuTongAGu[CHAR_LEN];
			strcpy(liuTongAGu, strVec[i++].c_str());
			if (0 == strcmp(liuTongAGu, nonChar))
			{
				tempZhangfu.liuTongAGu = FLT_MIN;
			}
			else tempZhangfu.liuTongAGu = atof(liuTongAGu);
		}
		else
		{
			tempZhangfu.zongGuBen = 0;
			tempZhangfu.liuTongGuBen = 0;
			tempZhangfu.liuTongAGu = 0;
		}*/

		//suoShuHangYe
		strcpy(tempZhangfu.suoShuHangYe, strVec[i++].c_str());

		//xingJi
		char xingJi[CHAR_LEN];
		strcpy(xingJi, strVec[i++].c_str());
		if (0 == strcmp(xingJi, nonChar))
		{
			tempZhangfu.xingJi = FLT_MIN;
		}
		else tempZhangfu.xingJi = atof(xingJi);

		//市盈率为亏损时，导出的数据为空，因此每行数据不固定
		tempZhangfu.shiYingDong = FLT_MIN;
		tempZhangfu.shiYingJing = FLT_MIN;
		tempZhangfu.shiYingTTM  = FLT_MIN;
		if (strVec.size() >= (i + 3))
		{
			//shiYingDong
			char shiYingDong[CHAR_LEN];
			strcpy(shiYingDong, strVec[i++].c_str());
			if (0 == strcmp(shiYingDong, nonChar))
			{
				tempZhangfu.shiYingDong = FLT_MIN;
			}
			else tempZhangfu.shiYingDong = atof(shiYingDong);
			//shiYingJing
			char shiYingJing[CHAR_LEN];
			strcpy(shiYingJing, strVec[i++].c_str());
			if (0 == strcmp(shiYingJing, nonChar))
			{
				tempZhangfu.shiYingJing = FLT_MIN;
			}
			else tempZhangfu.shiYingJing = atof(shiYingJing);
			//shiYingTTM
			char shiYingTTM[CHAR_LEN];
			strcpy(shiYingTTM, strVec[i++].c_str());
			if (0 == strcmp(shiYingTTM, nonChar))
			{
				tempZhangfu.shiYingTTM = FLT_MIN;
			}
			else tempZhangfu.shiYingTTM = atof(shiYingTTM);
		}
		
		zhangfuVec.push_back(tempZhangfu);
	}

	fclose(fp);
	return true;
}

void fileNameCat(char *ddeFileName, 
	char *zijinName, 
	char *zhuliFileName, 
	char *zhangfuFileName, 
	char *zhangtingFileName, 
	char *path, 
	char *date)
{
	memset(ddeFileName, 0, sizeof(ddeFileName));
	memset(zijinName, 0, sizeof(zijinName));
	memset(zhuliFileName, 0, sizeof(zhuliFileName));
	memset(zhangfuFileName, 0, sizeof(zhangfuFileName));
	memset(zhangtingFileName, 0, sizeof(zhangtingFileName));

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

	strcpy(zhangfuFileName, path);
	strcat(zhangfuFileName, "zhangfu_");
	strcat(zhangfuFileName, date);
	strcat(zhangfuFileName, ".txt");

	strcpy(zhangtingFileName, path);
	strcat(zhangtingFileName, "zhangting_");
	strncat(zhangtingFileName, date, 8);
	strcat(zhangtingFileName, ".txt");
}
}
