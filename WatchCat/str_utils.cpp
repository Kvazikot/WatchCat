#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <map>
#include <deque>
#include "str_utils.h"


std::string ExtractFileName(std::string& fullpath)
{
	return fullpath.substr(fullpath.find_last_of('\\') + 1);
}

std::string ExtractFileExt(std::string& fullpath)
{
	return fullpath.substr(fullpath.find_last_of('.') + 1);
}

bool StrHasToken(std::string s, std::string token, std::string seps)
{
    std::vector<std::string> tokens = StrSplitE(s, seps, false);
    for(int j=0; j<(int)tokens.size(); j++)
		if (tokens[j] == token ) 
			return true;
	return false;
}

void StrListToMap(std::vector<std::string>& from, map<std::string, int>& to)
{
    for(int i=0; i<(int)from.size(); i++)
		to[from[i]] = i;
}

//очистка строки от лишних последовательностей \r\n
void CleanEndingSpaces(std::string& s)
{
	for(int i=s.size()-1; i>0; i--)
	{
		if(s[i]==' ')
			s[i] = 0;
		else
			break;
	}
	s.resize(strlen( s.c_str() ));

	return;



}

std::string RemoveSeps(std::string str, std::string sep)
{
    std::string res;
    for(int i=0; i<(int)str.size(); i++)
	{
		if(str[i] != sep[0])
			res+=str[i];
	}
	return res;
}

bool isContains(std::string s, std::string substr)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	std::transform(substr.begin(), substr.end(), substr.begin(), ::tolower);
	return  (strstr(s.c_str(), substr.c_str())!=NULL);
}


std::string strf(std::string str, std::string sep, int num)
{
	int s = 0;
	//str+=sep;
	num++;
    for(int i=0; i<(int)str.size(); i++)
	{
		if(str[i] == sep[0])
		{
			num--;
			if(num < 0 ) break;
			if( num == 0 )
			{
				str = str.substr(s,i-s);
				if(str.substr(0,1) == sep )
					str = str.substr(1,str.size()-1);
				 
				//if(str.size()>0)					str.resize(str.size()-1);
				 return str;
			}
			s = i;	
		}
		
		
	}

    return std::string("");


}

std::string StrRep(std::string s, std::string f, std::string r)
{
	int i;

	if(f.size()==0) return s;
	if(s.size()==0) return s;
	//p = std::find(s.begin(), s.end(), f);	
    while( (i = s.find(f))!=std::string::npos )
	{
		s.replace(i, f.size(), r);
	}

	return s;


}


std::string StrJoin(std::vector<std::string>& list, std::string sep)
{
	int i;
    std::string s;
    for(i=0; i< (int)list.size(); i++)
		s+=s+list[i]+sep;
	return s;
}

std::vector<std::string> StrSplit(std::string str)
{
    std::vector<std::string> vec;
	int k=0;
	int i=0;

    for(i=0; i<(int)str.size(); i++)
	{
		if(str[i]=='\n')
		{
            std::string t = StrRep(str.substr(k,i-k),"\n","");
			t = StrRep(t,"\r","");
			vec.push_back(t);
			k=i;
		}
	
	}
	vec.push_back(StrRep(str.substr(k,i-k),"\n",""));
	return vec;
}



int NumLines(std::string str)
{
	int k=0;
	int l=0;

    for(int i=0; i<(int)str.size(); i++)
	{
		if(str[i]=='\n')
		{
			l++;
			k=i;
		}
	
	}
	l++;
	return l;
}


int StrToInt(std::string s)
{
	int i;
	if( sscanf(s.c_str(), "%d", &i)==1 )
		return i;
	else
		return INT_MAX;
}

std::string IntToStr(int i)
{
	char s[100];
	sprintf(s,"%d",i);
	return s;
}


std::string clean_string_seps(std::string& s, std::string seps)
{
    for(uint j=0; j< seps.size(); j++)
	{
        std::string sep=" ";
		sep[0] = seps[j];
		s = StrRep(s,sep,"");
	}
	return s;
}

std::string clean_string(std::string& s, std::string sub)
{
	s = StrRep(s,sub,"");
	return s;
}


