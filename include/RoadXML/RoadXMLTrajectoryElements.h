#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "CountedPtr.h"
#include <vector>

namespace RoadXML
{
	class SubNetworkElement;
	// Piece of a Trajectory
	// Contain the name of a TrajectoryPart,
	// The TrajectoryPart is retrieve in the parent subnetwork with it's name
	class ROADXML_API TrajectoryPieceElement : public Element
	{
	public:
		TrajectoryPieceElement();

		virtual const std::string& GetName() const;
		virtual void SetName(const std::string& name);

		virtual const std::string& GetTagName() const;

		virtual const std::string& GetTrajectoryPartName() const;
		virtual void SetTrajectoryPartName(const std::string& trajectoryPartName);

		virtual const std::string& GetCirculationWay() const;
		virtual void SetCirculationWay(const std::string& circulationWay);

		virtual	bool LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement* BuildXMLElement(IDOMParser* parser);

	protected:
		virtual ~TrajectoryPieceElement();

	protected:
		std::string myName;
		std::string myTrajectoryPartName;
		
		// Possible values: direct or inverse
		// Define the circulation way on the TrajectoryPart pointed by myTrajectoryPartName
		// Note: the circulation way can be different from a TrajectoryPart to another
		std::string myCirculationWay;
	};

	// Trajectory is a container of TrajectoryPieceElement
	class ROADXML_API TrajectoryElement : public Element
	{
	public:
		TrajectoryElement();

		virtual const std::string& GetName() const;
		virtual void				SetName( const std::string& name, SubNetworkElement* parent );

		virtual const std::string& GetTagName() const;

		virtual	bool LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement* BuildXMLElement(IDOMParser* parser);

	protected:
		virtual ~TrajectoryElement();
		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);

	public:
		std::vector<CountedPtr<TrajectoryPieceElement> > myTrajectoryPieceArray;

	protected:
		std::string myName;

	};
}
