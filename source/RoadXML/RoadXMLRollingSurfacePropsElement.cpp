#include "RoadXML/RoadXMLElements.h"
#include "RoadXML/RoadXMLVersion.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

#include "stk/Unit.h"
#include "stk/Math.h"

#include <math.h>
#include <string.h>




RoadXML::RollingSurfacePropsElement::RollingSurfacePropsElement() : 
mIsRollingSurfaceUpdateOnSave(false), m3DLayersToExportElement(NULL), m3DRoadsGenerationElement(NULL), mSolExportOptionsElement(NULL)
{
}

RoadXML::RollingSurfacePropsElement::~RollingSurfacePropsElement()
{
}


RoadXML::Export3DLayersElement::Export3DLayersElement() :
mRoadsAndIntersections(true),
mMarkings(false),
mGeneratedGround(false), m3DObjects(false), mExportGround(false), mExternalFile(false), mExternalFilePath(""), mMergeRollinSurfaces(false)
{
}

RoadXML::Export3DLayersElement::~Export3DLayersElement()
{
}

RoadXML::Generate3DRoadsElement::Generate3DRoadsElement() :
mCTol(0.), MminStep(0.1), mMaxStep(20.), mMaxNormalVariation(0.087266 /* 5deg*/), mMaxWidth(1000.0)
{
}

RoadXML::Generate3DRoadsElement::~Generate3DRoadsElement()
{
}

RoadXML::SolExportOptionsElement::SolExportOptionsElement() :
mMinPointsDistance(0.02), mGenerateDebugFile(false), mRemoveTriangles(true), mCheckOrientaions(true)
{
}

RoadXML::SolExportOptionsElement::~SolExportOptionsElement()
{
}



bool RoadXML::RollingSurfacePropsElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetBooleanAttribute(kIsRollingSurfaceUpdateOnSaveTag, mIsRollingSurfaceUpdateOnSave);
	
	LoadChildren(elem, parser);

	return true;
}

IDOMElement*  RoadXML::RollingSurfacePropsElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
	elementOut->SetBooleanAttribute(kIsRollingSurfaceUpdateOnSaveTag, mIsRollingSurfaceUpdateOnSave);
	elementOut->AddChild(m3DLayersToExportElement->BuildXMLElement(parser)); 
	elementOut->AddChild(m3DRoadsGenerationElement->BuildXMLElement(parser));
	elementOut->AddChild(mSolExportOptionsElement->BuildXMLElement(parser));
	return elementOut;
}

bool RoadXML::RollingSurfacePropsElement::LoadChild(IDOMElement* childElement, IDOMParser* parser)
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement &&
		newElement->LoadFromXMLElement(childElement, parser))
	{
		if (tagName == k3DLayersToExportTag)
		{
			m3DLayersToExportElement = static_cast<Export3DLayersElement*>(newElement.Get());
		}
		else if (tagName == k3DRoadsGenerationElementTag)
		{
			m3DRoadsGenerationElement = static_cast<Generate3DRoadsElement*>(newElement.Get());
		}

		else if (tagName == kSolExportOptionsElementTag)
		{
			mSolExportOptionsElement = static_cast<SolExportOptionsElement*>(newElement.Get());
		}
	}
	else
		return false;

	return true;
}



bool RoadXML::Export3DLayersElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	//const std::string& tagName = GetTagName();

	elem->GetBooleanAttribute(kRoadsAndIntersectionsTag, mRoadsAndIntersections);
	elem->GetBooleanAttribute(kExportMarkingsTag, mMarkings);
	elem->GetBooleanAttribute(kGeneratedGroundTag, mGeneratedGround);
	elem->GetBooleanAttribute(k3DObjectsTag, m3DObjects);
	elem->GetBooleanAttribute(kExportGroundTag, mExportGround);
	elem->GetBooleanAttribute(kExternalFileTag, mExternalFile);
	elem->GetStringAttribute(kExternalFilePathTag, mExternalFilePath);
	elem->GetBooleanAttribute(kMergeRollinSurfacesTag, mMergeRollinSurfaces);


	return true;
}

IDOMElement* RoadXML::Export3DLayersElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetBooleanAttribute(kRoadsAndIntersectionsTag, mRoadsAndIntersections);
	elementOut->SetBooleanAttribute(kExportMarkingsTag, mMarkings);
	elementOut->SetBooleanAttribute(kGeneratedGroundTag, mGeneratedGround);
	elementOut->SetBooleanAttribute(k3DObjectsTag, m3DObjects);
	elementOut->SetBooleanAttribute(kExportGroundTag, mExportGround);
	elementOut->SetBooleanAttribute(kExternalFileTag, mExternalFile);
	elementOut->SetStringAttribute(kExternalFilePathTag, mExternalFilePath);
	elementOut->SetBooleanAttribute(kMergeRollinSurfacesTag, mMergeRollinSurfaces);

	return elementOut;
}


bool RoadXML::Generate3DRoadsElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	const std::string& tagName = GetTagName();
	elem->GetDoubleAttribute(kCTolTag, mCTol);
	elem->GetDoubleAttribute(kminStepTag, MminStep);
	elem->GetDoubleAttribute(kMaxStepTag, mMaxStep);
	elem->GetDoubleAttribute(kmaxNormalVariationTag, mMaxNormalVariation);
	elem->GetDoubleAttribute(kmaxWidthTag, mMaxWidth);

	return true;
}

IDOMElement* RoadXML::Generate3DRoadsElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kCTolTag, mCTol);
	elementOut->SetDoubleAttribute(kminStepTag, MminStep);
	elementOut->SetDoubleAttribute(kMaxStepTag, mMaxStep);
	elementOut->SetDoubleAttribute(kmaxNormalVariationTag, mMaxNormalVariation);
	elementOut->SetDoubleAttribute(kmaxWidthTag, mMaxWidth);

	return elementOut;
}

bool RoadXML::SolExportOptionsElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	const std::string& tagName = GetTagName();
	elem->GetDoubleAttribute(kMinPointsDistanceTag, mMinPointsDistance);
	elem->GetBooleanAttribute(kGenerateDebugFileTag, mGenerateDebugFile);
	elem->GetBooleanAttribute(kRemoveTrianglesTag, mRemoveTriangles);
	elem->GetBooleanAttribute(kCheckOrientaionsTag, mCheckOrientaions);

	return true;
}

IDOMElement* RoadXML::SolExportOptionsElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kMinPointsDistanceTag, mMinPointsDistance);
	elementOut->SetBooleanAttribute(kGenerateDebugFileTag, mGenerateDebugFile);
	elementOut->SetBooleanAttribute(kRemoveTrianglesTag, mRemoveTriangles);
	elementOut->SetBooleanAttribute(kCheckOrientaionsTag, mCheckOrientaions);


	return elementOut;
}

