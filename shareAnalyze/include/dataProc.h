#ifndef _DATA_PROC_H_
#define _DATA_PROC_H_

#include <vector>
#include <stdio.h>

using namespace std;

namespace DATAPROC
{
	bool readDdeFile(char *fileName, vector<DDE_t> &ddeVec);
	bool readZijinFile(char *fileName, vector<ZIJIN_t> &zijinVec);
	bool readZhuliFile(char *fileName, vector<ZHULI_t> &zhuliVec);
	bool readZhangfuFile(char *fileName, vector<ZHANGFU_t> &zhangfuVec);
	void fileNameCat(char *ddeFileName, 
		char *zijinName, 
		char *zhuliFileName, 
		char *zhangfuFileName, 
		char *zhangtingFileName, 
		char *path, 
		char *date);
}
#endif