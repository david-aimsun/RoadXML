#pragma once

#include "RoadXML/IDOMParser.h"
#include "RoadXML/CountedObject.h"
#include "stk/StringTools.h"

#include "rapidxml.hpp"

#include <map>

class RapidXMLDOMElement;

//! XERCES DOM implementatoin of IDOMParser methods
/*! This class is used to load or save data into XML file format
*/
class RapidXMLDOMParserImpl : public IDOMParser
{
public:
	RapidXMLDOMParserImpl();
	virtual ~RapidXMLDOMParserImpl();

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


	virtual bool				SetCDATAContent(IDOMElement* element, const std::string& value);

private:
	void Clear();
private:
	rapidxml::xml_document<>*	mDoc;

	// Version ID and name are stored here before being saved or asked
	size_t			mVersionId;
	//The root element
	CountedPtr<IDOMElement> mRootElement;

	// The current file being loaded or saved
	std::string mFileName;
};
