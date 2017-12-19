#include "XercesDOMElementImpl.h"

#include "RoadXML/Encoding.h"
#include "Transcode.h"

#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>

#include <sstream>
#include <limits>

#if  (_MSC_VER <= 1400) && defined(_MSC_VER)
#define strtoull    _strtoui64
#endif

XERCES_CPP_NAMESPACE_USE


XercesDOMElementImpl::XercesDOMElementImpl(XERCES_CPP_NAMESPACE::DOMElement* elem) : mDOMElement(elem)
{

}

XercesDOMElementImpl::~XercesDOMElementImpl()
{
	// mDOMElement should be deleted by the DOM document 
}

bool XercesDOMElementImpl::SetStringAttribute( const std::string& attributeName, const std::string& attributeValue )
{
    RoadXML::XercesString attValue = RoadXML::Transcode(attributeValue);
	mDOMElement->setAttribute(RoadXML::Transcode(attributeName).c_str(), attValue.c_str());
	return true;
}

bool XercesDOMElementImpl::SetDoubleAttribute( const std::string& attributeName, double attributeValue )
{
	std::ostringstream sstr;
	sstr.precision(std::numeric_limits<double>::digits10);
	sstr << attributeValue;
    RoadXML::XercesString attValue = RoadXML::Transcode(sstr.str().c_str());

	mDOMElement->setAttribute(RoadXML::Transcode(attributeName).c_str(), attValue.c_str());

	return true;
}

bool XercesDOMElementImpl::SetBooleanAttribute( const std::string& attributeName, bool attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;
    RoadXML::XercesString attValue = RoadXML::Transcode(sstr.str().c_str());

	mDOMElement->setAttribute(RoadXML::Transcode(attributeName).c_str(), attValue.c_str());

	return true;
}

bool XercesDOMElementImpl::SetIntegerAttribute( const std::string& attributeName, size_t attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;
    RoadXML::XercesString attValue = RoadXML::Transcode(sstr.str().c_str());

	mDOMElement->setAttribute(RoadXML::Transcode(attributeName).c_str(), attValue.c_str());

	return true;
}

bool XercesDOMElementImpl::SetSignedIntegerAttribute( const std::string& attributeName, int attributeValue )
{
	std::ostringstream sstr;
	sstr << attributeValue;
    RoadXML::XercesString attValue = RoadXML::Transcode(sstr.str().c_str());

	mDOMElement->setAttribute(RoadXML::Transcode(attributeName).c_str(), attValue.c_str());

	return true;
}

//bool XercesDOMElementImpl::SetUIntegerAttribute( const std::string& attributeName, uint32 attributeValue )
//{
//	XMLCh * attName = XMLString::transcode(attName.c_str()); 
//	std::ostringstream sstr;
//	sstr << attributeValue;
//	XMLCh * attValue = XMLString::transcode(sstr.str().c_str()); 
//
//	mDOMElement->setAttribute(attributeName.c_str(), attValue.c_str());
//
//	XMLString::release(&attName);
//	XMLString::release(&attValue.c_str());
//	return true;
//}

bool XercesDOMElementImpl::SetTextContent( const std::string& value )
{
    RoadXML::XercesString nodeValue = RoadXML::Transcode(value);
	mDOMElement->setTextContent(nodeValue.c_str());
	return true;
}


bool  XercesDOMElementImpl::AddChild( IDOMElement* child )
{
	if( !child )
		return false;

	mChildren.push_back( child );

	mDOMElement->appendChild((static_cast<XercesDOMElementImpl*>(child))->GetElement());
	return true;
}

const std::string& XercesDOMElementImpl::GetTagName()
{
	mTagName = RoadXML::Transcode(mDOMElement->getTagName());
	return mTagName;
}


//! Returns the first child, or NULL if the child is not found.
CountedPtr<IDOMElement>	XercesDOMElementImpl::GetFirstChildElement(const char* name)
{
	CountedPtr<IDOMElement> result;

	DOMElement * el = NULL;

	if (name == 0)
		el = mDOMElement->getFirstElementChild();
	else
	{
		DOMNodeList * list = mDOMElement->getElementsByTagName(RoadXML::Transcode(name).c_str());
		if (list->getLength() > 0)
			el = dynamic_cast<DOMElement *>(list->item(0));

		delete list;
	}

	if (el)
		result = new XercesDOMElementImpl(el);

	return result;
}

