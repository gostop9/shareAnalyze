#include <iostream>
#include <cstdio>
#include "dataStructure.h"
#include "getShareFlag.h"
#include "getLimitUpInfo.h"

using namespace std;
using namespace GETLIMITUPINFO;
namespace SHARE_FLAG
{
	template<typename T>
	std::string getShareFlag(T &shareProperty)
	{
		string shareFlag;
		if (
			(
			(shareProperty.zhongXiaoDanJinBiLiuTong > 0.0)
				&& (shareProperty.zhongDanJinBiLiuTong > 0.0)
				)
			&& (shareProperty.zuoRiHuanShou < 25.0)
			&& ((shareProperty.jingJiaLiangBi / shareProperty.zuoRiHuanShou) < 5.9)
			&& (shareProperty.zhangFu > -0.88)
			&& (shareProperty.zuoRiLiangBi < 4.1)
			&& (shareProperty.zuoRiLiangBi > 0.8)
			)
		{
			shareFlag.append("#");
		}
		if (
			(shareProperty.weiBi < WEIBI_MIN)
			)
		{
			shareFlag.append("*");
		}
		if (
			(shareProperty.weiBi > 30)
			&& (shareProperty.zhangFu > 5.0)
			&& (shareProperty.zuoRiZhenFu > 0.0001)
			&& ((shareProperty.zuoRiLiangBi < 4.1) && ((shareProperty.jingJiaLiangBi / shareProperty.zuoRiHuanShou) < 5.9))
			)
		{
			shareFlag.append("*");
		}
		if ((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < 0.0001)
		{
			shareFlag.append("-"); //T字板
			if (shareProperty.zuoRiZhenFu < 0.0001) //昨日一字板
			{
				shareFlag.append("-");
				if ((shareProperty.waiPan < 0.0001) && (shareProperty.weiBi > WEIBI_MAX)) //一字开盘，连续一字板
				{
					shareFlag.append("-");
				}
			}
		}
		int ztTime = atoi(shareProperty.lastLimitTime);
		if (
			(ztTime < 103001)
			&& (shareProperty.limitOpenCount < 2)
			&& (shareProperty.zuoRiZhenFu > 3.0001)
			&& (shareProperty.zuoRiLiangBi < 4.1)
			&& ((shareProperty.jingJiaLiangBi / shareProperty.zuoRiHuanShou) < 5.9)
			)
		{
			shareFlag.append("=");
		}
		//弱转强
		if (
			(shareProperty.zhangFu > 1.8)
			&& (shareProperty.limitOpenCount > 3)
			&& (shareProperty.zuoRiZhenFu > 3.0001)
			&& (shareProperty.zuoRiLiangBi < 4.1)
			&& ((shareProperty.jingJiaLiangBi / shareProperty.zuoRiHuanShou) < 5.9)
			)
		{
			shareFlag.append("^");
		}

		return shareFlag;
	}
}