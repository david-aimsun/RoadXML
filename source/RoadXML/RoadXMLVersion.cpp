
#include "RoadXML/RoadXMLVersion.h"
#include <sstream>
#include <stdlib.h>

unsigned int MakeVersionID(int Major, int Minor, int Bugfix)
{
	return (Major*0x10000 + Minor*0x100 + Bugfix*0x1);
}

std::string VersionIDToString(unsigned int VersionID)
{
	std::ostringstream out;
	unsigned int rest = VersionID;
	unsigned int MajorPart = (rest/0x10000);
	out << MajorPart;
	rest -= MajorPart*0x10000;
	unsigned int MinorPart = (rest/0x100);
	out << "." << MinorPart;
	rest -= MinorPart*0x100;
	unsigned int BugfixPart = rest;
	out << "." << BugfixPart;
	return out.str();
}

unsigned int StringToVersionID(const char* VersionString)
{
	std::istringstream in(VersionString);

	std::string tmp;
	getline(in, tmp, '.');
	int Major = atol(tmp.c_str());
	getline(in, tmp, '.');
	int Minor = atol(tmp.c_str());
	getline(in, tmp);
	int Bugfix = atol(tmp.c_str());

	return MakeVersionID(Major, Minor, Bugfix);
}
