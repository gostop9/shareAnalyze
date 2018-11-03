/*
change log

20180522 资金流向页面增加 DDE净量 选项

20180526 资金流向页面增加 量比 委比 机构动向 净利润增长率
20180526 增加涨幅排名结构体

20180828 去除DDE页面数据。增加资金页面“委差”数据用于估算竞价后的涨停资金数据。

20180921 增加涨幅排名标签-自由流通市值，总股本，流通股本，流通A股。
*/

#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using std::string;

typedef long long int64;

const int TOTAL_NUMBER = (4000);
const int TOTAL_SHARE_NUMBERS = (4000);

extern float huanShouMax;
extern float liangBiMin;
extern float liangBiMax;
extern float zhuLiJingLiangMin;
extern float daDanJingEBMin;

struct DDE_t
{
    char  code[10];
    char  name[10];
    float zhuLiJingLiang;
    float sanHuShuLiang;
    float zhuLiJinE;
    float zhangFu;
    char  suoShuHangYe[20];
	float xingJi;
    float xianJia;
    float zhangSu;
    float zongShou;
    float huanShou;
    float liangBi;
    float zongJinE;
};

struct ZIJIN_t
{
    char  code[10];
    char  name[10];
    float liuTongShiZhi;
	float ziYouLiuTongShiZhi;//20180901 add，不用读取标签文件	
    float huanShou;
	float xianJia;
    float zhangFu;
	float zhuLiJingLiang; //20180522 add

	//20180526 add
	float liangBi;
	float weiBi;
	float weiCha;//2018828 add
	float jiGouDongXiang;
	float jingLiZengLv;

	//
    float jingLiuRu;
    float daDanLiuRu;
    float daDanLiuChu;
    float daDanJinE;
	float daDanJingEB;
    float daDanZongE;
    float daDanZongEB;
    
    float zhongDanLiuRu;
    float zhongDanLiuChu;
    float zhongDanJinE;
	float zhongDanJingEB;
	float zhongDanZongE;
    float zhongDanZongEB;
    
    float xiaoDanLiuRu;
    float xiaoDanLiuChu;
    float xiaoDanJinE;
    float xiaoDanJingEB;
    float xiaoDanZongE;
	float xiaoDanZongEB;

    float zhangSu;
    //float liangBi;//20180526 remove
    float zongShou;
    float zongJinE;

	float jingLiuBiLiuTong;
	float daDanBiLiuTong;
};
/*
struct ZIJIN_t
{
    char  code[10];
    char  name[10];
    float liuTongShiZhi;
    float huanShou;
    float zhangFu;
    float xianJia;
    float jingLiuRu;
    float daDanLiuRu;
    float daDanLiuChu;
    float daDanJinE;

    float zhongDanLiuRu;
    float zhongDanLiuChu;
    float zhongDanJinE;

    float xiaoDanLiuRu;
    float xiaoDanLiuChu;
    float xiaoDanJinE;
    
    float zhangSu;
    float liangBi;
    float zongShou;
    float zongJinE;
	
	float daDanJingEB;
    float daDanZongE;
    float daDanZongEB;
    float zhongDanJingEB;
	float zhongDanZongE;
    float zhongDanZongEB;
    float xiaoDanJingEB;
    float xiaoDanZongE;
	float xiaoDanZongEB;
};*/

struct ZHULI_t
{
    char  code[10];
    char  name[10];
    float liuTongShiZhi;
    float xianJia;
    float jinRiZengCangZhanBi;
    float jinRiPaiMing;
    float jinRiZhangFu;
    
    float erRiZengCangZhanBi;
    float erRiPaiMing;
    float erRiZhangFu;
    
    float sanRiZengCangZhanBi;
    float sanRiPaiMing;
    float sanRiZhangFu;

    float wuRiZengCangZhanBi;
    float wuRiPaiMing;
    float wuRiZhangFu;

    float shiRiZengCangZhanBi;
    float shiRiPaiMing;
    float shiRiZhangFu;

    float zhangSu;
    float liangBi;
    float huanShou;
    float zongShou;
    float zongJinE;
};


