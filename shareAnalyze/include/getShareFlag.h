#pragma once
#include "cstdio"
#include "shareDef.h"
#include "dataStructure.h"
#include "getLimitUpInfo.h"

using namespace SHAREDEF;
using namespace GETLIMITUPINFO;
namespace SHARE_FLAG
{
	//template<typename T>
	//std::string getShareFlag(T &shareProperty);
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
			shareFlag.append("-"); //T�ְ�
			if (shareProperty.zuoRiZhenFu < 0.0001) //����һ�ְ�
			{
				shareFlag.append("-");
				if ((shareProperty.waiPan < 0.0001) && (shareProperty.weiBi > WEIBI_MAX)) //һ�ֿ��̣�����һ�ְ�
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
		//��תǿ
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

	template<typename T>
	float getLimitUpMoney(T &shareProperty, float &zhanLiuBi)
	{
		float maiLiang = (shareProperty.weiCha * 100.0 / shareProperty.weiBi - shareProperty.weiCha) / 2.0;
		float zhangTingBan = shareProperty.zuoShou * 1.1 * maiLiang / 10000.0; //ǧ��������ʾ

		zhanLiuBi = zhangTingBan * 1000000 / shareProperty.ziYouLiuTongShiZhi;
		if (shareProperty.weiBi > WEIBI_MAX)
		{
			zhangTingBan = (shareProperty.buyLiang * shareProperty.xianJia) / 1000000.0;
			zhanLiuBi = zhangTingBan * 100000000.0 / shareProperty.ziYouLiuTongShiZhi;
		}
		else if (shareProperty.weiBi < -100.0) // ί��Ϊ��Чֵʱ
		{
			zhangTingBan = 0.0;
			zhanLiuBi = 0.0;
		}
		return zhangTingBan;
	}
}