//! Returns the next sibling child, or NULL if the child is not found.
CountedPtr<IDOMElement>	XercesDOMElementImpl::GetNextSiblingChildElement()
{
	CountedPtr<IDOMElement> result;

	DOMElement * el = mDOMElement->getNextElementSibling();
	if (el)
		result = new XercesDOMElementImpl(el);

	return result;
}

size_t XercesDOMElementImpl::GetAttributeCount()
{
	DOMNamedNodeMap* pNameNodeMap = mDOMElement->getAttributes();
	return pNameNodeMap->getLength();
}

bool XercesDOMElementImpl::GetAttributeNameByIndex(size_t index, std::string& allocatedString)
{
	DOMNamedNodeMap* pNameNodeMap = mDOMElement->getAttributes();
	const size_t nodeCount = pNameNodeMap->getLength();
	if( index<nodeCount )
	{
		DOMNode* pNode = pNameNodeMap->item((XMLSize_t)index);

		const short nodeType = pNode->getNodeType();
		if(nodeType == DOMNode::ATTRIBUTE_NODE )
		{
			DOMAttr* pAttr = dynamic_cast<DOMAttr*>(pNode);
			
			char* attName = XMLString::transcode(pAttr->getName());
			if( attName )
			{
				allocatedString = attName;
				XMLString::release(&attName);
				return true;
			}
		}
	}
	return false;
}

bool XercesDOMElementImpl::GetStringAttribute( const std::string& attributeName, std::string& result)
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());

	if (ret)
		result = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));

	return ret;
}

bool XercesDOMElementImpl::GetDoubleAttribute( const std::string& attributeName, double& attributeValue )
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());
	if (ret)
	{
		std::string res = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));
		attributeValue = strtod(res.c_str(), NULL);
	}
	return ret;
}

bool XercesDOMElementImpl::GetBooleanAttribute( const std::string& attributeName, bool& attributeValue )
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());
	if (ret)
	{
		std::string res = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));
		attributeValue = (strtoul(res.c_str(), NULL, 0) > 0);
	}
	return ret;
}

bool XercesDOMElementImpl::GetIntegerAttribute( const std::string& attributeName, size_t& attributeValue )
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());
	if (ret)
	{
		std::string res = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));
		attributeValue = (size_t)strtoull(res.c_str(), NULL, 0);
	}
	return ret;
}

bool XercesDOMElementImpl::GetUIntegerAttribute( const std::string& attributeName, unsigned int& attributeValue )
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());
	if (ret)
	{
		std::string res = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));
		attributeValue = strtoul(res.c_str(), NULL, 0);
	}
	return ret;
}

bool XercesDOMElementImpl::GetSignedIntegerAttribute( const std::string& attributeName, int& attributeValue )
{
    RoadXML::XercesString attName = RoadXML::Transcode(attributeName);
	bool ret = mDOMElement->hasAttribute(attName.c_str());
	if (ret)
	{
		std::string res = RoadXML::Transcode(mDOMElement->getAttribute(attName.c_str()));
		attributeValue = atoi(res.c_str());
	}
	return ret;
}
//bool XercesDOMElementImpl::GetUIntegerAttribute( const std::string& attributeName, uint32& attributeValue )
//{
//	XMLCh * attName = XMLString::transcode(attName.c_str()); 
//	bool ret = mDOMElement->hasAttribute(attName.c_str());
//	if (ret)
//	{
//		char * res = XMLString::transcode(mDOMElement->getAttribute(attName.c_str()));
//		std::istringstream tmpstr(res);
//		tmpstr >> attributeValue;
//		XMLString::release(&res);
//	}
//	XMLString::release(&attName);
//	return ret;
//}
bool XercesDOMElementImpl::GetTextContent( std::string& value )
{
	if( mDOMElement->hasChildNodes() )
		value = RoadXML::Transcode(mDOMElement->getFirstChild()->getNodeValue());
	return true;
}
