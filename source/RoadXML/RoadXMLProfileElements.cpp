#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLProfileElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"


/////////////////////////////////////////////////////////////////////////////////

const double RoadXML::VehicleTypeElement::gNoLimit				= -1;

RoadXML::VehicleTypeElement::VehicleTypeElement() :		
		mWeightLimit(gNoLimit),
		mWeightPerAxleLimit(gNoLimit),
		mTrailedWeightLimit(gNoLimit),
		mLengthLimit(gNoLimit),
		mWidthLimit(gNoLimit),
		mHeightLimit(gNoLimit)

{
}

bool RoadXML::VehicleTypeElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kCategoriesTag, mVehicleCategories);
	
	elem->GetDoubleAttribute(kWeightTag, mWeightLimit);
	elem->GetDoubleAttribute(kWeightPerAxleTag, mWeightPerAxleLimit);
	elem->GetDoubleAttribute(kTrailedWeightTag, mTrailedWeightLimit);
	elem->GetDoubleAttribute(kLengthTag, mLengthLimit);
	elem->GetDoubleAttribute(kWidthTag, mWidthLimit);
	elem->GetDoubleAttribute(kHeightTag, mHeightLimit);

	return true;
}

IDOMElement* RoadXML::VehicleTypeElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kCategoriesTag, mVehicleCategories);
	
	if (mWeightLimit != gNoLimit)			elementOut->SetDoubleAttribute(kWeightTag, mWeightLimit);
	if (mWeightPerAxleLimit != gNoLimit)	elementOut->SetDoubleAttribute(kWeightPerAxleTag, mWeightPerAxleLimit);
	if (mTrailedWeightLimit != gNoLimit)	elementOut->SetDoubleAttribute(kTrailedWeightTag, mTrailedWeightLimit);
	if (mLengthLimit != gNoLimit)			elementOut->SetDoubleAttribute(kLengthTag, mLengthLimit);
	if (mWidthLimit != gNoLimit)			elementOut->SetDoubleAttribute(kWidthTag, mWidthLimit);
	if (mHeightLimit != gNoLimit)			elementOut->SetDoubleAttribute(kHeightTag, mHeightLimit);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::DecorationElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kMaterialTag, mMaterialName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::DecorationElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kMaterialTag, mMaterialName);

	elementOut->AddChild(mUVMapping->BuildXMLElement(parser));
	elementOut->AddChild(mPolyLine->BuildXMLElement(parser));

	return elementOut;
}

bool RoadXML::DecorationElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		/*if (tagName == kMaterialTag)
		{
			mMaterial = dynamic_cast<MaterialElement*>(newElement.Get());
		}
		else*/ if (tagName == kUVMappingTag)
		{
			mUVMapping = dynamic_cast<UVMappingElement*>(newElement.Get());
		}
		else if (tagName == kPolyLineTag)
		{
			mPolyLine = dynamic_cast<PolyLineElement*>(newElement.Get());
		}
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

void RoadXML::MarkingElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mMarkingArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::MarkingElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute( kNameTag, mName );
	elem->GetDoubleAttribute(kLineLengthTag, mLineLength);
	elem->GetDoubleAttribute(kTotLengthTag, mTotLength);
	elem->GetDoubleAttribute(kWidthTag, mWidth);
	elem->GetStringAttribute(kLeftToRightTag, mLeftToRight);
	elem->GetStringAttribute(kRightToLeftTag, mRightToLeft);
	elem->GetBooleanAttribute(kDoubleMarkingTag, mDoubleMarking);
	elem->GetStringAttribute(kMaterialTag, mMaterialName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::MarkingElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kLineLengthTag, mLineLength);
	elementOut->SetStringAttribute( kNameTag, mName );
	elementOut->SetDoubleAttribute(kTotLengthTag, mTotLength);
	elementOut->SetDoubleAttribute(kWidthTag, mWidth);
	elementOut->SetStringAttribute(kLeftToRightTag, mLeftToRight);
	elementOut->SetStringAttribute(kRightToLeftTag, mRightToLeft);
	elementOut->SetBooleanAttribute(kDoubleMarkingTag, mDoubleMarking);
	elementOut->SetStringAttribute(kMaterialTag, mMaterialName);

	//elementOut->AddChild(mMaterial->BuildXMLElement(parser));
	elementOut->AddChild(mUVMapping->BuildXMLElement(parser));

	return elementOut;
}

