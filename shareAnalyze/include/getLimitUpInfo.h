#pragma once
#include <cstdio>
#include <vector>
#include <set>
#include "dataStructure.h"

namespace GETLIMITUPINFO
{
	class limitUpReason_t
	{
	public:
		class limitUpInfo_t : public PROPERTY_t
		{
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
		int         zfAvgOder;
		float       zongShiZhi;
		float       danWeiShiZhi;
		float       jingJiaLiangBiZong;
		float       jingLiuRuBiLiuTongZong;
		float       zongLiuRuBiLiuTongZong;
		float       shiZhiXzhangFuZong;
		//std::string suoShuHangYe;
		//int         hyCount;

	public:
		//sort
		struct zfAvg_t
		{
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.zhangFuZong / a.ztCount) > (b.zhangFuZong / b.ztCount)); }
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.shiZhiXzhangFuZong) > (b.shiZhiXzhangFuZong)); }
			//bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return ((a.zongShiZhi / a.danWeiShiZhi) > (b.zongShiZhi / b.danWeiShiZhi)); }
		};

		//sort
		struct ztrCount_t
		{
			bool operator() (const limitUpReason_t &a, const limitUpReason_t &b) { return (a.ztCount > b.ztCount); }
		};

	private:
		void limitShareSummary(std::string &ztStr, PROPERTY_t &analyProty, std::vector<limitUpReason_t> &ztrVec);

	public:
		void getLimitUpReason(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec);

		void getLimitUpHy(std::vector<PROPERTY_t> &propertyAnalyVec, std::vector<limitUpReason_t> &ztrVec, std::vector<std::string> &newShareCodeVec);

		void limitShareSort(FILE * fp, std::vector<limitUpReason_t> &ztrVec);
	};
}
