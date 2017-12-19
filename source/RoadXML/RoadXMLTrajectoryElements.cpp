#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLTrajectoryElements.h"
#include "RoadXML/RoadXMLTrajectoryPartElements.h"


//--------------------------TrajectoryPieceElement-----------------------------
//-----------------------------------------------------------------------------
RoadXML::TrajectoryPieceElement::TrajectoryPieceElement()
{
}

//-----------------------------------------------------------------------------
RoadXML::TrajectoryPieceElement::~TrajectoryPieceElement()
{
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPieceElement::GetName() const
{
	return myName;
}

//-----------------------------------------------------------------------------
void RoadXML::TrajectoryPieceElement::SetName(const std::string& name)
{
	myName = name;
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPieceElement::GetTagName() const
{
	return kTrajectoryElementTag;
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPieceElement::GetTrajectoryPartName() const
{
	return myTrajectoryPartName;
}

//-----------------------------------------------------------------------------
void RoadXML::TrajectoryPieceElement::SetTrajectoryPartName(const std::string& trajectoryPartName)
{
	myTrajectoryPartName = trajectoryPartName;
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryPieceElement::GetCirculationWay() const
{
	return myCirculationWay;
}

//-----------------------------------------------------------------------------
void RoadXML::TrajectoryPieceElement::SetCirculationWay(const std::string& circulationWay)
{
	myCirculationWay = circulationWay;
}

//-----------------------------------------------------------------------------
bool RoadXML::TrajectoryPieceElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetStringAttribute(kTrajectoryPartNameTag, myTrajectoryPartName);
	iDOMElem->GetStringAttribute(kCirculationWayTag, myCirculationWay);
	return true;
}

//-----------------------------------------------------------------------------
IDOMElement* RoadXML::TrajectoryPieceElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kTrajectoryPartNameTag, myTrajectoryPartName);
	elementOut->SetStringAttribute(kCirculationWayTag, myCirculationWay);

	return elementOut;
}


//----------------------------TrajectoryElement--------------------------------
//-----------------------------------------------------------------------------
RoadXML::TrajectoryElement::TrajectoryElement()
{
}

//-----------------------------------------------------------------------------
RoadXML::TrajectoryElement::~TrajectoryElement()
{
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryElement::GetName() const
{
	return myName;
}

//-----------------------------------------------------------------------------
void RoadXML::TrajectoryElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = myName;
	myName=name; 
	if(parent) 
	{
		parent->myTrajectoryArray.updateKey( this, oldname, name );
	} 
}

//-----------------------------------------------------------------------------
const std::string& RoadXML::TrajectoryElement::GetTagName() const
{
	return kTrajectoryTag;
}

//-----------------------------------------------------------------------------
bool RoadXML::TrajectoryElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetStringAttribute(kNameTag, myName);

	LoadChildren(iDOMElem, parser);
	
	return true;
}

//-----------------------------------------------------------------------------
IDOMElement* RoadXML::TrajectoryElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, myName);

	std::vector<CountedPtr<TrajectoryPieceElement>>::iterator itTrajectoryPiece = myTrajectoryPieceArray.begin();
	for(; itTrajectoryPiece != myTrajectoryPieceArray.end(); ++itTrajectoryPiece)
	{
		elementOut->AddChild( (*itTrajectoryPiece).Get()->BuildXMLElement(parser) );
	}

	return elementOut;
}

//-----------------------------------------------------------------------------
bool RoadXML::TrajectoryElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement && newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == kTrajectoryElementTag)
		{
			myTrajectoryPieceArray.push_back(dynamic_cast<TrajectoryPieceElement*>(newElement.Get()));
		}
	}
	else 
	{
		return false;
	}

	return true;
}