bool RoadXML::MarkingElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		/*if (tagName == kMaterialTag)
		{
			mMaterial = dynamic_cast<MaterialElement*>(newElement.Get());
		}
		else*/ if (tagName == kUVMappingTag)
		{
			mUVMapping = dynamic_cast<UVMappingElement*>(newElement.Get());
		}
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::LaneBorderElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kDistanceTag, mDistance);
	elem->GetDoubleAttribute(kHeightTag, mHeight);
	elem->GetDoubleAttribute(kMarkingOffsetTag, mMarkingOffset);
	elem->GetStringAttribute(kMarkingNameTag, mMarkingName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::LaneBorderElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kDistanceTag, mDistance);
	elementOut->SetDoubleAttribute(kHeightTag, mHeight);
	elementOut->SetDoubleAttribute(kMarkingOffsetTag, mMarkingOffset);
	if( mMarkingName.size() )
		elementOut->SetStringAttribute(kMarkingNameTag, mMarkingName);

	if( mDecoration &&
		mDecoration->mPolyLine && 
		!mDecoration->mPolyLine->mVertexArray.empty() ) 
	{
		elementOut->AddChild( mDecoration->BuildXMLElement( parser ) );
	}


	return elementOut;
}

bool RoadXML::LaneBorderElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if (tagName == kDecorationTag)
		{
			mDecoration = dynamic_cast<DecorationElement*>(newElement.Get());
		}
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::LaneElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kNameTag, mName);
	elem->GetStringAttribute(kTypeTag, mType);
	elem->GetStringAttribute(kCirculationWayTag, mCirculationWay);
	elem->GetDoubleAttribute(kSpeedLimitTag, mSpeedLimit);
	elem->GetStringAttribute(kGroundTag, mGroundName );
	elem->GetStringAttribute(kMaterialTag, mMaterialName );

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::LaneElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetStringAttribute(kTypeTag, mType);
	elementOut->SetStringAttribute(kCirculationWayTag, mCirculationWay);
	elementOut->SetDoubleAttribute(kSpeedLimitTag, mSpeedLimit);
	if( mGroundName.size() )
		elementOut->SetStringAttribute(kGroundTag, mGroundName );
	elementOut->SetStringAttribute(kMaterialTag, mMaterialName );

	elementOut->AddChild(mVehicleAuth->BuildXMLElement(parser));
	//elementOut->AddChild(mMaterial->BuildXMLElement(parser));
	elementOut->AddChild(mUVMapping->BuildXMLElement(parser));

	return elementOut;
}

bool RoadXML::LaneElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if (tagName == kVehicleTypeTag)
		{
			mVehicleAuth = dynamic_cast<VehicleTypeElement*>(newElement.Get());
		}
		//if (tagName == kMaterialTag)
		//{
		//	mMaterial = dynamic_cast<MaterialElement*>(newElement.Get());
		//}
		if (tagName == kUVMappingTag)
		{
			mUVMapping = dynamic_cast<UVMappingElement*>(newElement.Get());
		}
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

void RoadXML::ProfileElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mProfileArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::ProfileElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mLaneArray.clear();
	mLaneBorderArray.clear();

	elem->GetStringAttribute(kTypeTag, mRoadType);
	elem->GetStringAttribute(kNameTag, mName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::ProfileElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kTypeTag, mRoadType);
	elementOut->SetStringAttribute(kNameTag, mName);

	const size_t laneCount = mLaneArray.size();
	assert( mLaneBorderArray.size()==laneCount + 1 );

	elementOut->AddChild(mLaneBorderArray[0]->BuildXMLElement(parser));
	for( size_t iLane=0 ; iLane<laneCount ; iLane++ )
	{
		elementOut->AddChild(mLaneArray[iLane]->BuildXMLElement(parser));
		elementOut->AddChild(mLaneBorderArray[iLane+1]->BuildXMLElement(parser));
	}

	return elementOut;
}

bool RoadXML::ProfileElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if (tagName == kLaneTag)
		{
			mLaneArray.push_back( dynamic_cast<LaneElement*>(newElement.Get()) );
		}
		else if(tagName == kLaneBorderTag)
		{
			mLaneBorderArray.push_back( dynamic_cast<LaneBorderElement*>(newElement.Get()) );
		}
	}
	else 
		return false;

	return true;
}

