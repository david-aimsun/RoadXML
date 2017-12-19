#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "RoadXMLProfileElements.h"
#include "CountedPtr.h"
#include "stk/Macro.h"
#include <vector>

namespace RoadXML
{
	class SubNetworkElement;
	class ROADXML_API TimerElement : public Element
	{
	public:
		TimerElement() {}

		std::string mText;

		virtual const std::string&	GetTagName() const{ return kTimerTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~TimerElement(){}
	};

	class ROADXML_API LightElement : public Element
	{
	public:
		LightElement() {}
	
		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name ) {mName=name;}

		virtual const std::string&	GetColor() const {return mColor;}
		virtual void				SetColor(const std::string& color) {mDirection = color;}

		virtual const std::string&	GetIllumination() const {return mIllumination;}
		virtual void				SetIllumination(const std::string& ill) {mIllumination = ill;}

		virtual const std::string&	GetDirection() const {return mDirection;}
		virtual void				SetDirection(const std::string& type) {mDirection = type;}

		virtual const std::string&	GetVehicle() const {return mVehicle;}
		virtual void				SetVehicle(const std::string& vhc) {mVehicle = vhc;}

		//!< Optional timer: when a light is a timer, it display the number of second before the next light change in the signal
		/*! For a timer that doesn't change color, set the Light color to Red, Green or Orange.
			For a timer that change color according to the current signal status, set its color to NoLightColor
		*/
		CountedPtr<TimerElement> mTimer;

		virtual const std::string&	GetTagName() const{ return kLightTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;
		std::string mColor;
		std::string mIllumination;
		std::string mDirection;
		std::string mVehicle;

		virtual ~LightElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//!< RoadSign	
	/*! A signalisation object. 
		Attached to a Track with a ClippedData.
	*/
	class ROADXML_API RoadSignElement : public Element
	{
	public:
		RoadSignElement() : mValue(0.0), mTemporary(false), mDistance(0.) {}
	
		virtual const std::string&	GetSignType() const {return mSignType;}
		virtual void				SetSignType(const std::string& type) {mSignType = type;}

		virtual const std::string&	GetTextMessage() const {return mTextMessage;}
		virtual void				SetTextMessage(const std::string& value) {mTextMessage = value;}

		virtual const std::string&	GetDirection() const {return mDirection;}
		virtual void				SetDirection(const std::string& type) {mDirection = type;}

		virtual double				GetValue() const {return mValue;}
		virtual void				SetValue(double value) {mValue=value;}

		virtual double				GetDistance() const { return mDistance; }
		virtual void				SetDistance(double value) { mDistance = value; }

		virtual bool				GetTemporary() const { return mTemporary; }
		virtual void				SetTemporary(bool value) { mTemporary=value; }

		virtual const std::string&	GetTargetName() const {return mTargetName;}
		virtual void				SetTargetName(const std::string& type) {mTargetName = type;}

		//!< Target of the sign ( pedestrians, cars, ...)
		CountedPtr<VehicleTypeElement> mVehicleType; 

		//!< Array of lights (usualy 3 for a traffic light: one per color)
		std::vector<CountedPtr<LightElement> > mLightArray;

		virtual const std::string&	GetTagName() const{ return kRoadSignTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~RoadSignElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

		//!< Identification of the road sign. 
		/*! Check also the other parameters   (mDirection, mVehicle,...) to have a more precise
			description of the road sign. For example, a wrong way sign can regard only trucks.
			Or a traffic ligh can be used for pedestrians.
		*/
		std::string mSignType;

		//!< Some signs countain specific messages.
		/*! This message can be display on a board for the driver to see it.
			This message can change with time.
		*/
		std::string mTextMessage;

		//!< Some signs, before an intersection, have an indication on the direction.
		/*! Can be "straight", "left", "right" or a combinaison of these separated by a comma. For example: "straight,right" */
		std::string mDirection; 

		//!< Store data related to the sign ( speed limit, ... ) 
		/*! This value can change with time.
		*/
		double mValue;

		bool mTemporary; //!< For temporary signalisation (road work signs) 

		//!< signal can be signaled in advance
		/*!
			for example a yield in 150m
		*/
		double mDistance;

		//!< Name of another ClippedData targeted by this RoadSign
		/*! The traffic lights sometimes need to animate a 3D objects contained by another ClipData.
		*/
		std::string mTargetName;
	};

	//!< State of the lights for a Traffic Light signal	
	/*! For animation purpose.
	*/
	class ROADXML_API TrafficLightStateElement : public Element
	{
	public:
		TrafficLightStateElement() : mTimeLength(0) {}

		double mTimeLength; //!< time duration of the state
		std::vector<std::string> mIlluminationArray;	//!<  Illumination status for each light of the traffic light

		virtual const std::string&	GetTagName() const{ return kTrafficLightStateTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~TrafficLightStateElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//!< Animation cycle for a single Traffic Light	
	/*! Contains an array of the states of the cycle.
	*/
	class ROADXML_API TrafficLightAnimationElement : public Element
	{
	public:
		TrafficLightAnimationElement(){}

		virtual const std::string& GetRoadSignName() const { return mRoadSignName; }
		const std::string& GetCycleName() const { return mCycleName; }

		void SetRoadSignName(const std::string& name) { mRoadSignName = name; }
		void SetCycleName(const std::string& name) { mCycleName = name; }

		std::vector<CountedPtr<TrafficLightStateElement> > mStates;

		virtual const std::string&	GetTagName() const{ return kTrafficLightAnimationTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mRoadSignName;
		std::string mCycleName;

		virtual ~TrafficLightAnimationElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//!< A group of Traffic Light	
	/*! Contains the animation cycles of a group of Traffic Lights.
		It is possible to define several cycles for the same group.
	*/
	class ROADXML_API TrafficLightGroupElement : public Element
	{
	public:
		TrafficLightGroupElement(){}

		virtual const std::string&	GetName() const { return mName; }
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		//!< All the animations available for this group.	
		/*! There's no special organization for this array: it contains all the animation available.
			An animation has:
				- a CycleName: it determines to which cycle it belongs.
				- a RoadSignName: the name of the traffic light 

			So 1 traffic light can have several cycles.
		*/
		std::vector<CountedPtr<TrafficLightAnimationElement> > mAnimationArray;

		virtual const std::string&	GetTagName() const{ return kTrafficLightGroupTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mName;

		virtual ~TrafficLightGroupElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

}
