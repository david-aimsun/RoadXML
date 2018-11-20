#pragma once

#include "Element.h"
#include "CountedPtr.h"
#include "RoadXMLTags.h"

#include <vector>

namespace RoadXML
{
	// =========================================================================================================
	class ROADXML_API BorderOffsetElement : public Element
	{
	public:

		BorderOffsetElement();
		BorderOffsetElement(double t, double yOffset, double directionIn, double directionOut);
				
		virtual const std::string&	GetTagName() const;
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
		
		double GetT()const;
		void SetT(double paramT );

		double GetYOffset()const;
		void SetYOffset(double yOffset);

		double GetDirIn()const;
		void SetDirIn(double dirIn);
		
		double GetDirOut()const;
		void SetDirOut(double dirOut);

	protected:
		virtual ~BorderOffsetElement();

		std::string mTagName;

		double mT;
		double mYOffset;
		double mDirectionIn;
		double mDirectionOut;
	};
	
	// =========================================================================================================
	class ROADXML_API BorderModifierElement : public Element
	{
	public:

		BorderModifierElement();
				
		virtual const std::string&	GetTagName() const;
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
		
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
		
		size_t GetId()const;
		void SetId(size_t id);

	protected:
		virtual ~BorderModifierElement();

		std::string mTagName;

		size_t mId;
		std::vector<CountedPtr<BorderOffsetElement> > mBorderOffsets;
	};
	
	// =========================================================================================================
	class ROADXML_API ProfileModifierElement : public Element
	{
	public:

		ProfileModifierElement();
				
		virtual const std::string&	GetTagName() const;
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
		
	protected:
		virtual ~ProfileModifierElement();

		std::string mTagName;

		std::vector<CountedPtr<BorderModifierElement> > mBorderModifiers;
	};
}