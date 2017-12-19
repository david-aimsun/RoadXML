#pragma once

#include "RoadXML/IDOMParser.h"
#include "RoadXML/CountedObject.h"
#include "stk/StringTools.h"

#include "DOMTreeErrorReporter.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>

#include <map>

class XercesDOMElement;

//! XERCES DOM implementatoin of IDOMParser methods
/*! This class is used to load or save data into XML file format
*/
class XercesDOMParserImpl : public IDOMParser
{
public:
	XercesDOMParserImpl();
	virtual ~XercesDOMParserImpl();

	// IDOMParser methods
	virtual IDOMElement*		GetRootElement();
	virtual bool				 Load();

	virtual IDOMElement* 		 CreateDOMDocument(const char* rootElementName);
	virtual IDOMElement*		 CreateDOMElement(const std::string& elementName);
	virtual bool				 Add(IDOMElement* element);
	virtual bool				 Save();

	virtual void				 SetVersionId(size_t ID) {mVersionId = ID;}
	virtual size_t				 GetVersionId() const {return mVersionId;}

	virtual void				 SetFileName(const char* name) { mFileName = name; }
	virtual const char*			 GetFileName() const { return mFileName.c_str(); }

	virtual bool		SetCDATAContent(IDOMElement* element, const std::string& elementName);
private:
	void Clear();
private:
	XERCES_CPP_NAMESPACE::DOMDocument*	mDoc;

	// Version ID and name are stored here before being saved or asked
	size_t			mVersionId;
	//The root element
	CountedPtr<IDOMElement> mRootElement;

	// The current file being loaded or saved
	std::string mFileName;


	XERCES_CPP_NAMESPACE::XercesDOMParser* ourParser;
	DOMTreeErrorReporter ourErrorHandler;
};
