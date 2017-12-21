#include <algorithm>
#include "RoadXML/RoadXMLElements.h"

#include "RoadXML/RoadXMLVersion.h"
#include "RoadXML/RoadXMLTags.h"
#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

#include "stk/Vector3.h"
#include "stk/StringTools.h"

#include <stdlib.h> 
#include <sstream>


std::string RoadXML::ourLastError = "";

/////////////////////////////////////////////////////////////////////////////////

void LoadContour( std::vector< stk::Vector3 >& contour, IDOMElement* iDOMElem)
{
	std::string pointsStr;
	iDOMElem->GetStringAttribute(RoadXML::kContourPointsTag, pointsStr);

	std::vector<std::string> tokens = stk::SplitString( pointsStr, ' ');

	for( size_t i = 0; i < tokens.size(); i += 3)
	{
		stk::Vector3 vec(	atof( tokens[i].c_str() ),
							atof( tokens[i+1].c_str() ),
							atof( tokens[i+2].c_str() ) );

		contour.push_back( vec );
	}
}

IDOMElement* BuildXMLContour( const std::vector< stk::Vector3 >& contour, IDOMParser* parser)
{
	IDOMElement* childOut = parser->CreateDOMElement(RoadXML::kTerrainContourTag);

	std::stringstream ss;

	for( size_t i = 0 ; i < contour.size(); i++)
	{
		ss << contour[i].x << " " << contour[i].y << " " << contour[i].z << " ";
	}

	childOut->SetStringAttribute( RoadXML::kContourPointsTag, ss.str() );

	return childOut;
}

/////////////////////////////////////////////////////////////////////////////////

RoadXML::Element* RoadXML::Load( const char * sInputFileName )
{
	ourLastError.clear();

	 //Create a parser 
	CountedPtr<IDOMParser> pParser = NewDOMParserRapid( sInputFileName );

	IDOMElement* pRootElement = RoadXML::LoadRootElement( pParser );

	// Load the file
	if ( !pRootElement )
		return NULL;

	CountedPtr<IDOMElement> childElement = pRootElement->GetFirstChildElement();

	while (childElement)
	{
		// Do not take a counted pointer on the element: we let this responsibility to the Load() caller.
		RoadXML::Element* roadXMLChild = RoadXML::CreateElement( childElement->GetTagName() );
		if( roadXMLChild )
		{
			if( roadXMLChild->LoadFromXMLElement( childElement, pParser ) )
			{
				return roadXMLChild;
			}
			else
			{	// Since we didn't take a counted ptr on this object, we need to delete it manually.
				delete roadXMLChild;
			}
		}

		childElement = childElement->GetNextSiblingChildElement();
	}

	return NULL;
}

IDOMElement* RoadXML::LoadRootElement( IDOMParser* pParser )
{
	// Load the file
	if ( !pParser->Load() )
	{
		return NULL;
	}

	// Translate the file content

	IDOMElement* pRootElement = pParser->GetRootElement(); // The Root Element is the element named RoadXML.

	std::string version;
	pRootElement->GetStringAttribute(kVersionTag, version);
	std::string versionRevision;
	pRootElement->GetStringAttribute(kVersionRevisionTag, versionRevision);
	
	if( version!=RoadXML_VERSION_STRING )
	{	// Warning: the parser is not the same one as the file
	}
	
	pParser->SetVersionId(StringToVersionID(version.c_str())); // Store the RoadXML version ID in the parser

	return pRootElement;
}

bool RoadXML::Save( RoadXML::Element* pElementToSave, const char * sOutputFilePath )
{
	if( !sOutputFilePath )
		return false;

	// Create a parser 
	CountedPtr<IDOMParser> pParser = CreateRoadXMLParser(sOutputFilePath); // Set the file before building: the external files will use it as a reference for relative path

	CountedPtr<IDOMElement> docElement = pParser->GetRootElement();

	if( pElementToSave )
		docElement->AddChild( pElementToSave->BuildXMLElement( pParser ) );

	// Save the DOM document into the file
	return pParser->Save();
}

