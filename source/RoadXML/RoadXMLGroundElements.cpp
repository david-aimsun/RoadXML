#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLVersion.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

#include "stk/Unit.h"
#include "stk/Math.h"

#include <math.h>
#include <string.h>

const char* RoadXML::ToText(RoadXML::RoughnessType type)
{
	switch (type)
	{
	case RoadXML::RoughnessAnalytic: return "analytic";
	case RoadXML::RoughnessNone: return "none";
	case RoadXML::Roughness2x2D: return "2x2D";
	default: return "analytic";
	}
}

RoadXML::RoughnessType RoadXML::ToGranulosityType(const char* name)
{
	for (int i = RoughnessNone; i <= Roughness2x2D; ++i)
	{
		if (strcmp(name, ToText((RoadXML::RoughnessType)i)) == 0)
			return (RoadXML::RoughnessType)i;
	}
	return RoadXML::RoughnessAnalytic;
}

/////////////////////////////////////////////////////////////////////////////////
//
// GroundElement
//

RoadXML::GroundElement::GroundElement() :
	mAverageGrip(1.0),
	mRoughness( new RoughnessElement())
{
	
}

void RoadXML::GroundElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mGroundArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::GroundElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kNameTag, mName );
	elem->GetDoubleAttribute(kGripTag, mAverageGrip);
	elem->GetStringAttribute(kTypeTag, mType);

	//Previous versions
	if (parser->GetVersionId() < MakeVersionID(2, 4, 1))
	{
		double averageGranulosityTmp;
		elem->GetDoubleAttribute(kGranulosityTag, averageGranulosityTmp);

		mRoughness->GetAnalytic()->SetAverageGranulosity(averageGranulosityTmp);

	}

	LoadChildren(elem, parser);

	return true;
}

IDOMElement* RoadXML::GroundElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName );
	elementOut->SetDoubleAttribute(kGripTag, mAverageGrip);
	elementOut->SetStringAttribute(kTypeTag, mType );

	elementOut->AddChild(mRoughness->BuildXMLElement(parser));

	return elementOut;
}

bool RoadXML::GroundElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement &&
		newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == kRoughnessTag)
		{
			mRoughness = static_cast<RoughnessElement*>(newElement.Get());
		}
	}
	else
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// RoughnessElement
//
// Describes what is the Roughness of the ground

RoadXML::RoughnessElement::RoughnessElement() :
mRoughnessType(RoadXML::RoughnessAnalytic),
mAnalytic(new AnalyticElement()),
mIsPatternRepeated(true),
mScale(new Point2x2DElement(1.,1.,1.,1.,1.,kScale2x2DPointsTag)),
mOffset(new Point2x2DElement(kOffset2x2DPointsTag))
{
}

void RoadXML::RoughnessElement::Set2x2DPointArray(const std::vector<CountedPtr<Point2x2DElement> >& roughnessProfile)
{
	m2x2DPointArray = roughnessProfile;
}

bool RoadXML::RoughnessElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetBooleanAttribute(kRepeatPatternTag, mIsPatternRepeated);
	std::string granulosityTypeStr;
	elem->GetStringAttribute(kTypeTag, granulosityTypeStr);
	mRoughnessType = ToGranulosityType(granulosityTypeStr.c_str());

	// Read all 2x2D points of the ground
	LoadChildren(elem, parser);

	return true;
}

IDOMElement* RoadXML::RoughnessElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	std::string roughnessTypeStr = ToText(mRoughnessType);
	elementOut->SetStringAttribute(kTypeTag, roughnessTypeStr);

	elementOut->SetBooleanAttribute(kRepeatPatternTag, mIsPatternRepeated);

	elementOut->AddChild(mAnalytic->BuildXMLElement(parser));

	IDOMElement* elementOutChild = parser->CreateDOMElement(kRoughness2x2DTag);
	elementOut->AddChild(elementOutChild);
	elementOutChild->AddChild(mScale->BuildXMLElement(parser));
	elementOutChild->AddChild(mOffset->BuildXMLElement(parser));

	// Write all 2x2D points of the ground
	BuildChildrenArray(m2x2DPointArray, k2x2DPointsTag, elementOutChild, parser);


	return elementOut;
}

bool RoadXML::RoughnessElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();
	// if array
	if (tagName == k2x2DPointsTag || tagName == kRoughness2x2DTag)
	{
		LoadChildren(childElement, parser);
	}
	//if single element
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if (newElement &&
			newElement->LoadFromXMLElement(childElement, parser))
		{

			if (tagName == k2x2DPointTag)
			{
				m2x2DPointArray.push_back(static_cast<Point2x2DElement*>(newElement.Get()));
			}
			else if (tagName == kRoughnessAnalyticTag)
			{
				mAnalytic = static_cast<AnalyticElement*>(newElement.Get());
			}
			else if (tagName == kScale2x2DPointsTag)
			{
				mScale = static_cast<Point2x2DElement*>(newElement.Get());
			}
			else if (tagName == kOffset2x2DPointsTag)
			{
				mOffset = static_cast<Point2x2DElement*>(newElement.Get());
			}
		}
		else
			return false;
	}

	return true;
}

// Return the last point with a curvilinear distance inferior of the abscissa given in parameter
size_t RoadXML::RoughnessElement::Get2x2DPointIndex(double abscissaInMeter) const
{
	// Find which points index we are between
	size_t left = 0, right = m2x2DPointArray.size() - 1;
	while (right - left > 1)
	{
		size_t middle = left + (right - left) / 2;
		if ( abscissaInMeter >= (m2x2DPointArray[middle]->GetCurvDist() * mScale->GetCurvDist() + mOffset->GetCurvDist()) )
			left = middle;
		else
			right = middle;
	}

	return left;
}

