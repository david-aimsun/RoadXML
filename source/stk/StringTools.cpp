#include "stk/StringTools.h"

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

std::string stk::ToLower(const std::string& str)
{
	std::string result;
	for(size_t i=0; i<str.size();++i)
		result.push_back( tolower(str[i]) );

	return result;
}

std::string stk::ToUpper(const std::string& str)
{
	std::string result;
	for(size_t i=0; i<str.size();++i)
		result.push_back( toupper(str[i]) );

	return result;
}

std::string stk::WCharToSTDString(const wchar_t* orig)
{
	std::string tmp(orig, orig+wcslen(orig));
	return tmp;
}

std::wstring stk::CharToSTDWString(const char* orig)
{
	std::wstring tmp(orig, orig+strlen(orig));
	return tmp;
}

double stk::StringToDouble( const std::string& str)
{
	return ::atof(str.c_str());
}

std::string stk::DoubleToString( double value )
{
	std::stringstream result;
	result << value;
	return result.str();
}

std::vector<std::string> stk::SplitString(const std::string &str, char delim) 
{
	std::vector<std::string> elems;

    std::stringstream sstr(str);
    std::string item;
    while(std::getline(sstr, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