IDOMParser* RoadXML::CreateRoadXMLParser(const char * sOutputFilePath, bool rapid)
{
	IDOMParser* pParser;

	if (rapid)
		pParser = NewDOMParserRapid(sOutputFilePath);
	else
		pParser = NewDOMParser(sOutputFilePath);

	// Built the DOM node

	// RoadXML element is the ROOT  element: create the document with this root name
	CountedPtr<IDOMElement> docElement = pParser->CreateDOMDocument(RoadXML::kRoadXMLTag.c_str());
	pParser->SetVersionId(StringToVersionID(RoadXML_VERSION_STRING));

	docElement->SetStringAttribute(kVersionTag, RoadXML_VERSION_STRING );
	docElement->SetStringAttribute(kVersionRevisionTag, RoadXML_VERSION_DETAILS );
	docElement->SetStringAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");

	return pParser;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::NetworkElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	// Clear previous data
	mSubNetworkArray.clear();
	mSubNetworkJunctionArray.clear();
	mRoadArray.clear();

	elem->GetStringAttribute(kNameTag, mName);
	elem->GetStringAttribute(kTrafficHandTypeTag, mHandType);

	LoadChildren( elem, parser );

	return true;
}

bool RoadXML::NetworkElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	// if array
	if( tagName == kSubNetworkJunctionsTag ||
		tagName == kSubNetworksTag ||
		tagName == kRoadsTag )
	{
		LoadChildren( childElement, parser );
	}
	//if single element
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{

			if (tagName == kGeolocalisationTag)
			{
				mGeolocalisation = dynamic_cast<GeolocalisationElement*>(newElement.Get());	// add geoloc info to document.
			}
			else if (tagName == kSubNetworkTag)
			{
				mSubNetworkArray.push_back( dynamic_cast<SubNetworkElement*>(newElement.Get()) );
			}
			else if(tagName == kRoadTag)
			{
				mRoadArray.push_back( dynamic_cast<RoadElement*>(newElement.Get()) );
			}
			else if (tagName == kSubNetworkJunctionTag)
			{
				mSubNetworkJunctionArray.push_back( dynamic_cast<SubNetworkJunctionElement*>(newElement.Get()) );
			}
			else if( tagName == kExternalTerrainTag )
			{
				mTerrain = dynamic_cast<ExternalFileElement*>(newElement.Get());
			}
			else if(tagName == kExternalSurfaceTag)
			{
				mRollingSurface = dynamic_cast<ExternalFileElement*>(newElement.Get() );
			}
			else if( tagName == kExternalPhysicalSurfaceTag )
			{
				mPhysicalSurface = dynamic_cast<ExternalFileElement*>(newElement.Get());
			}
			else if( tagName == kGeneratedTerrainMaterialTag )
			{
				mGeneratedTerrainMaterial = dynamic_cast<GeneratedTerrainMaterialElement*>(newElement.Get());
			}
		}
		else 
			return false;
	}

	return true;
}	
	
IDOMElement* RoadXML::NetworkElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* networkElement = parser->CreateDOMElement(GetTagName());

	networkElement->SetStringAttribute(kNameTag, mName);
	networkElement->SetStringAttribute(kTrafficHandTypeTag, mHandType);

	if(mGeolocalisation != NULL)
	{
		networkElement->AddChild( mGeolocalisation->BuildXMLElement( parser ) );
	}

	if( mTerrain )
	{
		networkElement->AddChild( mTerrain->BuildXMLElement(parser) );
	}
	
	if( mRollingSurface )
	{
		networkElement->AddChild(mRollingSurface->BuildXMLElement(parser));
	}

	if( mPhysicalSurface )
	{
		networkElement->AddChild( mPhysicalSurface->BuildXMLElement(parser) );
	}

	if( mGeneratedTerrainMaterial )
	{
		networkElement->AddChild( mGeneratedTerrainMaterial->BuildXMLElement(parser) );
	}

	BuildChildrenArray( mSubNetworkJunctionArray, kSubNetworkJunctionsTag, networkElement, parser);
	BuildChildrenArray( mSubNetworkArray, kSubNetworksTag, networkElement, parser);
	BuildChildrenArray( mRoadArray, kRoadsTag, networkElement, parser);



	return networkElement;
}

