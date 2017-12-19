#include "RapidXMLDOMElementImpl.h"

#include "RoadXML/Encoding.h"
#include "Transcode.h"

#include <sstream>
#include <limits>

#if  (_MSC_VER <= 1400) && defined(_MSC_VER)
#define strtoull    _strtoui64
#endif

#ifndef WIN32
#define _snprintf snprintf
#endif

RapidXMLDOMElementImpl::RapidXMLDOMElementImpl(rapidxml::xml_node<>* elem, rapidxml::xml_document<>* doc) : mDOMElement(elem), mDoc(doc)
{

}

RapidXMLDOMElementImpl::~RapidXMLDOMElementImpl()
{
	// mDOMElement should be deleted by the DOM document 
}

bool RapidXMLDOMElementImpl::SetStringAttribute( const std::string& attributeName, const std::string& attributeValue )
{
	// Not need because attributeName are constant strings
	//char *attr_name = mDoc->allocate_string(attributeName.c_str());
	const char *attr_name = attributeName.c_str();
	char *attr_value = mDoc->allocate_string(attributeValue.c_str());

	rapidxml::xml_attribute<> *attr = mDoc->allocate_attribute(attr_name, attr_value);
	mDOMElement->append_attribute(attr);
	return true;
}

bool RapidXMLDOMElementImpl::SetDoubleAttribute( const std::string& attributeName, double attributeValue )
{
	char buf[80];
	_snprintf(buf, 79, "%.17g", attributeValue);
	buf[79] = 0;

	// Not need because attributeName are constant strings
	//char *attr_name = mDoc->allocate_string(attributeName.c_str());
	const char *attr_name = attributeName.c_str();
	char *attr_value = mDoc->allocate_string(buf);

	rapidxml::xml_attribute<> *attr = mDoc->allocate_attribute(attr_name, attr_value);
	mDOMElement->append_attribute(attr);
	return true;
}

bool RapidXMLDOMElementImpl::SetBooleanAttribute( const std::string& attributeName, bool attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;

	// Not need because attributeName are constant strings
	//char *attr_name = mDoc->allocate_string(attributeName.c_str());
	const char *attr_name = attributeName.c_str();
	char *attr_value = mDoc->allocate_string(sstr.str().c_str());

	rapidxml::xml_attribute<> *attr = mDoc->allocate_attribute(attr_name, attr_value);
	mDOMElement->append_attribute(attr);
	return true;
}

bool RapidXMLDOMElementImpl::SetIntegerAttribute( const std::string& attributeName, size_t attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;

	// Not need because attributeName are constant strings
	//char *attr_name = mDoc->allocate_string(attributeName.c_str());
	const char *attr_name = attributeName.c_str();
	char *attr_value = mDoc->allocate_string(sstr.str().c_str());

	rapidxml::xml_attribute<> *attr = mDoc->allocate_attribute(attr_name, attr_value);
	mDOMElement->append_attribute(attr);
	return true;
}

bool RapidXMLDOMElementImpl::SetSignedIntegerAttribute( const std::string& attributeName, int attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;

	// Not need because attributeName are constant strings
	//char *attr_name = mDoc->allocate_string(attributeName.c_str());
	const char *attr_name = attributeName.c_str();
	char *attr_value = mDoc->allocate_string(sstr.str().c_str());

	rapidxml::xml_attribute<> *attr = mDoc->allocate_attribute(attr_name, attr_value);
	mDOMElement->append_attribute(attr);
	return true;
}

bool RapidXMLDOMElementImpl::SetTextContent( const std::string& value )
{
	char *attr_value = mDoc->allocate_string(value.c_str());
	mDOMElement->value(attr_value, value.size());
	return true;
}


bool  RapidXMLDOMElementImpl::AddChild( IDOMElement* child )
{
	if( !child )
		return false;

	mChildren.push_back( child );
	mDOMElement->append_node((static_cast<RapidXMLDOMElementImpl*>(child))->GetElement());

	return true;
}

const std::string& RapidXMLDOMElementImpl::GetTagName()
{
	mTagName = mDOMElement->name();
	return mTagName;
}


//! Returns the first child, or NULL if the child is not found.
CountedPtr<IDOMElement>	RapidXMLDOMElementImpl::GetFirstChildElement(const char* name)
{
	CountedPtr<IDOMElement> result;

	rapidxml::xml_node<> * el = mDOMElement->first_node(name);
	if (el)
		result = new RapidXMLDOMElementImpl(el, mDoc);

	return result;
}

//! Returns the next sibling child, or NULL if the child is not found.
CountedPtr<IDOMElement>	RapidXMLDOMElementImpl::GetNextSiblingChildElement()
{
	CountedPtr<IDOMElement> result;

	rapidxml::xml_node<> * el = mDOMElement->next_sibling();
	if (el)
		result = new RapidXMLDOMElementImpl(el, mDoc);

	return result;
}

size_t RapidXMLDOMElementImpl::GetAttributeCount()
{
	int count = 0;
	for (rapidxml::xml_attribute<> *attr = mDOMElement->first_attribute(); attr; attr = attr->next_attribute())
		count++;
	return count;
}

bool RapidXMLDOMElementImpl::GetAttributeNameByIndex(size_t index, std::string& allocatedString)
{
	int count = 0;
	for (rapidxml::xml_attribute<> *attr = mDOMElement->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (count == index)
		{
			allocatedString = attr->name();
			return true;
		}

		count++;
	}

	return false;
}

bool RapidXMLDOMElementImpl::GetStringAttribute( const std::string& attributeName, std::string& result)
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		result = attr->value();
	
	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetDoubleAttribute( const std::string& attributeName, double& attributeValue )
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		attributeValue = strtod(attr->value(), NULL);

	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetBooleanAttribute( const std::string& attributeName, bool& attributeValue )
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		attributeValue = (strtoul(attr->value(), NULL, 0) != 0);

	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetIntegerAttribute( const std::string& attributeName, size_t& attributeValue )
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		attributeValue = strtoul(attr->value(), NULL, 0);

	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetUIntegerAttribute( const std::string& attributeName, unsigned int& attributeValue )
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		attributeValue = strtoul(attr->value(), NULL, 0);

	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetSignedIntegerAttribute( const std::string& attributeName, int& attributeValue )
{
	rapidxml::xml_attribute<> * attr = mDOMElement->first_attribute(attributeName.c_str());

	if (attr)
		attributeValue = atoi(attr->value());

	return (attr != NULL);
}

bool RapidXMLDOMElementImpl::GetTextContent( std::string& value )
{
	value = mDOMElement->value();
	return true;
}
