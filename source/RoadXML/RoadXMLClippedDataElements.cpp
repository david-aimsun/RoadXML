#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLTrackElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

void AddOptionalFrameElement( IDOMElement* elementOut, const stk::Mark& frame, IDOMParser* parser )
{
	// Add the frame element only if the frame is not NULL
	if( frame.x != 0.0 ||
		frame.y != 0.0 ||
		frame.z != 0.0 ||
		frame.roll != 0.0 ||
		frame.pitch != 0.0 ||
		frame.heading != 0.0 )
	{
		RoadXML::FrameElement* frameElem = new RoadXML::FrameElement( frame );
		elementOut->AddChild( frameElem->BuildXMLElement(parser) );
	}
}

bool TryLoadFrameElem( IDOMElement* childElement, IDOMParser* parser, CountedPtr<RoadXML::FrameElement>& result )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName==RoadXML::kFrameTag )
	{
		CountedPtr<RoadXML::Element> newElement = RoadXML::CreateElement(tagName);
		if( newElement && newElement->LoadFromXMLElement(childElement, parser) )
		{
			result = dynamic_cast<RoadXML::FrameElement*>(newElement.Get());
		}
	}
	return result!=NULL;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::ExternalFileElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute( kPathTag, mFilePath );
	elem->GetBooleanAttribute( kIsInvertedTag, mIsFlipped );

	LoadChildren( elem, parser );

	return mFilePath.size()>0;
}

IDOMElement* RoadXML::ExternalFileElement::BuildXMLElement(IDOMParser* parser)
{
	if( !mFilePath.size() )
		return NULL;

	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName() );
	elementOut->SetStringAttribute(kPathTag, mFilePath);
	elementOut->SetBooleanAttribute(kIsInvertedTag, mIsFlipped);
	
	AddOptionalFrameElement( elementOut, mFrame, parser );

	return elementOut;
}

bool RoadXML::ExternalFileElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	CountedPtr<RoadXML::FrameElement> result;
	if( TryLoadFrameElem( childElement, parser, result ) )
	{
		mFrame = result->GetFrame();
		return true;
	}
	
	return false;
}


/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::SurfaceModifierElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::SurfaceModifierElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	return elementOut;
}

bool RoadXML::SurfaceModifierElement::LoadChild(IDOMElement *childElement, IDOMParser *parser )
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::ObstacleElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	LoadChildren( elem, parser );

	elem->GetStringAttribute( kDodgeTag, mDodgeWay );

	return true;
}

IDOMElement* RoadXML::ObstacleElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute( kDodgeTag, mDodgeWay );

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::SequenceElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	LoadChildren( elem, parser );

	elem->GetIntegerAttribute(kNumberTag, mStepCount);
	elem->GetDoubleAttribute(kDistanceTag, mStepLength);

	return true;
}

IDOMElement* RoadXML::SequenceElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetIntegerAttribute(kNumberTag, mStepCount);
	elementOut->SetDoubleAttribute(kDistanceTag, mStepLength);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::TrackScopeElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mLaneScope.clear();

	elem->GetDoubleAttribute(kFromTag, mFrom);
	elem->GetDoubleAttribute(kToTag, mTo);
	elem->GetDoubleAttribute(kLeftTag, mLeftLimit);
	elem->GetDoubleAttribute(kRightTag, mRightLimit);
	elem->GetDoubleAttribute(kTopTag, mTopLimit);
	elem->GetDoubleAttribute(kBottomTag, mBottomLimit);
	elem->GetStringAttribute(kWayTag, mCirculationWay);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TrackScopeElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kFromTag, mFrom);
	elementOut->SetDoubleAttribute(kToTag, mTo);
	elementOut->SetDoubleAttribute(kLeftTag, mLeftLimit);
	elementOut->SetDoubleAttribute(kRightTag, mRightLimit);
	elementOut->SetDoubleAttribute(kTopTag, mTopLimit);
	elementOut->SetDoubleAttribute(kBottomTag, mBottomLimit);
	elementOut->SetStringAttribute(kWayTag, mCirculationWay);

	std::set<size_t>::iterator itr = mLaneScope.begin();
	while( itr!=mLaneScope.end() )
	{
		CountedPtr<IDOMElement> elemLane = parser->CreateDOMElement(kLaneScopeTag);

        elemLane->SetIntegerAttribute(kIndexTag, (int32_t)*itr);

		elementOut->AddChild( elemLane );

		++itr;
	}

	return elementOut;
}

