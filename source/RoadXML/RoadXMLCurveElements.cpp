#include "RoadXML/RoadXMLCurveElements.h"


bool RoadXML::XYCurveElement::LoadFromXMLElement(IDOMElement *elem, IDOMParser *parser)
{
	mFunctions.clear();

	elem->GetDoubleAttribute( kDirectionTag, mDirection );
	elem->GetDoubleAttribute( kXTag, mXValue );
	elem->GetDoubleAttribute( kYTag, mYValue );

	LoadChildren( elem, parser );

	return true;
}

bool RoadXML::XYCurveElement::LoadChild(IDOMElement *childElement, IDOMParser *parser)
{
	const std::string& tagName = childElement->GetTagName();
	CountedPtr<Element> newElement = CreateElement(tagName);

	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		mFunctions.push_back( newElement );
	}
	else 
		return false;

	return true;
}

IDOMElement* RoadXML::XYCurveElement::BuildXMLElement(IDOMParser * parser)
{
	IDOMElement* XYCurveElement = parser->CreateDOMElement(GetTagName());

	XYCurveElement->SetDoubleAttribute(kDirectionTag, mDirection);
	XYCurveElement->SetDoubleAttribute(kXTag, mXValue);
	XYCurveElement->SetDoubleAttribute(kYTag, mYValue);

	BuildChildrenArray( mFunctions, "", XYCurveElement, parser);

	return XYCurveElement;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::CartesianCurveElement::LoadFromXMLElement(IDOMElement *elem, IDOMParser *parser)
{
	mFunctions.clear();

	LoadChildren( elem, parser );

	return true;
}

bool RoadXML::CartesianCurveElement::LoadChild(IDOMElement *childElement, IDOMParser *parser)
{
	const std::string& tagName = childElement->GetTagName();
	CountedPtr<Element> newElement = CreateElement(tagName);

	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if(tagName == kPolynomialTag)
		{
			mFunctions.push_back( dynamic_cast<PolynomialFunctionElement*>(newElement.Get()) );
		}
	}
	else 
		return false;

	return true;
}

IDOMElement* RoadXML::CartesianCurveElement::BuildXMLElement(IDOMParser * parser)
{
	IDOMElement* szCurveElement = parser->CreateDOMElement(GetTagName());

	BuildChildrenArray( mFunctions, "", szCurveElement, parser);

	return szCurveElement;
}