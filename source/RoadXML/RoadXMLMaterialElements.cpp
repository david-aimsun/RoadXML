#include "RoadXML/RoadXMLElements.h"

#include "RoadXML/IDOMElement.h"
#include "RoadXML/IDOMParser.h"

#include "stk/Math.h"
#include <float.h>


//////////////////////////////////////////////////////////////////////////////

IDOMElement* RoadXML::BuildVector2Elem(const stk::Vector2& value, IDOMParser* parser) 
{
	IDOMElement* childOut = parser->CreateDOMElement(kVectord2Tag);

	childOut->SetDoubleAttribute(kXTag, value.x);
	childOut->SetDoubleAttribute(kYTag, value.y);

	return childOut;
}

bool RoadXML::LoadVector2Elem(stk::Vector2& value, IDOMElement* domObject) 
{
	if( kVectord2Tag == domObject->GetTagName() )
	{
		domObject->GetDoubleAttribute(kXTag, value.x);
		domObject->GetDoubleAttribute(kYTag, value.y);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////

IDOMElement* RoadXML::BuildColorRGBAElem(const stk::Color& value, IDOMParser* parser) 
{
	IDOMElement* childOut = parser->CreateDOMElement( kColorElementTag );

	childOut->SetDoubleAttribute( kRedTag, value.red );
	childOut->SetDoubleAttribute( kGreenTag, value.green );
	childOut->SetDoubleAttribute( kBlueTag, value.blue );
	childOut->SetDoubleAttribute( kAlphaTag, value.alpha );
	return childOut;
}

bool RoadXML::LoadColorRGBAElem(stk::Color& value, IDOMElement* domObject) 
{
	if( kColorElementTag == domObject->GetTagName() )
	{
		domObject->GetDoubleAttribute( kRedTag, value.red );
		domObject->GetDoubleAttribute( kGreenTag, value.green );
		domObject->GetDoubleAttribute( kBlueTag, value.blue );
		domObject->GetDoubleAttribute( kAlphaTag, value.alpha );
		return true;
	}
	return false;
}



/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::UVMappingElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{	
	elem->GetDoubleAttribute( kLeftUTag, mLeftU );
	elem->GetDoubleAttribute( kRightUTag, mRightU );
	elem->GetDoubleAttribute( kLengthVTag, mLengthV );

	LoadChildren(elem, parser);

	return true;
}

IDOMElement* RoadXML::UVMappingElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	elementOut->SetDoubleAttribute(kLeftUTag, mLeftU);
	elementOut->SetDoubleAttribute(kRightUTag, mRightU);
	elementOut->SetDoubleAttribute(kLengthVTag, mLengthV);

	return elementOut;
}

bool RoadXML::UVMappingElement::LoadChild( IDOMElement *childElement, IDOMParser *parser )
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::MaterialElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	elem->GetDoubleAttribute(kShininessTag, mShininess);
	elem->GetStringAttribute(kNameTag, mName);
	elem->GetStringAttribute(kMaterialBRDFTag, mBRDF);

	LoadChildren( elem, parser );

	return true;
}

IDOMElement* RoadXML::MaterialElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
		
	if( !mName.empty() )
		elementOut->SetStringAttribute(kNameTag, mName);

	if( !stk::isEqual( mShininess, 0, FLT_EPSILON ))
	{
		elementOut->SetDoubleAttribute(kShininessTag, mShininess);
	}

	if( mDiffuseChanel ) 
		elementOut->AddChild( mDiffuseChanel->BuildXMLElement(parser) );
	if( mAmbientChanel ) 
		elementOut->AddChild( mAmbientChanel->BuildXMLElement(parser) );
	if( mSpecularChanel ) 
		elementOut->AddChild( mSpecularChanel->BuildXMLElement(parser) );
	if( mEmissiveChanel ) 
		elementOut->AddChild( mEmissiveChanel->BuildXMLElement(parser) );

	if( mNormalMapChanel ) 
		elementOut->AddChild( mNormalMapChanel->BuildXMLElement(parser) );
	if( mReflectionMapChanel ) 
		elementOut->AddChild( mReflectionMapChanel->BuildXMLElement(parser) );
	if( mSpecularMapChanel ) 
		elementOut->AddChild( mSpecularMapChanel->BuildXMLElement(parser) );
	if( mAmbientOcclusionMapChanel ) 
		elementOut->AddChild( mAmbientOcclusionMapChanel->BuildXMLElement(parser) );
	
	if( !mBRDF.empty())
		elementOut->SetStringAttribute( kMaterialBRDFTag, mBRDF );

	return elementOut;
}