/////////////////////////////////////////////////////////////////////////////////

void RoadXML::SubNetworkElement::Clear()
{
	mPosition.x = mPosition.y = mPosition.z =0;
	mHeading = 0;
	mTerrain = NULL;
	mRollingSurface = NULL;
	mPhysicalSurface = NULL;
	mRollingSurfacePropsElement = NULL;
	mProxyFile = NULL;
	mGroundArray.clear();
	mMarkingArray.clear();
	mProfileArray.clear();
	mIntersectionArray.clear();
	mTrackArray.clear();
	mTrafficLightGroupArray.clear();
	mGeneratedTerrainMaterial = NULL;
	mTerrainContours.clear();

}

bool RoadXML::SubNetworkElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	Clear();

	elem->GetDoubleAttribute(kXTag, mPosition.x);
	elem->GetDoubleAttribute(kYTag, mPosition.y);
	elem->GetDoubleAttribute(kZTag, mPosition.z);
	elem->GetDoubleAttribute(kHeadingTag, mHeading);
	elem->GetStringAttribute(kNameTag, mName );

	LoadChildren( elem, parser );
	
	return true;
}

IDOMElement* RoadXML::SubNetworkElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kHeadingTag, mHeading);
	elementOut->SetStringAttribute(kNameTag, mName);

	elementOut->SetDoubleAttribute(kXTag, mPosition.x);
	elementOut->SetDoubleAttribute(kYTag, mPosition.y);
	elementOut->SetDoubleAttribute(kZTag, mPosition.z);


	if( mProxyFile )
	{
		elementOut->AddChild( mProxyFile->BuildXMLElement(parser) );
	}
	else
	{	
		if( mTerrain )
		{
			elementOut->AddChild( mTerrain->BuildXMLElement(parser) );
		}
		if( mRollingSurface )
		{
			elementOut->AddChild(mRollingSurface->BuildXMLElement(parser));
		}
		if (mRollingSurfacePropsElement) // Rolling surface propreties
		{
			elementOut->AddChild(mRollingSurfacePropsElement->BuildXMLElement(parser));
		}
		if (mPhysicalSurface)
		{
			elementOut->AddChild(mPhysicalSurface->BuildXMLElement(parser));
		}

				
		{	// RoadNetwork Part
			IDOMElement* roadNetwork = parser->CreateDOMElement(kRoadNetworkTag);
			elementOut->AddChild(roadNetwork);

			BuildChildrenArray( mGroundArray, kGroundsTag, roadNetwork, parser);
			BuildChildrenArray( mMarkingArray, kMarkingsTag, roadNetwork, parser);
			BuildChildrenArray( mProfileArray, kProfilesTag, roadNetwork, parser);
			BuildChildrenArray( mIntersectionArray, kIntersectionsTag, roadNetwork, parser);
			BuildChildrenArray( mTrackArray, kTracksTag, roadNetwork, parser);
			BuildChildrenArray( mTrafficLightGroupArray, kTrafficLightGroupsTag, roadNetwork, parser);
			BuildChildrenArray( mMaterialArray, kMaterialsTag, roadNetwork, parser);
		}

		BuildChildrenArray(myTrajectoryArray, kTrajectoriesTag, elementOut, parser);
		BuildChildrenArray(myTrajectoryPartArray, kTrajectoryPartsTag, elementOut, parser);
		
		IDOMElement* arrayElem = parser->CreateDOMElement( kTerrainContoursTag );
		elementOut->AddChild( arrayElem );

		for( size_t i = 0; i < mTerrainContours.size() ; i++ )
		{
			IDOMElement* contourElem = BuildXMLContour( mTerrainContours[i], parser );
			
			arrayElem->AddChild(contourElem);
		}

		if( mGeneratedTerrainMaterial )
		{
			elementOut->AddChild( mGeneratedTerrainMaterial->BuildXMLElement(parser) );
		}
	}

	return elementOut;
}

