#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "RoadXMLMaterialElements.h"
#include "RoadXMLFunctionElements.h"
#include "CountedPtr.h"
#include "stk/Macro.h"
#include <vector>


namespace RoadXML
{
	class SubNetworkElement;
	//! Vehicle description.
	class ROADXML_API VehicleTypeElement : public Element
	{
	public:
		VehicleTypeElement();

		std::string mVehicleCategories;
		//! Some other limitations. They're set to gNoLimit when they're not used.
		static const double gNoLimit;
		double mWeightLimit;
		double mWeightPerAxleLimit;
		double mTrailedWeightLimit;
		double mLengthLimit;
		double mWidthLimit;
		double mHeightLimit;

		virtual const std::string&	GetTagName() const{ return kVehicleTypeTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~VehicleTypeElement(){}
	};

	//! Longitudinal road marking.
	class ROADXML_API MarkingElement : public Element
	{
	public:
		MarkingElement() :
			mWidth(0),
			mLineLength(0),
			mTotLength(0),
			mDoubleMarking(false),
			mDissuasive(false),
			mDestinationSeparation(false),
			mNoStop(false),
			mNoParking(false),
			mAlternate(false),
			mQuality(1.0)
		{}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		std::string	mCategory;
		std::string mLeftToRight; //! Authorisation for a mobile to cross the marking from the left to the right. Can be either "can", "warning" or "cannot"
		std::string mRightToLeft; //! Authorisation for a mobile to cross the marking from the right to the left. Can be either "can", "warning" or "cannot"
		std::string mMaterialName;

		double mWidth;
		double mLineLength; //! length in meters fo the marking line
		double mTotLength; //! total length of the marking repetition

		CountedPtr<MaterialElement>	mMaterial;
		CountedPtr<UVMappingElement> mUVMapping;

		bool mDoubleMarking;
		bool mDissuasive;
		bool mDestinationSeparation;
		bool mNoStop;
		bool mNoParking;
		bool mAlternate;
		double mQuality;

		virtual const std::string&	GetTagName() const{ return kMarkingTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~MarkingElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! 2D shape made of a polyline and a material.
	class ROADXML_API DecorationElement : public Element
	{
	public:
		DecorationElement(){}
		
		std::string mMaterialName;

		CountedPtr<MaterialElement>	mMaterial;
		CountedPtr<UVMappingElement> mUVMapping;
		CountedPtr<PolyLineElement> mPolyLine;

		virtual const std::string&	GetTagName() const{ return kDecorationTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~DecorationElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Lane in Profile
	class ROADXML_API LaneElement : public Element
	{
	public:
		LaneElement() : mSpeedLimit(0){}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		std::string mType;
		std::string mCirculationWay;
		std::string mGroundName;
		std::string mMaterialName;
		double mSpeedLimit;

		CountedPtr<VehicleTypeElement> mVehicleAuth;
		CountedPtr<UVMappingElement> mUVMapping;

		virtual const std::string&	GetTagName() const{ return kLaneTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~LaneElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Border between 2 lanes.
	class ROADXML_API LaneBorderElement : public Element
	{
	public:
		LaneBorderElement() : mDistance(0), mHeight(0), mMarkingOffset(0) {}

		double mDistance;
		double mHeight;

		double mMarkingOffset; //! offset the marking position (in meter)
		std::string mMarkingName;

		CountedPtr<DecorationElement> mDecoration;

		virtual const std::string&	GetTagName() const{ return kLaneBorderTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~LaneBorderElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Road cross profile
	class ROADXML_API ProfileElement : public Element
	{
	public:
		ProfileElement(){}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		std::string mRoadType;
		std::vector<CountedPtr<LaneElement> >		mLaneArray;
		std::vector<CountedPtr<LaneBorderElement> >	mLaneBorderArray;

		virtual const std::string&	GetTagName() const{ return kProfileTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~ProfileElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

}
