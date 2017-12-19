#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLGroundElements.h"
#include "RoadXMLMaterialElements.h"
#include "RoadXMLProfileElements.h"
#include "RoadXMLSignalElements.h"
#include "RoadXMLTrackElements.h"
#include "RoadXMLTrajectoryElements.h"
#include "RoadXMLTrajectoryPartElements.h"
#include "RoadXMLTags.h"
#include "CountedPtr.h"
#include "HashVector.h"
#include "stk/Macro.h"

#include <vector>

namespace RoadXML
{
	//! Load a RoadXML file. Returns the top most element
	ROADXML_API RoadXML::Element* Load( const char * sInputFileName );

	//! Save a RoadXML file
	ROADXML_API bool Save( Element* pRoadXML, const char * sOutputFilePath );

	//! Create a parser with a RoadXML root node and the right RoadXML version number 
	ROADXML_API IDOMParser* CreateRoadXMLParser(const char * sOutputFilePath, bool rapid = false);

	//! returns the DOM root node of a DOM document
	ROADXML_API IDOMElement* LoadRootElement( IDOMParser* pParser );

	//! A contour piece of an Intersection.
	/*! A contour piece is defined by
		- a start track.
		- some user defined points
		- an end track
	*/
	class ROADXML_API ContourPieceElement : public Element
	{
	public:
		ContourPieceElement(){}

		struct ROADXML_API TrackAnchor
		{
			TrackAnchor() : mIsOnTrackEnd(false), mIsOnLeftSide(false) {}
			std::string mTrackName;
			bool mIsOnTrackEnd;
			bool mIsOnLeftSide;
			std::string mSubNetworkName;
		};

		CountedPtr<PolyLineElement> mPolyLine;	//coords relative to intersection center
		TrackAnchor mStartTrackAnchor;
		TrackAnchor mEndTrackAnchor;

