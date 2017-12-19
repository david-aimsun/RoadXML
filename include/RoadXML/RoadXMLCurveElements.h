#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"
#include "RoadXMLFunctionElements.h"
#include "CountedPtr.h"
#include <vector>

namespace RoadXML
{
	//! Planar description of the road axis.
	/*! Described by an array of parametric fucntion.
	*/
	class ROADXML_API XYCurveElement : public Element
	{
	public:
		XYCurveElement():mDirection(0),mXValue(0),mYValue(0) {}

		double mDirection; //! start direction of the curve.
		double mXValue;	//! start X position of the curve.
		double mYValue;	//! start Y position of the curve.

		std::vector<CountedPtr<Element> > mFunctions; //! Parametric functions of the curve.

		virtual const std::string&	GetTagName() const{ return kXYCurveTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~XYCurveElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

	};

	class ROADXML_API CartesianCurveElement : public Element
	{
	public:
		//CartesianCurveElement() {mCurveType = "";}
		CartesianCurveElement(const std::string& curveType) : mCurveType(curveType) {}

		std::vector<CountedPtr<PolynomialFunctionElement> > mFunctions;

		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		virtual const std::string&	GetTagName() const{ return mCurveType; }

	protected:
		virtual ~CartesianCurveElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

		std::string mCurveType;
	};
}