void RoadXML::RoughnessElement::GetInterpolatedOffsetZAndBanking(double distanceFromStartPortionInDirectWay, double offsetFromMiddleWayToWheelInDirectWay, double& offsetZ, double& banking) const
{
	if (mRoughnessType == RoadXML::Roughness2x2D && m2x2DPointArray.size()>1)
	{
		double distanceMax2x2D = m2x2DPointArray[m2x2DPointArray.size() - 1]->GetCurvDist() * mScale->GetCurvDist() + mOffset->GetCurvDist();

		if ((mIsPatternRepeated || distanceFromStartPortionInDirectWay < distanceMax2x2D))
		{
			if (distanceFromStartPortionInDirectWay < m2x2DPointArray[0]->GetCurvDist() * mScale->GetCurvDist() + mOffset->GetCurvDist())
				return;

			// If the last point of roughness has been reached and the pattern of the granulosity is not a loop, just follow the basic information of the ground
			double distanceRelative = fmod((distanceFromStartPortionInDirectWay - mOffset->GetCurvDist()), (distanceMax2x2D - mOffset->GetCurvDist()));
			size_t currentPointIndex = Get2x2DPointIndex(distanceRelative);
			// X > 0 on the Right
			if (offsetFromMiddleWayToWheelInDirectWay < 0)
			{// Left case
				offsetZ = stk::intlin(m2x2DPointArray[currentPointIndex]->GetZ_Left() * mScale->GetZ_Left() + mOffset->GetZ_Left(),
					m2x2DPointArray[currentPointIndex + 1]->GetZ_Left() * mScale->GetZ_Left() + mOffset->GetZ_Left(),
					m2x2DPointArray[currentPointIndex]->GetCurvDist() * mScale->GetCurvDist(),
					m2x2DPointArray[currentPointIndex + 1]->GetCurvDist() * mScale->GetCurvDist(),
					distanceRelative);

				banking = stk::intlin(m2x2DPointArray[currentPointIndex]->GetBanking_Left() * mScale->GetBanking_Left() + mOffset->GetBanking_Left(),
					m2x2DPointArray[currentPointIndex + 1]->GetBanking_Left() * mScale->GetBanking_Left() + mOffset->GetBanking_Left(),
					m2x2DPointArray[currentPointIndex]->GetCurvDist() * mScale->GetCurvDist(),
					m2x2DPointArray[currentPointIndex + 1]->GetCurvDist() * mScale->GetCurvDist(),
					distanceRelative);
			}
			else
			{// Right case
				offsetZ = stk::intlin(m2x2DPointArray[currentPointIndex]->GetZ_Right() * mScale->GetZ_Right() + mOffset->GetZ_Right(),
					m2x2DPointArray[currentPointIndex + 1]->GetZ_Right() * mScale->GetZ_Right() + mOffset->GetZ_Right(),
					m2x2DPointArray[currentPointIndex]->GetCurvDist() * mScale->GetCurvDist(),
					m2x2DPointArray[currentPointIndex + 1]->GetCurvDist() * mScale->GetCurvDist(),
					distanceRelative);

				banking = stk::intlin(m2x2DPointArray[currentPointIndex]->GetBanking_Right() * mScale->GetBanking_Right() + mOffset->GetBanking_Right(),
					m2x2DPointArray[currentPointIndex + 1]->GetBanking_Right() * mScale->GetBanking_Right() + mOffset->GetBanking_Right(),
					m2x2DPointArray[currentPointIndex]->GetCurvDist() * mScale->GetCurvDist(),
					m2x2DPointArray[currentPointIndex + 1]->GetCurvDist() * mScale->GetCurvDist(),
					distanceRelative);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
//
// AnalyticElement
//
// Describes how is the Analytic ground has roughness

RoadXML::AnalyticElement::AnalyticElement() :
mAverageGranulosity(0.5)
{
}

bool RoadXML::AnalyticElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kGranulosityTag, mAverageGranulosity);

	LoadChildren(elem, parser);

	return true;
}


IDOMElement* RoadXML::AnalyticElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kGranulosityTag, mAverageGranulosity);

	return elementOut;
}


/////////////////////////////////////////////////////////////////////////////////
//
// Point2x2DElement
//
// Describes how is the ground on each wheels line (Left/Right)

RoadXML::Point2x2DElement::Point2x2DElement(std::string tag) :
mCurvDist(0),
mZ_Left(0),
mZ_Right(0),
mBanking_Left(0),
mBanking_Right(0),
mTag(tag)
{
}

RoadXML::Point2x2DElement::Point2x2DElement(double dist, double Z_Left, double Z_Right, double banking_Left, double banking_Right, std::string tag) :
mCurvDist(dist),
mZ_Left(Z_Left),
mZ_Right(Z_Right),
mBanking_Left(banking_Left),
mBanking_Right(banking_Right),
mTag(tag)
{
}

bool RoadXML::Point2x2DElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kCurvilinearDistanceTag, mCurvDist);
	elem->GetDoubleAttribute(kZLeftTag, mZ_Left);
	elem->GetDoubleAttribute(kZRightTag, mZ_Right);
	elem->GetDoubleAttribute(kBankingLeftTag, mBanking_Left);
	elem->GetDoubleAttribute(kBankingRightTag, mBanking_Right);

	LoadChildren(elem, parser);

	return true;
}


IDOMElement* RoadXML::Point2x2DElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kCurvilinearDistanceTag, mCurvDist);
	elementOut->SetDoubleAttribute(kZLeftTag, mZ_Left);
	elementOut->SetDoubleAttribute(kZRightTag, mZ_Right);
	elementOut->SetDoubleAttribute(kBankingLeftTag, mBanking_Left);
	elementOut->SetDoubleAttribute(kBankingRightTag, mBanking_Right);

	return elementOut;
}
