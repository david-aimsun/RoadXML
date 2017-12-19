#pragma once
#include "RoadXML/RoadXML.h"

#include "CountedObject.h"
#include "RoadXML/IDOMParser.h"
#include "RoadXML/IDOMElement.h"

#include "HashVector.h"

#include <string>
#include <vector>

namespace RoadXML
{

	//! Base class for all the RoadXML elements.
	/*! All the RoadXML elements inherit from this class. 
		If you want to add a new Element in the format, you should inherit from this class and implement the virtual methods.
	*/
	class ROADXML_API Element : public CountedObject
	{
	public:

		Element() {}
		virtual ~Element(){}

		//! Returns the name of the element. 
		/*! Returns the name of the element as it appears in the RoadXML file.
		*/
		virtual const std::string&	GetTagName() const = 0;

		//! Fill the RoadXML class from the IDOMElement.
		/*! The content of the IDOMElement 
		*/
		virtual	bool				LoadFromXMLElement(IDOMElement*, IDOMParser*) = 0;
		virtual IDOMElement*		BuildXMLElement(IDOMParser*) = 0;

	protected:
		//! Utility method to load all the children of the element.
		/*! This method will loop on all the children of he Element and call the 
			virtual method LoadChild() on each one of them.
		*/
		void LoadChildren( IDOMElement* parent, IDOMParser* parser );
		
		//! Implement this method in your class to store loaded children data.
		virtual bool LoadChild( IDOMElement* childElement, IDOMParser* parser ) {return false;}
	};

	//! Utility function that creates add child on the IDOMElement for each element of the array.
	template<typename T>
	void BuildChildrenArray(const HashVector<T>& elemArray, const std::string& arrayName, IDOMElement* elem, IDOMParser* parser)
	{
		if( !arrayName.empty() )
		{
			IDOMElement* arrayElem = parser->CreateDOMElement( arrayName );
			elem->AddChild( arrayElem );
			// Point on the array elem to fill it
			elem = arrayElem;
		}

		typename HashVector<T>::const_iterator it = elemArray.begin();
		for (; it != elemArray.end(); ++it)
		{
			elem->AddChild((*it)->BuildXMLElement(parser));
		}
	}

	//! Utility function that creates add child on the IDOMElement for each element of the array.
	template<typename T>
	void BuildChildrenArray(const std::vector<T>& elemArray, const std::string& arrayName, IDOMElement* elem, IDOMParser* parser)
	{
		if (!arrayName.empty())
		{
			IDOMElement* arrayElem = parser->CreateDOMElement(arrayName);
			elem->AddChild(arrayElem);
			// Point on the array elem to fill it
			elem = arrayElem;
		}

		const size_t count = elemArray.size();
		for (size_t i = 0; i<count; i++)
		{
			elem->AddChild(elemArray[i]->BuildXMLElement(parser));
		}
	}


	//! Creates an element from the element's name.
	/*! The RoadXML library uses this method instead of using the C++ "new" operator.
		This function creates the new element using the Factory. This gives a way to the user to creates 
		it own C++ objects instead of the one created by default.
		To create user specific object, first implement another factory that inherit from Factory. This new factory implement:
			virtual Element* Create( const std::string& tagName );
		and can return a user specific element for some tagName. 
	*/
	ROADXML_API Element* CreateElement( const std::string& tagName );

	//! RoadXML element factory.
	/*! RoadXML element are created with a factory.
		To create user specific element, inherit a class from this one and implement the Create method.
	*/
	class ROADXML_API Factory : public CountedObject
	{
	public:
		//! Creates a new Element for a specific element name.
		virtual Element* Create( const std::string& tagName );
	};

	//! Replace the default factory by a user specific one.
	ROADXML_API void SetFactory( Factory* );
	ROADXML_API Factory* GetFactory();
}
