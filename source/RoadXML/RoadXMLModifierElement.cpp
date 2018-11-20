#include "RoadXML/RoadXMLModifierElement.h"

// =========================================================================================================
RoadXML::BorderOffsetElement::BorderOffsetElement() :
mT(0.0),
mYOffset(0.0),
mDirectionIn(0.0),
mDirectionOut(0.0)
{}

/////////////////////////////////////////////////////////////////////////////////
RoadXML::BorderOffsetElement::BorderOffsetElement(double t, double yOffset, double directionIn, double directionOut) :
mT(t),
mYOffset(yOffset),
mDirectionIn(directionIn),
mDirectionOut(directionOut)
{}

/////////////////////////////////////////////////////////////////////////////////
RoadXML::BorderOffsetElement::~BorderOffsetElement()
{}

/////////////////////////////////////////////////////////////////////////////////
const std::string&	RoadXML::BorderOffsetElement::GetTagName() const
{
	return kBorderOffsetTag;
}

/////////////////////////////////////////////////////////////////////////////////
bool RoadXML::BorderOffsetElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetDoubleAttribute(kTTag, mT);
	iDOMElem->GetDoubleAttribute(kYOffsetTag, mYOffset);
	iDOMElem->GetDoubleAttribute(kDirInTag, mDirectionIn);
	iDOMElem->GetDoubleAttribute(kDirOutTag, mDirectionOut);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
IDOMElement*		RoadXML::BorderOffsetElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kTTag, mT);
	elementOut->SetDoubleAttribute(kYOffsetTag, mYOffset);
	elementOut->SetDoubleAttribute(kDirInTag, mDirectionIn);
	elementOut->SetDoubleAttribute(kDirOutTag, mDirectionOut);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////	
double RoadXML::BorderOffsetElement::GetT()const
{
	return mT;
}

/////////////////////////////////////////////////////////////////////////////////
void RoadXML::BorderOffsetElement::SetT(double paramT)
{
	mT = paramT;
}

/////////////////////////////////////////////////////////////////////////////////
double RoadXML::BorderOffsetElement::GetYOffset()const
{
	return mYOffset;
}

/////////////////////////////////////////////////////////////////////////////////
void RoadXML::BorderOffsetElement::SetYOffset(double yOffset)
{
	mYOffset = yOffset;
}

/////////////////////////////////////////////////////////////////////////////////
double RoadXML::BorderOffsetElement::GetDirIn()const
{
	return mDirectionIn;
}

/////////////////////////////////////////////////////////////////////////////////
void RoadXML::BorderOffsetElement::SetDirIn(double dirIn)
{
	mDirectionIn = dirIn;
}

/////////////////////////////////////////////////////////////////////////////////		
double RoadXML::BorderOffsetElement::GetDirOut()const
{
	return mDirectionOut;
}
/////////////////////////////////////////////////////////////////////////////////
void RoadXML::BorderOffsetElement::SetDirOut(double dirOut)
{
	mDirectionOut = dirOut;
}

// =========================================================================================================
RoadXML::BorderModifierElement::BorderModifierElement() :
mId(0)
{}

/////////////////////////////////////////////////////////////////////////////////
RoadXML::BorderModifierElement::~BorderModifierElement()
{}

/////////////////////////////////////////////////////////////////////////////////			
const std::string& RoadXML::BorderModifierElement::GetTagName() const
{
	return kBorderModifierTag;
}

/////////////////////////////////////////////////////////////////////////////////
bool RoadXML::BorderModifierElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetIntegerAttribute(kIdTag, mId);
	LoadChildren(iDOMElem, parser);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
IDOMElement* RoadXML::BorderModifierElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetIntegerAttribute(kIdTag, mId);

	BuildChildrenArray(mBorderOffsets, "", elementOut, parser);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////
bool RoadXML::BorderModifierElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement &&
		newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == kBorderOffsetTag)
		{
			mBorderOffsets.push_back(static_cast<BorderOffsetElement*>(newElement.Get()));
		}
	}
	else
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////	
size_t RoadXML::BorderModifierElement::GetId()const
{
	return mId;
}

/////////////////////////////////////////////////////////////////////////////////
void RoadXML::BorderModifierElement::SetId(size_t id)
{
	mId = id;
}

// =========================================================================================================

RoadXML::ProfileModifierElement::ProfileModifierElement()
{}

RoadXML::ProfileModifierElement::~ProfileModifierElement()
{}

/////////////////////////////////////////////////////////////////////////////////			
const std::string&	RoadXML::ProfileModifierElement::GetTagName() const
{
	return kProfileModifierTag;
}

/////////////////////////////////////////////////////////////////////////////////
bool RoadXML::ProfileModifierElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	LoadChildren(iDOMElem, parser);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
bool RoadXML::ProfileModifierElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement &&
		newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == kBorderModifierTag)
		{
			mBorderModifiers.push_back(static_cast<BorderModifierElement*>(newElement.Get()));
		}
	}
	else
		return false;


	return true;
}

/////////////////////////////////////////////////////////////////////////////////
IDOMElement* RoadXML::ProfileModifierElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
	BuildChildrenArray(mBorderModifiers, "", elementOut, parser);

	return elementOut;
}