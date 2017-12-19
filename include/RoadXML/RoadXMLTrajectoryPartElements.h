#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLFunctionElements.h"
#include "RoadXML/RoadXMLCurveElements.h"
#include "CountedPtr.h"
#include <vector>

namespace RoadXML
{
	class SubNetworkElement;

	class ROADXML_API TrajectoryPartElement : public Element
	{
	public:
		TrajectoryPartElement();

		virtual const std::string& GetName() const;
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		virtual const std::string& GetTagName() const;

		virtual	bool LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement* BuildXMLElement(IDOMParser* parser);

	protected:
		virtual ~TrajectoryPartElement();

		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);

	protected:
		std::string myName;

	public:
		//! XY axis description
		/*! A parametric curve made of segment, circle arcs, clothoid arcs...
		*/
		CountedPtr<XYCurveElement> myXYCurve;

		//! Elevation profile
		/*! Altitude (Z) of the road function of its abscissa (S).
		*/
		CountedPtr<CartesianCurveElement> mySZCurve;
	};
}