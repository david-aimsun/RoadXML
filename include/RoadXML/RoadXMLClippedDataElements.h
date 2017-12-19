#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "RoadXMLSignalElements.h"
#include "CountedPtr.h"
#include "stk/Macro.h"
#include "stk/Mark.h"
#include <vector>
#include <set>

namespace RoadXML
{
	class SubNetworkElement;
	//! Reference on a file.	
	class ROADXML_API ExternalFileElement : public Element
	{
	public:
		ExternalFileElement(const std::string& xmlTag=kExternalElementTag) 
			: mFrame(stk::Vector3(0., 0., 0.), 0., 0., 0.), mXmlTag(xmlTag), mIsFlipped(false) {}

		//! File path
		const std::string&	GetFilePath() const { return mFilePath; }
		void				SetFilePath(const std::string& path) { mFilePath = path; }
		std::string mFilePath;

		//! Indicates if the clipped data is inverted. Used for left-hand/right-hand traffic switching
		bool	mIsFlipped; 

		//! Optional frame
		const stk::Mark&	GetFrame() const { return mFrame; }
		void				SetFrame(const stk::Mark& frame) { mFrame = frame; }

		virtual const std::string&	GetTagName() const{ return mXmlTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~ExternalFileElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

		std::string mXmlTag;

		//! optional local frame
		stk::Mark mFrame;
	};

	//! Local surface modification.	
	/*! Modify the properties of the road. 
		Attached to a Track with a ClippedData.
	*/
	class ROADXML_API SurfaceModifierElement : public Element
	{
	public:
		SurfaceModifierElement() {}

