#pragma once
#include "RoadXML/RoadXML.h"

#include <string>

#include "stk/Macro.h"
#include "CountedObject.h"
#include "CountedPtr.h"

class IDOMElement;

//! Abstract interface on a XML DOM document.
/*! Contains the root element of an XML document and the methods to load an existing file and acces its content, 
	or to create a new document and	save it into a file.
*/
class ROADXML_API IDOMParser : public CountedObject
{
public:
	virtual void				SetFileName(const char* name) = 0;
	virtual const char*			GetFileName() const = 0;

	// Load and retreive data

	//! Retreive the whole file content. 
	virtual IDOMElement* GetRootElement() = 0;
	virtual bool Load() = 0;


	// Create and save data

	//! Create a new DOM document. 
	/*! Returns a pointer on the root element of the document.
		If a document was previously loaded, the loaded data will be lost.
	*/
	virtual IDOMElement* 		 CreateDOMDocument(const char* rootElementName) = 0;

	//! Create a new DOM element inside the currently opened DOM document. 
	virtual IDOMElement*		CreateDOMElement(const std::string& elementName) = 0;

	//! Save the DOM element array into the currently opened DOM document file.
	virtual bool				Add(IDOMElement* element) = 0;
	virtual bool				Save() = 0;

	//! Version of the document
	/*! This version number is different from the XML version saved in the file (1.0, UTF-8 for example).
		It's a private version id.
	*/
	virtual void				SetVersionId(size_t ID) = 0;
	virtual size_t				GetVersionId() const = 0;
	
	//! Set CDATA content to an element
	/*! Replaces all children the element may have had with a CDATA text element (it is interpreted purely as textual data )
	*/
	virtual bool				SetCDATAContent(IDOMElement* element, const std::string& elementName) = 0;

protected:
	virtual ~IDOMParser() {}
};

//! Parser's creator
/*! Everything is then handled through the interfaces IDOMParser and IDOMElement.
	"fileName" can be NULL.
*/
ROADXML_API IDOMParser* NewDOMParser(const char* fileName);
ROADXML_API IDOMParser* NewDOMParserRapid(const char* fileName);


