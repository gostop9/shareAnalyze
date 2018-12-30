#pragma once

#include <vector>
#include <cstdio>
#include "dataStructure.h"
namespace AUTOSHAREBUY
{
void autoShareBuy(FILE *buyFp, std::vector<PROPERTY_t> &propertyAnalyVec, PROPERTY_t &programBuyProty, int &programFindFlag);
void selectFirstShare(FILE *rstFp, 
	FILE *buyFp, 
	PROPERTY_t programBuyProty, 
	int programFindFlag, 
	std::vector<PROPERTY_t> &propertyAnalyVec, 
	std::vector<PROPERTY_t> &propertyAnalyVecSort, 
	std::vector<PROPERTY_t> &propertyAnalyVecPre);
}