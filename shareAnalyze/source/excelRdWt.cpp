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

ExcelRwC::ExcelRwC()
{
	if (CoInitialize(NULL) != 0 )
	{
		printf("��ʼ��ʧ��");
		exit(1);
	}
}

ExcelRwC::~ExcelRwC()
{
	CoUninitialize();  //�ͷ�com��Դ
}

void ExcelRwC::excelWriteHeader(BasicExcelWorksheet *sheet)
{
	int eRow = 0;
	int eColumn = 0;
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��Ʊ���").c_str());
	sheet->Cell(eRow, eColumn++)->SetString("CODE");
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("�ּ�").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("NAME").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("�Ƿ�").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("���۳ɽ�").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("���վ��۳ɽ�").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("������").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��������").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("ί��").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("����").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("����").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("���ճɽ�").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("������ͣ�ⵥ").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��ͣ�򿪴���").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("������ͨ��ֵ").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��ͣԭ��").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("���ջ���").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��������").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��ҵ").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("��������").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("�״���ͣʱ��").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("������ͣʱ��").c_str());
	sheet->Cell(eRow, eColumn++)->SetWString(ANSIToUnicode("������").c_str());
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
	Cnterior interior; // ��ɫ����  
	CRange0	 colorRge;
	CString range1, range2;
	range1.Format("%c%d", 'A', row);
	range2.Format("%c%d", headerSymbol, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)color)); // ��ɫ����Ϊǳ��ɫ 20

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setCellBackColor(CWorksheet &wsMySheet, char &headerSymbol, char column, int &row, int color)
{
	Cnterior interior; // ��ɫ����  
	CRange0	 colorRge;
	CString range1, range2;
	range1.Format("%c%d", column, row);
	range2.Format("%c%d", column, row);
	colorRge.AttachDispatch(wsMySheet.get_Range(COleVariant(_T(range1.GetString())), COleVariant(_T(range2.GetString()))), true);
	interior.AttachDispatch(colorRge.get_Interior());
	interior.put_ColorIndex(COleVariant((long)color)); // ��ɫ����Ϊǳ��ɫ 20

	colorRge.ReleaseDispatch();
	interior.ReleaseDispatch();
}

void ExcelRwC::setContinueDayBackColor(CWorksheet &wsMySheet, char &headerSymbol, int continueDay, int &row)
{
	{
		CRange0	colorRge;
		CFont0  ft;    // Ҫ����excel��������Font�࣬��������  
		CString tempString;
		tempString.Format("%c%d", 'K', row);
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
			setCellBackColor(wsMySheet, headerSymbol, 'K', row, 8);//ǳ��ɫ
																 //ft.put_Color(COleVariant((long)RGB(0, 255, 125)));
			break;
		case 3:
			setCellBackColor(wsMySheet, headerSymbol, 'K', row, 26);//����ɫ
			break;
		default:
			//ft.put_Color(COleVariant((long)RGB(255, 0, 0)));
			setCellBackColor(wsMySheet, headerSymbol, 'K', row, 3); //��ɫ
			break;
		}
		ft.ReleaseDispatch();
		colorRge.ReleaseDispatch();
	}
}

