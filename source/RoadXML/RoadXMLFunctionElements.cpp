#include "RoadXML/RoadXMLTrackElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"


/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::FrameElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kXTag, mFrame.x);
	elem->GetDoubleAttribute(kYTag, mFrame.y);
	elem->GetDoubleAttribute(kZTag, mFrame.z);
	elem->GetDoubleAttribute(kRollTag, mFrame.roll);
	elem->GetDoubleAttribute(kPitchTag, mFrame.pitch);
	elem->GetDoubleAttribute(kHeadingTag, mFrame.heading);

	return true;
}

IDOMElement* RoadXML::FrameElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kXTag, mFrame.x);
	elementOut->SetDoubleAttribute(kYTag, mFrame.y);
	elementOut->SetDoubleAttribute(kZTag, mFrame.z);
	elementOut->SetDoubleAttribute(kRollTag, mFrame.roll);
	elementOut->SetDoubleAttribute(kPitchTag, mFrame.pitch);
	elementOut->SetDoubleAttribute(kHeadingTag, mFrame.heading);

	return elementOut;
}


bool RoadXML::PolynomialFunctionElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::PolynomialFunctionElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	IDOMElement* inChild = parser->CreateDOMElement(kBeginTag);
	inChild->SetDoubleAttribute( kDirectionTag, mDerivativeIn );
	inChild->SetDoubleAttribute( kXTag, mPosIn.x );
	inChild->SetDoubleAttribute( kYTag, mPosIn.y );
	elementOut->AddChild( inChild );

	IDOMElement* outChild = parser->CreateDOMElement(kEndTag);
	outChild->SetDoubleAttribute( kDirectionTag, mDerivativeOut );
	outChild->SetDoubleAttribute( kXTag, mPosOut.x );
	outChild->SetDoubleAttribute( kYTag, mPosOut.y );
	elementOut->AddChild( outChild );

	return elementOut;
}

bool RoadXML::PolynomialFunctionElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	if (tagName == kBeginTag)
	{
		childElement->GetDoubleAttribute(kDirectionTag, mDerivativeIn);
		childElement->GetDoubleAttribute(kXTag, mPosIn.x);
		childElement->GetDoubleAttribute(kYTag, mPosIn.y);
	}
	else if (tagName == kEndTag)
	{
		childElement->GetDoubleAttribute(kDirectionTag, mDerivativeOut);
		childElement->GetDoubleAttribute(kXTag, mPosOut.x);
		childElement->GetDoubleAttribute(kYTag, mPosOut.y);
	}
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::SegmentElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kLengthTag, mLength);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::SegmentElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kLengthTag, mLength);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::CircleArcElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kLengthTag, mLength);
	elem->GetDoubleAttribute(kCurvatureTag, mCurvature);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::CircleArcElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kLengthTag, mLength);
	elementOut->SetDoubleAttribute(kCurvatureTag, mCurvature);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////


bool RoadXML::ClothoidSplineElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mVertexArray.clear();

	elem->GetBooleanAttribute(kSymmetricTag, mSymmetricBlending);
	elem->GetDoubleAttribute(kThresholdTag, mThreshold);

	LoadChildren( elem, parser );

	return true;
}

bool RoadXML::ClothoidSplineElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	stk::Vector2 value;
	if( RoadXML::LoadVector2Elem( value, childElement ) )
		mVertexArray.push_back( value );
	else 
		return false;

	return true;
}

IDOMElement* RoadXML::ClothoidSplineElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetBooleanAttribute(kSymmetricTag, mSymmetricBlending);
	elementOut->SetDoubleAttribute(kThresholdTag, mThreshold);

	const size_t elemCount = mVertexArray.size();
	for (size_t iElem=0; iElem < elemCount; iElem++)
	{
		elementOut->AddChild( RoadXML::BuildVector2Elem(mVertexArray[iElem], parser) );
	}

	return elementOut;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool RoadXML::ClothoidArcElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kLengthTag, mLength);
	elem->GetDoubleAttribute(kStartCurvatureTag, mStartCurvature);
	elem->GetDoubleAttribute(kEndCurvatureTag, mEndCurvature);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::ClothoidArcElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kLengthTag, mLength);
	elementOut->SetDoubleAttribute(kStartCurvatureTag, mStartCurvature);
	elementOut->SetDoubleAttribute(kEndCurvatureTag, mEndCurvature);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////

// EType
const RoadXML::PolyLineElement::EType RoadXML::PolyLineElement::ToEType(const std::string & strTypeValue)
{
	if (strTypeValue == "spline")
		return eSpline;

	if (strTypeValue == "bezier")
		return eBezier;

	return eSegment;
}

const std::string RoadXML::PolyLineElement::ToText(const RoadXML::PolyLineElement::EType & eTypeValue)
{
	switch (eTypeValue)
	{
	default:
	case eSegment:
		return "segment";
	case eSpline:
		return "spline";
	case eBezier:
		return "bezier";
	}
}


bool RoadXML::PolyLineElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mVertexArray.clear();

	std::string val;
	elem->GetStringAttribute(kTypeTag, val);
	mInterpolationType = ToEType(val);

	elem->GetDoubleAttribute(kEndHeadingTag, mEndHeading);
	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::PolyLineElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kTypeTag, ToText(mInterpolationType));

	elementOut->SetDoubleAttribute(kEndHeadingTag, mEndHeading);

	const size_t elemCount = mVertexArray.size();
	for (size_t iElem=0; iElem < elemCount; iElem++)
	{
		elementOut->AddChild( BuildVector2Elem(mVertexArray[iElem], parser) );
	}

	return elementOut;
}

bool RoadXML::PolyLineElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	stk::Vector2 value;
	if( LoadVector2Elem( value, childElement ) )
		mVertexArray.push_back( value );
	else 
		return false;

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
