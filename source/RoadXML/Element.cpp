#include "RoadXML/Element.h"

#include "RoadXML/CountedPtr.h"
#include "RoadXML/RoadXMLElements.h"


void RoadXML::Element::LoadChildren( IDOMElement* parent, IDOMParser* parser )
{
	CountedPtr<IDOMElement> childElement = parent->GetFirstChildElement();
	if (!childElement)
		return;

	while (childElement)
	{
		LoadChild(childElement, parser); //writes data into the object
		childElement = childElement->GetNextSiblingChildElement();
	}
}

namespace RoadXML
{

	CountedPtr<Factory> gFactory = NULL;

	Element* Factory::Create( const std::string& tagName )
	{
		if( tagName == kFrameTag )					return new FrameElement();
		if( tagName == kXYCurveTag )				return new XYCurveElement();
		if( tagName == kSZCurveTag )				return new CartesianCurveElement(kSZCurveTag);
		if( tagName == kBankingCurveTag )			return new CartesianCurveElement(kBankingCurveTag);
		if (tagName == kSurfaceModifierTag)			return new SurfaceModifierElement();
		if (tagName == kObstacleTag)				return new ObstacleElement();
		if (tagName == kSequenceTag)				return new SequenceElement(tagName);
		if (tagName == kSequenceYTag)				return new SequenceElement(tagName);
		if (tagName == kSequenceZTag)				return new SequenceElement(tagName);
		if (tagName == kTrackScopeTag)				return new TrackScopeElement();
		if (tagName == kTrackClipTag)				return new TrackClipElement();
		if (tagName == kTrackClippedDataTag)		return new TrackClippedDataElement();
		if (tagName == kMarkClippedDataTag)			return new MarkClippedDataElement();

		if (tagName == kNetworkTag)					return new NetworkElement();
		if (tagName == kRoadTag)					return new RoadElement();
		if (tagName == kRoadElementTag)				return new RoadPieceElement();
		if (tagName == kIntersectionTag)			return new IntersectionElement();
		if (tagName == kExternalElementTag)			return new ExternalFileElement(tagName);
		if( tagName == kExternalSurfaceTag)			return new ExternalFileElement(tagName);
		if( tagName == kExternalTerrainTag)			return new ExternalFileElement(tagName);
		if( tagName == kExternalPhysicalSurfaceTag)	return new ExternalFileElement(tagName);
		if( tagName == kProxyFileTag)				return new ExternalFileElement(tagName);
		if (tagName == kLanePairTag)				return new LanePairElement();
		if (tagName == kTrackTag)					return new TrackElement();
		if (tagName == kSubNetworkJunctionTag)		return new SubNetworkJunctionElement();
		if (tagName == kSubNetworkTag)				return new SubNetworkElement();
		if (tagName == kSubNetworkExitTag)			return new SubNetworkExitElement();

		if (tagName == kPolynomialTag)				return new PolynomialFunctionElement();
		if (tagName == kSegmentTag)					return new SegmentElement();
		if (tagName == kCircleArcTag)				return new CircleArcElement();
		if( tagName == kClothoArcTag )				return new ClothoidArcElement();
		if (tagName == kClothoidSplineTag)			return new ClothoidSplineElement();
		if (tagName == kPolyLineTag)				return new PolyLineElement();

		if (tagName == kLightTag)					return new LightElement();
		if (tagName == kRoadSignTag)				return new RoadSignElement();
		if (tagName == kTimerTag)					return new TimerElement();
		if (tagName == kTrafficLightGroupTag)		return new TrafficLightGroupElement();
		if (tagName == kTrafficLightAnimationTag)	return new TrafficLightAnimationElement();
		if (tagName == kTrafficLightStateTag)		return new TrafficLightStateElement();	

		if (tagName == kPortionTag)					return new PortionElement();	

		if (tagName == kProfileTag)					return new ProfileElement();
		if (tagName == kLaneTag)					return new LaneElement();
		if (tagName == kLaneBorderTag)				return new LaneBorderElement();
		if (tagName == kDecorationTag)				return new DecorationElement();
		if (tagName == kMarkingTag)					return new MarkingElement();
		if (tagName == kVehicleTypeTag)				return new VehicleTypeElement();


		if (tagName == kRoughnessTag)				return new RoughnessElement();
		if (tagName == kRoughnessAnalyticTag)		return new AnalyticElement();
		if (tagName == k2x2DPointTag)				return new Point2x2DElement();
		if (tagName == kScale2x2DPointsTag)			return new Point2x2DElement(1., 1., 1., 1., 1., tagName);
		if (tagName == kOffset2x2DPointsTag)		return new Point2x2DElement(tagName);

		if (tagName == kGroundTag)					return new GroundElement();
		if (tagName == kUVMappingTag)				return new UVMappingElement();
		if (tagName == kMaterialTag)				return new MaterialElement();
		if (tagName == kDiffuseTag)					return new MaterialChanelElement(kDiffuseTag);
		if (tagName == kAmbientTag)					return new MaterialChanelElement(kAmbientTag);
		if (tagName == kSpecularTag)				return new MaterialChanelElement(kSpecularTag);
		if (tagName == kEmissiveTag)				return new MaterialChanelElement(kEmissiveTag);

		if (tagName == kNormalMapTag)				return new MaterialChanelElement(kNormalMapTag);
		if (tagName == kSpecularMapTag)				return new MaterialChanelElement(kSpecularMapTag	);
		if (tagName == kReflectionMapTag)			return new MaterialChanelElement(kReflectionMapTag);
		if (tagName == kAmbientOcclusionMapTag)		return new MaterialChanelElement(kAmbientOcclusionMapTag);

		if (tagName == kContourPieceTag)			return new ContourPieceElement();
		if (tagName == kGeolocalisationTag)			return new GeolocalisationElement();

		if (tagName == kTrajectoryTag)				return new TrajectoryElement();
		if (tagName == kTrajectoryPartTag)			return new TrajectoryPartElement();

		if (tagName == kTrajectoryElementTag)		return new TrajectoryPieceElement();
		if (tagName == kGeneratedTerrainMaterialTag) return new GeneratedTerrainMaterialElement();

		if (tagName == kRollingSurfacePropsTag)				return new RollingSurfacePropsElement();
		if (tagName == k3DLayersToExportTag)				return new Export3DLayersElement();
		if (tagName == k3DRoadsGenerationElementTag)		return new Generate3DRoadsElement();
		if (tagName == kSolExportOptionsElementTag)			return new SolExportOptionsElement();


		return NULL;// Unknown element
	}
}

RoadXML::Element* RoadXML::CreateElement( const std::string& tagName )
{
	if( !gFactory )
		gFactory = new Factory();

	return gFactory->Create( tagName );
}

//! Replace the default factory by a specific one
void RoadXML::SetFactory( Factory* factory )
{
	gFactory = factory;
}

RoadXML::Factory* RoadXML::GetFactory()
{
	return RoadXML::gFactory;
}
