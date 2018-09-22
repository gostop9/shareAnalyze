#ifndef _DATA_LINE_PROC_H_
#define _DATA_LINE_PROC_H_

#include <vector>
#include <stdio.h>

using namespace std;

bool readDataLineFile(char *fileName, int &lineCountInFile, vector<RIXIAN_t> &riXianVec);

#endif