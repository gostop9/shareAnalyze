#ifndef _DATA_PROC_H_
#define _DATA_PROC_H_

#include <vector>
#include <stdio.h>

using namespace std;

const int lineDataLen = 1000;

extern char lineDateBuf[];

bool readDdeFile(char *fileName, vector<DDE_t> &ddeVec);
bool readZijinFile(char *fileName, vector<ZIJIN_t> &zijinVec);
bool readZhuliFile(char *fileName, vector<ZHULI_t> &zhuliVec);
bool readZhangfuFile(char *fileName, vector<ZHANGFU_t> &zhangfuVec);

#endif