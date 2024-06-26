#pragma once
#include <cstdio>
#include <vector>
#include <set>
#include "dataStructure.h"
#include "excelRdWt.h"

using namespace EXCELREADWRITE;

namespace GETLIMITUPINFO
{
	class limitUpReason_t
	{
	public:
		class limitUpInfo_t : public PROPERTY_t
		{
		public:
			limitUpInfo_t(PROPERTY_t &p)
			{
				ddeIdx = p.ddeIdx;
				strcpy(code, p.code);
				strcpy(name, p.name);
				//code[10] = code[10];
				//name[10] = name[10];
				zhuLiJingLiang = p.zhuLiJingLiang;
				sanHuShuLiang = p.sanHuShuLiang;
				zhuLiJinE = p.zhuLiJinE;
				zhangFu = p.zhangFu;
				strcpy(suoShuHangYe, p.suoShuHangYe);
				//suoShuHangYe[20] = p.suoShuHangYe[20];
				xingJi = p.xingJi;
				xianJia = p.xianJia;
				zhangSu = p.zhangSu;
				zongShou = p.zongShou;
				huanShou = p.huanShou;
				liangBi = p.liangBi;
				zongJinE = p.zongJinE;
				preJingJiaZongJinE = p.preJingJiaZongJinE;

				zijinIdx = p.zijinIdx;
				zijinChuIdx = p.zijinChuIdx;
				liuRuBiZuoRiChengJiaoIdx = p.liuRuBiZuoRiChengJiaoIdx;
				liuTongShiZhi = p.liuTongShiZhi;
				jingLiuRu = p.jingLiuRu;
				daDanLiuRu = p.daDanLiuRu;
				daDanLiuChu = p.daDanLiuChu;
				daDanJinE = p.daDanJinE;

				zhongDanLiuRu = p.zhongDanLiuRu;
				zhongDanLiuChu = p.zhongDanLiuChu;
				zhongDanJinE = p.zhongDanJinE;

				xiaoDanLiuRu = p.xiaoDanLiuRu;
				xiaoDanLiuChu = p.xiaoDanLiuChu;
				xiaoDanJinE = p.xiaoDanJinE;


				daDanJingEB = p.daDanJingEB;
				daDanZongE = p.daDanZongE;
				daDanZongEB = p.daDanZongEB;
				zhongDanJingEB = p.zhongDanJingEB;
				zhongDanZongE = p.zhongDanZongE;
				zhongDanZongEB = p.zhongDanZongEB;
				xiaoDanJingEB = p.xiaoDanJingEB;
				xiaoDanZongE = p.xiaoDanZongE;
				xiaoDanZongEB = p.xiaoDanZongEB;

				zhuliIdx = p.zhuliIdx;
				jinRiZengCangZhanBi = p.jinRiZengCangZhanBi;
				jinRiPaiMing = p.jinRiPaiMing;
				jinRiZhangFu = p.jinRiZhangFu;

				erRiZengCangZhanBi = p.erRiZengCangZhanBi;
				erRiPaiMing = p.erRiPaiMing;
				erRiZhangFu = p.erRiZhangFu;

				sanRiZengCangZhanBi = p.sanRiZengCangZhanBi;
				sanRiPaiMing = p.sanRiPaiMing;
				sanRiZhangFu = p.sanRiZhangFu;

				wuRiZengCangZhanBi = p.wuRiZengCangZhanBi;
				wuRiPaiMing = p.wuRiPaiMing;
				wuRiZhangFu = p.wuRiZhangFu;

				shiRiZengCangZhanBi = p.shiRiZengCangZhanBi;
				shiRiPaiMing = p.shiRiPaiMing;
				shiRiZhangFu = p.shiRiZhangFu;

				daDanBiLiuTong = p.daDanBiLiuTong;


				zhangfuIdx = p.zhangfuIdx;
				zhangDie = p.zhangDie;
				zhuLiJingE = p.zhuLiJingE;

				guXingPingFen = p.guXingPingFen;
				weiBi = p.weiBi;
				weiCha = p.weiCha;
				jiGouDongXiang = p.jiGouDongXiang;
				jingLiZengLv = p.jingLiZengLv;
				jingLiRun = p.jingLiRun;
				shiJingLv = p.shiJingLv;
				waiPan = p.waiPan;
				neiPan = p.neiPan;
				zhenFu = p.zhenFu;

				zongShiZhi = p.zongShiZhi;
				xianShou = p.xianShou;
				kaiPan = p.kaiPan;
				zuoShou = p.zuoShou;
				zuiGao = p.zuiGao;
				zuiDi = p.zuiDi;
				wuRiJunXian = p.wuRiJunXian;
				buyLiang = p.buyLiang;
				sellLiant = p.sellLiant;

				jinChaGeShu = p.jinChaGeShu;
				shouMeiBi = p.shouMeiBi;

				shiYingDong = p.shiYingDong;
				shiYingJing = p.shiYingJing;
				shiYingTTM = p.shiYingTTM;

				ziYouLiuTongShiZhi = p.ziYouLiuTongShiZhi;

				ziYouLiuTongGu = p.ziYouLiuTongGu;


				zhuLiJingBiLiuTong = p.zhuLiJingBiLiuTong;
				neiWaiPanBiLiuTongGu = p.neiWaiPanBiLiuTongGu;
				jingLiuRuBiLiuTong = p.jingLiuRuBiLiuTong;
				daDanJinBiLiuTong = p.daDanJinBiLiuTong;
				zhongDanJinBiLiuTong = p.zhongDanJinBiLiuTong;
				xiaoDanJinBiLiuTong = p.xiaoDanJinBiLiuTong;
				zhongXiaoDanJinBiLiuTong = p.zhongXiaoDanJinBiLiuTong;
				zhongXiaoDanZongBiLiuTong = p.zhongXiaoDanZongBiLiuTong;
				daDanLiuRuBiLiuTong = p.daDanLiuRuBiLiuTong;
				daDanLiuChuBiLiuTong = p.daDanLiuChuBiLiuTong;
				zhongXiaoLiuRuBiLiuTong = p.zhongXiaoLiuRuBiLiuTong;
				daXiaoLiuRuBiLiuTong = p.daXiaoLiuRuBiLiuTong;
				xiaoDanLiuRuBiLiuTong = p.xiaoDanLiuRuBiLiuTong;
				zhongDanLiuRuBiLiuTong = p.zhongDanLiuRuBiLiuTong;
				daZhongLiuRuBiLiuTong = p.daZhongLiuRuBiLiuTong;
				zongLiuRuBiLiuTong = p.zongLiuRuBiLiuTong;
				zongLiuChuBiLiuTong = p.zongLiuChuBiLiuTong;
				chengJiaoBiLiuTong = p.chengJiaoBiLiuTong;
				//chengJiaoJinE = p.chengJiaoJinE;
				jingJiaLiangBi = p.jingJiaLiangBi;
				zhongXiaoBiJingLiuRu = p.zhongXiaoBiJingLiuRu;
				zhangTingBan = p.zhangTingBan;
				dianDanJinE = p.dianDanJinE;
				zuoRiHuanShou = p.zuoRiHuanShou;
				zuoRiKaiPan = p.zuoRiKaiPan;
				zuoRiZuiGao = p.zuoRiZuiGao;
				zuoRiLiangBi = p.zuoRiLiangBi;
				zuoRiZhenFu = p.zuoRiZhenFu;
				zuoRiZhangFu = p.zuoRiZhangFu;
				zuoRiZhuLiJingLiang = p.zuoRiZhuLiJingLiang;
				zuoRiDaDanJingEB = p.zuoRiDaDanJingEB;
				zuoRiJingLiuRuBiLiuTong = p.zuoRiJingLiuRuBiLiuTong;
				zuoRidaDanJinBiLiuTong = p.zuoRidaDanJinBiLiuTong;
				zuoRidaDanLiuRuBiLiuTong = p.zuoRidaDanLiuRuBiLiuTong;
				zuoRiDaDanLiuChuBiLiuTong = p.zuoRiDaDanLiuChuBiLiuTong;
				zuoRiChengJiaoBiLiuTong = p.zuoRiChengJiaoBiLiuTong;
				zuoRiJiGouDongXiang = p.zuoRiJiGouDongXiang;
				zuoRiSanHuShuLiang = p.zuoRiSanHuShuLiang;
				zuoRiZongJinE = p.zuoRiZongJinE;
				zongLiuRuBiZuoRiZongJinE = p.zongLiuRuBiZuoRiZongJinE;
				zuoRiKaiPanZhangFu = p.zuoRiKaiPanZhangFu;
				preZuoShou = p.preZuoShou;
				limitVsDealJingJia = p.limitVsDealJingJia;
				limitVsCirculateJingJia = p.limitVsCirculateJingJia;

				jingLiuRuBiLiuTongIndex = p.jingLiuRuBiLiuTongIndex;
				zongLiuRuBiLiuTongIndex = p.zongLiuRuBiLiuTongIndex;
				zongLiuChuBiLiuTongIndex = p.zongLiuChuBiLiuTongIndex;
				chengJiaoBiLiuTongIndex = p.chengJiaoBiLiuTongIndex;
				indexLvsC = p.indexLvsC;
				continueDay = p.continueDay;
				strcpy(firstLimitTime, p.firstLimitTime);
				strcpy(lastLimitTime, p.lastLimitTime);
				//firstLimitTime[10] = firstLimitTime[10];
				//lastLimitTime[10] = lastLimitTime[10];
				limitOpenCount = p.limitOpenCount;
				limitVsDeal = p.limitVsDeal;
				limitVsCirculate = p.limitVsCirculate;
				limitUpMoney = p.limitUpMoney;
				strcpy(limitReason, p.limitReason);
				//limitReason[50] = limitReason[50];

				jingJiaLiangBiIndex = p.jingJiaLiangBiIndex;
				weiBiIndex = p.weiBiIndex;
				zhangFuIndex = p.zhangFuIndex;
				ziYouLiuTongShiZhiIndex = p.ziYouLiuTongShiZhiIndex;
				zuoRiZhuLiJingLiangIndex = p.zuoRiZhuLiJingLiangIndex;
				zhangTingBanIndex = p.zhangTingBanIndex;
				xianJiaIndex = p.xianJiaIndex;
				zuoRiDaDanJingEBIndex = p.zuoRiDaDanJingEBIndex;
				//limitVsCirculateIndex = p.limitVsCirculateIndex;
				zuoRiHuanShouIndex = p.zuoRiHuanShouIndex;
				guXingPingFenIndex = p.guXingPingFenIndex;
				firstLimitTimeIndex = p.firstLimitTimeIndex;
				lastLimitTimeIndex = p.lastLimitTimeIndex;
				zuoRiJingLiuRuBiLiuTongIndex = p.zuoRiJingLiuRuBiLiuTongIndex;
				limitUpMoneyIndex = p.limitUpMoneyIndex;
				zhongXiaoLiuRuBiLiuTongIndex = p.zhongXiaoLiuRuBiLiuTongIndex;
				liangBiIndex = p.liangBiIndex;
				zhongDanJinBiLiuTongIndex = p.zhongDanJinBiLiuTongIndex;
				zhongXiaoDanJinBiLiuTongIndex = p.zhongXiaoDanJinBiLiuTongIndex;
				daDanLiuChuBiLiuTongIndex = p.daDanLiuChuBiLiuTongIndex;
				continueDayIndex = p.continueDayIndex;
				xiaoDanJinBiLiuTongIndex = p.xiaoDanJinBiLiuTongIndex;
				xiaoDanLiuRuBiLiuTongIndex = p.xiaoDanLiuRuBiLiuTongIndex;
				zhongDanLiuRuBiLiuTongIndex = p.zhongDanLiuRuBiLiuTongIndex;
				zhongDanLiuChuBiLiuTongIndex = p.zhongDanLiuChuBiLiuTongIndex;
				limitVsDealIndex = p.limitVsDealIndex;
				zuoRidaDanJinBiLiuTongIndex = p.zuoRidaDanJinBiLiuTongIndex;
				zuoRidaDanLiuRuBiLiuTongIndex = p.zuoRidaDanLiuRuBiLiuTongIndex;
				zuoRiDaDanLiuChuBiLiuTongIndex = p.zuoRiDaDanLiuChuBiLiuTongIndex;
				daDanLiuRuBiLiuTongIndex = p.daDanLiuRuBiLiuTongIndex;
				zuoRiChengJiaoBiLiuTongIndex = p.zuoRiChengJiaoBiLiuTongIndex;
				zuoRiJiGouDongXiangIndex = p.zuoRiJiGouDongXiangIndex;
				zuoRiSanHuShuLiangIndex = p.zuoRiSanHuShuLiangIndex;
				sanHuShuLiangIndex = p.sanHuShuLiangIndex;
				banKuaiFirstFlag = p.banKuaiFirstFlag;
				totalIndex = p.totalIndex;
			}
		/*public:
			std::string code;
			std::string name;
			float       weiBi;
			float       waiPan;
			float       zhangFu;
			float       jingJiaLiangBi;
			float       zuoRiHuanShou;
			float       zuoRiKaiPan;
			float       zuoRiZuiGao;
			float       zuoRiLiangBi;
			float       zuoRiZhenFu;
			int			continueDay;
			float		ziYouLiuTongShiZhi;
			float       zhongDanJinBiLiuTong; 
			float       zhongXiaoDanJinBiLiuTong;
			std::string lastLimitTime;
			int         limitOpenCount;*/