bool RoadXML::SubNetworkElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kRoadNetworkTag ||
		tagName == kGroundsTag ||
		tagName == kMaterialsTag ||
		tagName == kMarkingsTag ||
		tagName == kProfilesTag ||
		tagName == kIntersectionsTag ||
		tagName == kTracksTag ||
		tagName == kTrafficLightGroupsTag ||
		tagName == kTrajectoryPartsTag ||
		tagName == kTrajectoriesTag || 
		tagName == kTerrainContoursTag)
	{
		LoadChildren( childElement, parser );
	}
	else if ( tagName == kTerrainContourTag)
	{
		std::vector< stk::Vector3 > contour;
		LoadContour( contour, childElement);

		mTerrainContours.push_back( contour );

	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if (tagName == kIntersectionTag)
			{
				mIntersectionArray.push_back( dynamic_cast<IntersectionElement*>(newElement.Get()) );
			}
			else if(tagName == kTrackTag)
			{
				mTrackArray.push_back( dynamic_cast<TrackElement*>(newElement.Get()) );
			}
			else if (tagName == kTrafficLightGroupTag)
			{
				mTrafficLightGroupArray.push_back( dynamic_cast<TrafficLightGroupElement*>(newElement.Get()) );
			}
			else if(tagName == kExternalTerrainTag)
			{
				mTerrain = dynamic_cast<ExternalFileElement*>(newElement.Get());
			}
			else if(tagName == kExternalSurfaceTag)
			{
				mRollingSurface = dynamic_cast<ExternalFileElement*>(newElement.Get() );

			}
			else if (tagName == kRollingSurfacePropsTag)
			{
				mRollingSurfacePropsElement = dynamic_cast<RollingSurfacePropsElement*>(newElement.Get());
			}
			else if(tagName == kExternalPhysicalSurfaceTag)
			{
				mPhysicalSurface = dynamic_cast<ExternalFileElement*>(newElement.Get());
			}
			
			else if(tagName == kProxyFileTag)
			{
				mProxyFile = dynamic_cast<ExternalFileElement*>(newElement.Get() );
			}
			else if (tagName == kProfileTag)
			{
				mProfileArray.push_back( dynamic_cast<ProfileElement*>(newElement.Get()) );
			}
			else if (tagName == kGroundTag)
			{
				mGroundArray.push_back( dynamic_cast<GroundElement*>(newElement.Get()) );
			}
			else if (tagName == kMarkingTag)
			{
				mMarkingArray.push_back( dynamic_cast<MarkingElement*>(newElement.Get()) );
			}
			else if (tagName == kTrajectoryTag)
			{
				myTrajectoryArray.push_back( dynamic_cast<TrajectoryElement*>(newElement.Get()) );
			}
			else if (tagName == kTrajectoryPartTag)
			{
				myTrajectoryPartArray.push_back( dynamic_cast<TrajectoryPartElement*>(newElement.Get()) );
			}
			else if (tagName == kGeneratedTerrainMaterialTag )
			{
				mGeneratedTerrainMaterial = dynamic_cast<GeneratedTerrainMaterialElement*>(newElement.Get());
			}
			else if (tagName == kMaterialTag)
			{
				mMaterialArray.push_back( dynamic_cast<MaterialElement*>(newElement.Get()) );
			}
		}
		else 
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

RoadXML::SubNetworkExitElement::SubNetworkExitElement()
{
}

bool RoadXML::SubNetworkExitElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser )
{
	elem->GetStringAttribute(kSubNetworkNameTag, mSubNetworkName);
	elem->GetStringAttribute(kTrackNameTag, mTrackName);
	elem->GetStringAttribute(kTrackBoundInfoTag, mTrackExtremity);

	return true;
}

IDOMElement* RoadXML::SubNetworkExitElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* domElement = parser->CreateDOMElement(GetTagName()); 

	if( domElement )
	{
		domElement->SetStringAttribute(kSubNetworkNameTag, mSubNetworkName);
		domElement->SetStringAttribute(kTrackNameTag, mTrackName);
		domElement->SetStringAttribute(kTrackBoundInfoTag, mTrackExtremity);
	}

	return domElement;
}

/////////////////////////////////////////////////////////////////////////////////

RoadXML::SubNetworkJunctionElement::SubNetworkJunctionElement()
{
}

