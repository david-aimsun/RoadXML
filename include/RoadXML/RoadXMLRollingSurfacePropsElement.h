#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"

namespace RoadXML
{
	class ROADXML_API Export3DLayersElement : public Element
	{
	public:
		Export3DLayersElement();


		virtual const std::string&	GetTagName() const{ return k3DLayersToExportTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		bool mRoadsAndIntersections;
		bool mMarkings;
		bool mGeneratedGround;
		bool m3DObjects;
		bool mExportGround;
		bool mExternalFile;
		std::string mExternalFilePath;
		bool mMergeRollinSurfaces;

	protected:

		virtual ~Export3DLayersElement();

	};


	class ROADXML_API Generate3DRoadsElement : public Element
	{
	public:
		Generate3DRoadsElement();


		virtual const std::string&	GetTagName() const{ return k3DRoadsGenerationElementTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		double mCTol;
		double MminStep;
		double mMaxStep;
		double mMaxNormalVariation;
		double mMaxWidth;

	protected:

		virtual ~Generate3DRoadsElement();

	};


	class ROADXML_API SolExportOptionsElement : public Element
	{
	public:
		SolExportOptionsElement();


		virtual const std::string&	GetTagName() const{ return kSolExportOptionsElementTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

		double mMinPointsDistance;
		bool   mGenerateDebugFile;
		bool   mRemoveTriangles;
		bool   mCheckOrientaions;

	protected:

		virtual ~SolExportOptionsElement();

	};

	class ROADXML_API RollingSurfacePropsElement : public Element
	{
	public:
		RollingSurfacePropsElement();

		virtual const std::string&	GetTagName() const{ return kRollingSurfacePropsTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);


		bool mIsRollingSurfaceUpdateOnSave;
		CountedPtr<Export3DLayersElement> m3DLayersToExportElement;
		CountedPtr<Generate3DRoadsElement> m3DRoadsGenerationElement;
		CountedPtr<SolExportOptionsElement> mSolExportOptionsElement;
	protected:
		virtual ~RollingSurfacePropsElement();
		virtual bool LoadChild(IDOMElement* childElement, IDOMParser* parser);



	};


	
};