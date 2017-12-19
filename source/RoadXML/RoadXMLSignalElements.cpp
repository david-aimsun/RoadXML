#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLSignalElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::TimerElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kTextTag, mText);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TimerElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kTextTag, mText);
	
	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::LightElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kNameTag, mName);
	elem->GetStringAttribute(kColorTag, mColor);
	elem->GetStringAttribute(kIlluminationTag, mIllumination);
	elem->GetStringAttribute(kDirectionTag, mDirection);
	elem->GetStringAttribute(kVehicleTypeTag, mVehicle);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::LightElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetStringAttribute(kColorTag, mColor);
	elementOut->SetStringAttribute(kIlluminationTag, mIllumination);
	elementOut->SetStringAttribute(kDirectionTag, mDirection);
	elementOut->SetStringAttribute(kVehicleTypeTag, mVehicle);

	if( mTimer )
		elementOut->AddChild( mTimer->BuildXMLElement(parser) );
	
	return elementOut;
}

bool RoadXML::LightElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if( tagName == kTimerTag )
		{
			mTimer = dynamic_cast<TimerElement*>(newElement.Get());
		}
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::RoadSignElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kSignTypeTag, mSignType);
	elem->GetStringAttribute(kDirectionTag, mDirection);
	elem->GetStringAttribute(kMessageTag, mTextMessage);
	elem->GetDoubleAttribute(kValueTag, mValue);
	elem->GetDoubleAttribute(kDistanceTag, mDistance);
	elem->GetBooleanAttribute(kTemporaryTag, mTemporary);
	elem->GetStringAttribute(kTargetNameTag, mTargetName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::RoadSignElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
	
	elementOut->SetStringAttribute(kSignTypeTag, mSignType);
	elementOut->SetStringAttribute(kDirectionTag, mDirection);
	elementOut->SetStringAttribute(kMessageTag, mTextMessage);
	elementOut->SetDoubleAttribute(kValueTag, mValue);
	elementOut->SetDoubleAttribute(kDistanceTag, mDistance);
	elementOut->SetBooleanAttribute(kTemporaryTag, mTemporary);
	if( mTargetName.size() )
		elementOut->SetStringAttribute(kTargetNameTag, mTargetName);
	
	elementOut->AddChild(mVehicleType->BuildXMLElement(parser));

	// Light array
	const size_t lightCount = mLightArray.size();
	for(size_t iLight=0 ; iLight<lightCount ; iLight++)
	{
		elementOut->AddChild( mLightArray[iLight]->BuildXMLElement(parser) );
	}

	return elementOut;
}

bool RoadXML::RoadSignElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if( tagName == kVehicleTypeTag )
		{
			mVehicleType = dynamic_cast<VehicleTypeElement*>(newElement.Get());
		}
		else if( tagName == kLightTag )
		{
			mLightArray.push_back( dynamic_cast<LightElement*>(newElement.Get()));
		}
	}
	else 
		return false;

	return true;
}


/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::TrafficLightStateElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mIlluminationArray.clear();

	elem->GetDoubleAttribute(kTimeLengthTag, mTimeLength);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TrafficLightStateElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kTimeLengthTag, mTimeLength);

	const size_t lightCount = mIlluminationArray.size();
	for(size_t iLight=0 ; iLight<lightCount ; iLight++)
	{
		CountedPtr<IDOMElement> child = parser->CreateDOMElement(kStateTag);
		child->SetStringAttribute(kIlluminationTag, mIlluminationArray[iLight]);
		elementOut->AddChild(child);
	}

	return elementOut;
}

bool RoadXML::TrafficLightStateElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	if( tagName == kStateTag )
	{
		std::string newState;
		childElement->GetStringAttribute( kIlluminationTag, newState );
		mIlluminationArray.push_back(newState);
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::TrafficLightAnimationElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kRoadSignNameTag, mRoadSignName);
	elem->GetStringAttribute(kCycleNameTag, mCycleName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TrafficLightAnimationElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kRoadSignNameTag, mRoadSignName);
	elementOut->SetStringAttribute(kCycleNameTag, mCycleName);

	BuildChildrenArray( mStates, kStatesTag, elementOut, parser);

	return elementOut;
}

bool RoadXML::TrafficLightAnimationElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kStatesTag )
	{
		LoadChildren( childElement, parser );
	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if( tagName==kTrafficLightStateTag )
				mStates.push_back( dynamic_cast<TrafficLightStateElement*>(newElement.Get()) );
		}
		else 
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

void RoadXML::TrafficLightGroupElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mTrafficLightGroupArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::TrafficLightGroupElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mAnimationArray.clear();

	elem->GetStringAttribute(kNameTag, mName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TrafficLightGroupElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);

	BuildChildrenArray( mAnimationArray, kAnimationsTag, elementOut, parser);

	return elementOut;
}

bool RoadXML::TrafficLightGroupElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kAnimationsTag )
	{
		LoadChildren( childElement, parser );
	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if( tagName==kTrafficLightAnimationTag )
				mAnimationArray.push_back( dynamic_cast<TrafficLightAnimationElement*>(newElement.Get()));
		}
		else 
			return false;
	}

	return true;
}