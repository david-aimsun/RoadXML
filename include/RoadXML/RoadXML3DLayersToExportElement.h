#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "RoadXMLTags.h"

namespace RoadXML
{

	class ROADXML_API RoadXML3DLayersToExportElement : public Element
	{
	public:
		RoadXML3DLayersToExportElement() : mRoadsAndIntersections(true), mMarkings(false), mGeneratedGround(false), m3DObjects(false), mGround(false), mExternalFile(false), mExternalFilePath(""), mMergeRollinSurfaces(false){}
		
		void setRoadsAndIntersections(bool chek){ mRoadsAndIntersections = chek; }
		bool getRoadsAndIntersections() const{ return mRoadsAndIntersections; }
		void setMarkings(bool chek){ mMarkings = chek; }
		bool getMarkings() const{ return mMarkings; }
		void setGeneratedGround(bool chek){ mMarkings = chek; }
		bool getGeneratedGround() const{ return mGeneratedGround; }
		void set3DObjects(bool chek){ m3DObjects = chek; }
		bool get3DObjects() const{ return m3DObjects; }
		void setGround(bool chek){ mGround = chek; }
		bool getGround() const{ return mGround; }
		void setExternalFile(bool chek){ mExternalFile = chek; }
		bool getExternalFile() const{ return mExternalFile; }
		void setExternalFilePath(std::string path){ mExternalFilePath = path; }
		std::string getExternalFilePath() const{ return mExternalFilePath; }
		void setMergeRollinSurfaces(bool chek){ mMergeRollinSurfaces = chek; }
		bool getMergeRollinSurfaces() const{ return mMergeRollinSurfaces; }


		virtual const std::string&	Get3DLayersToExportTag() const{ return k3DLayersToExportTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);
	protected:
		bool mRoadsAndIntersections;
		bool mMarkings;
		bool mGeneratedGround;
		bool m3DObjects;
		bool mGround;
		bool mExternalFile;
		std::string mExternalFilePath;
		bool mMergeRollinSurfaces;
		virtual ~RoadXML3DLayersToExportElement();



	};

};