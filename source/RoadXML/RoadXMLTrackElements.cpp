#include "RoadXML/RoadXMLTrackElements.h"
#include "RoadXML/RoadXMLElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"


/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::PortionElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kNameTag, mName);
	elem->GetDoubleAttribute(kEndDistance, mEndDistance);
	elem->GetStringAttribute(kStartProfile, mStartProfileName);
	elem->GetStringAttribute(kEndProfile, mEndProfileName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::PortionElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetDoubleAttribute(kEndDistance, mEndDistance);
	elementOut->SetStringAttribute(kStartProfile, mStartProfileName);
	elementOut->SetStringAttribute(kEndProfile, mEndProfileName);

	return elementOut;
}

bool RoadXML::PortionElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool RoadXML::TrackElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mPortions.clear();
	mClippedDataArray.clear();

	elem->GetStringAttribute(kNameTag, mName);

	elem->GetStringAttribute( kStartNodeTag, mStartIntersectionName ); //! Intersection or SubNetworkJunction name. Can be empty.
	elem->GetStringAttribute( kEndNodeTag, mEndIntersectionName ); //! Intersection or SubNetworkJunction name. Can be empty.
	elem->GetStringAttribute( kRoadCategoryTag, mRoadCategory );
	elem->GetBooleanAttribute( kExitAtEndTag, mIsExitAtEnd);
	elem->GetBooleanAttribute( kExitAtStartTag, mIsExitAtStart);

	LoadChildren( elem, parser );

	return true;
}

bool RoadXML::TrackElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kPortionsTag ||
		tagName == kClippedDatasTag )
	{
		LoadChildren( childElement, parser );
	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if (tagName == kPortionTag)
			{
				mPortions.push_back( dynamic_cast<PortionElement*>(newElement.Get()) );
			}
			else if (tagName == kXYCurveTag)
			{
				mXYCurve = dynamic_cast<XYCurveElement*>(newElement.Get());
			}
			else if (tagName == kSZCurveTag)
			{
				mSZCurve = dynamic_cast<CartesianCurveElement*>(newElement.Get());
			}
			else if (tagName == kBankingCurveTag)
			{
				mBankingCurve = dynamic_cast<CartesianCurveElement*>(newElement.Get());
			}
			else if (tagName == kTrackClippedDataTag)
			{
				mClippedDataArray.push_back( dynamic_cast<TrackClippedDataElement*>(newElement.Get()) );
			}
		}
		else 
			return false;
	}

	return true;
}

IDOMElement* RoadXML::TrackElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);

	elementOut->SetStringAttribute( kStartNodeTag, mStartIntersectionName ); //! Intersection or SubNetworkJunction name. Can be empty.
	elementOut->SetStringAttribute( kEndNodeTag, mEndIntersectionName ); //! Intersection or SubNetworkJunction name. Can be empty.
	elementOut->SetBooleanAttribute(kExitAtStartTag, mIsExitAtStart);
	elementOut->SetBooleanAttribute(kExitAtEndTag, mIsExitAtEnd);
	
	if( mRoadCategory.size()) 
		elementOut->SetStringAttribute( kRoadCategoryTag, mRoadCategory );

	if( mXYCurve )
		elementOut->AddChild( mXYCurve->BuildXMLElement(parser) );

	if( mSZCurve )
		elementOut->AddChild( mSZCurve->BuildXMLElement(parser) );

	if (mBankingCurve)
		elementOut->AddChild( mBankingCurve->BuildXMLElement(parser) );

	BuildChildrenArray( mPortions, kPortionsTag, elementOut, parser );
	BuildChildrenArray( mClippedDataArray, kClippedDatasTag, elementOut, parser );

	return elementOut;
}

void RoadXML::TrackElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mTrackArray.updateKey( this, oldname, name );
	} 
}