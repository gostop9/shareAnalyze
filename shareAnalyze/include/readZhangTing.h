#pragma once

#include <vector>
#include <cstdio>
namespace ZHANGTING
{
struct zhangTing_t
{
    char  code[10];
    char  name[10];
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
bool readZhangTingFile(char *fileName, std::vector<zhangTing_t> &zhangtingVec);
}
