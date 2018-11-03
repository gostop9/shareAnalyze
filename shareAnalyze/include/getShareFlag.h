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
		/*if (
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
			&& (shareProperty.zuoRiZhenFu > FLT_MIN)
			&& ((shareProperty.zuoRiLiangBi < 4.1) && ((shareProperty.jingJiaLiangBi / shareProperty.zuoRiHuanShou) < 5.9))
			)
		{
			shareFlag.append("*");
		}
		if ((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < FLT_MIN)
		{
			shareFlag.append("-"); //T�ְ�
			if (shareProperty.zuoRiZhenFu < FLT_MIN) //����һ�ְ�
			{
				shareFlag.append("-");
				//if ((shareProperty.waiPan < FLT_MIN) && (shareProperty.weiBi > WEIBI_MAX)) //һ�ֿ��̣�����һ�ְ�
				if ((shareProperty.weiBi > WEIBI_MAX)) //һ�ֿ��̣�����һ�ְ�
				{
					shareFlag.append("-");
				}
			}
		}*/
		if (
			(shareProperty.zhongDanJinBiLiuTong * BI_LIUTONG > 0.9)
			&& (shareProperty.zhongXiaoDanJinBiLiuTong * BI_LIUTONG > 0.9)
			&& (shareProperty.jingJiaLiangBi > 9.5)
			&& (shareProperty.zuoRiHuanShou < 15.9)
			)
		{
			shareFlag.append("=");
		}
		if (
			((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < FLT_MIN)
			&& (shareProperty.zuoRiZhenFu > FLT_MIN)
			)
		{
			shareFlag.append("T"); //����T�ְ�
		}
		if (
			((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < FLT_MIN)
			&& (shareProperty.zuoRiZhenFu < FLT_MIN)
			)
		{
			shareFlag.append("~"); //����һ�ְ�
		}
		if ((shareProperty.weiBi > WEIBI_MAX)) //һ�ֿ���
		{
			shareFlag.append("-");
		}

		if (
			(shareProperty.weiBi > -83.0)
			&& (shareProperty.jingJiaLiangBi > 9.1)
			&& (shareProperty.zuoRiLiangBi < 4.67)
			//&& (shareProperty.zuoRiHuanShou < 15.9)
			&& (shareProperty.ddeIdx < 18)
			&& (shareProperty.zijinIdx < 46)
			&& (shareProperty.jingLiuRuBiLiuTongIndex < 21)
			&& (shareProperty.zongLiuRuBiLiuTongIndex < 15)
			&& (shareProperty.chengJiaoBiLiuTongIndex < 15)
			&& (shareProperty.indexLvsC < 21)
			&& (shareProperty.limitUpMoney >(900 * TENTHOUSAND))
			)
		{
			if (//����һ�ְ�
				((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < FLT_MIN)
				&& (shareProperty.zuoRiZhenFu < FLT_MIN)
				&& (shareProperty.zuoRiHuanShou > 5.9)
				)
			{				
			}
			else
			{
				float zhanLiuBi = 0;
				float zhangTingBan = getLimitUpMoney<T>(shareProperty, zhanLiuBi);
				if (
					(shareProperty.weiBi > WEIBI_MAX) //һ�ֿ���
					)
				{
					if (
						((shareProperty.zuoRiZuiGao - shareProperty.zuoRiKaiPan) < FLT_MIN)//����һ�ְ�
						&& (shareProperty.zuoRiZhenFu < FLT_MIN)
						)
					{
						if ((shareProperty.zuoRiHuanShou < 2.9))
						{
							shareFlag.append("#");
						}
					}
					else
					{
						shareFlag.append("#");
					}
				}
				else if (zhangTingBan < (4900.0))//3800
				{
					shareFlag.append("#");
				}
			}
		}
		/*if (
			(shareProperty.weiBi > -83.0)
			&& ( 
				(
					(shareProperty.zhongDanJinBiLiuTong * BI_LIUTONG > -70.0) 
					&& (shareProperty.xiaoDanJinBiLiuTong * BI_LIUTONG > -70.0)
					&& (shareProperty.zijinIdx < 169)
				) 
				|| (shareProperty.zijinIdx < 29) 
				|| (shareProperty.limitUpMoney >(10000 * TENTHOUSAND))
				)
			&& (shareProperty.jingJiaLiangBi > 9.5)
			&& (
				(shareProperty.zuoRiHuanShou < 10.0)
				|| (
					(shareProperty.jingJiaLiangBi > ((shareProperty.zuoRiHuanShou) * 4.0))
					&& (shareProperty.zuoRiHuanShou < 15.0)
					)
				|| (
					(shareProperty.zijinIdx < 47)
					&& ((shareProperty.indexLvsC < 15))
					&& (shareProperty.zuoRiHuanShou < 15.0)
					)
				)
			&& (shareProperty.zuoRiLiangBi < 6.1)
			&& (shareProperty.indexLvsC < 20)
			&& (shareProperty.limitOpenCount < 5)
			&& (shareProperty.zhangFu > -2.0)
			&& (shareProperty.huanShou < 6.0)
			//&& (shareProperty.limitUpMoney > (1000 * TENTHOUSAND))
			)
		{
			shareFlag.append("#");
		}*/
		/*if (
			(shareProperty.jingJiaLiangBi > 20.0)
			&& (shareProperty.jingJiaLiangBi < 74.0)
			&& (shareProperty.huanShou > 0.3)
			&& (shareProperty.huanShou < 2.0)
			&& (shareProperty.liangBi > 36.0)
			&& (shareProperty.liangBi < 155.0)
			)
		{
			shareFlag.append("#");
		}*/
		/*int ztTime = atoi(shareProperty.lastLimitTime);
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
		}*/

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