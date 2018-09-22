#ifndef _SHARE_SELECT_H_
#define _SHARE_SELECT_H_

#include <vector>
#include <stdio.h>
#include <unordered_set>

#include "readZhangTing.h"

using namespace std;
using namespace ZHANGTING;

void shareSelect(FILE *fp, vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, 
	int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet);

void shareSelectZiJin(FILE *fp, vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::unordered_set<string> &resultSet);

void shareSelectZDZ(FILE *fp, vector<PROPERTY_t> &propertyAnalyVec, int ddeSelectCount, int zijinSelectCount, int zhuliSelectCount, std::vector<string> &resultSet);

void shareParaFuse(vector<DDE_t> &dde, vector<ZIJIN_t> &zijin, vector<ZHULI_t> &zhuli, vector<ZHANGFU_t> &zhangfu, vector<guBen_t> &gubenVec, vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec);

void shareParaFuseWithoutZhuli(vector<ZIJIN_t> &zijin, vector<ZHANGFU_t> &zhangfu, vector<guBen_t> &gubenVec, vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec);

int shareFuseSelect(int &fileIndex, PROPERTY_t &property, vector<vector<PROPERTY_t>> &propertyVecV, vector<string> &selectVec);

void shareSelectPrint(FILE *fp, PROPERTY_t &property, vector<PROPERTY_t> &propertyPre);

void calculateOtherPara(FILE *fp, vector<PROPERTY_t> &propertyVec, vector<PROPERTY_t> &propertyAnalyVec, vector<ZHANGTING::zhangTing_t> &zhangTingVec);

void chooseAnalyzeProperty(vector<PROPERTY_t> &propertyVec, vector<analyseCode_t> &analyVec, vector<PROPERTY_t> &propertyAnalyVec);

#endif