struct ZHANGFU_t
{
    char  code[10];
    char  name[10];
    float liuTongShiZhi;
	float huanShou;
	float xianJia;
	float zhangFu;
	float zhangDie;
	float zhuLiJingLiang;
	float sanHuShuLiang;
	float zhuLiJingE;

	float guXingPingFen;	
	float liangBi;
	float weiBi;
	float jiGouDongXiang;
	float jingLiZengLv;
	float jingLiRun;
	float shiJingLv;
	float waiPan;
	float neiPan;
	float zhenFu;
	float zhangSu;

	float zongJinE;
	float zongShiZhi;
	float zongShou;
	float xianShou;
	float kaiPan;
	float zuoShou;
	float zuiGao;
	float zuiDi;
	float wuRiJunXian;
	float buyLiang;
	float sellLiant;

	int   jinChaGeShu;
	int   shouMeiBi;

	char  suoShuHangYe[20];
	float xingJi;

	float shiYingDong;
	float shiYingJing;
	float shiYingTTM;

	//20180901 add
	float ziYouLiuTongShiZhi;
	float zongGuBen;
	float liuTongGuBen;
	float liuTongAGu;
	//
};

struct RIXIAN_t
{
	char  date[20];
	float kaiPan;
	float zuiGao;
	float zuiDi;
	float shouPan;
	float chengJiaoLiang;
	float chengJiaoE;
};

struct PROPERTY_t
{
	//DDE
	int   ddeIdx;
	char  code[10];
	char  name[10];
	float zhuLiJingLiang;
	float sanHuShuLiang;
	float zhuLiJinE;
	float zhangFu;
	char  suoShuHangYe[20];
	float xingJi;
	float xianJia;
	float zhangSu;
	float zongShou;
	float huanShou;
	float liangBi;
	float zongJinE;

	//zijin
	int   zijinIdx;
	float liuTongShiZhi;
	//float huanShou;
	//float zhangFu;
	//float xianJia;
	float jingLiuRu;
	float daDanLiuRu;
	float daDanLiuChu;
	float daDanJinE;

	float zhongDanLiuRu;
	float zhongDanLiuChu;
	float zhongDanJinE;

	float xiaoDanLiuRu;
	float xiaoDanLiuChu;
	float xiaoDanJinE;

	//float zhangSu;
	//float liangBi;
	//float zongShou;
	//float zongJinE;

	float daDanJingEB;
	float daDanZongE;
	float daDanZongEB;
	float zhongDanJingEB;
	float zhongDanZongE;
	float zhongDanZongEB;
	float xiaoDanJingEB;
	float xiaoDanZongE;
	float xiaoDanZongEB;

	//zhuli
	int   zhuliIdx;
	//float liuTongShiZhi;
	//float xianJia;
	float jinRiZengCangZhanBi;
	float jinRiPaiMing;
	float jinRiZhangFu;

	float erRiZengCangZhanBi;
	float erRiPaiMing;
	float erRiZhangFu;

	float sanRiZengCangZhanBi;
	float sanRiPaiMing;
	float sanRiZhangFu;

	float wuRiZengCangZhanBi;
	float wuRiPaiMing;
	float wuRiZhangFu;

	float shiRiZengCangZhanBi;
	float shiRiPaiMing;
	float shiRiZhangFu;

	float daDanBiLiuTong;

	//float zhangSu;
	//float liangBi;
	//float huanShou;
	//float zongShou;
	//float zongJinE;

	//zhangfu
	int   zhangfuIdx;
//	float liuTongShiZhi;
//	float huanShou;
//	float xianJia;
//	float zhangFu;
	float zhangDie;
//	float zhuLiJingLiang;
//	float sanHuShuLiang;
	float zhuLiJingE;

	float guXingPingFen;
//	float liangBi;
	float weiBi;
	float weiCha;
	float jiGouDongXiang;
	float jingLiZengLv;
	float jingLiRun;
	float shiJingLv;
	float waiPan;
	float neiPan;
	float zhenFu;
//	float zhangSu;

//	float zongJinE;
	float zongShiZhi;
//	float zongShou;
	float xianShou;
	float kaiPan;
	float zuoShou;
	float zuiGao;
	float zuiDi;
	float wuRiJunXian;
	float buyLiang;
	float sellLiant;

