#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLTrajectoryPartElements.h"

#include "RoadXML/RoadXMLTags.h"


//-----------------------------------------------------------------------------
RoadXML::TrajectoryPartElement::TrajectoryPartElement()
{
}

//-----------------------------------------------------------------------------
RoadXML::TrajectoryPartElement::~TrajectoryPartElement()
{
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPartElement::GetName() const
{
	return myName;
}

//-----------------------------------------------------------------------------
void RoadXML::TrajectoryPartElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = myName;
	myName=name; 
	if(parent) 
	{
		parent->myTrajectoryPartArray.updateKey( this, oldname, name );
	} 
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPartElement::GetTagName() const
{
	return kTrajectoryPartTag;
}

//-----------------------------------------------------------------------------
bool RoadXML::TrajectoryPartElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetStringAttribute(kNameTag, myName);

	LoadChildren(iDOMElem, parser);
	
	return true;
}

//-----------------------------------------------------------------------------
IDOMElement* RoadXML::TrajectoryPartElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, myName);

	if( myXYCurve )
		elementOut->AddChild( myXYCurve->BuildXMLElement(parser) );

	if( mySZCurve )
		elementOut->AddChild( mySZCurve->BuildXMLElement(parser) );

	return elementOut;
}

//-----------------------------------------------------------------------------
bool RoadXML::TrajectoryPartElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement && newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == kXYCurveTag)
		{
			myXYCurve = dynamic_cast<XYCurveElement*>(newElement.Get());
		}
		else if (tagName == kSZCurveTag)
		{
			mySZCurve = dynamic_cast<CartesianCurveElement*>(newElement.Get());
		}
	}
	else 
	{
		return false;
	}

	return true;
}