bool RoadXML::SubNetworkJunctionElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mExits.clear();
	return ConnectionElement::LoadFromXMLElement( elem, parser);
}

IDOMElement* RoadXML::SubNetworkJunctionElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = ConnectionElement::BuildXMLElement(parser);

	BuildChildrenArray( mExits, kSubNetworkExitsTag, elementOut, parser);

	return elementOut;
}

bool RoadXML::SubNetworkJunctionElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kSubNetworkExitsTag )
	{
		LoadChildren( childElement, parser );
	}
	else if( tagName == kSubNetworkExitTag )
	{
		CountedPtr<Element> newElement = CreateElement(tagName);

		if( newElement &&
			newElement->LoadFromXMLElement( childElement, parser ) )
		{
			mExits.push_back( dynamic_cast<SubNetworkExitElement*>( newElement.Get() ) );
		}
		else
		{
			return false;
		}

		return true;
	}
	
	return ConnectionElement::LoadChild(childElement, parser);
}


/////////////////////////////////////////////////////////////////////////////////

void RoadXML::IntersectionElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent)
	{
		parent->mIntersectionArray.updateKey( this, oldname, name );
	} 
}

bool RoadXML::IntersectionElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mBannedLinks.clear();

	elem->GetStringAttribute(kMaterialNameTag, mMaterialName);

	return ConnectionElement::LoadFromXMLElement( elem, parser);
}

IDOMElement* RoadXML::IntersectionElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
	
	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetDoubleAttribute(kXTag, mPosition.x);
	elementOut->SetDoubleAttribute(kYTag, mPosition.y);
	elementOut->SetDoubleAttribute(kZTag, mPosition.z);
	if( mMaterialName.size() )
		elementOut->SetStringAttribute(kMaterialNameTag, mMaterialName);

	BuildChildrenArray( mClippedDataArray, kClippedDatasTag, elementOut, parser);
	BuildChildrenArray(mContours, kContoursTag, elementOut, parser);


	if( mGroundName.size() )
		elementOut->SetStringAttribute(kGroundNameTag, mGroundName);
	
	BuildChildrenArray( mBannedLinks, kBannedLinksTag, elementOut, parser);

	return elementOut;
}

bool RoadXML::IntersectionElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kBannedLinksTag)
	{
		LoadChildren( childElement, parser );
	}
	else if(tagName == kLanePairTag) 
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if (tagName == kLanePairTag) 
				mBannedLinks.push_back( dynamic_cast<LanePairElement*>(newElement.Get()) );
		}
		else 
			return false;

		return true;
	}

	return ConnectionElement::LoadChild( childElement, parser );
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::ContourPieceElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetStringAttribute(kStartAnchorTrackTag, mStartTrackAnchor.mTrackName);
	iDOMElem->GetBooleanAttribute(kIsStartAnchorOnTrackEndTag, mStartTrackAnchor.mIsOnTrackEnd);
	iDOMElem->GetBooleanAttribute(kIsStartAnchorOnLeftSideTag, mStartTrackAnchor.mIsOnLeftSide);
	
	iDOMElem->GetStringAttribute(kEndAnchorTrackTag, mEndTrackAnchor.mTrackName);
	iDOMElem->GetBooleanAttribute(kIsEndAnchorOnTrackEndTag, mEndTrackAnchor.mIsOnTrackEnd);
	iDOMElem->GetBooleanAttribute(kIsEndAnchorOnLeftSideTag, mEndTrackAnchor.mIsOnLeftSide);

	LoadChildren(iDOMElem, parser);

	return true;
}

