#ifndef _SHARE_SELECT_H_
#define _SHARE_SELECT_H_

#include <vector>
#include <stdio.h>
#include <unordered_set>

#include "readZhangTing.h"

using namespace ZHANGTING;

void shareSelect(FILE *fp, std::vector<DDE_t> &dde, std::vector<ZIJIN_t> &zijin, std::vector<ZHULI_t> &zhuli, 
	int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet);

void shareSelectZiJin(FILE *fp, std::vector<DDE_t> &dde, std::vector<ZIJIN_t> &zijin, std::vector<ZHULI_t> &zhuli, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet);

void shareSelectZDZ(FILE *fp, std::vector<PROPERTY_t> &propertyAnalyVec, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::vector<string> &resultSet);

void shareSelectFinal(FILE *fp, std::vector<PROPERTY_t> &propertyVec);

void shareParaFuse(std::vector<DDE_t> &dde, std::vector<ZIJIN_t> &zijin, std::vector<ZHULI_t> &zhuli, std::vector<ZHANGFU_t> &zhangfu, std::vector<ZHANGFU_t> &preJingJiazhangfu, std::vector<guBen_t> &gubenVec, std::vector<PROPERTY_t> &propertyVec, std::vector<analyseCode_t> &analyVec, std::vector<PROPERTY_t> &propertyAnalyVec);

void shareParaFuseWithoutZhuli(std::vector<ZIJIN_t> &zijin, std::vector<ZHANGFU_t> &zhangfu, std::vector<ZHANGFU_t> &preJingJiazhangfu, std::vector<guBen_t> &gubenVec, std::vector<PROPERTY_t> &propertyVec, std::vector<analyseCode_t> &analyVec, std::vector<PROPERTY_t> &propertyAnalyVec);

int shareFuseSelect(int &fileIndex, PROPERTY_t &property, std::vector<std::vector<PROPERTY_t>> &propertyVecV, std::vector<string> &selectVec);

void shareSelectPrint(FILE *fp, PROPERTY_t &property, std::vector<PROPERTY_t> &propertyPre);

void calculateOtherPara(FILE *fp, std::vector<PROPERTY_t> &propertyVec, std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<ZHANGTING::zhangTing_t> &zhangTingVec);

void chooseAnalyzeProperty(std::vector<PROPERTY_t> &propertyVec, std::vector<analyseCode_t> &analyVec, std::vector<PROPERTY_t> &propertyAnalyVec);

bool yiZiBanJudge(PROPERTY_t &property);

bool zuoRiYiZiBanTbanJudge(PROPERTY_t &property);

bool zuoRiYiZiBanJudge(PROPERTY_t &property);

void zhangFuSummarize(PROPERTY_t &property, int &zhangCount, int &dieCount, int &yiZiBanCount, int &xinGuCount, int &stCount, float &averageIncrease, int &codeCount);

#endif

