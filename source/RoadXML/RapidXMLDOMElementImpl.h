#pragma once

#include "RoadXML/IDOMElement.h"
#include "RoadXML/CountedObject.h"

#include "rapidxml.hpp"

#include <vector>

//! XERCES DOM implementation of IDOMElement.
/*! This class gives access to the Element attributes and children.
*/
class RapidXMLDOMElementImpl : public IDOMElement
{
public:
	RapidXMLDOMElementImpl(rapidxml::xml_node<> * elem, rapidxml::xml_document<>* doc);
	virtual ~RapidXMLDOMElementImpl();

	// IDOMElement methods
	virtual bool				 AddChild( IDOMElement* child );

	//! Returns the first child, or NULL if the child is not found.
	virtual CountedPtr<IDOMElement>	GetFirstChildElement(const char* name = 0);

	//! Returns the next sibling child, or NULL if the child is not found.
	virtual CountedPtr<IDOMElement>	GetNextSiblingChildElement();

	// Return the number of attributes
	virtual size_t					 GetAttributeCount();
	// Return false if the node is not a DOMAttribute
	virtual bool				 GetAttributeNameByIndex(size_t index, std::string& allocatedString);


	virtual const std::string&	 GetTagName();

	virtual bool				 SetStringAttribute( const std::string& attributeName, const std::string& attributeValue );
	virtual bool				 SetDoubleAttribute( const std::string& attributeName, double attributeValue );
	virtual bool				 SetBooleanAttribute( const std::string& attributeName, bool attributeValue );
	virtual bool				 SetIntegerAttribute( const std::string& attributeName, size_t attributeValue );
	virtual bool				 SetSignedIntegerAttribute( const std::string& attributeName, int attributeValue );
	virtual bool				 SetTextContent( const std::string& value );

	virtual bool				 GetStringAttribute( const std::string& attributeName, std::string& result);
	virtual bool				 GetDoubleAttribute( const std::string& attributeName, double& attributeValue );
	virtual bool				 GetBooleanAttribute( const std::string& attributeName, bool& attributeValue );
	virtual bool				 GetIntegerAttribute( const std::string& attributeName, size_t& attributeValue );
	virtual bool				 GetSignedIntegerAttribute( const std::string& attributeName, int& attributeValue );
	virtual bool				 GetUIntegerAttribute( const std::string& attributeName, unsigned int& attributeValue );
	virtual bool				 GetTextContent( std::string& value );

	rapidxml::xml_node<>*		GetElement(){ return mDOMElement; }
protected:

	rapidxml::xml_node<>* mDOMElement;
	rapidxml::xml_document<>* mDoc;

	std::vector<CountedPtr<IDOMElement> > mChildren; // Need to store somewhere the list of counted pointers to keep track of ownership
private:
	std::string mTagName;
};