		virtual const std::string&	GetTagName() const {return kContourPieceTag;}
		virtual bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected :
		virtual ~ContourPieceElement(){}
		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);
	};

	//! A pair of lane on a Intersection.
	/*! A lane is found by:
		- the name of the Track.
		- the extremity of the Track used (the start or the end of the Track).
		- the name of the Lane in the first or the last Profile of the Track (depending on the Track extremity).
	*/
	class ROADXML_API LanePairElement : public Element
	{
	public:
		LanePairElement(){}

		struct LaneData
		{
			std::string mTrackName;
			std::string	mTrackExtremity; //! Can be either "start" or "end"
			std::string mLaneName;	//! name of the lane in the first or the last Profile of the Track, depending on the mTrackExtremity value
		};
		LaneData mFromLane;
		LaneData mToLane;

		virtual const std::string&	GetTagName() const{ return kLanePairTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~LanePairElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Connection between tracks 
	class ROADXML_API ConnectionElement : public Element
	{
	public:
		ConnectionElement() : mUVHeading(0.0), mUVScale(1.0, 1.0), mUVOffset(0.0, 0.0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		const	std::string& GetGroundName() const {return mGroundName;}
		void	SetGroundName(const std::string& name) {mGroundName = name;}

		const stk::Vector2& GetUVScale() const { return	mUVScale; }
		void				SetUVScale(const stk::Vector2& value) { mUVScale = value; }

		const stk::Vector2&	GetUVOffset() const {return mUVOffset; }
		void				SetUVOffset(const stk::Vector2& value) {mUVOffset = value; }

		double				GetUVHeading() const { return mUVHeading; }
		void				SetUVHeading(double value) { mUVHeading = value; }

		bool				IsSpecificMaterial() const {return (mMaterial!=NULL);}
		void				RemoveMaterial() {mMaterial.Release();}

		//! Position in X, Y, Z in meters of the intersection
		stk::Vector3 mPosition;


		//! An array of data attached to the Intersection
		/*! 3D objects, Signals, Obstacles, SurfaceModifiers, ...
			The position of the clipped data is relative to the Connection position.
		*/
		std::vector<CountedPtr<MarkClippedDataElement> > mClippedDataArray;

		//! Contours of the intersection
		/*! List of polylines connected to track ends, defining the contours of the junction
		*/
		std::vector<CountedPtr<ContourPieceElement> > mContours;	// ! contains coordinates relative to connection center

		//! 3D Material
		CountedPtr<MaterialElement>	mMaterial;

		virtual const std::string&	GetTagName() const = 0;
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:

		virtual ~ConnectionElement(){}
		
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

	protected:		
		std::string mName;
		
		//! Planar projection of the texture
		stk::Vector2	mUVScale;
		stk::Vector2	mUVOffset;
		double			mUVHeading;

		//! Physical properties
		std::string		mGroundName;
	};



	//! Intersection between tracks
	class ROADXML_API IntersectionElement : public ConnectionElement
	{
	public:
		IntersectionElement(){}

		virtual const std::string&	GetTagName() const{ return kIntersectionTag; }
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );
		
		std::string mMaterialName;

		//! Optional information regarding the traffic
		/*! List of the forbidden exits on the Intersection.
		*/
		std::vector<CountedPtr<LanePairElement> > mBannedLinks;
		
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
	protected:

		virtual ~IntersectionElement(){}
		
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	class ROADXML_API SubNetworkExitElement : public Element
	{
	public:
		SubNetworkExitElement();

		virtual const std::string&	GetTagName() const{return kSubNetworkExitTag;}
		virtual bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~SubNetworkExitElement(){}

		std::string mSubNetworkName;

		std::string mTrackName;

		std::string mTrackExtremity;

	};

	//! Junction between 2 subnetworks
	class ROADXML_API SubNetworkJunctionElement : public ConnectionElement
	{
	public:
		SubNetworkJunctionElement();


		virtual const std::string&	GetTagName() const{ return kSubNetworkJunctionTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
	protected:

		virtual ~SubNetworkJunctionElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

	protected:
		//! Linked SubNetwork exits
		std::vector<CountedPtr<SubNetworkExitElement> > mExits;

	};

	//! Road piece
	/*! A Road is composed of RoadPieces.
		Tracks are found by their name inside a SubNetwork.
	*/
	class ROADXML_API RoadPieceElement : public Element
	{
	public:
		RoadPieceElement() : mIsTrackStart(true) {}

		std::string mSubNetworkName;
		std::string mTrackName;
		//! Start of the track
		/*! This variable is true if the route goes from beginning to end,
		    for this road piece, false otherwise.
		*/
		bool		mIsTrackStart;

		virtual const std::string&	GetTagName() const{ return kRoadElementTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~RoadPieceElement(){}
	};

	//! Road in the network
	/*!	A Road is a composition of several Tracks.
	*/
	class ROADXML_API RoadElement : public Element
	{
	public:
		RoadElement() : mPriorityLevel(0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		size_t mPriorityLevel;
		std::vector<CountedPtr<RoadPieceElement> > mRoadPieceArray;

		virtual const std::string&	GetTagName() const{ return kRoadTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~RoadElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! tile of the network
	class ROADXML_API SubNetworkElement : public Element
	{
	public:
		SubNetworkElement() : mHeading(0), mGeneratedTerrainMaterial(0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		//! Ground array: contains all the ground properties used by the subnetwork.
		/*! The profile's lanes contain a reference to this data. 
		*/
		HashVector<CountedPtr<GroundElement> > mGroundArray;

		HashVector<CountedPtr<MaterialElement> > mMaterialArray;

		//! Marking array: contains all the road marking used by the subnetwork.
		/*! The profile's laneBorders contain a reference to this data. 
		*/
		HashVector<CountedPtr<MarkingElement> > mMarkingArray;

		//! Profile array: road cross sections of this subnetwork.
		/*! These are referenced by the Portions. 
		*/
		HashVector<CountedPtr<ProfileElement> > mProfileArray;

		//! An array of data attached to the SubNetwork
		/*! 3D objects, Signals, Obstacles, SurfaceModifiers, ...
			The position of the clipped data is relative to the SubNetwork position.

			WARNING: these objects are global to the subnetwork. They're not relative to a Track or an Intersection.
			It's usually used for the landscape features, not for the roads ones.
		*/
		HashVector<CountedPtr<MarkClippedDataElement> > mClippedDataArray;

		HashVector<CountedPtr<IntersectionElement> > mIntersectionArray;
		HashVector<CountedPtr<TrackElement> > mTrackArray;
		HashVector<CountedPtr<TrafficLightGroupElement> > mTrafficLightGroupArray; //! Synchronization between the traffic lights
		
		HashVector<CountedPtr<TrajectoryElement> > myTrajectoryArray;
		HashVector<CountedPtr<TrajectoryPartElement> > myTrajectoryPartArray;

		CountedPtr<ExternalFileElement> mTerrain; //! Terrain external file
		CountedPtr<ExternalFileElement> mRollingSurface; //! Surface external file
		CountedPtr<ExternalFileElement> mPhysicalSurface; //! Physic surface
	
		//! File containing the SubNetwork.
		/*! Optional attribute for SubNetwork defined in an external file. 
		*/
		CountedPtr<ExternalFileElement> mProxyFile;
		
		virtual const std::string&	GetTagName() const{ return kSubNetworkTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		virtual const stk::Vector3 & GetPosition() const { return mPosition; }
		virtual void SetPosition(const stk::Vector3 & position) { mPosition = position; }
		virtual const double GetHeading() const { return mHeading; }
		virtual void SetHeading(double heading) { mHeading = heading; }
		
		typedef std::vector<stk::Vector3> Contour;
		virtual std::vector<Contour>& GetTerrainContours(){return mTerrainContours;}

		virtual GeneratedTerrainMaterialElement* GetGeneratedTerrainMaterial(){return mGeneratedTerrainMaterial;}

	protected:

		void Clear();

		//! SubNetwork unique name in the Network.
		/*! A SubNetwork has a unique name in the Network. 
		*/
		std::string		mName;

		//! Position of the SubNetwork in the Network.
		stk::Vector3	mPosition;
		double			mHeading;

		std::vector<Contour> mTerrainContours;
		
		CountedPtr<GeneratedTerrainMaterialElement> mGeneratedTerrainMaterial;

		virtual ~SubNetworkElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	class ROADXML_API GeolocalisationElement : public Element
	{
	public:
		GeolocalisationElement():mWKT(""), mEpsgCode(0), mCenteringTranslation(0,0,0), mAzimuth(0), mTimeOffset(0.0) {}

		virtual const std::string&	GetTagName()const{ return kGeolocalisationTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement*, IDOMParser*);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
		bool						LoadChild( IDOMElement* childElement, IDOMParser* parser );
		
		stk::Vector3 mCenteringTranslation;
		
		std::string mWKT;		//WKT text with substitution : " -> '
		unsigned int mEpsgCode;			//epsg code if recognized (std proj...)

		double mAzimuth;

		double mTimeOffset;
	};

	class ROADXML_API NetworkElement : public Element
	{
	public:
		NetworkElement()
		:mGeolocalisation(0), mTerrain(0), mGeneratedTerrainMaterial(0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		std::string mHandType;

		std::vector<CountedPtr<SubNetworkElement> > mSubNetworkArray;

		CountedPtr<GeolocalisationElement> mGeolocalisation;

		std::vector<CountedPtr<SubNetworkJunctionElement> > mSubNetworkJunctionArray;

		//! Roads running through the network.
		/*! A road is a collection of Tracks of the network. Road pieces can 
			be contained by several SubNetworks. 
		*/
		std::vector<CountedPtr<RoadElement> > mRoadArray;

		CountedPtr<ExternalFileElement> mTerrain; //! Terrain external file
		CountedPtr<ExternalFileElement> mRollingSurface; //! Rolling surface
		CountedPtr<ExternalFileElement> mPhysicalSurface; //! Physic surface

		virtual GeneratedTerrainMaterialElement* GetGeneratedTerrainMaterial(){return mGeneratedTerrainMaterial;}

		virtual const std::string&	GetTagName() const {return kNetworkTag;}
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~NetworkElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

		CountedPtr<GeneratedTerrainMaterialElement> mGeneratedTerrainMaterial;
	};

	//! Top level element
	//class RoadXMLElement : public Element
	//{
	//public:
	//	RoadXMLElement() {}

	//	std::string mVersion;
	//	std::string mVersionRevision;
	//	CountedPtr<Element> mRootElem; // Can be a NetworkElement, a ProfileElement, ...

	//	virtual const std::string&	GetTagName() const {return kRoadXMLTag;}
	//	virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
	//	virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	//protected:
	//	virtual ~RoadXMLElement(){}

	//	virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	//};

}


