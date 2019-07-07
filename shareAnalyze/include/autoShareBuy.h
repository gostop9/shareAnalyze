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
void selectFirstShare(FILE *rstFp, 
	FILE *buyFp, 
	PROPERTY_t programBuyProty, 
	int programFindFlag, 
	std::vector<PROPERTY_t> &propertyAnalyVec, 
	std::vector<PROPERTY_t> &propertyAnalyVecSort, 
	std::vector<PROPERTY_t> &propertyAnalyVecPre);
float getZuoRiJiaCha(PROPERTY_t &analyProty);
void changeFinalBuy(PROPERTY_t &finalBuy, PROPERTY_t &analyProty, int &buyFlag);
}