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
	ExcelRwC();
	~ExcelRwC();

    void excelWriteHeader(BasicExcelWorksheet* sheet);    
    void excelWriteRow(string sheetName, std::vector<PROPERTY_t> &propertyVec);
	void setBackColor(CWorksheet &wsMySheet, char &headerSymbol, int &row, int color);
	void setCellBackColor(CWorksheet &wsMySheet, char &headerSymbol, char column, int &row, int color);
	void setContinueDayBackColor(CWorksheet &wsMySheet, char &headerSymbol, int continueDay, int &row);
	char excelWriteHeaderC(CRange0 &rgMyRge);
	void excelWriteRowC(string sheetName, std::vector<PROPERTY_t> &propertyVec, kaiPanSummary_t &summary);

public:
};
    
}
