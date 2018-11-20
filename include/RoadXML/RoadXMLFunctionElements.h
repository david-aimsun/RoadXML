#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "CountedPtr.h"
#include "stk/Macro.h"
#include "stk/Vector2.h"
#include "stk/Mark.h"
#include <vector>

namespace RoadXML
{
	//! Frame
	/*! 3D frame with x,y,z position and h,p,r orientation.
	*/
	class ROADXML_API FrameElement : public Element
	{
	public:
		FrameElement() : mFrame(stk::Vector3(0., 0., 0.), 0., 0., 0.), mTagName(kFrameTag) {}
		FrameElement(const stk::Mark& frame, const std::string& frameName=kFrameTag) : mFrame(frame), mTagName(frameName) {}

		const stk::Mark&	GetFrame() const {return mFrame;}
		void				SetFrame(const stk::Mark& frame) {mFrame = frame;}

		virtual const std::string&	GetTagName() const{ return mTagName; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		std::string mTagName;
		stk::Mark mFrame;

		virtual ~FrameElement(){}
	};

	//! Segment description
	/*! Described by its length.
	*/
	class ROADXML_API SegmentElement : public Element
	{
	public:
		SegmentElement() : mLength(0) {}

		double mLength;

		virtual const std::string&	GetTagName() const{ return kSegmentTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~SegmentElement(){}
	};

	//! Circle Arc description
	/*! Described by its length and curvature (curvature=1/radius).
	*/
	class ROADXML_API CircleArcElement : public Element
	{
	public:
		CircleArcElement() : mLength(0), mCurvature(0) {}

		double			mLength;
		double			mCurvature; //! = 1/radius

		virtual const std::string&	GetTagName() const{ return kCircleArcTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~CircleArcElement(){}
	};
	
	class ROADXML_API ClothoidSplineElement : public Element
	{
	public:
		ClothoidSplineElement() : mThreshold(0.75), mSymmetricBlending(false) {}

		std::vector<stk::Vector2>	mVertexArray;
		bool	mSymmetricBlending;
		double	mThreshold;

		virtual const std::string&	GetTagName() const{ return kClothoidSplineTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~ClothoidSplineElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Clothoid Arc description
	/*! Described by its length, start curvature and end curvature (curvature=1/radius).
		Also known as Cornu Spiral.
	*/
	class ROADXML_API ClothoidArcElement : public Element
	{
	public:
		ClothoidArcElement() : mLength(0), mStartCurvature(0), mEndCurvature(0) {}

		double			mLength;
		double			mStartCurvature; // == 1/startRadius
		double			mEndCurvature; // == 1/endRadius

		virtual const std::string&	GetTagName() const{ return kClothoArcTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~ClothoidArcElement(){}
	};

	//! Polyline description
	/*! Described by a vertex array.
		The interpolation between the vertices can be a spline interpolation.
	*/
	class ROADXML_API PolyLineElement : public Element
	{
	public:
		enum EType
		{
			eSegment = 0,
			eSpline,	// If EType change, think about ToEType and ToText functions changes
			eBezier
		};
		static const EType ToEType(const std::string & strTypeValue);
		static const std::string ToText(const EType & eTypeValue);

		PolyLineElement() : mEndHeading(0), mInterpolationType(eSegment) {}
		virtual ~PolyLineElement(){}

		EType mInterpolationType; // "segment" or "spline"
		double mEndHeading;
		std::vector<stk::Vector2>	mVertexArray;

		virtual const std::string&	GetTagName() const{ return kPolyLineTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	// Cartesian function y=f(x)
	class ROADXML_API PolynomialFunctionElement : public Element
	{
	public:
		PolynomialFunctionElement() : mDerivativeIn(0), mDerivativeOut(0) {}

		stk::Vector2 mPosIn;
		double mDerivativeIn;
		stk::Vector2 mPosOut;
		double mDerivativeOut;

		virtual const std::string&	GetTagName() const{ return kPolynomialTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~PolynomialFunctionElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};
}