#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "RoadXMLClippedDataElements.h"
#include "RoadXML/RoadXMLCurveElements.h"
#include "HashVector.h"
#include "CountedPtr.h"
#include "stk/Macro.h"
#include <vector>

namespace RoadXML
{
	class SubNetworkElement;

	//! A portion of the Track.
	class ROADXML_API PortionElement : public Element
	{
	public:
		PortionElement():mEndDistance(0) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		virtual const std::string&	GetStartProfileName() const { return mStartProfileName; }
		virtual void				SetStartProfileName( const std::string& name ) {mStartProfileName=name;}

		virtual const std::string&	GetEndProfileName() const { return mEndProfileName; }
		virtual void				SetEndProfileName( const std::string& name ) {mEndProfileName=name;}

		//! Distance on the XY curve.
		/*! To get the start distance, check the previous portion.
			If there's none, then it's 0
		*/
		double mEndDistance;

		virtual const std::string&	GetTagName() const{ return kPortionTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		std::string mStartProfileName;
		std::string mEndProfileName;

		virtual ~PortionElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! A piece of road between 2 intersections.
	class ROADXML_API TrackElement : public Element
	{
	public:
		TrackElement(): mIsExitAtStart(false), mIsExitAtEnd(false) {}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		std::string mStartIntersectionName; //! Intersection or SubNetworkJunction name. Can be empty.
		std::string mEndIntersectionName; //! Intersection or SubNetworkJunction name. Can be empty.
		
		//! Describes the Track with a name
		/*! This field can be used to store data such as "Highway", "Freeway", or "Autoroute", "Nationale", ...
			It's up to the user to create the categories he wants to use.
			This information doesn't replace the Road class information. A Road is a collection of Tracks associated 
			with a name. So a Track can be describes as "Autoroute A4" and "Europeenne E20" through the Road class.
		*/
		std::string mRoadCategory;

		//! XY axis description
		/*! A parametric curve made of segment, circle arcs, clothoid arcs...
		*/
		CountedPtr<XYCurveElement> mXYCurve;

		//! Elevation profile
		/*! Altitude (Z) of the road function of its abscissa (S).
		*/
		CountedPtr<CartesianCurveElement> mSZCurve;

		//! Road banking of the road.
		/*!	Banking of the road (in Rad) function of its abscissa (S).
		*/
		CountedPtr<CartesianCurveElement> mBankingCurve;

		std::vector<CountedPtr<PortionElement> > mPortions;

		//! An array of data attached to the Track
		/*! RoadSign, Obstacle, SurfaceModifiers, ...
		*/
		std::vector<CountedPtr<TrackClippedDataElement> > mClippedDataArray;

		bool mIsExitAtStart;
		bool mIsExitAtEnd;

		virtual const std::string&	GetTagName() const{ return kTrackTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~TrackElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};
	
}
