#pragma once

#include <vector>
#include <cstdio>
#include "dataStructure.h"
namespace AUTOSHAREBUY
{
void autoShareBuy(FILE *buyFp, std::vector<PROPERTY_t> &propertyAnalyVec, PROPERTY_t &programBuyProty, int &programFindFlag);
void sortByGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByZhangfuGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByTimeZhangfuGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByTimeZhangfuLimitVsDealJingJia(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByZhangfuLimitVsDealJingJiaGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByFirstLimitTimeGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);
void sortByLimitVsCirculateGaodu(std::vector<PROPERTY_t> &propertyAnalyVec);

void selectFirstShare(
	int fileIndex,
	FILE *rstFp,
	FILE *buyFp, 
	PROPERTY_t programBuyProty, 
	int programFindFlag, 
	std::vector<PROPERTY_t> &propertyAnalyVec, 
	std::vector<PROPERTY_t> &propertyAnalyVecSort, 
	std::vector<PROPERTY_t> &propertyAnalyVecPre,
	std::vector<std::string> &resultSet,
	std::vector<std::string> &resultSetWbgk,
	std::vector<PROPERTY_t> &resultVecWbgk,
	std::vector<PROPERTY_t> &propertyAnalyVecBlock);
float getZuoRiJiaCha(PROPERTY_t &analyProty);
void changeFinalBuy(PROPERTY_t &finalBuy, PROPERTY_t &analyProty, int &buyFlag);
void getYiZiBanToday(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<PROPERTY_t> &yiZiBanTodayPropertyAnalyVec);
void saveResultCode(char *rstCode, std::vector<std::string> &resultSet);
}