std::vector<std::string> VarsSplit(std::string str)
{
    std::vector<std::string> vec;
    std::vector<std::string> vec2;
	int k=0;
	int i=0;

    for(i=0; i<(int)str.size(); i++)
	{
		if(str[i]==' ')
		{
            std::string aa = str.substr(k,i-k);
            std::string bb = "=  ;";
            std::string t = clean_string_seps(aa,bb);
            //std::string t = clean_string_seps(str.substr(k,i-k),aa);
			vec.push_back(t);
			k=i;
		}
		if(str[i]=='=')
		{
            std::string aa = str.substr(k,i-k);
            std::string bb = "=  ;";
            std::string t = clean_string_seps(aa,bb);
            vec.push_back(t);
			k=i;
		}
		if(str[i]==';')
		{
            std::string aa = str.substr(k,i-k);
            std::string bb = "=  ;";
            std::string t = clean_string_seps(aa,bb);
            vec.push_back(t);
			k=i;
		}
	
	}
	vec.push_back(StrRep(str.substr(k,i-k),"\n",""));
	//clean vector from empty 
    for(i=0; i<(int)vec.size(); i++)
		if( vec[i] != "")
			vec2.push_back(vec[i]);

	return vec2;
}



bool isSymbol(char c){	return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); }
bool isSymbol(std::string s){ for(int i=0; i<(int)s.size(); i++) if(!isSymbol(s[i])) return 0; return 1; }
bool isNumber(char c){	return ((c >= '0') && (c <= '9')); }
bool isSep(char c)
{
    std::string all_seps=" ;\t()[],/{}\"";
    for(int i=0; i<(int)all_seps.size(); i++)
		if(c == all_seps[i]) return true;
	return false;
}
bool isSep(std::string s){ for(int i=0; i<(int)s.size(); i++) if(!isSep(s[i])) return 0; return 1; }

double StrToDouble(std::string s)
{
	double d;
	if( sscanf(s.c_str(),"%lf",&d) == 1 )
		return d;
	else
		return 0;
}

std::string DoubleToStr(double d)
{
	char s[255];
	sprintf(s,"%lf",d);
	return s;
}

std::string DoubleToStr(double d, char* fmt)
{
    char s[255];
    sprintf(s,fmt,d);
    return s;
}



std::string DoubleArrToStr(std::string name, std::vector<double>& parr)
{
    std::string s;
	s+=name + "=[";
    for(int i=0; i<(int)parr.size();i++)
		s+=DoubleToStr(parr[i]) +" ";
	s+="] ";
	return s;
}


std::vector<std::string> StrSplitW(std::string str)
{
	return StrSplitE(str," ", false);
}

std::vector<std::string> StrSplitZ(std::string str)
{
	return StrSplitE(str,",", false);
}


std::vector<std::string> StrSplitE(std::string str, std::string seps, bool withSep)
{
    std::string tmp,sub;
    std::vector<std::string> vec;
    std::vector<std::string> vec2;
    uint k=0;
    uint i=0;
    uint j=0;

	tmp = " ";
    for(i=0; i<str.size(); i++)
	{
		if( isSymbol(str[i]) ) continue;
		if( isNumber(str[i]) ) continue;	
        for(j=0; j<seps.size(); j++)
		{		
			if(str[i]==seps[j])
			{
                //std::string t = clean_std::string_seps(str.substr(k,i-k),seps);
								
				
                sub = str.substr(k,i-k);
                std::string::iterator I = find(sub.begin(),sub.end(),seps[j]);
				if(I!=sub.end()) sub.erase(I);
				
				
				//vec.push_back(t);			
				vec.push_back(sub);

				if(withSep) 
				{
					tmp[0] = seps[j];
					vec.push_back(tmp);
				}			
                k=i+1; //-1
			}
		}

	
	}
	
    std::string t45 = str.substr(k,i-k);
    vec.push_back(clean_string_seps(t45,seps));
	//clean vector from empty 
    for(i=0; i<vec.size(); i++)
		if( vec[i] != "")
			vec2.push_back(vec[i]);

	return vec2;
}


deque<std::string> StrSplitD(std::string str, std::string seps, bool withSep)
{
    std::string tmp;
    deque<std::string> vec;
    deque<std::string> vec2;
	int k=0;
	int i=0; 
	int j=0;

	tmp = " ";
    for(i=0; i<(int)str.size(); i++)
	{
        for(j=0; j<(int)seps.size(); j++)
		{
		
		if(str[i]==seps[j])
		{
            std::string t2 = str.substr(k,i-k);
            std::string t = clean_string_seps(t2,seps);
			vec.push_back(t);			
			if(withSep) 
			{
				tmp[0] = seps[j];
				vec.push_back(tmp);
			}			
			k=i;
		}
		}

	
	}
	
    std::string t2 = str.substr(k,i-k);
    vec.push_back(clean_string_seps(t2,seps));
	//clean deque from empty 
    for(i=0; i<(int)vec.size(); i++)
		if( vec[i] != "")
			vec2.push_back(vec[i]);

	return vec2;
}

int CountSeps(std::string str, std::string sep)
{
	int num = 0;
    for(int i=0; i<(int)str.size(); i++)
	{
		if(str[i] == sep[0])
			num++;	
	}

	return num;

}
