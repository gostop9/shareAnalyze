#pragma once

#include <vector>
#include <set>
#include <cstdio>
#include "dataStructure.h"

namespace ZHANGTING
{
struct zhangTing_t
{
    char  code[10];
    char  name[10];
	int   indexLvsC;
    float liangBi;
    float ziYouHuanShouLv;
    float ziYouLiuTongShiZhi;
    int   continueDay;
	char  firstLimitTime[10];
	char  lastLimitTime[10];
    int   limitOpenCount;
    float limitVsDeal;
    float limitVsCirculate;
    float limitUpMoney;
    char  limitReason[50];
    float guXingPingFen;
    float limitShareNumber;
    float startPrice;
    float endPrice;
    float maxPrice;
    float minPrice;
	float zhenFu;
    float chengJiaoE;
    float chengJiaoLiang;
    float huanShouLv;
    float ziYouLiuTongGu;
    float liuTongGu;
    float liuTongShiZhi;
    int   ipoDays;
    float zongGuBen;
    float hangYe;
    int   minLen;
    int   dateYMD;
};

struct limitVsCirculate_t
{
	bool operator() (const zhangTing_t &a, const zhangTing_t &b) { return ((a.limitVsCirculate) > (b.limitVsCirculate)); }
};

struct continueDay_t
{
	bool operator() (const zhangTing_t &a, const zhangTing_t &b) { return ((a.continueDay) > (b.continueDay)); }
};

void getZhangTingCode(std::vector<zhangTing_t> &zhangTingVec, std::vector<analyseCode_t> &codeVec);

bool readZhangTingFile(char *fileName, std::vector<zhangTing_t> &zhangtingVec);
}