		public:
			bool operator< (const PROPERTY_t &x) const
			{
				return (jingJiaLiangBi > x.jingJiaLiangBi);
			};
		};
	public:
		std::multiset<limitUpInfo_t> limitInfo;
		std::string reason;
		int         ztCount;
		float       zhangFuZong;
		float       zhangFuMax;
		int         zfAvgOder;
		float       zongShiZhi;
		float       danWeiShiZhi;
		float       jingJiaLiangBiZong;
		float       jingLiuRuBiLiuTongZong;
		float       zongLiuRuBiLiuTongZong;
		float       shiZhiXzhangFuZong;
		float       zongJinEXZong;
		float       zongLimitVsCirculateJingJia;
		//std::string suoShuHangYe;
		//int         hyCount;

	public:
		//sort
		struct zfAvg_t
		{
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.zhangFuZong / a.ztCount) > (b.zhangFuZong / b.ztCount)); }
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zhangFuZong > b.zhangFuZong); }
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zongLimitVsCirculateJingJia > b.zongLimitVsCirculateJingJia); }
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.zongJinEXZong > b.zongJinEXZong); }
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.shiZhiXzhangFuZong) > (b.shiZhiXzhangFuZong)); }
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.zongShiZhi / a.danWeiShiZhi) > (b.zongShiZhi / b.danWeiShiZhi)); }
		};

		//sort
		struct ztrCount_t
		{
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.ztCount > b.ztCount); }
		};

	private:
		void jingBiConclude(PROPERTY_t &propertyAnaly, std::vector<float> &marginVec, std::vector<float> &jingBi);

		void limitShareSummary(std::string &ztStr, PROPERTY_t &analyProty, std::vector<limitUpReason_t> &ztrVec);

	public:
		void continueLimitSave(std::vector<PROPERTY_t > propertyAnalyVec, std::vector<analyseCode_t> limitTodayVec);

		void getLimitUpReason(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec);

		void getLimitUpHy(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec);

		void sortByZhangFuZongLimitVsCirculateJingJiaZong(std::vector<limitUpReason_t> &ztrVec);

		void limitShareSort(FILE *fp, ExcelRwC &excelReadWrite, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &resultSetBlock, int fileIndex, std::vector<PROPERTY_t> &propertyAnalyVecBlock, std::vector<PROPERTY_t> &proBanKuaiFirstVec, int flag_guXinZhengShu);

		void limitShareOrdering(std::vector<PROPERTY_t> &propertyAnalyVec);

	limitUpReason_t()
	{
		ztCount = 0;
		zhangFuZong = 0.0;
		zhangFuMax = 0.0;
		zfAvgOder = 0;
		zongShiZhi = 0.0;
		danWeiShiZhi = 0.0;
		jingJiaLiangBiZong = 0.0;
		jingLiuRuBiLiuTongZong = 0.0;
		zongLiuRuBiLiuTongZong = 0.0;
		shiZhiXzhangFuZong = 0.0;
		zongJinEXZong = 0.0;
		zongLimitVsCirculateJingJia = 0.0;
	}
	};
}
