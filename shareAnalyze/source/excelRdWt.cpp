#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include "shareDef.h"
#include "dataStructure.h"
#include "shareSelect.h"
#include "excelRdWt.h"
#include <memory>
#include <codecvt>
#include <BasicExcel.hpp>
#include <objbase.h>

#pragma comment(lib, "ole32.lib")

using namespace YExcel;
using namespace std;
using namespace SHAREDEF;
namespace EXCELREADWRITE
{
std::string UnicodeToUTF8(const std::wstring & wstr)
{
	std::string ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}

std::wstring UTF8ToUnicode(const std::string & str)
{
	std::wstring ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}
std::string UnicodeToANSI(const std::wstring & wstr)
{
	std::string ret;
	std::mbstate_t state = {};
	const wchar_t *src = wstr.data();
	size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
	if (static_cast<size_t>(-1) != len) {
		std::unique_ptr< char[] > buff(new char[len + 1]);
		len = std::wcsrtombs(buff.get(), &src, len, &state);
		if (static_cast<size_t>(-1) != len) {
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::wstring ANSIToUnicode(const std::string & str)
{
	std::wstring ret;
	std::mbstate_t state = {};
	const char *src = str.data();
	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
	if (static_cast<size_t>(-1) != len) {
		std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
		len = std::mbsrtowcs(buff.get(), &src, len, &state);
		if (static_cast<size_t>(-1) != len) {
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::string UTF8ToANSI(const std::string & str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}

std::string ANSIToUTF8(const std::string & str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}

ExcelRwC::ExcelRwC(int ztfpFlag)
{
	if (CoInitialize(NULL) != 0)
	{
		printf("初始化失败");
		exit(1);
	}

	excelRowIndex = 1;

	if (ztfpFlag == 0)
	{
		ZTFPfileName = "ZTFP.xlsx";
	}
	else
	{
		ZTFPfileName = "ZTFPZT.xlsx";
	}
}

ExcelRwC::~ExcelRwC()
{
	CoUninitialize();  //释放com资源
}

void ExcelRwC::excelWriteHeader(BasicExcelWorksheet *sheet)
{
	int eRow = 0;
	int eColumn = 0;
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("股票标记").c_str());
	sheet->Cell(eRow, eColumn++)->SetString("CODE");
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("现价").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("NAME").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("涨幅").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("竞价成交").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("昨日竞价成交").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("连板数").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("竞价量比").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("委比").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("总卖").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("总买").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("昨日成交").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("昨日涨停封单").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("涨停打开次数").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("自由流通市值").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("涨停原因").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("昨日换手").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("昨日量比").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("行业").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("股性评分").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("首次涨停时间").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("最终涨停时间").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("封流比").c_str());
	//sheet->Cell(eRow, eColumn++)->SetWString("CODE");
	
}

void ExcelRwC::excelWriteRow(string sheetName, vector<PROPERTY_t> &propertyVec)
{
	setlocale(LC_ALL, "Chinese-simplified");
	BasicExcel excel;
	//BasicExcelCell* cell;
	BasicExcelWorksheet* sheet;

	// Load a workbook with one sheet, display its contents and save into another file.
	if (excel.Load("ZTFP.xls"))
	{
		sheet = excel.AddWorksheet(sheetName.c_str(), 1);
		sheet = excel.GetWorksheet(1);
	}
	else
	{
		excel.New(1);
		excel.RenameWorksheet("Sheet1", sheetName.c_str());
		sheet = excel.GetWorksheet(sheetName.c_str());
	}
	if (sheet)
	{
		//vector<PROPERTY_t>::iterator it = propertyVec.begin();
		int i = 1;
		excelWriteHeader(sheet);
		for (vector<PROPERTY_t>::iterator it = propertyVec.begin(); it != propertyVec.end(); it++, i++)
		{
			int j = 1;
			//string printFlag = getShareFlag<PROPERTY_t>(*it);
			sheet->Cell(i, j++)->SetString(it->code);
			sheet->Cell(i, j++)->SetDouble(it->xianJia);
			sheet->Cell(i, j++)->SetWString(ANSIToUnicode(it->name).c_str());
			sheet->Cell(i, j++)->SetDouble(it->zhangFu);
			sheet->Cell(i, j++)->SetDouble(it->zongJinE / TENTHOUSAND);
			sheet->Cell(i, j++)->SetDouble(it->preJingJiaZongJinE / TENTHOUSAND);
			sheet->Cell(i, j++)->Set((int)(it->continueDay));
			sheet->Cell(i, j++)->SetDouble(it->jingJiaLiangBi);
			sheet->Cell(i, j++)->SetDouble(it->weiBi);
			sheet->Cell(i, j++)->SetDouble(it->zhangTingBan);
			sheet->Cell(i, j++)->SetDouble(it->dianDanJinE);
			sheet->Cell(i, j++)->SetDouble(it->zuoRiZongJinE / TENTHOUSAND);
			sheet->Cell(i, j++)->SetDouble(it->limitUpMoney / TENTHOUSAND);
			sheet->Cell(i, j++)->Set((int)(it->limitOpenCount));
			sheet->Cell(i, j++)->SetDouble(it->ziYouLiuTongShiZhi / DIVIDE);
			sheet->Cell(i, j++)->SetWString(ANSIToUnicode(it->limitReason).c_str());
			sheet->Cell(i, j++)->SetDouble(it->zuoRiHuanShou);
			sheet->Cell(i, j++)->SetDouble(it->zuoRiLiangBi);
			sheet->Cell(i, j++)->SetWString(ANSIToUnicode(it->suoShuHangYe).c_str());
			sheet->Cell(i, j++)->SetDouble(it->guXingPingFen);
			sheet->Cell(i, j++)->SetString(it->firstLimitTime);
			sheet->Cell(i, j++)->SetString(it->lastLimitTime);
			sheet->Cell(i, j++)->SetDouble(it->limitVsCirculate);
			//sheet->Cell(i, j++)->SetString(it->code);
		}
	}
	excel.SaveAs("ZTFP.xls");
	setlocale(LC_ALL, "C");
}

void ExcelRwC::setBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row, int color)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	range1.Format("%c%d", 'A', row);
	range2.Format("%c%d", headerSymbol, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)color)); // 底色设置为浅青色 20

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setCellBackColor(CWorksheet &wsMySheet, char &headerSymbol, char column, int &row, int color)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)color)); // 底色设置为浅青色 20

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setYiZiBanBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	char column = 'O';
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)3)); // 底色设置为红色

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setJingJiaJinEBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	char column = 'J';
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)46));

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setYiZiBanGuXingBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	char column = 'I';
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)16));

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setLimitOpenCountBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	Cnterior interior; // 底色设置  
	CRange0	 colorRge;
	CString range1, range2;
	char column = 'S';
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)14));

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setContinueDayBackColor(CWorksheet &wsMySheet, char &headerSymbol, int continueDay, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'L';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		switch (continueDay)
		{
		case 0:
			ft.put_Color(COleVariant((long)RGB(0, 0, 0)));
			break;
		case 1:
			ft.put_Color(COleVariant((long)RGB(0, 0, 0)));
			break;
		case 2:
			setCellBackColor(wsMySheet, headerSymbol, column, row, 8);//浅青色
																 //ft.put_Color(COleVariant((long)RGB(0, 255, 125)));
			break;
		case 3:
			setCellBackColor(wsMySheet, headerSymbol, column, row, 26);//粉紫色
			break;
		default:
			//ft.put_Color(COleVariant((long)RGB(255, 0, 0)));
			setCellBackColor(wsMySheet, headerSymbol, column, row, 3); //红色
			break;
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setZuoRiZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, float zuoRiZhangFu, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'G';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if (15.0 < zuoRiZhangFu)
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 3); //红色
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setJingJiaZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, float zhangFu, int continueDay, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'E';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if ((6.0 < zhangFu) && (1 == continueDay))
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 6); //红色
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setZuoRiJingJiaZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'D';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());

		setCellBackColor(wsMySheet, headerSymbol, column, row, 6);

		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setYiZiBanJingJiaJinEColor(CWorksheet &wsMySheet, char &headerSymbol, float zongJinE, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'J';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if ((1800.0 * TENTHOUSAND) < zongJinE)
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 6); //红色
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setZuoRiChengJiaoJinEColor(CWorksheet &wsMySheet, char &headerSymbol, float zuoRiChengJiaoJinE, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'C';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if ((5000.0 * TENTHOUSAND) > zuoRiChengJiaoJinE)
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 17);
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setShouBanGaoKaiErBanYiZiColor(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'C';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if((3.99 < it->zuoRiKaiPanZhangFu) && (1 == it->continueDay))
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 17);
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setZuoRiYiZiFangLiangJinYiZiColor(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &headerSymbol, int &row)
{
	{
		bool zuoRiyiZiBanFlag = zuoRiYiZiBanJudge(*it);
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
		CString tempString;
		char column = 'Q';
		tempString.Format("%c%d", column, row);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		if ((zuoRiyiZiBanFlag) && (0 < it->continueDay) && ((8000.0 * TENTHOUSAND) < it->zuoRiZongJinE))
		{
			setCellBackColor(wsMySheet, headerSymbol, column, row, 17);
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

void ExcelRwC::setSpecificColors(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &excelHeaderSymbol, int &row, int flag_guXinZhengShu)
{
	PROPERTY_t &analyProty = *it;
	bool zuoRiyiZiBanTFlag = zuoRiYiZiBanTbanJudge(analyProty);
	bool yiZiBanFlag = yiZiBanJudge(analyProty);
	float zhangFuMargin = 1.2;
	float kaiPanZfMax = zhangFuMargin + 2.0;
	float zuoRiKaiPanZfMax = 3.3;
	float zuoRiChengJiaoJinEMin = 5000.0;

	if ((!yiZiBanFlag) && (analyProty.zongJinE > jingJiaJinEThreshould))
	{
		setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, row);
	}
	if (yiZiBanFlag)
	{
		if (((analyProty.guXingPingFen < 49.0) && (0 == flag_guXinZhengShu)) ||
			((analyProty.guXingPingFen > 2012) && (1 == flag_guXinZhengShu)))
		{
			setYiZiBanGuXingBackColor(wsMySheet, excelHeaderSymbol, row);
		}
	}
	if (0 == analyProty.limitOpenCount)
	{
		setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, row);
	}
	if (-4.0 > analyProty.zhangFu) // -6.0 
	{
		char column = 'E';
		setCellBackColor(wsMySheet, excelHeaderSymbol, column, row, 16);
	}
	if ((0.006 < analyProty.zongLiuRuBiLiuTong) && 
		((danRiJingJiaThreshould * TENTHOUSAND) < analyProty.zongJinE) && 
		((analyProty.preJingJiaZongJinE < analyProty.zongJinE) || (jingJiaJinEThreshould < analyProty.zongJinE))
		)
	{
		char column = 'X';
		setCellBackColor(wsMySheet, excelHeaderSymbol, column, row, 46);
	}
	if (((analyProty.guXingPingFen > 89.0) && (0 == flag_guXinZhengShu)) ||
		((analyProty.guXingPingFen < 1000.0) && (1 == flag_guXinZhengShu)))
	{
		char column = 'I';
		setCellBackColor(wsMySheet, excelHeaderSymbol, column, row, 46);
	}

	if (analyProty.weiBi > -35.0)
	{
		char column = 'N';
		setCellBackColor(wsMySheet, excelHeaderSymbol, column, row, 46);
	}

	if (analyProty.dianDanJinE > 8000.0)
	{
		char column = 'P';
		setCellBackColor(wsMySheet, excelHeaderSymbol, column, row, 26);//粉紫色
	}

	//昨日非一字或T字的
	if (!zuoRiyiZiBanTFlag)
	{
		setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, row);
		setShouBanGaoKaiErBanYiZiColor(it, wsMySheet, excelHeaderSymbol, row);
	}
	if (
		((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
			&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
		|| zuoRiyiZiBanTFlag)
	{
		if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
		{
			setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, row);
		}
	}
}

char ExcelRwC::excelWriteHeaderC(CRange0 &rgMyRge)
{
	int eRow = excelRowIndex;
	int eColumn = 1;
	char alphabet = 'A';
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("股票标记"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("CODE"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("现价"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("NAME"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("昨日开盘涨幅"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("涨幅"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("自由流通市值"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("昨日涨幅"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("涨停原因"));

	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("股性评分"));
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("主力比流通"));
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("主买"));
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("排名"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("竞价成交"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("昨日竞价成交"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("连板数"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("竞价量比"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("委比"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("涨停"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("垫单"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("昨日成交"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("昨日涨停封单"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("涨停打开次数"));

	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("首次涨停时间"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("最终涨停时间"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("行业"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("换手"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("量比"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("竞价比流通"));

	alphabet += eColumn;

	return alphabet-2;
}

void ExcelRwC::createExcelSheet(string sheetName)
{
	COleVariant	covTrue((short)TRUE);
	COleVariant	covFalse((short)FALSE);
	COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	LPDISPATCH lpDisp = NULL;
	//创建一个EXCEL应用程序实例	
	if (!ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("创建Excel程序失败"));
		exit(1);
	}
	ExcelApp.put_Visible(FALSE);	//使程序可见	
	ExcelApp.put_UserControl(FALSE);
	//将工作簿容器对象与应用程序工作簿关联	
	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks());

	//打开一个工作簿，如果不存在，则新增一个工作簿	
	CString strFile = _T(ZTFPfileName.c_str());
	try {
		//打开一个工作簿		
		lpDisp = wbsMyBooks.Open(strFile, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional);
		//将工作簿对象和打开的工作簿关联		
		wbMyBook.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		//增加一个新的工作簿		
		lpDisp = wbsMyBooks.Add(covOptional);
		wbMyBook.AttachDispatch(lpDisp);
	}

	//获得工作表容器对象实例	
	wsMySheets.AttachDispatch(wbMyBook.get_Sheets());

	//打开一个工作表，如果不存在则新增一个	
	CString strSheetName = _T(sheetName.c_str());

	try
	{
		//打开一个已有的工作表(wooksheet)		
		lpDisp = wsMySheets.get_Item(COleVariant(strSheetName));
		wsMySheet.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		lpDisp = wsMySheets.Add(covOptional, covOptional, COleVariant((long)1), covOptional);
		wsMySheet.AttachDispatch(lpDisp);
		wsMySheet.put_Name(strSheetName);		//创建工作表名	
	}

	//system("pause");
	/************************************************************************/
	/* 添加单元格数据之方法二                                                                     */
	/************************************************************************/
	//设置单元格的范围为全部区域	
	lpDisp = wsMySheet.get_Cells();
	rgMyRge.AttachDispatch(lpDisp);
	//插入标题	
	excelHeaderSymbol = excelWriteHeaderC(rgMyRge);
	excelRowIndex = 2;
}

void ExcelRwC::closeExcelSheet()
{
	//////////////////////////////////////////////////////////////////////////////////////////// 	
	/*根据文件的后缀名选择保存文件的格式*/
	//CString strSaveAsName = _T("C:\\new.xlsx");
	ExcelApp.put_DisplayAlerts(FALSE);
	ExcelApp.put_AlertBeforeOverwriting(FALSE); // 不弹出对话框询问是否保存
	CString strSaveAsName = _T(ZTFPfileName.c_str());
	//wbMyBook.SaveCopyAs(COleVariant(_T(strSaveAsName.GetString())));
	wbMyBook.Save();
	wbMyBook.put_Saved(TRUE);
	ExcelApp.put_Visible(FALSE);
	//system("pause");
	ExcelApp.Quit();

	//释放资源	
	//chart.ReleaseDispatch();
	//chartObject.ReleaseDispatch();
	//chartObjects.ReleaseDispatch();
	rgMyRge.ReleaseDispatch();
	wsMySheet.ReleaseDispatch();
	wsMySheets.ReleaseDispatch();
	wbMyBook.ReleaseDispatch();
	wbsMyBooks.ReleaseDispatch();
	ExcelApp.ReleaseDispatch();
}

void ExcelRwC::writeExcelSheet(vector<PROPERTY_t> &propertyVec, int flag_guXinZhengShu)
{
	int &i = excelRowIndex;
	// 将数据填入Excel表格
	int showNum = 1;
	if (0 >= propertyVec.size())
	{
		return;
	}
	vector<PROPERTY_t>::iterator it = propertyVec.begin();
	rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(1)), COleVariant(it->limitReason));
	while (it != propertyVec.end())
	{
		int j = 1;
		PROPERTY_t &analyProty = *it;
		string name = analyProty.name;
		std::size_t found = name.find("ST");
		if (
			(found != std::string::npos)//剔除ST股
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE_ON.c_str()))
			)
		{
			++it;
			continue;
		}
		// 显示涨停原因后换行显示信息。
		if (1 == showNum)
		{
			i++;
		}
		showNum++;

		//string printFlag = getShareFlag<PROPERTY_t>(*it);
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->code));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->xianJia));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->name));
		float tempZhangfu = ((float)((int)((it->zuoRiKaiPanZhangFu + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempZhangfu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zhangFu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->ziYouLiuTongShiZhi / DIVIDE)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zuoRiZhangFu));

		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->limitReason));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->guXingPingFen));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiZuoRiZongJinE*100.0));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiLiuTong*100.0));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zijinIdx)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->preJingJiaZongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->continueDay)));
		setContinueDayBackColor(wsMySheet, excelHeaderSymbol, it->continueDay, i);

		float tempJingJiaLiangBi = ((float)((int)((it->jingJiaLiangBi + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempJingJiaLiangBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->weiBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zhangTingBan)));
		/*{
		CRange0	colorRge;
		CFont0  ft;    // 要插入excel类库里面的Font类，下面类似
		CString tempString;
		tempString.Format("%c%d", 'N', i);
		colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
		ft.AttachDispatch(colorRge.get_Font());
		ft.put_Color(COleVariant((long)RGB(255, 33, 33)));
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
		}*/
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->dianDanJinE)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zuoRiZongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->limitUpMoney / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->limitOpenCount)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->firstLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->lastLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->suoShuHangYe));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)it->zijinIdx));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiLiuTong * 100.0));
		float jingJiaBiLiutong = (it->zongJinE * TENTHOUSAND) / (it->ziYouLiuTongShiZhi);
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(jingJiaBiLiutong));

		bool yiZiBanFlag = yiZiBanJudge(analyProty);
		if (1)
		{
			//加入一字开盘的股票，显示红色
			{
				if ((true == yiZiBanFlag)
					//&& (analyProty.guXingPingFen > 34.0))
					)
				{
					setBackColor(wsMySheet, excelHeaderSymbol, i, 38);
					setYiZiBanBackColor(wsMySheet, excelHeaderSymbol, i); //红色
					if ((analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi)
						//&& ((analyProty.guXingPingFen > 30.0) && (0 == flag_guXinZhengShu) || (analyProty.guXingPingFen < 2712) && (1 == flag_guXinZhengShu))
						)
					{
						char column = 'F';
						setBackColor(wsMySheet, column, i, 20);
					}
					setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
					setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, excelHeaderSymbol, it->zuoRiZhangFu, i);
					setYiZiBanJingJiaJinEColor(wsMySheet, excelHeaderSymbol, it->zongJinE, i);
					//setJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, it->zhangFu, it->continueDay, i);

					/*if ((analyProty.zongJinE > 970.0 * TENTHOUSAND))
					{
						//setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					if (0 == analyProty.limitOpenCount)
					{
						setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					//昨日非一字或T字的
					if (!zuoRiyiZiBanTFlag)
					{
						setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, i);
					}
					if (
						((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
							&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
						|| zuoRiyiZiBanTFlag)
					{
						if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
						{
							setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, i);
						}
					}*/
					setSpecificColors(it, wsMySheet, excelHeaderSymbol, i, flag_guXinZhengShu);

					++it;
					i++;
					continue;
				}
			}
			//股性太差的，显示黑色
			{
				if (((analyProty.guXingPingFen < 49.0) && (0 == flag_guXinZhengShu)) ||
					((analyProty.guXingPingFen > 2012) && (1 == flag_guXinZhengShu)))
					//if(0)
				{
					setBackColor(wsMySheet, excelHeaderSymbol, i, 15);
					if ((true == yiZiBanFlag) || (zuoRiYiZiBanJudge(*it)))
					{
						//setYiZiBanBackColor(wsMySheet, excelHeaderSymbol, i); //红色
						setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
					}
					setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, excelHeaderSymbol, it->zuoRiZhangFu, i);
					setJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, it->zhangFu, it->continueDay, i);

					/*if ((analyProty.zongJinE > 970.0 * TENTHOUSAND))
					{
						//setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					if (0 == analyProty.limitOpenCount)
					{
						setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					//昨日非一字或T字的
					if (!zuoRiyiZiBanTFlag)
					{
						setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, i);
					}
					if (
						((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
							&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
						|| zuoRiyiZiBanTFlag)
					{
						if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
						{
							setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, i);
						}
					}*/
					setSpecificColors(it, wsMySheet, excelHeaderSymbol, i, flag_guXinZhengShu);

					++it;
					i++;
					continue;
				}
			}
			if (1
				//&& (analyProty.jingLiuRu < (analyProty.daDanJinE))
				//&& (analyProty.zhangFu > 1.5)
				//&& (analyProty.zongJinE > 1300 * TENTHOUSAND)
				//&& (analyProty.zhangTingBan > -6900.0)
				//&& (analyProty.weiBi > 0.0)
				&& (analyProty.weiBi > weiBiThreshold)
				&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				)
			{
				//跳过昨日涨停原因为超跌反弹
				string limitR = analyProty.limitReason;
				string::size_type idx;
				idx = limitR.find("超跌反弹");//在a中查找b.
				if (1
					&& (idx != string::npos)
					&& (analyProty.continueDay > 0)
					)
				{
					if (true == yiZiBanFlag)
					{
						setYiZiBanBackColor(wsMySheet, excelHeaderSymbol, i); //红色
						setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
						if ((analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi)
							//&& ((analyProty.guXingPingFen > 30.0) && (0 == flag_guXinZhengShu) || (analyProty.guXingPingFen < 2712) && (1 == flag_guXinZhengShu))
							)
						{
							char column = 'F';
							setBackColor(wsMySheet, column, i, 20);
						}
					}
					setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, excelHeaderSymbol, it->zuoRiZhangFu, i);
					setJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, it->zhangFu, it->continueDay, i);

					/*if ((analyProty.zongJinE > 970.0 * TENTHOUSAND))
					{
						//setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					if (0 == analyProty.limitOpenCount)
					{
						setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, i);
					}
					//昨日非一字或T字的
					if (!zuoRiyiZiBanTFlag)
					{
						setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, i);
					}
					if (
						((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
							&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
						|| zuoRiyiZiBanTFlag)
					{
						if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
						{
							setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, i);
						}
					}*/
					setSpecificColors(it, wsMySheet, excelHeaderSymbol, i, flag_guXinZhengShu);

					++it;
					i++;
					continue;
				}

				if (false == yiZiBanFlag)// 非一字开盘
				{
					if ((analyProty.dianDanJinE < dianDanThreshold) ||
					    (analyProty.zhangTingBan > zhangTingBanThreshold)
						)
					{
						if (zuoRiYiZiBanJudge(*it))
						{
							setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
						}
						setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
						setZuoRiZhangFuColor(wsMySheet, excelHeaderSymbol, it->zuoRiZhangFu, i);
						setJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, it->zhangFu, it->continueDay, i);

						/*if ((analyProty.zongJinE > 970.0 * TENTHOUSAND))
						{
							//setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, i);
						}
						if (0 == analyProty.limitOpenCount)
						{
							setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, i);
						}
						//昨日非一字或T字的
						if (!zuoRiyiZiBanTFlag)
						{
							setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, i);
						}
						if (
							((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
								&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
							|| zuoRiyiZiBanTFlag)
						{
							if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
							{
								setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, i);
							}
						}*/
						setSpecificColors(it, wsMySheet, excelHeaderSymbol, i, flag_guXinZhengShu);

						++it;
						i++;
						continue;
					}
				}
				if (1
					&& ((analyProty.zhangFu > kaiPanZfMax)
						|| (analyProty.zhangFu < kaiPanZfMin))
					//&& (i < 12)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 30)
					//&& (analyProty.zijinIdx < MAX_ZIJIN_IDX)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 3000)
					&& (analyProty.continueDay > 0)
					&& (
					((analyProty.zijinIdx < MAX_ZIJIN_IDX) && (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + 0.001))
						|| (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi))
					&& (analyProty.zongJinE > (danRiJingJiaThreshould * TENTHOUSAND))
					//&& (analyProty.zhangFu > analyProty.zuoRiKaiPanZhangFu)
					//&& (analyProty.dianDanJinE > dianDanThreshold)
					)
				{
					//昨日非一字或T字的，成交需大于2500万，避免庄股
					if (
						((abs(analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) > FLT_MIN)
							|| (abs(analyProty.zuoShou - analyProty.zuoRiKaiPan) > FLT_MIN))
						)
					{
						if ((analyProty.zuoRiZongJinE > (4000.0 * TENTHOUSAND))
							//&& (analyProty.zuoRiLiangBi > (0.5))
							)
						{
							if (((danRiJingJiaThreshould * TENTHOUSAND) < analyProty.zongJinE) && 
								(limitOpenThreshould > analyProty.limitOpenCount) &&
								(zuoRiLimitUpMoney < analyProty.limitUpMoney) &&
								(jingJiaChaThreshould < (analyProty.zhangFu - analyProty.zuoRiKaiPanZhangFu)) &&
							    (1 == analyProty.banKuaiFirstFlag) &&
								((analyProty.preJingJiaZongJinE < analyProty.zongJinE) || (jingJiaJinEThreshould < analyProty.zongJinE))
								)
							{
								setBackColor(wsMySheet, excelHeaderSymbol, i, 20);
							}
							if (true == yiZiBanFlag)
							{
								setYiZiBanBackColor(wsMySheet, excelHeaderSymbol, i); //红色
								setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
								if ((analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi)
									//&& ((analyProty.guXingPingFen > 30.0) && (0 == flag_guXinZhengShu) || (analyProty.guXingPingFen < 2712) && (1 == flag_guXinZhengShu))
									&& (jingJiaChaThreshould < (analyProty.zhangFu - analyProty.zuoRiKaiPanZhangFu))
									)
								{
									char column = 'F';
									setBackColor(wsMySheet, column, i, 20);
								}
							}
							setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
						}
					}
					else
					{
						if (((danRiJingJiaThreshould * TENTHOUSAND) < analyProty.zongJinE) &&
							(limitOpenThreshould > analyProty.limitOpenCount) &&
							(zuoRiLimitUpMoney < analyProty.limitUpMoney) &&
							(1 == analyProty.banKuaiFirstFlag) &&
							((analyProty.preJingJiaZongJinE < analyProty.zongJinE) || (jingJiaJinEThreshould < analyProty.zongJinE))
							)
						{
							setBackColor(wsMySheet, excelHeaderSymbol, i, 20);
						}
						if (true == yiZiBanFlag)
						{
							setYiZiBanBackColor(wsMySheet, excelHeaderSymbol, i); //红色
							setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
							if ((analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi) 
								//&& ((analyProty.guXingPingFen > 30.0) && (0 == flag_guXinZhengShu) || (analyProty.guXingPingFen < 2712) && (1 == flag_guXinZhengShu))
								)
							{
								char column = 'F';
								setBackColor(wsMySheet, column, i, 20);
							}
						}
						setContinueDayBackColor(wsMySheet, excelHeaderSymbol, analyProty.continueDay, i);
					}
				}
				else
				{
					if (zuoRiYiZiBanJudge(*it))
					{
						setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
					}
				}
			}
			else
			{
				if (zuoRiYiZiBanJudge(*it))
				{
					setZuoRiYiZiFangLiangJinYiZiColor(it, wsMySheet, excelHeaderSymbol, i);
				}
			}
		}

		/*if ((analyProty.zongJinE > 970.0 * TENTHOUSAND))
		{
			//setJingJiaJinEBackColor(wsMySheet, excelHeaderSymbol, i);
		}
		if (0 == analyProty.limitOpenCount)
		{
			setLimitOpenCountBackColor(wsMySheet, excelHeaderSymbol, i);
		}
		//昨日非一字或T字的
		if (!zuoRiyiZiBanTFlag)
		{
			setZuoRiChengJiaoJinEColor(wsMySheet, excelHeaderSymbol, it->zuoRiZongJinE, i);
		}
		if (
			((zhangFuMargin < (it->zhangFu - it->zuoRiKaiPanZhangFu)) && (it->zhangFu >= kaiPanZfMax)
				&& ((zuoRiKaiPanZfMax > it->zuoRiKaiPanZhangFu) || (1 < it->continueDay)))
			|| zuoRiyiZiBanTFlag)
		{
			if (zuoRiyiZiBanTFlag || ((zuoRiChengJiaoJinEMin * TENTHOUSAND) < it->zuoRiZongJinE))
			{
				setZuoRiJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, i);
			}
		}*/
		setSpecificColors(it, wsMySheet, excelHeaderSymbol, i, flag_guXinZhengShu);

		setZuoRiZhangFuColor(wsMySheet, excelHeaderSymbol, it->zuoRiZhangFu, i);
		setJingJiaZhangFuColor(wsMySheet, excelHeaderSymbol, it->zhangFu, it->continueDay, i);
		++it;
		i++;
	}
}
/*
void ExcelRwC::excelWriteRowC(string sheetName, vector<PROPERTY_t> &propertyVec, kaiPanSummary_t &summary)
{
	CApplication	ExcelApp;
	CWorkbooks0		wbsMyBooks;
	CWorkbook		wbMyBook;
	CWorksheets0	wsMySheets;
	CWorksheet		wsMySheet;
	CRange0			rgMyRge;
	CChart			chart;
	CChartObjects	chartObjects;		//图表容器对象	
	CChartObject	chartObject;		//图表对象 	
	COleVariant	covTrue((short)TRUE);
	COleVariant	covFalse((short)FALSE);
	COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	LPDISPATCH lpDisp = NULL;
		//创建一个EXCEL应用程序实例	
	if (!ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))	
	{		
		AfxMessageBox(_T("创建Excel程序失败"));
		exit(1);
	} 	
	ExcelApp.put_Visible(FALSE);	//使程序可见	
	ExcelApp.put_UserControl(FALSE);
	//将工作簿容器对象与应用程序工作簿关联	
	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks());

	string ZTFPfileName = "ZTFP.xlsx";
	//打开一个工作簿，如果不存在，则新增一个工作簿	
	CString strFile = _T(ZTFPfileName.c_str());
	try {		
		//打开一个工作簿		
		lpDisp = wbsMyBooks.Open(strFile, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional);
		//将工作簿对象和打开的工作簿关联		
		wbMyBook.AttachDispatch(lpDisp);
	}
	catch (...) 
	{		
		//增加一个新的工作簿		
		lpDisp = wbsMyBooks.Add(covOptional);
		wbMyBook.AttachDispatch(lpDisp);
	}

	//获得工作表容器对象实例	
	wsMySheets.AttachDispatch(wbMyBook.get_Sheets());

	//打开一个工作表，如果不存在则新增一个	
	CString strSheetName = _T(sheetName.c_str());

	try 
	{		
		//打开一个已有的工作表(wooksheet)		
		lpDisp = wsMySheets.get_Item(COleVariant(strSheetName));
		wsMySheet.AttachDispatch(lpDisp);
	}
	catch (...) 
	{
		lpDisp = wsMySheets.Add(covOptional, covOptional, COleVariant((long)1), covOptional);
		wsMySheet.AttachDispatch(lpDisp);
		wsMySheet.put_Name(strSheetName);		//创建工作表名	
	} 	*/

	//system("pause");
	/************************************************************************/	
	/* 添加单元格数据之方法二                                                                     */	
	/************************************************************************/	
	//设置单元格的范围为全部区域	
	/*
	lpDisp = wsMySheet.get_Cells();
	rgMyRge.AttachDispatch(lpDisp);
	//插入标题	
	char headerSymbol = excelWriteHeaderC(rgMyRge);

	// 将数据填入Excel表格	
	int i = 3;
	int showNum = 1;
	vector<PROPERTY_t>::iterator it = propertyVec.begin();
	string limitReason = it->limitReason;
	float  preLimitVsCirculateJingJia = it->limitVsCirculateJingJia;
	float  preZhangFu = it->zhangFu;
	rgMyRge.put_Item(COleVariant((LONG)(i-1)), COleVariant((LONG)(1)), COleVariant(it->limitReason));
	while (it != propertyVec.end())
	{
		int j = 1;
		PROPERTY_t &analyProty = *it;
		string name = analyProty.name;
		std::size_t found = name.find("ST");
		if (
			(found != std::string::npos)//剔除ST股
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE_ON.c_str()))
			)
		{
			++it;
			continue;
		}

		string limitR = it->limitReason;
		string::size_type idx0, idx1;
		idx0 = limitR.find(limitReason);//在a中查找b.
		idx1 = limitReason.find(limitR);
		if ((((idx0 == string::npos) || (idx1 == string::npos)) && (preZhangFu < it->zhangFu - 5.0))
			|| ((idx0 == string::npos) && (idx1 == string::npos)))
		{
			rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j)), COleVariant(it->limitReason));
			i++;
		}
		limitReason = limitR;
		preLimitVsCirculateJingJia = it->limitVsCirculateJingJia;
		preZhangFu = it->zhangFu;

		showNum++;

		//string printFlag = getShareFlag<PROPERTY_t>(*it);
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->code));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->xianJia));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->name));
		float tempZhangfu = ((float)((int)((it->zuoRiKaiPanZhangFu + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempZhangfu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zhangFu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zuoRiZhangFu));

		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->limitReason));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->guXingPingFen));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiZuoRiZongJinE*100.0));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiLiuTong*100.0));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zijinIdx)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->preJingJiaZongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->continueDay)));
		setContinueDayBackColor(wsMySheet, headerSymbol, it->continueDay, i);
		
		float tempJingJiaLiangBi = ((float)((int)((it->jingJiaLiangBi + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempJingJiaLiangBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->weiBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zhangTingBan)));
		*/
		/*{
			CRange0	colorRge;
			CFont0  ft;    // 要插入excel类库里面的Font类，下面类似  
			CString tempString;
			tempString.Format("%c%d", 'N', i);
			colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(tempString.GetString())), COleVariant(_T(tempString.GetString()))));
			ft.AttachDispatch(colorRge.get_Font());
			ft.put_Color(COleVariant((long)RGB(255, 33, 33)));
			ft.ReleaseDispatch();
			colorRge.ReleaseDispatch();
		}*/
		/*
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->dianDanJinE)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zuoRiZongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->limitUpMoney / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->limitOpenCount)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->ziYouLiuTongShiZhi / DIVIDE)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->firstLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->lastLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->suoShuHangYe));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->huanShou));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiLiuTong));

		if(1)
		{
			//加入一字开盘的股票，显示红色
			{
				if ((analyProty.guXingPingFen > 44.0)
					&& (true == yiZiBanJudge(analyProty)))
				{
					setBackColor(wsMySheet, headerSymbol, i, 38);
					setYiZiBanBackColor(wsMySheet, headerSymbol, i); //红色
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, headerSymbol, it->zuoRiZhangFu, i);
					++it;
					i++;
					continue;
				}
			}
			//股性太差的，显示黑色
			if(0)
			{
				if (analyProty.guXingPingFen < 49.0)
				{
					setBackColor(wsMySheet, headerSymbol, i, 15);
					if (true == yiZiBanJudge(analyProty))
					{
						setYiZiBanBackColor(wsMySheet, headerSymbol, i); //红色
					}
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, headerSymbol, it->zuoRiZhangFu, i);
					++it;
					i++;
					continue;
				}
			}
			if (1
				//&& (analyProty.jingLiuRu < (analyProty.daDanJinE))
				//&& (analyProty.zhangFu > 1.5)
				//&& (analyProty.zongJinE > 1300 * TENTHOUSAND)
				//&& (analyProty.zhangTingBan > -6900.0)
				//&& (analyProty.weiBi > 0.0)
				&& (analyProty.weiBi > weiBiThreshold)
				&& (0 != strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
				)
			{
				//跳过昨日涨停原因为超跌反弹
				string limitR = analyProty.limitReason;
				string::size_type idx;
				idx = limitR.find("超跌反弹");//在a中查找b.
				if (1
					&& (idx != string::npos)
					&& (analyProty.continueDay > 0)
					)
				{
					if (true == yiZiBanJudge(analyProty))
					{
						setYiZiBanBackColor(wsMySheet, headerSymbol, i); //红色
					}
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					setZuoRiZhangFuColor(wsMySheet, headerSymbol, it->zuoRiZhangFu, i);
					++it;
					i++;
					continue;
				}

				if (false == yiZiBanJudge(analyProty))// 非一字开盘
				{
					if (analyProty.dianDanJinE < dianDanThreshold)
					{
						setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
						setZuoRiZhangFuColor(wsMySheet, headerSymbol, it->zuoRiZhangFu, i);
						++it;
						i++;
						continue;
					}
				}
				if (1
					&& ((analyProty.zhangFu > kaiPanZfMax)
						|| (analyProty.zhangFu < kaiPanZfMin))
					//&& (i < 12)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 30)
					//&& (analyProty.zijinIdx < MAX_ZIJIN_IDX)
					//&& (analyProty.liuRuBiZuoRiChengJiaoIdx < 3000)
					&& (analyProty.continueDay > 0)
					&& (
					((analyProty.zijinIdx < MAX_ZIJIN_IDX) && (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1))
						|| (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + zongLiuRuThreshold_for_yizi))
					&& (analyProty.zongJinE > (998.0 * TENTHOUSAND))
					//&& (analyProty.zhangFu > analyProty.zuoRiKaiPanZhangFu)
					//&& (analyProty.dianDanJinE > dianDanThreshold)
					)
				{
					//昨日非一字或T字的，成交需大于2500万，避免庄股
					if (
						((abs(analyProty.zuoRiZuiGao - analyProty.zuoRiKaiPan) > FLT_MIN)
							|| (abs(analyProty.zuoShou - analyProty.zuoRiKaiPan) > FLT_MIN))
						)
					{
						if ((analyProty.zuoRiZongJinE > (4000.0 * TENTHOUSAND))
							//&& (analyProty.zuoRiLiangBi > (0.5))
							)
						{
							setBackColor(wsMySheet, headerSymbol, i, 20);
							if (true == yiZiBanJudge(analyProty))
							{
								setYiZiBanBackColor(wsMySheet, headerSymbol, i); //红色
							}
							setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
						}
					}
					else
					{
						setBackColor(wsMySheet, headerSymbol, i, 20);
						if (true == yiZiBanJudge(analyProty))
						{
							setYiZiBanBackColor(wsMySheet, headerSymbol, i); //红色
						}
						setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					}
				}
			}
		}
		setZuoRiZhangFuColor(wsMySheet, headerSymbol, it->zuoRiZhangFu, i);
		++it;
		i++;
	}*/
	/*/
	//////////////////////////////////////////////图表操作//////////////////////////////////////////	
	//图表chart	
	double left = 200;
	double top = 450;
	double width = 700;
	double height = 360;
	//关联图表容器对象	
	lpDisp = wsMySheet.ChartObjects(covOptional);
	ASSERT(lpDisp);
	chartObjects.AttachDispatch(lpDisp);
	//新建一个图表对象	
	chartObject = chartObjects.Add(left, top, width, height);
	//关联图表对象	
	lpDisp = chartObject.get_Chart();
	chart.AttachDispatch(lpDisp);
	chart.put_ChartType(72);	//设置图表类型 	

	//设置数据的有效区域	
	CString tempString;
	tempString.Format("%c%d", 'E', showNum);
	//printf("%s", tempString);
	lpDisp = wsMySheet.get_Range(COleVariant(_T("E2")), COleVariant(_T(tempString.GetString())));
	ASSERT(lpDisp);
	VARIANT var;
	var.vt = VT_DISPATCH;
	var.pdispVal = lpDisp;

	CString Title;
	float shangZhangLv = (float)summary.zhangCount / (float)(summary.zhangCount + summary.dieCount);
	Title.Format("一字开盘:%d, 上涨率:%5.2f, 平均涨幅:%5.2f, 上涨数:%d, 下跌数:%d, 新股:%d, 连续一字板:%d, ST股数:%d\n\n连板涨幅::%5.2f, 昨日连板个数:%d\n", 
		summary.yiZiBanCount, shangZhangLv, summary.avgZhang, summary.zhangCount, summary.dieCount, summary.xinGuCount, summary.zuoRiYiZiCount, summary.stCount, summary.conAvgIncrease / float(summary.conZhangCount), summary.conZhangCount);
	chart.ChartWizard(
		var,                // Source.		
		covOptional,  // Gallery		
		covOptional,   // Format: 1~6.		
		COleVariant((short)2),   // PlotBy: 指定系列中的数据是来自行(1)还是来自列(2).		
		COleVariant((short)0),   // CategoryLabels.		
		COleVariant((short)1),   // SeriesLabels.		
		COleVariant((short)TRUE),  // HasLegend.		
		COleVariant(_T(Title)),  // Title.		
		COleVariant(_T("高度板")),    // CategoryTitle.		
		COleVariant(_T("涨幅")),  // ValueTitles.		
		covOptional              // ExtraTitle.		
	);
	
	//system("pause");
	//////////////////////////////////////////////////////////////////////////////////////////// 	
	/*根据文件的后缀名选择保存文件的格式*/	
	//CString strSaveAsName = _T("C:\\new.xlsx");
	/*
	ExcelApp.put_DisplayAlerts(FALSE);
	ExcelApp.put_AlertBeforeOverwriting(FALSE); // 不弹出对话框询问是否保存
	CString strSaveAsName = _T(ZTFPfileName.c_str());
	//wbMyBook.SaveCopyAs(COleVariant(_T(strSaveAsName.GetString())));
	wbMyBook.Save();
	wbMyBook.put_Saved(TRUE);
	ExcelApp.put_Visible(FALSE);
	//system("pause");
	ExcelApp.Quit();

	//释放资源	
	//chart.ReleaseDispatch();
	//chartObject.ReleaseDispatch();
	//chartObjects.ReleaseDispatch();
	rgMyRge.ReleaseDispatch();
	wsMySheet.ReleaseDispatch();
	wsMySheets.ReleaseDispatch();
	wbMyBook.ReleaseDispatch();
	wbsMyBooks.ReleaseDispatch();
	ExcelApp.ReleaseDispatch();
}*/

}
