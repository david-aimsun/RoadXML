#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "CountedPtr.h"
#include "RoadXMLTags.h"
#include "stk/Macro.h"
#include "RoughnessType.h"

namespace RoadXML
{
	class SubNetworkElement;

	static const char*	ToText(RoadXML::RoughnessType type);
	static RoadXML::RoughnessType	ToGranulosityType(const char* name);

	//! 2x2D point properties
	/*! Description of the ground profile on each wheel line (Left/Right).
	*/
	class ROADXML_API Point2x2DElement : public Element
	{
	public:
		Point2x2DElement(std::string tag = RoadXML::k2x2DPointTag);
		Point2x2DElement(double dist, double Z_Left, double Z_Right, double banking_Left, double banking_Right, std::string tag = RoadXML::k2x2DPointTag);

		double GetCurvDist()	const			{ return mCurvDist; }
		double GetZ_Left()	const				{ return mZ_Left; }
		double GetZ_Right()	const				{ return mZ_Right; }
		double GetBanking_Left()	const		{ return mBanking_Left; }
		double GetBanking_Right()	const		{ return mBanking_Right; }

		
		void SetCurvDist(double d)			{ mCurvDist = d; }
		void SetZ_Left(double d)			{ mZ_Left = d; }
		void SetZ_Right(double d)			{ mZ_Right = d; }
		void SetBanking_Left(double d)		{ mBanking_Left = d; }
		void SetBanking_Right(double d)		{ mBanking_Right = d; }
		void SetTagName(const std::string& tag)				{ mTag = tag; }

		virtual const std::string&	GetTagName() const{ return mTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		double mCurvDist;
		double mZ_Left;
		double mZ_Right;
		double mBanking_Left;
		double mBanking_Right;
		std::string mTag;

	};


	//! Analytic properties
	/*! Granulosity of the ground when the Roughness type is Analytic.
	*/
	class ROADXML_API AnalyticElement : public Element
	{
	public:
		AnalyticElement();

		double GetRoughnessHeight() const		{ return mRoughnessHeight; }
		double GetRoughnessLength() const		{ return mRoughnessLength; }
		double GetRoughnessHeightStdDeviation() const		{ return mRoughnessHeightStdDeviation; }
		double GetRoughnessLengthStdDeviation() const		{ return mRoughnessLengthStdDeviation; }

		void SetRoughnessHeight(double d)	{ mRoughnessHeight = d; }
		void SetRoughnessLength(double d)	{ mRoughnessLength = d; }
		void SetRoughnessHeightStdDeviation(double d)	{ mRoughnessHeightStdDeviation = d; }
		void SetRoughnessLengthStdDeviation(double d)	{ mRoughnessLengthStdDeviation = d; }


		virtual const std::string&	GetTagName() const{ return kRoughnessAnalyticTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		double mRoughnessHeight;
		double mRoughnessLength;
		double mRoughnessHeightStdDeviation;
		double mRoughnessLengthStdDeviation;

	};


	//! Roughness properties
	/*! Description of the ground profile with the granulosity type.
	*/
	class ROADXML_API RoughnessElement : public Element
	{
	public:
		RoughnessElement();

		const RoughnessType										GetRoughnessType() const		{ return mRoughnessType; }
		const CountedPtr<AnalyticElement>&						GetAnalytic() const				{ return mAnalytic; }
		const std::vector<CountedPtr<Point2x2DElement> >&		Get2x2DPointArray()	const		{ return m2x2DPointArray; }
		bool													GetIsPatternRepeated() const	{ return mIsPatternRepeated; }
		const CountedPtr<Point2x2DElement>&						GetScale2x2DPoint() const		{ return mScale; }
		const CountedPtr<Point2x2DElement>&						GetOffset2x2DPoint() const		{ return mOffset; }

		void			SetRoughnessType(const RoughnessType roughnessType)				{ mRoughnessType = roughnessType; }
		void			SetAnalytic(const CountedPtr<AnalyticElement>& analytic)				{ mAnalytic = analytic; }
		void			Set2x2DPointArray(const std::vector<CountedPtr<Point2x2DElement> >& roughnessProfile);
		void			SetIsPatternRepeated(bool value)										{ mIsPatternRepeated = value; }
		void			SetScale2x2DPoint(const CountedPtr<Point2x2DElement>& scale)		{ mScale = scale; }
		void			SetOffset2x2DPoint(const CountedPtr<Point2x2DElement>& offset)	{ mOffset = offset; }

		void			GetInterpolatedOffsetZAndBanking(double distanceFromStartPortionInDirectWay, double offsetFromMiddleWayToWheelInDirectWay, double& offsetZ, double& banking) const;

		virtual const std::string&	GetTagName() const{ return kRoughnessTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);


	protected:
		RoughnessType mRoughnessType;
		CountedPtr<AnalyticElement> mAnalytic;
		std::vector<CountedPtr<Point2x2DElement> > m2x2DPointArray;
		CountedPtr<Point2x2DElement> mScale;
		CountedPtr<Point2x2DElement> mOffset;
		bool mIsPatternRepeated;

		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);
		//Return the last point with a curvilinear distance inferior of the abscissaInMeter given in parameter
		size_t			Get2x2DPointIndex(double abscissaInMeter) const;
	};


	//! Ground properties
	/*! Description of a ground physical material.
	*/
	class ROADXML_API GroundElement : public Element
	{
	public:
		GroundElement();

		const std::string&					GetName() const				{ return mName; }
		const std::string&					GetType() const				{ return mType; }
		double								GetAverageGrip() const		{ return mAverageGrip; }
		const CountedPtr<RoughnessElement>&	GetRoughness() const			{ return mRoughness; };
		double								GetWaterLevel() const			{ return mWaterLevel; };


		virtual void						SetName( const std::string& name, SubNetworkElement* parent );
		void								SetType(const std::string& str)					{ mType = str; }
		void								SetAverageGrip(double d)					{ mAverageGrip = d; }
		void								SetRoughness(RoughnessElement* const roughnessPt)	{ mRoughness = roughnessPt; }
		void								SetWaterLevel(double level)	{ mWaterLevel = level; }


		virtual const std::string&					GetTagName() const	{ return kGroundTag; }
		virtual	bool								LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*						BuildXMLElement(IDOMParser*);
		

	protected:
		std::string	mName;
		std::string	mType;
		double	mAverageGrip;

		CountedPtr<RoughnessElement> mRoughness;
		double mWaterLevel;

		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);

	};
}