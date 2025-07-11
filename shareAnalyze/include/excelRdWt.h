#pragma once
#include <cstdio>
#include <vector>
#include <set>
#include "dataStructure.h"
#include <BasicExcel.hpp>
#include <afxdisp.h>        // MFCAutomation classes
#include "CApplication.h"
#include "CRange0.h"
#include "CChart.h"
#include "CFont0.h"
#include "CChartObject.h"
#include "CChartObjects.h"
#include "CWorkbook.h"
#include "CWorkbooks0.h"
#include "CWorksheet.h"
#include "CWorksheets0.h"
#include "Cnterior.h"

using namespace YExcel;

namespace EXCELREADWRITE
{
class ExcelRwC
{
public:
	ExcelRwC(int ztfpFlag);
	~ExcelRwC();

    void excelWriteHeader(BasicExcelWorksheet* sheet);    
    void excelWriteRow(string sheetName, std::vector<PROPERTY_t> &propertyVec);
	void setBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row, int color);
	void setCellBackColor(CWorksheet &wsMySheet, char &headerSymbol, char column, int &row, int color);
	void setYiZiBanBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setJingJiaJinEBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setYiZiBanGuXingBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setLimitOpenCountBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setContinueDayBackColor(CWorksheet &wsMySheet, char &headerSymbol, int continueDay, int &row);
	void setZuoRiZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, float zuoRiZhangFu, int &row);
	void setJingJiaZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, float zhangFu, int continueDay, int &row);
	void setZuoRiJingJiaZhangFuColor(CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setYiZiBanJingJiaJinEColor(CWorksheet &wsMySheet, char &headerSymbol, float zongJinE, int &row);
	void setZuoRiChengJiaoJinEColor(CWorksheet &wsMySheet, char &headerSymbol, float zuoRiChengJiaoJinE, int &row);
	void setShouBanGaoKaiErBanYiZiColor(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setZuoRiYiZiFangLiangJinYiZiColor(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &headerSymbol, int &row);
	void setSpecificColors(vector<PROPERTY_t>::iterator it, CWorksheet &wsMySheet, char &excelHeaderSymbol, int &row, int flag_guXinZhengShu);
	char excelWriteHeaderC(CRange0 &rgMyRge);
	void createExcelSheet(string sheetName);
	void closeExcelSheet();
	void writeExcelSheet(vector<PROPERTY_t> &propertyVec, int flag_guXinZhengShu);
	void excelWriteRowC(string sheetName, std::vector<PROPERTY_t> &propertyVec, kaiPanSummary_t &summary);

public:

	CApplication ExcelApp;
	CWorkbooks0 wbsMyBooks;
	CWorkbook wbMyBook;
	CWorksheets0 wsMySheets;
	CWorksheet wsMySheet;
	CRange0 rgMyRge;
	string ZTFPfileName;
	int excelRowIndex;
	char excelHeaderSymbol;
};
}
