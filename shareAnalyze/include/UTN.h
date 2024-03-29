#pragma once  
//#include "stdafx.h"  
#include <cstdio>
#include "windows.h"  
#include <iostream>  
#include <string>  

using namespace std;  

//**************string******************//  

// ASCII与Unicode互转  

wstring AsciiToUnicode(const std::string& str);  

std::string  UnicodeToAscii(const wstring& wstr);  

// UTF8与Unicode互转  

wstring Utf8ToUnicode(const std::string& str);  

std::string  UnicodeToUtf8(const wstring& wstr);  

// ASCII与UTF8互转  

std::string  AsciiToUtf8(const std::string& str);

std::string  Utf8ToAscii(const std::string& str);

/************string-int***************/  

// string 转 Int  


int StringToInt(const std::string& str);

std::string IntToString(int i);

std::string IntToString(char i);

//std::string IntToString(double i);