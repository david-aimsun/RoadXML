#pragma once

#include "stk/Macro.h"
#include "CountedObject.h"
#include "CountedPtr.h"
#include <string>

/*! IDOMElement is the abstract representation of an XML element.
/*! The name, attributes and children of an XML element are accessible through this interface.
*/
class IDOMElement : public CountedObject
{
public:
	//! Returns the name of the XML element.
	virtual const std::string&	GetTagName() = 0;

	//! Returns the first child, or NULL if the child is not found.
	virtual CountedPtr<IDOMElement>	GetFirstChildElement(const char* name = 0) = 0;

	//! Returns the next sibling child, or NULL if the child is not found.
	virtual CountedPtr<IDOMElement>	GetNextSiblingChildElement() = 0;

	//! Add a new child to the XML element.
	virtual bool				AddChild( IDOMElement* child ) = 0;

	//! Returns the number of attributes.
	virtual size_t					GetAttributeCount() = 0;

	//! Returns a specific attribute name, or false if the attribute is not found.
	virtual bool				GetAttributeNameByIndex(size_t index, std::string& allocatedString) = 0;

	//! Set element's attributes
	virtual bool				SetStringAttribute( const std::string& attributeName, const std::string& attributeValue ) = 0;
	virtual bool				SetDoubleAttribute( const std::string& attributeName, double attributeValue ) = 0;
	virtual bool				SetBooleanAttribute( const std::string& attributeName, bool attributeValue ) = 0;
	virtual bool				SetIntegerAttribute( const std::string& attributeName, size_t attributeValue ) = 0;
	virtual bool				SetSignedIntegerAttribute( const std::string& attributeName, int attributeValue ) = 0;
//	virtual bool				SetUIntegerAttribute( const std::string& attributeName, uint32 attributeValue ) = 0;
	virtual bool				SetTextContent( const std::string& textContent ) = 0;

	//! Access element's attributes
	virtual bool				GetStringAttribute( const std::string& attributeName, std::string& attributeValue ) = 0;
	virtual bool				GetDoubleAttribute( const std::string& attributeName, double& attributeValue ) = 0;
	virtual bool				GetBooleanAttribute( const std::string& attributeName, bool& attributeValue ) = 0;
	virtual bool				GetIntegerAttribute( const std::string& attributeName, size_t& attributeValue ) = 0;
	virtual bool				GetSignedIntegerAttribute( const std::string& attributeName, int& attributeValue ) = 0;
	virtual bool				GetUIntegerAttribute( const std::string& attributeName, unsigned int& attributeValue ) = 0;
//	virtual bool				GetUIntegerAttribute( const std::string& attributeName, uint32& attributeValue ) = 0;
	virtual bool				GetTextContent( std::string& textContent ) = 0;

protected:
	virtual ~IDOMElement() {}
};
