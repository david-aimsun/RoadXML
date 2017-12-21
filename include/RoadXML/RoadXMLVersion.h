#pragma once
#include "RoadXML/RoadXML.h"

#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//! Version number	
/*! The RND version number is defined by 3 numbers. It can be for example "1.0.2". These numbers mean:
	-"1": major release number.
	-".5": minor release number, increased when the interfaces were changed for a bug fix.
	-".2": dev release number, increased during the development of a new version.

	- gRNDVersionString is a string for the user information. It contains data such as
		"Release", "Beta" or "Alpha". Or the SVN revision number.
*/

#define RoadXML_VERSION_MAJOR 2
#define RoadXML_VERSION_MINOR 4
#define RoadXML_VERSION_DEV 3

#define STR(S) #S
#define XSTR(S) STR(S)

#define RoadXML_VERSION_STRING XSTR(RoadXML_VERSION_MAJOR) "." XSTR(RoadXML_VERSION_MINOR) "." XSTR(RoadXML_VERSION_DEV)

// Text tag displaying the SVN revision number: $Revision: 23033 $ and $Date: 2011-05-02 11:01:27 +0200 (lun., 02 mai 2011) $
// The data between the $$ is automatically replaced by the SVN information when a commit is done
#define RoadXML_VERSION_DETAILS RoadXML_VERSION_STRING "-$Revision: 23033 $-$Date: 2011-05-02 11:01:27 +0200 (lun., 02 mai 2011) $"

ROADXML_API unsigned int MakeVersionID(int Major, int Minor, int Dev);
ROADXML_API std::string VersionIDToString(unsigned int VersionID);
ROADXML_API unsigned int StringToVersionID(const char* VersionString);


