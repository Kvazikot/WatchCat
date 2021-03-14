#pragma once
#include <string>
#include <vector>
using namespace std;

// СТРОКИ
bool isSymbol(char c);
bool isNumber(char c);
bool StrHasToken(string s, string token, string seps);
string StrRep(string s, string f, string r);
string strf(string str, string sep, int num);
vector<string> StrSplit(string str);
int CountSeps(string str, string sep);
string RemoveSeps(string str, string sep);
vector<string> VarsSplit(string str);
string clean_string_seps(string& s, string seps);
vector<string> StrSplitE(string str, string seps, bool withSep);
int NumLines(string str);
int StrToInt(string s);
string IntToStr(int i);
double StrToDouble(string s);
string DoubleToStr(double d);
bool isContains(string s, string substr);
void CleanEndingSpaces(string& s);
string ExtractFileName(string& fullpath);
string ExtractFileExt(string& fullpath);
string DoubleToStr(double d, char* fmt);
