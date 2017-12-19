
#include "DOMTreeErrorReporter.h"
#include <xercesc/sax/SAXParseException.hpp> 

#ifndef USES_QT_DEBUG
#define qDebug printf 
#define qWarning printf 
#define qFatal printf 
#else
#include <qglobal.h>
#endif

#include <fstream>

XERCES_CPP_NAMESPACE_USE

//////////////////
// DOMTreeErrorReporter implementation
//////////////////
const char* getErrorLineString(const SAXParseException& toCatch)
{
	static char buf[10000];
	XMLFileLoc line   = toCatch.getLineNumber();
	XMLFileLoc column = toCatch.getColumnNumber();

    char* szSystemId = XMLString::transcode(toCatch.getSystemId());
    std::ifstream ifs(szSystemId);
    XMLString::release(&szSystemId);

	for (XMLFileLoc i = 0; i < line; i++)
	{
		ifs.getline(buf, 10000);
	}
	
	static std::string s = buf;
	s += "\n";
	if (column>0)
	{
		for(XMLFileLoc j = 0; j <column - 1; j++)
			s += " ";
	}
	s += "^";
	return s.c_str();
}

DOMTreeErrorReporter::DOMTreeErrorReporter() : fSawErrors(false)
{}

DOMTreeErrorReporter::~DOMTreeErrorReporter()
{}

void DOMTreeErrorReporter::resetErrors()
{}

bool DOMTreeErrorReporter::getSawErrors() const
{
    return fSawErrors;
}

void DOMTreeErrorReporter::warning(const SAXParseException& toCatch)
{
	char* szSystemId = XMLString::transcode(toCatch.getSystemId());
	char* szMessage = XMLString::transcode(toCatch.getMessage());

	qFatal("Warning at file %s, line %d, char %d\nMessage %s\n%s",
		   szSystemId,
		   (int) toCatch.getLineNumber(),
		   (int) toCatch.getColumnNumber(),
		   szMessage,
		   getErrorLineString(toCatch)
		   );

	XMLString::release(&szSystemId);
	XMLString::release(&szMessage);
}
	
void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
    fSawErrors = true;
	char* szSystemId = XMLString::transcode(toCatch.getSystemId());
	char* szMessage = XMLString::transcode(toCatch.getMessage());

	qFatal("Error at file %s, line %d, char %d\nMessage %s\n%s",
			szSystemId,
			(int) toCatch.getLineNumber(),
			(int) toCatch.getColumnNumber(),
			szMessage,
			getErrorLineString(toCatch)
		   );

	XMLString::release(&szSystemId);
	XMLString::release(&szMessage);
}
	
void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
	fSawErrors = true;
	char* szSystemId = XMLString::transcode(toCatch.getSystemId());
	char* szMessage = XMLString::transcode(toCatch.getMessage());

	qWarning("Fatal Error at file %s, line %d, char %d\nMessage %s\n%s",
			szSystemId,
			(int) toCatch.getLineNumber(),
			(int) toCatch.getColumnNumber(),
			szMessage,
			getErrorLineString(toCatch)
		   );

	XMLString::release(&szSystemId);
	XMLString::release(&szMessage);
}