char ExcelRwC::excelWriteHeaderC(CRange0 &rgMyRge)
{
	int eRow = 1;
	int eColumn = 1;
	char alphabet = 'A';
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��Ʊ���"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("CODE"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("�ּ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("NAME"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("���տ����Ƿ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("�Ƿ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��ͣԭ��"));

	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��������"));
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��������ͨ"));
	//rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("����"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("����"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("���۳ɽ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("���վ��۳ɽ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("������"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��������"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("ί��"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��ͣ"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("�浥"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("���ճɽ�"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("������ͣ�ⵥ"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��ͣ�򿪴���"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("������ͨ��ֵ"));

	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("�״���ͣʱ��"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("������ͣʱ��"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("��ҵ"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("����"));
	rgMyRge.put_Item(COleVariant((LONG)eRow), COleVariant((LONG)(eColumn++)), COleVariant("����"));

	alphabet += eColumn;

	return alphabet-2;
}

void ExcelRwC::excelWriteRowC(string sheetName, vector<PROPERTY_t> &propertyVec, kaiPanSummary_t &summary)
{
	CApplication	ExcelApp;
	CWorkbooks0		wbsMyBooks;
	CWorkbook		wbMyBook;
	CWorksheets0	wsMySheets;
	CWorksheet		wsMySheet;
	CRange0			rgMyRge;
	CChart			chart;
	CChartObjects	chartObjects;		//ͼ����������	
	CChartObject	chartObject;		//ͼ����� 	
	COleVariant	covTrue((short)TRUE);
	COleVariant	covFalse((short)FALSE);
	COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	LPDISPATCH lpDisp = NULL;
		//����һ��EXCELӦ�ó���ʵ��	
	if (!ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))	
	{		
		AfxMessageBox(_T("����Excel����ʧ��"));
		exit(1);
	} 	
	ExcelApp.put_Visible(FALSE);	//ʹ����ɼ�	
	ExcelApp.put_UserControl(FALSE);
	//������������������Ӧ�ó�����������	
	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks());

	string ZTFPfileName = "ZTFP.xlsx";
	//��һ������������������ڣ�������һ��������	
	CString strFile = _T(ZTFPfileName.c_str());
	try {		
		//��һ��������		
		lpDisp = wbsMyBooks.Open(strFile, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional);
		//������������ʹ򿪵Ĺ���������		
		wbMyBook.AttachDispatch(lpDisp);
	}
	catch (...) 
	{		
		//����һ���µĹ�����		
		lpDisp = wbsMyBooks.Add(covOptional);
		wbMyBook.AttachDispatch(lpDisp);
	}

	//��ù�������������ʵ��	
	wsMySheets.AttachDispatch(wbMyBook.get_Sheets());

	//��һ�����������������������һ��	
	CString strSheetName = _T(sheetName.c_str());

	try 
	{		
		//��һ�����еĹ�����(wooksheet)		
		lpDisp = wsMySheets.get_Item(COleVariant(strSheetName));
		wsMySheet.AttachDispatch(lpDisp);
	}
	catch (...) 
	{
		lpDisp = wsMySheets.Add(covOptional, covOptional, COleVariant((long)1), covOptional);
		wsMySheet.AttachDispatch(lpDisp);
		wsMySheet.put_Name(strSheetName);		//������������	
	} 	

	//system("pause");
	/************************************************************************/	
	/* ��ӵ�Ԫ������֮������                                                                     */	
	/************************************************************************/	
	//���õ�Ԫ��ķ�ΧΪȫ������	
	lpDisp = wsMySheet.get_Cells();
	rgMyRge.AttachDispatch(lpDisp);
	//�������	
	char headerSymbol = excelWriteHeaderC(rgMyRge);

	// ����������Excel���	
	int i = 2;
	int showNum = 1;
	vector<PROPERTY_t>::iterator it = propertyVec.begin();
	string limitReason = it->limitReason;
	rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(1)), COleVariant(it->limitReason));
	while (it != propertyVec.end())
	{
		int j = 1;
		PROPERTY_t &analyProty = *it;
		string name = analyProty.name;
		std::size_t found = name.find("ST");
		if (
			(found != std::string::npos)//�޳�ST��
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE.c_str()))
			|| (0 == strcmp(analyProty.limitReason, NEW_SHARE_ON.c_str()))
			)
		{
			++it;
			continue;
		}

		string limitR = it->limitReason;
		string::size_type idx;
		idx = limitR.find(limitReason);//��a�в���b.
		if (idx == string::npos)
		{
			idx = limitReason.find(limitR);
			if (idx == string::npos)
			{
				rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j)), COleVariant(it->limitReason));
				limitReason = limitR;
				i++;
			}
		}

		showNum++;

		//string printFlag = getShareFlag<PROPERTY_t>(*it);
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->code));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->xianJia));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->name));
		float tempZhangfu = ((float)((int)((it->zuoRiKaiPanZhangFu + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempZhangfu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zhangFu));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->limitReason));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->guXingPingFen));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiZuoRiZongJinE*100.0));
		//rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->zongLiuRuBiLiuTong*100.0));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zijinIdx)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->preJingJiaZongJinE / TENTHOUSAND)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->continueDay)));
		setContinueDayBackColor(wsMySheet, headerSymbol, it->continueDay, i);
		
		float tempJingJiaLiangBi = ((float)((int)((it->jingJiaLiangBi + 0.005) * 100))) / 100;
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(tempJingJiaLiangBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->weiBi));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->zhangTingBan)));
		/*{
			CRange0	colorRge;
			CFont0  ft;    // Ҫ����excel��������Font�࣬��������  
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
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant((LONG)(it->ziYouLiuTongShiZhi / DIVIDE)));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->firstLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->lastLimitTime));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->suoShuHangYe));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->huanShou));
		rgMyRge.put_Item(COleVariant((LONG)(i)), COleVariant((LONG)(j++)), COleVariant(it->liangBi));

		if(1)
		{
			//����һ�ֿ��̵Ĺ�Ʊ����ʾ��ɫ
			{
				if ((analyProty.guXingPingFen > 44.0)
					&& (true == yiZiBanJudge(analyProty)))
				{
					setBackColor(wsMySheet, headerSymbol, i, 38);
					setCellBackColor(wsMySheet, headerSymbol, 'N', i, 3); //��ɫ
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					++it;
					i++;
					continue;
				}
			}
			//����̫��ģ���ʾ��ɫ
			{
				if (analyProty.guXingPingFen < 49.0)
				{
					setBackColor(wsMySheet, headerSymbol, i, 15);
					if (true == yiZiBanJudge(analyProty))
					{
						setCellBackColor(wsMySheet, headerSymbol, 'N', i, 3); //��ɫ
					}
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
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
				//����������ͣԭ��Ϊ��������
				string limitR = analyProty.limitReason;
				string::size_type idx;
				idx = limitR.find("��������");//��a�в���b.
				if (1
					&& (idx != string::npos)
					&& (analyProty.continueDay > 0)
					)
				{
					if (true == yiZiBanJudge(analyProty))
					{
						setCellBackColor(wsMySheet, headerSymbol, 'N', i, 3); //��ɫ
					}
					setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					++it;
					i++;
					continue;
				}

				if (false == yiZiBanJudge(analyProty))// ��һ�ֿ���
				{
					if (analyProty.dianDanJinE < dianDanThreshold)
					{
						setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
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
						|| (analyProty.zongLiuRuBiLiuTong > zongLiuRuThreshold1 + 0.003))
					&& (analyProty.zongJinE > (998.0 * TENTHOUSAND))
					//&& (analyProty.zhangFu > analyProty.zuoRiKaiPanZhangFu)
					//&& (analyProty.dianDanJinE > dianDanThreshold)
					)
				{
					//���շ�һ�ֻ�T�ֵģ��ɽ������2500�򣬱���ׯ��
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
								setCellBackColor(wsMySheet, headerSymbol, 'N', i, 3); //��ɫ
							}
							setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
						}
					}
					else
					{
						setBackColor(wsMySheet, headerSymbol, i, 20);
						if (true == yiZiBanJudge(analyProty))
						{
							setCellBackColor(wsMySheet, headerSymbol, 'N', i, 3); //��ɫ
						}
						setContinueDayBackColor(wsMySheet, headerSymbol, analyProty.continueDay, i);
					}
				}
			}
		}
		++it;
		i++;
	}
	/*/
	//////////////////////////////////////////////ͼ�����//////////////////////////////////////////	
	//ͼ��chart	
	double left = 200;
	double top = 450;
	double width = 700;
	double height = 360;
	//����ͼ����������	
	lpDisp = wsMySheet.ChartObjects(covOptional);
	ASSERT(lpDisp);
	chartObjects.AttachDispatch(lpDisp);
	//�½�һ��ͼ�����	
	chartObject = chartObjects.Add(left, top, width, height);
	//����ͼ�����	
	lpDisp = chartObject.get_Chart();
	chart.AttachDispatch(lpDisp);
	chart.put_ChartType(72);	//����ͼ������ 	

	//�������ݵ���Ч����	
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
	Title.Format("һ�ֿ���:%d, ������:%5.2f, ƽ���Ƿ�:%5.2f, ������:%d, �µ���:%d, �¹�:%d, ����һ�ְ�:%d, ST����:%d\n\n�����Ƿ�::%5.2f, �����������:%d\n", 
		summary.yiZiBanCount, shangZhangLv, summary.avgZhang, summary.zhangCount, summary.dieCount, summary.xinGuCount, summary.zuoRiYiZiCount, summary.stCount, summary.conAvgIncrease / float(summary.conZhangCount), summary.conZhangCount);
	chart.ChartWizard(
		var,                // Source.		
		covOptional,  // Gallery		
		covOptional,   // Format: 1~6.		
		COleVariant((short)2),   // PlotBy: ָ��ϵ���е�������������(1)����������(2).		
		COleVariant((short)0),   // CategoryLabels.		
		COleVariant((short)1),   // SeriesLabels.		
		COleVariant((short)TRUE),  // HasLegend.		
		COleVariant(_T(Title)),  // Title.		
		COleVariant(_T("�߶Ȱ�")),    // CategoryTitle.		
		COleVariant(_T("�Ƿ�")),  // ValueTitles.		
		covOptional              // ExtraTitle.		
	);
	
	//system("pause");
	//////////////////////////////////////////////////////////////////////////////////////////// 	
	/*�����ļ��ĺ�׺��ѡ�񱣴��ļ��ĸ�ʽ*/	
	//CString strSaveAsName = _T("C:\\new.xlsx");
	ExcelApp.put_DisplayAlerts(FALSE);
	ExcelApp.put_AlertBeforeOverwriting(FALSE); // �������Ի���ѯ���Ƿ񱣴�
	CString strSaveAsName = _T(ZTFPfileName.c_str());
	//wbMyBook.SaveCopyAs(COleVariant(_T(strSaveAsName.GetString())));
	wbMyBook.Save();
	wbMyBook.put_Saved(TRUE);
	ExcelApp.put_Visible(FALSE);
	//system("pause");
	ExcelApp.Quit();

	//�ͷ���Դ	
	chart.ReleaseDispatch();
	chartObject.ReleaseDispatch();
	chartObjects.ReleaseDispatch();
	rgMyRge.ReleaseDispatch();
	wsMySheet.ReleaseDispatch();
	wsMySheets.ReleaseDispatch();
	wbMyBook.ReleaseDispatch();
	wbsMyBooks.ReleaseDispatch();
	ExcelApp.ReleaseDispatch();
}

}
