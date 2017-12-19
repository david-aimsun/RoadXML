#pragma once

#include "RoadXML/IDOMElement.h"

#include "RoadXML/CountedObject.h"

#include <xercesc/dom/DOMElement.hpp>

#include <vector>

//! XERCES DOM implementation of IDOMElement.
/*! This class gives access to the Element attributes and children.
*/
class XercesDOMElementImpl : public IDOMElement
{
public:
	// The method used by the object factory
	//static void			CreateObject(CountedObject **object);

	//static void CreateXercesDOMElement(XercesDOMElementImpl** newElem, XERCES_CPP_NAMESPACE::DOMElement* elem);
	XercesDOMElementImpl(XERCES_CPP_NAMESPACE::DOMElement* elem);
	virtual ~XercesDOMElementImpl();

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
//	virtual bool				 SetUIntegerAttribute( const std::string& attributeName, uint32 attributeValue );
	virtual bool				 SetTextContent( const std::string& value );

	virtual bool				 GetStringAttribute( const std::string& attributeName, std::string& result);
	virtual bool				 GetDoubleAttribute( const std::string& attributeName, double& attributeValue );
	virtual bool				 GetBooleanAttribute( const std::string& attributeName, bool& attributeValue );
	virtual bool				 GetIntegerAttribute( const std::string& attributeName, size_t& attributeValue );
	virtual bool				 GetSignedIntegerAttribute( const std::string& attributeName, int& attributeValue );
	virtual bool				 GetUIntegerAttribute( const std::string& attributeName, unsigned int& attributeValue );
//	virtual bool				 GetUIntegerAttribute( const std::string& attributeName, uint32& attributeValue );
	virtual bool				 GetTextContent( std::string& value );

	XERCES_CPP_NAMESPACE::DOMElement*		GetElement(){return mDOMElement;}
	//void				SetElement(XERCES_CPP_NAMESPACE::DOMElement* elem){mDOMElement = elem;}
protected:

	XERCES_CPP_NAMESPACE::DOMElement* mDOMElement;

	std::vector<CountedPtr<IDOMElement> > mChildren; // Need to store somewhere the list of counted pointers to keep track of ownership
private:
	std::string mTagName;
};