	int   jinChaGeShu;
	int   shouMeiBi;

	//char  suoShuHangYe[20];
	//float xingJi;

	float shiYingDong;
	float shiYingJing;
	float shiYingTTM;

	//20180901 add
	float ziYouLiuTongShiZhi;
	//float zongGuBen;
	//float liuTongGuBen;
	//float liuTongAGu;
	//
	float ziYouLiuTongGu;
	//

	float zhuLiJingBiLiuTong;
	float neiWaiPanBiLiuTongGu;
	float jingLiuRuBiLiuTong;
	float daDanJinBiLiuTong;
	float zhongDanJinBiLiuTong;
	float xiaoDanJinBiLiuTong;
	float zhongXiaoDanJinBiLiuTong;
	float zhongXiaoDanZongBiLiuTong;
	float daDanLiuRuBiLiuTong;
	float zhongXiaoLiuRuBiLiuTong;
	float daXiaoLiuRuBiLiuTong;
	float xiaoDanLiuRuBiLiuTong;
	float zhongDanLiuRuBiLiuTong;
	float daZhongLiuRuBiLiuTong;
	float zongLiuRuBiLiuTong;
	float zongLiuChuBiLiuTong;
	float chengJiaoBiLiuTong;
	float chengJiaoJinE;
	float jingJiaLiangBi;
	float zhongXiaoBiJingLiuRu;
	float zuoRiHuanShou;
	float zuoRiKaiPan;
	float zuoRiZuiGao;
	float zuoRiLiangBi;
	float zuoRiZhenFu;

	//zhang ting
	int   jingLiuRuBiLiuTongIndex;
	int   zongLiuRuBiLiuTongIndex;
	int   zongLiuChuBiLiuTongIndex;
	int   chengJiaoBiLiuTongIndex;
	int   indexLvsC;
	int   continueDay;
	char  firstLimitTime[10];
	char  lastLimitTime[10];
	int   limitOpenCount;
	float limitVsDeal;
	float limitVsCirculate;
	float limitUpMoney;
	char  limitReason[50];
};

/*
struct DDE_t
{
    char  code[10];
    char  name[10];
    float zhuLiJingLiang;
    float sanHuShuLiang;
    float zhuLiJinE;
    float zhangFu;
    char  suoShuHangYe[20];
    float xingJi;
    float xianJia;
    float zhangSu;
    int64 zongShou;
    float huanShou;
    float liangBi;
    int64 zongJinE;
};

struct ZIJIN_t
{
    char  code[10];
    char  name[10];
    int64 liuTongShiZhi;
    float huanShou;
    float zhangFu;
    float xianJia;
    int64 jingLiuRu;
    int64 daDanLiuRu;
    int64 daDanLiuChu;
    int64 daDanJinE;

    int64 zhongDanLiuRu;
    int64 zhongDanLiuChu;
    int64 zhongDanJinE;

    int64 xiaoDanLiuRu;
    int64 xiaoDanLiuChu;
    int64 xiaoDanJinE;
    
    float zhangSu;
    float liangBi;
    int64 zongShou;
    int64 zongJinE;
};

struct ZHULI_t
{
    char  code[10];
    char  name[10];
    int64 liuTongShiZhi;
    float xianJia;
    float jinRiZengCangZhanBi;
    int   jinRiPaiMing;
    float jinRiZhangFu;
    
    float erRiZengCangZhanBi;
    int   erRiPaiMing;
    float erRiZhangFu;
    
    float sanRiZengCangZhanBi;
    int   sanRiPaiMing;
    float sanRiZhangFu;

    float wuRiZengCangZhanBi;
    int   wuRiPaiMing;
    float wuRiZhangFu;

    float shiRiZengCangZhanBi;
    int   shiRiPaiMing;
    float shiRiZhangFu;

    float zhangSu;
    float liangBi;
    float huanShou;
    int64 zongShou;
    int64 zongJinE;
};*/

struct analyseCode_t
{
	char  code[10];
};

struct guBen_t
{
	char  code[10];
	char  name[20];
	//float ziYouLiuTongShiZhi;
	float ziYouLiuTongGu;
	float guXingPingFen;
};

#endif