		virtual const std::string&	GetTagName() const{ return kSurfaceModifierTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~SurfaceModifierElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Obstacle on the road.	
	/*! A collision object for the traffic. 
		Attached to a Track with a ClippedData.
	*/
	class ROADXML_API ObstacleElement : public Element
	{
	public:
		ObstacleElement() {}

		std::string mDodgeWay;

		virtual const std::string&	GetTagName() const{ return kObstacleTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~ObstacleElement(){}
	};

	//! Describe a sequence of object.	
	/*! Element used to describe a sequence of object along a Track.
	*/
	class ROADXML_API SequenceElement : public Element
	{
	public:
		//SequenceElement():mStepCount(0),mStepLength(0), mTag(kSequenceTag) {}
		SequenceElement(const std::string& tag):mStepCount(0),mStepLength(0), mTag(tag) {}

		size_t	mStepCount; //! Number of objects in the sequence
		double	mStepLength; //! Distance in meters between two objects in the sequence

		virtual const std::string&	GetTagName() const{ return mTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~SequenceElement(){}
		std::string mTag;
	};

	//! Describe a volume along the track
	/*! In abscissa: starting from mFrom until mFrom+mRange
		In distance to the axis: starting from mLeftLimit until mRightLimit
		In elevation: starting from mBottomLimit until mTopLimit
		The scope can be reduce to a particular way (direct or inverse), or to a particular number of lanes.
	*/
	class ROADXML_API TrackScopeElement : public Element
	{
	public:
		TrackScopeElement():mFrom(0),mTo(0),mLeftLimit(0),mRightLimit(0),mBottomLimit(0),mTopLimit(0) {}

		//! Distance in meters, relative to the origin point (the TrackClip position ).
		/*! If mFrom == kNoLimitNeg, the limit is the begining of the track.
		*/
		double mFrom; 

		//! Distance in meters, relative to the origin point (the TrackClip position ). 
		/*! If mTo == kNoLimitPos, the limit is the end of the track.
		*/
		double mTo;

		//! distance in meters, relative to the origin point (TrackClip).
		/*! If mLeftLimit == kNoLimitNeg, the limit is the left side of the road
		*/
		double mLeftLimit; 

		//! distance in meters, relative to the origin point (TrackClip).
		/*! If mRightLimit == kNoLimitPos, the limit is the right side of the road
		*/
		double mRightLimit;

		//! distance in meters, relative to the origin point (TrackClip).
		/*! If mBottomLimit == kNoLimitNeg, all the values inferior to mTopLimit are in the scope
		*/
		double mBottomLimit;

		//! distance in meters, relative to the origin point (TrackClip).
		/*! If mTopLimit == kNoLimitPos, all the values superior to mBottomLimit are in the scope
		*/
		double mTopLimit;

		std::string mCirculationWay; //!< to limit the scope to 1 way

		//! Limit the scope to some lanes only
		/*! If the array is not empty, the scope is only for these lanes.
		*/
		std::set<size_t> mLaneScope;

		virtual const std::string&	GetTagName() const{ return kTrackScopeTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~TrackScopeElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Position/Orientation on a Track
	/*! The position is described in abscissa, distance and elevation.
	*/
	class ROADXML_API TrackClipElement : public Element
	{
	public:
		TrackClipElement() : mAbscissa(0), mDistance(0), mElevation(0), mIsVertical(true), mHeading(0) {}

		double mAbscissa; //! Abscissa in meters, relative to the beginning of the Track
		double mDistance; //! Distance to the axis in meters
		double mElevation; //! Elevation in meters
		bool	mIsVertical; //! tells wether the object is aligned on the vertical or on the normal to the Track
		double	mHeading; //! heading in rad, in counter-clockwise order. 0 is in the direction of the Track

		virtual const std::string&	GetTagName() const{ return kTrackClipTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~TrackClipElement(){}
	};

	//! Data attached to network elements.
	/*! Base class for the data attached to the Track, Intersection, ...
		Can attached a road sign, a 3D object or other kind of object on a Track.
	*/
	class ROADXML_API ClippedDataElement : public Element
	{
	public:
		//! Set the mass to gNoMass to have a static object.
		static const double gNoMass;

		ClippedDataElement() : mMass(gNoMass), mSequenceOrientation(0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		virtual const double		GetMass() const { return mMass; }
		virtual void				SetMass( double mass ) {mMass=mass;}
		virtual const double		GetSequenceOrientation() const { return mSequenceOrientation; }
		virtual void				SetSequenceOrientation(double sequenceOrientation) { mSequenceOrientation = sequenceOrientation; }

		CountedPtr<SequenceElement>			mSequenceX;
		CountedPtr<SequenceElement>			mSequenceY;
		CountedPtr<SequenceElement>			mSequenceZ;
		CountedPtr<ExternalFileElement>		mObject;
		CountedPtr<RoadSignElement>			mRoadSign;
		CountedPtr<ObstacleElement>			mObstacle;
		CountedPtr<SurfaceModifierElement>	mSurfaceModifier;

		//virtual const std::string&	GetTagName() const{ return kClippedDataTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		//! Object mass. If the mass is == gNoMass, the object is a static object (no part of the physic simulation)
		/*! Mass unit is kilograms (International System of Units).
		*/
		double	mMass;

		// Orientation applyed to the entire sequence, only heading
		/*! Orientation in Radian, defined in [0, 2*PI]
		*/
		double mSequenceOrientation;

		virtual ~ClippedDataElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Data attached to the Track.
	/*! Can attached a road sign, a 3D object or other kind of object on a Track.
	*/
	class ROADXML_API TrackClippedDataElement : virtual public ClippedDataElement
	{
	public:
		CountedPtr<TrackClipElement>		mClip;
		CountedPtr<TrackScopeElement>		mScope;
		
		virtual const std::string&	GetTagName() const{ return kTrackClippedDataTag; }

		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:

		virtual ~TrackClippedDataElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};
	
	//! Data attached to the Track.
	/*! Can attached a road sign, a 3D object or other kind of object on a Track.
	*/
	class ROADXML_API MarkClippedDataElement : virtual public ClippedDataElement
	{
	public:
		MarkClippedDataElement() : mFrame(stk::Vector3(0., 0., 0.), 0., 0., 0.) {}
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		stk::Mark mFrame;
		
		virtual const std::string&	GetTagName() const{ return kMarkClippedDataTag; }

		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:

		virtual ~MarkClippedDataElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};
	
}