IDOMElement* RoadXML::ContourPieceElement::BuildXMLElement(IDOMParser* parser)
{
	if (mPolyLine->mVertexArray.empty())
		return NULL;

	IDOMElement* domElement = parser->CreateDOMElement(GetTagName()); 
	
	if (domElement)
	{
		domElement->AddChild(mPolyLine->BuildXMLElement(parser));

		domElement->SetStringAttribute(kStartAnchorTrackTag, mStartTrackAnchor.mTrackName);
		domElement->SetBooleanAttribute(kIsStartAnchorOnTrackEndTag, mStartTrackAnchor.mIsOnTrackEnd);
		domElement->SetBooleanAttribute(kIsStartAnchorOnLeftSideTag, mStartTrackAnchor.mIsOnLeftSide);
		
		domElement->SetStringAttribute(kEndAnchorTrackTag, mEndTrackAnchor.mTrackName);
		domElement->SetBooleanAttribute(kIsEndAnchorOnTrackEndTag, mEndTrackAnchor.mIsOnTrackEnd);
		domElement->SetBooleanAttribute(kIsEndAnchorOnLeftSideTag, mEndTrackAnchor.mIsOnLeftSide);
	}

	return domElement;
}

bool RoadXML::ContourPieceElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if (newElement && newElement->LoadFromXMLElement(childElement, parser))
		mPolyLine = dynamic_cast<PolyLineElement*>(newElement.Get());
	else 
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::LanePairElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kFromTrackTag, mFromLane.mTrackName);
	elem->GetStringAttribute(kFromLaneTag, mFromLane.mLaneName);
	elem->GetStringAttribute(kFromTrackBoundInfoTag, mFromLane.mTrackExtremity);

	elem->GetStringAttribute(kToTrackTag, mToLane.mTrackName);
	elem->GetStringAttribute(kToLaneTag, mToLane.mLaneName);
	elem->GetStringAttribute(kToTrackBoundInfoTag, mToLane.mTrackExtremity);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::LanePairElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kFromTrackTag, mFromLane.mTrackName);
	elementOut->SetStringAttribute(kFromLaneTag, mFromLane.mLaneName);
	elementOut->SetStringAttribute(kFromTrackBoundInfoTag, mFromLane.mTrackExtremity);

	elementOut->SetStringAttribute(kToTrackTag, mToLane.mTrackName);
	elementOut->SetStringAttribute(kToLaneTag, mToLane.mLaneName);
	elementOut->SetStringAttribute(kToTrackBoundInfoTag, mToLane.mTrackExtremity);

	return elementOut;
}

bool RoadXML::LanePairElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////



bool RoadXML::ConnectionElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mClippedDataArray.clear();
	mContours.clear();

	elem->GetStringAttribute(kNameTag, mName);
	elem->GetDoubleAttribute(kXTag, mPosition.x);
	elem->GetDoubleAttribute(kYTag, mPosition.y);
	elem->GetDoubleAttribute(kZTag, mPosition.z);

	elem->GetDoubleAttribute(kUVScaleXTag, mUVScale.x);
	elem->GetDoubleAttribute(kUVScaleYTag, mUVScale.y);
	
	elem->GetDoubleAttribute(kUVOffsetXTag, mUVOffset.x);
	elem->GetDoubleAttribute(kUVOffsetYTag, mUVOffset.y);

	elem->GetDoubleAttribute(kUVHeadingTag, mUVHeading);
	
	elem->GetStringAttribute(kGroundNameTag, mGroundName);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::ConnectionElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
	
	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetDoubleAttribute(kXTag, mPosition.x);
	elementOut->SetDoubleAttribute(kYTag, mPosition.y);
	elementOut->SetDoubleAttribute(kZTag, mPosition.z);

	BuildChildrenArray( mClippedDataArray, kClippedDatasTag, elementOut, parser);
	BuildChildrenArray(mContours, kContoursTag, elementOut, parser);

	if (mMaterial)
	{
		elementOut->AddChild(mMaterial->BuildXMLElement(parser));
		
		elementOut->SetDoubleAttribute(kUVScaleXTag, mUVScale.x);
		elementOut->SetDoubleAttribute(kUVScaleYTag, mUVScale.y);
		
		elementOut->SetDoubleAttribute(kUVOffsetXTag, mUVOffset.x);
		elementOut->SetDoubleAttribute(kUVOffsetYTag, mUVOffset.y);

		elementOut->SetDoubleAttribute(kUVHeadingTag, mUVHeading);
	}
	if( mGroundName.size() )
		elementOut->SetStringAttribute(kGroundNameTag, mGroundName);

	return elementOut;
}