bool RoadXML::MaterialElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();

	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement && newElement->LoadFromXMLElement(childElement, parser) )
	{
		if (tagName == kDiffuseTag) 
			mDiffuseChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kAmbientTag) 
			mAmbientChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kSpecularTag) 
			mSpecularChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kEmissiveTag) 
			mEmissiveChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kNormalMapTag) 
			mNormalMapChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kReflectionMapTag) 
			mReflectionMapChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kSpecularMapTag) 
			mSpecularMapChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
		else if (tagName == kAmbientOcclusionMapTag) 
			mAmbientOcclusionMapChanel = dynamic_cast<MaterialChanelElement*>(newElement.Get());
	}
	else 
		return false;

	return true;
}

void RoadXML::MaterialElement::SetName( const std::string& name, RoadXML::SubNetworkElement* parent )
{
	std::string oldname = mName;
	mName=name; 
	if(parent) 
	{
		parent->mMaterialArray.updateKey( this, oldname, name );
	} 
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::MaterialChanelElement::LoadFromXMLElement(IDOMElement* elem, IDOMParser* parser)
{
	mTexture.clear();


	CountedPtr<IDOMElement> childElement = elem->GetFirstChildElement();

	while (childElement)
	{
		const std::string& tagName = childElement->GetTagName();

		if (tagName == kColorElementTag) 
			LoadColorRGBAElem(mColor, childElement);
		else if (tagName == kTextureTag) 
			childElement->GetStringAttribute(kPathTag, mTexture);

		childElement = childElement->GetNextSiblingChildElement();
	}

	// Call it anyway, for optional user data
	LoadChildren(elem, parser);

	return true;
}

IDOMElement* RoadXML::MaterialChanelElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());
		
	if( mTexture.size() )
	{
		IDOMElement* textureChild = parser->CreateDOMElement(kTextureTag);
		
		textureChild->SetStringAttribute( kPathTag, mTexture );

		elementOut->AddChild( textureChild );
	}
	elementOut->AddChild( BuildColorRGBAElem( mColor, parser ) );

	return elementOut;
}

/////////////////////////////////////////////////////////////////////////////////

bool RoadXML::GeneratedTerrainMaterialElement::LoadFromXMLElement(IDOMElement* iDOMElem, IDOMParser* parser)
{
	iDOMElem->GetDoubleAttribute(kUVScaleXTag, mUVScale.x);
	iDOMElem->GetDoubleAttribute(kUVScaleYTag, mUVScale.y);
	
	iDOMElem->GetDoubleAttribute(kUVOffsetXTag, mUVOffset.x);
	iDOMElem->GetDoubleAttribute(kUVOffsetYTag, mUVOffset.y);
	
	iDOMElem->GetDoubleAttribute(kUVHeadingTag, mUVHeading);

	iDOMElem->GetStringAttribute(kMaterialNameTag, mMaterialName);
	
	LoadChildren( iDOMElem, parser );

	return true;
}

IDOMElement* RoadXML::GeneratedTerrainMaterialElement::BuildXMLElement(IDOMParser* parser)
{
	IDOMElement* elementOut = parser->CreateDOMElement(GetTagName());

	if( mMaterialName.size() )
		elementOut->SetStringAttribute(kMaterialNameTag, mMaterialName);
	else if(mMaterial)
		elementOut->AddChild(mMaterial->BuildXMLElement(parser));
		
	elementOut->SetDoubleAttribute(kUVScaleXTag, mUVScale.x);
	elementOut->SetDoubleAttribute(kUVScaleYTag, mUVScale.y);
		
	elementOut->SetDoubleAttribute(kUVOffsetXTag, mUVOffset.x);
	elementOut->SetDoubleAttribute(kUVOffsetYTag, mUVOffset.y);

	elementOut->SetDoubleAttribute(kUVHeadingTag, mUVHeading);
	

	return elementOut;
}


bool RoadXML::GeneratedTerrainMaterialElement::LoadChild( IDOMElement* childElement, IDOMParser* parser )
{
	const std::string& tagName = childElement->GetTagName();
	
	CountedPtr<Element> newElement = CreateElement(tagName);
	if( newElement && newElement->LoadFromXMLElement(childElement, parser) )
	{
		if (tagName == kMaterialTag)
		{
			if(mMaterialName.empty())
				mMaterial = dynamic_cast<MaterialElement*>(newElement.Get()) ;
		}
	}
	else
		return false;

	return true;
}