bool RoadXML::TrackScopeElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	if( tagName==kLaneScopeTag )
	{
		size_t index=0;
		if( childElement->GetIntegerAttribute(kIndexTag, index) )
			mLaneScope.insert( (size_t)index );
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::TrackClipElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{

	elem->GetDoubleAttribute(kAbscissaTag, mAbscissa);
	elem->GetDoubleAttribute(kDistanceTag, mDistance);
	elem->GetDoubleAttribute(kElevationTag, mElevation);

	elem->GetBooleanAttribute(kVerticalTag, mIsVertical);
	elem->GetDoubleAttribute(kHeadingTag, mHeading);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::TrackClipElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kAbscissaTag, mAbscissa);
	elementOut->SetDoubleAttribute(kDistanceTag, mDistance);
	elementOut->SetDoubleAttribute(kElevationTag, mElevation);

	elementOut->SetBooleanAttribute(kVerticalTag, mIsVertical);
	elementOut->SetDoubleAttribute(kHeadingTag, mHeading);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////

const double RoadXML::ClippedDataElement::gNoMass = -1;

bool RoadXML::ClippedDataElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kNameTag, mName);
	elem->GetDoubleAttribute(kMassTag, mMass);
	elem->GetDoubleAttribute(kSequenceOrientationTag, mSequenceOrientation);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::ClippedDataElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetDoubleAttribute(kMassTag, mMass);
	elementOut->SetDoubleAttribute(kSequenceOrientationTag, mSequenceOrientation);

	if( mSequenceX ) elementOut->AddChild( mSequenceX->BuildXMLElement(parser) );
	if( mSequenceY ) elementOut->AddChild( mSequenceY->BuildXMLElement(parser) );
	if( mSequenceZ ) elementOut->AddChild( mSequenceZ->BuildXMLElement(parser) );
	if( mObject ) elementOut->AddChild( mObject->BuildXMLElement(parser) );
	if( mRoadSign ) elementOut->AddChild( mRoadSign->BuildXMLElement(parser) );
	if( mObstacle ) elementOut->AddChild( mObstacle->BuildXMLElement(parser) );
	if( mSurfaceModifier ) elementOut->AddChild( mSurfaceModifier->BuildXMLElement(parser) );

	return elementOut;
}

bool RoadXML::ClippedDataElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement &&
		newElement->LoadFromXMLElement(childElement, parser) )
	{
		if( tagName==kSequenceTag ) mSequenceX = dynamic_cast<SequenceElement*>(newElement.Get());
		else if( tagName==kSequenceYTag ) mSequenceY = dynamic_cast<SequenceElement*>(newElement.Get());
		else if( tagName==kSequenceZTag ) mSequenceZ = dynamic_cast<SequenceElement*>(newElement.Get());
		else if( tagName==kExternalElementTag ) mObject = dynamic_cast<ExternalFileElement*>(newElement.Get());
		else if( tagName==kRoadSignTag ) mRoadSign = dynamic_cast<RoadSignElement*>(newElement.Get());
		else if( tagName==kObstacleTag ) mObstacle = dynamic_cast<ObstacleElement*>(newElement.Get());
		else if( tagName==kSurfaceModifierTag ) mSurfaceModifier = dynamic_cast<SurfaceModifierElement*>(newElement.Get());
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

IDOMElement* RoadXML::TrackClippedDataElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = ClippedDataElement::BuildXMLElement(parser);

	if( mClip ) elementOut->AddChild( mClip->BuildXMLElement(parser) );
	if( mScope ) elementOut->AddChild( mScope->BuildXMLElement(parser) );

	return elementOut;
}

bool RoadXML::TrackClippedDataElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName==kTrackClipTag )
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement && newElement->LoadFromXMLElement(childElement, parser) )
			mClip = dynamic_cast<TrackClipElement*>(newElement.Get());
	}
	else if( tagName==kTrackScopeTag )
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement && newElement->LoadFromXMLElement(childElement, parser) )
			mScope = dynamic_cast<TrackScopeElement*>(newElement.Get());
	}
	else if( !ClippedDataElement::LoadChild( childElement, parser ) )
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

void RoadXML::MarkClippedDataElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mClippedDataArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::MarkClippedDataElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	return ClippedDataElement::LoadFromXMLElement(elem, parser);
}

bool RoadXML::MarkClippedDataElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	CountedPtr<RoadXML::FrameElement> result;
	if( TryLoadFrameElem( childElement, parser, result ) )
	{
		mFrame = result->GetFrame();
	}
	else if( !ClippedDataElement::LoadChild( childElement, parser ) )
		return false;

	return true;
}

IDOMElement* RoadXML::MarkClippedDataElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = ClippedDataElement::BuildXMLElement(parser);

	AddOptionalFrameElement( elementOut, mFrame, parser );

	return elementOut;
}


/////////////////////////////////////////////////////////////////////////////////
