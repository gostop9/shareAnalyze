/*
change log

20180522 资金流向页面增加 DDE净量 选项

20180526 资金流向页面增加 量比 委比 机构动向 净利润增长率
20180526 增加涨幅排名结构体

20180828 去除DDE页面数据。增加资金页面“委差”数据用于估算竞价后的涨停资金数据。

20180921 增加涨幅排名标签-自由流通市值，总股本，流通股本，流通A股。

20190120 增加涨幅排名标签-资金流入/自由流通市值，资金流出/自由流通市值，资金流向；调整股性评分的位置。
*/

#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "shareDef.h"

using std::string;
using namespace SHAREDEF;

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
    char  name[12];
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
    char  name[12];
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
    char  name[12];
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
    char  name[12];
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

	//20190120 add
	float liuRuBiZiYouLiuTong;
	float liuChuBiZiYouLiuTong;
	float ziJinLiuXiang;
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
	char  name[12];
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

	float preJingJiaZongJinE;

	//zijin
	int   zijinIdx;
	int   zijinChuIdx;
	int   liuRuBiZuoRiChengJiaoIdx;
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
	float daDanLiuChuBiLiuTong;
	float zhongXiaoLiuRuBiLiuTong;
	float daXiaoLiuRuBiLiuTong;
	float xiaoDanLiuRuBiLiuTong;
	float zhongDanLiuRuBiLiuTong;
	float zhongDanLiuChuBiLiuTong;
	float daZhongLiuRuBiLiuTong;
	float zongLiuRuBiLiuTong;
	float zongLiuChuBiLiuTong;
	float chengJiaoBiLiuTong;
	//float chengJiaoJinE;
	float jingJiaLiangBi;
	float zhongXiaoBiJingLiuRu;
	float zhangTingBan;
	float dianDanJinE;
	float zuoRiHuanShou;
	float zuoRiKaiPan;
	float zuoRiZuiGao;
	float zuoRiLiangBi;
	float zuoRiZhenFu;
	float zuoRiZhuLiJingLiang;
	float zuoRiDaDanJingEB;
	float zuoRiJingLiuRuBiLiuTong;
	float zuoRidaDanJinBiLiuTong;
	float zuoRidaDanLiuRuBiLiuTong;
	float zuoRiDaDanLiuChuBiLiuTong;
	float zuoRiChengJiaoBiLiuTong;
	float zuoRiJiGouDongXiang;
	float zuoRiSanHuShuLiang;
	float zuoRiZongJinE;
	float preZuoShou;

	float zongLiuRuBiZuoRiZongJinE;
	float zuoRiKaiPanZhangFu;

	float limitVsDealJingJia;
	float limitVsCirculateJingJia;

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

	int   jingJiaLiangBiIndex;
	int   weiBiIndex;
	int   zhangFuIndex;
	int   ziYouLiuTongShiZhiIndex;
	int   zuoRiZhuLiJingLiangIndex;
	int   zhangTingBanIndex;
	int   xianJiaIndex;
	int   zuoRiDaDanJingEBIndex;
	//int   limitVsCirculateIndex;
	int   zuoRiHuanShouIndex;
	int   guXingPingFenIndex;
	int   firstLimitTimeIndex;
	int   lastLimitTimeIndex;
	int   zuoRiJingLiuRuBiLiuTongIndex;
	int   limitUpMoneyIndex;
	int   zhongXiaoLiuRuBiLiuTongIndex;
	int   liangBiIndex;
	int   zhongDanJinBiLiuTongIndex;
	int   zhongXiaoDanJinBiLiuTongIndex;
	int   daDanLiuChuBiLiuTongIndex;
	int   continueDayIndex;
	int   xiaoDanJinBiLiuTongIndex;
	int   xiaoDanLiuRuBiLiuTongIndex;
	int   zhongDanLiuRuBiLiuTongIndex;
	int   zhongDanLiuChuBiLiuTongIndex;
	int   limitVsDealIndex;
	int   zuoRidaDanJinBiLiuTongIndex;
	int   zuoRidaDanLiuRuBiLiuTongIndex;
	int   zuoRiDaDanLiuChuBiLiuTongIndex;
	int   daDanLiuRuBiLiuTongIndex;
	int   zuoRiChengJiaoBiLiuTongIndex;
	int   zuoRiJiGouDongXiangIndex;
	int   zuoRiSanHuShuLiangIndex;
	int   sanHuShuLiangIndex;
	int   totalIndex;

	//构造函数初始化
	PROPERTY_t()
	{
		zhangFu = -12.0;
		limitUpMoney = 0;
		ddeIdx = 0;
		zijinIdx = MAX_SHARE_NUM;
		zijinChuIdx = MAX_SHARE_NUM;
		liuRuBiZuoRiChengJiaoIdx = MAX_SHARE_NUM;
		preJingJiaZongJinE = 0.0;
		zhuLiJingBiLiuTong = 0.0;
		neiWaiPanBiLiuTongGu = 0.0;
		jingLiuRuBiLiuTong = 0.0;
		daDanJinBiLiuTong = 0.0;
		zhongDanJinBiLiuTong = 0.0;
		xiaoDanJinBiLiuTong = 0.0;
		zhongXiaoDanJinBiLiuTong = 0.0;
		zhongXiaoDanZongBiLiuTong = 0.0;
		daDanLiuRuBiLiuTong = 0.0;
		daDanLiuChuBiLiuTong = 0.0;
		zhongXiaoLiuRuBiLiuTong = 0.0;
		daXiaoLiuRuBiLiuTong = 0.0;
		xiaoDanLiuRuBiLiuTong = 0.0;
		zhongDanLiuRuBiLiuTong = 0.0;
		zhongDanLiuChuBiLiuTong = 0.0;
		daZhongLiuRuBiLiuTong = 0.0;
		zongLiuRuBiLiuTong = 0.0;
		zongLiuChuBiLiuTong = 0.0;
		chengJiaoBiLiuTong = 0.0;
		jingJiaLiangBi = 0.0;

		zuoRiHuanShou = 0.0;
		zuoRiKaiPan = 0.0;
		zuoRiZuiGao = 0.0;
		zuoRiLiangBi = 0.0;
		zuoRiZhenFu = 0.0;
		zuoRiZhuLiJingLiang = 0.0;
		zuoRiZongJinE = 0.0;
		zongLiuRuBiZuoRiZongJinE = -100.0;
		zuoRiKaiPanZhangFu = -15.0;
		preZuoShou = 0.0;

		limitVsDealJingJia = 0.0;
		limitVsCirculateJingJia = 0.0;

		//zhang ting
		indexLvsC = 0;
		continueDay = 0;
		limitOpenCount = 0;
		limitVsDeal = 0.0;
		limitVsCirculate = 0.0;
		limitUpMoney = 0.0;

		jingLiuRuBiLiuTongIndex = 0;
		zongLiuRuBiLiuTongIndex = 0;
		zongLiuChuBiLiuTongIndex = 0;
		chengJiaoBiLiuTongIndex = 0;
		indexLvsC = 0;
		jingJiaLiangBiIndex = 0;
		weiBiIndex = 0;
		zhangFuIndex = 0;
		ziYouLiuTongShiZhiIndex = 0;
		zuoRiZhuLiJingLiangIndex = 0;
		zhangTingBanIndex = 0;
		xianJiaIndex = 0;
		zuoRiDaDanJingEBIndex = 0;
		zuoRiHuanShouIndex = 0;
		guXingPingFenIndex = 0;
		firstLimitTimeIndex = 0;
		lastLimitTimeIndex = 0;
		zuoRiJingLiuRuBiLiuTongIndex = 0;
		limitUpMoneyIndex = 0;
		zhongXiaoLiuRuBiLiuTongIndex = 0;
		liangBiIndex = 0;
		zhongDanJinBiLiuTongIndex = 0;
		zhongXiaoDanJinBiLiuTongIndex = 0;
		daDanLiuChuBiLiuTongIndex = 0;
		continueDayIndex = 0;
		xiaoDanJinBiLiuTongIndex = 0;
		xiaoDanLiuRuBiLiuTongIndex = 0;
		zhongDanLiuRuBiLiuTongIndex = 0;
		zhongDanLiuChuBiLiuTongIndex = 0;
		limitVsDealIndex = 0;
		zuoRidaDanJinBiLiuTongIndex = 0;
		zuoRidaDanLiuRuBiLiuTongIndex = 0;
		zuoRiDaDanLiuChuBiLiuTongIndex = 0;
		daDanLiuRuBiLiuTongIndex = 0;
		zuoRiChengJiaoBiLiuTongIndex = 0;
		zuoRiJiGouDongXiangIndex = 0;
		zuoRiSanHuShuLiangIndex = 0;
		sanHuShuLiangIndex = 0;
		totalIndex = 0;
		//memset(this, 0, sizeof(PROPERTY_t));
		//或者是下面的格式 
		//memset(&nNum,0,sizeof(Stu));
	}
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

struct kaiPanSummary_t
{
	int   yiZiBanCount;
	int   zhangCount;
	int   dieCount;
	float avgZhang;
	int   xinGuCount;
	int   zuoRiYiZiCount;
	int   stCount;
	float conAvgIncrease;
	int   conZhangCount;

	kaiPanSummary_t()
	{
		yiZiBanCount = 0;
		zhangCount = 0;
		dieCount = 0;
		avgZhang = 0.0;
		xinGuCount = 0;
		zuoRiYiZiCount = 0;
		stCount = 0;
		conAvgIncrease = 0.0;
		conZhangCount = 0;
	}
};

#endif