bool RoadXML::ConnectionElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kClippedDatasTag ||
		tagName == kContoursTag )
	{
		LoadChildren( childElement, parser );
	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if (tagName == kMarkClippedDataTag) 
				mClippedDataArray.push_back( dynamic_cast<MarkClippedDataElement*>(newElement.Get()) );
			else if (tagName == kContourPieceTag) 
				mContours.push_back( dynamic_cast<ContourPieceElement*>(newElement.Get()) );
			else if (tagName == kMaterialTag)
				mMaterial = dynamic_cast<MaterialElement*>(newElement.Get()) ;
		}
		else 
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::RoadPieceElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetStringAttribute(kSubNetworkNameTag, mSubNetworkName);
	elem->GetStringAttribute(kTrackNameTag, mTrackName);
	elem->GetBooleanAttribute(kIsTrackStart, mIsTrackStart);

	return true;
}

IDOMElement* RoadXML::RoadPieceElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kSubNetworkNameTag, mSubNetworkName);
	elementOut->SetStringAttribute(kTrackNameTag, mTrackName);
	elementOut->SetBooleanAttribute(kIsTrackStart, mIsTrackStart);

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::RoadElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mRoadPieceArray.clear();

	elem->GetStringAttribute(kNameTag, mName);
	elem->GetIntegerAttribute(kPriorityLevelTag, mPriorityLevel);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::RoadElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetStringAttribute(kNameTag, mName);
	elementOut->SetIntegerAttribute(kPriorityLevelTag, mPriorityLevel);

	BuildChildrenArray( mRoadPieceArray, kRoadPiecesTag, elementOut, parser);

	return elementOut;
}

bool RoadXML::RoadElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	if( tagName == kRoadPiecesTag )
	{
		LoadChildren( childElement, parser );
	}
	else
	{
		CountedPtr<Element> newElement = CreateElement(tagName);
		if( newElement &&
			newElement->LoadFromXMLElement(childElement, parser) )
		{
			if( tagName == kRoadElementTag )
				mRoadPieceArray.push_back( dynamic_cast<RoadPieceElement*>(newElement.Get()) );
		}
		else 
			return false;
	}

	return true;
}

////// geolocalisation element //////////////////////////////////////////


bool RoadXML::GeolocalisationElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{

	// attribs in the Geolocalisation tag

	elem->GetUIntegerAttribute(kEpsgCodeTag, mEpsgCode);
	elem->GetDoubleAttribute(kAzimuthTag, mAzimuth);
	elem->GetStringAttribute(kWellKnownTextTag, mWKT);
	elem->GetDoubleAttribute(kTimeOffsetTag, mTimeOffset);
	std::replace( mWKT.begin(), mWKT.end(), '\'', '\"');

	LoadChildren( elem, parser );
	
	return true;
}

bool RoadXML::GeolocalisationElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	if(tagName==kTranslationTag)
	{
		childElement->GetDoubleAttribute(kXTag, mCenteringTranslation.x);
		childElement->GetDoubleAttribute(kYTag, mCenteringTranslation.y);
		childElement->GetDoubleAttribute(kZTag, mCenteringTranslation.z);
	}
	
	return true;
}


IDOMElement* RoadXML::GeolocalisationElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetIntegerAttribute(kEpsgCodeTag, mEpsgCode);

	elementOut->SetDoubleAttribute(kAzimuthTag, mAzimuth);

	elementOut->SetDoubleAttribute(kTimeOffsetTag, mTimeOffset);

	std::string str = mWKT;
	std::replace( str.begin(), str.end(), '\"', '\'');
	elementOut->SetStringAttribute(kWellKnownTextTag, str );

	
	IDOMElement* elementOutTranslationChild = parser->CreateDOMElement(kTranslationTag);
	elementOutTranslationChild->SetDoubleAttribute(kXTag, mCenteringTranslation.x );
	elementOutTranslationChild->SetDoubleAttribute(kYTag, mCenteringTranslation.y);
	elementOutTranslationChild->SetDoubleAttribute(kZTag, mCenteringTranslation.z);
	elementOut->AddChild( elementOutTranslationChild );

	return elementOut;
}
