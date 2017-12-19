#pragma once
#include "RoadXML/RoadXML.h"

#include "Element.h"
#include "CountedPtr.h"
#include "RoadXMLTags.h"
#include "stk/Macro.h"
#include "stk/Vector2.h"
#include "stk/Color.h"
#include <vector>

namespace RoadXML
{
	class SubNetworkElement;
	//! Utility function to build/load a Vector2 element.
	ROADXML_API IDOMElement* BuildVector2Elem(const stk::Vector2& value, IDOMParser* parser);
	ROADXML_API bool LoadVector2Elem(stk::Vector2& value, IDOMElement* domObject);

	//! Utility function to build/load a Color element.
	ROADXML_API IDOMElement* BuildColorRGBAElem(const stk::Color& value, IDOMParser* parser);
	ROADXML_API bool LoadColorRGBAElem(stk::Color& value, IDOMElement* domObject);

	//! Common class to all the material chanels
	class ROADXML_API MaterialChanelElement : public Element
	{
	public:
		MaterialChanelElement(const std::string& tagName) : mTagName(tagName) {}

		stk::Color mColor;
		std::string mTexture;

		virtual const std::string&	GetTagName() const{ return mTagName; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~MaterialChanelElement(){}

		std::string mTagName;
	};

	//! Mapping on a axial object.
	/*! U in width, V in length.
	*/
	class ROADXML_API UVMappingElement : public Element
	{
	public:
		UVMappingElement(): mLeftU(0), mRightU(1), mLengthV(1) {}

		double mLeftU;
		double mRightU;

		double mLengthV;

		virtual const std::string&	GetTagName() const{ return kUVMappingTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~UVMappingElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};

	//! Visual Material
	class ROADXML_API MaterialElement : public Element
	{
	public:
		MaterialElement():mShininess(0){}

		double mShininess;

		CountedPtr<MaterialChanelElement> mDiffuseChanel;
		CountedPtr<MaterialChanelElement> mAmbientChanel;
		CountedPtr<MaterialChanelElement> mSpecularChanel;
		CountedPtr<MaterialChanelElement> mEmissiveChanel;

		// Maps
		CountedPtr<MaterialChanelElement> mNormalMapChanel;
		CountedPtr<MaterialChanelElement> mSpecularMapChanel;
		CountedPtr<MaterialChanelElement> mReflectionMapChanel;
		CountedPtr<MaterialChanelElement> mAmbientOcclusionMapChanel;

		// BRDF
		std::string mBRDF;

		const std::string&					GetName() const				{ return mName; }
		virtual void						SetName( const std::string& name, SubNetworkElement* parent );

		virtual const std::string&	GetTagName() const{ return kMaterialTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~MaterialElement(){}

		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );

		std::string mName;
	};


	class ROADXML_API GeneratedTerrainMaterialElement : public Element
	{
	public:
		GeneratedTerrainMaterialElement():mShininess(0),mUVScale(stk::Vector2(50.0,50.0)),mUVHeading(0.0) {}

		std::string mMaterialName;
		CountedPtr<MaterialElement>	 mMaterial;

		double mShininess;
		
		stk::Vector2	mUVScale;
		stk::Vector2	mUVOffset;
		double			mUVHeading;

		virtual const std::string&	GetTagName() const{ return kGeneratedTerrainMaterialTag; }
		virtual	bool				LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser);
		virtual IDOMElement*		BuildXMLElement(IDOMParser*);

	protected:
		virtual ~GeneratedTerrainMaterialElement(){}



		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser );
	};
}