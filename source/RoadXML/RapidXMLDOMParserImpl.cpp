

#include "RapidXMLDOMParserImpl.h"
#include "RapidXMLDOMElementImpl.h"

#include "Transcode.h"
#include "RoadXML/RoadXMLTags.h"
#include <algorithm>
#include <iostream>
#include <fstream>

#include "rapidxml_print.hpp"

#ifdef WIN32
#include <Windows.h>

std::wstring Transcode(const std::string& input)
{
	std::wstring result;

	if (input.empty())
		return result;

	size_t size = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), NULL, 0);
	// Allocate a buffer of that size plus one for the null and transcode
	wchar_t * buf = new wchar_t[size + 1];
	buf[size] = 0;
	::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), (LPWSTR)buf, (int)size + 1);
	result = buf;
	delete[] buf;
	return result;
}
#endif

//! Parser's creator
/*! This is the only function known from outside the dll.
Everything is then handled through the interfaces IDOMParser, IDOMElement, ...
*/
IDOMParser* NewDOMParserRapid(const char* fileName)
{
	RapidXMLDOMParserImpl* result = new RapidXMLDOMParserImpl();
	if (fileName)
		result->SetFileName(fileName);
	return result;
}

RapidXMLDOMParserImpl::RapidXMLDOMParserImpl() :
mDoc(NULL),
mVersionId(0)
{
}

void RapidXMLDOMParserImpl::Clear()
{
	if (mDoc != NULL)
	{
		delete mDoc;
		mDoc = 0;
	}
	mVersionId = 0;
	mRootElement = NULL;
}

RapidXMLDOMParserImpl::~RapidXMLDOMParserImpl()
{
	Clear();
}

IDOMElement* RapidXMLDOMParserImpl::GetRootElement()
{
	rapidxml::xml_node<> *rootnode = mDoc->first_node();
	if (rootnode)
		mRootElement = new RapidXMLDOMElementImpl(rootnode, mDoc);
	else
		mRootElement = NULL;

	return mRootElement;
}

bool RapidXMLDOMParserImpl::Load()
{
	if (mFileName.empty())
		return false;

	Clear();

	char *contents;
	size_t size = 0;

#ifdef WIN32
	FILE *fp = _wfopen(Transcode(mFileName).c_str(), L"rb");
#else
	FILE *fp = fopen(mFileName.c_str(), "rb");
#endif

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		contents = new char[size + 1];
		rewind(fp);
		fread(contents, 1, size, fp);
		contents[size] = 0;
		fclose(fp);
	}

	if (size == 0)
	{
		delete[] contents;
		return false;
	}

	mDoc = new rapidxml::xml_document<>();

	try {
		mDoc->parse<0>(contents);
	}
	catch (rapidxml::parse_error& e)
	{
		std::cout << "Error while parsing: " << mFileName << std::endl;
		std::cout << "Error: " << e.what() << std::endl;
		std::cout << "Here : " << e.where<char>() << std::endl;
		Clear();
		return false;
	}
	catch (...)
	{
		printf("An error occured during parsing\n ");
		Clear();
		return false;
	}

	rapidxml::xml_node<> *root = mDoc->first_node();
	if (!root)
	{
		assert("Error : No root element node in document ");
		Clear();
		return false;
	}

	mRootElement = new RapidXMLDOMElementImpl(root, mDoc);
	return true;
}

IDOMElement* RapidXMLDOMParserImpl::CreateDOMDocument(const char* rootElementName)
{
	Clear();

	mDoc = new rapidxml::xml_document<>();
	char * name = mDoc->allocate_string(rootElementName);
	rapidxml::xml_node<>* rootnode = mDoc->allocate_node(rapidxml::node_element, name);

	if (rootnode)
	{
		mDoc->append_node(rootnode);
		mRootElement = new RapidXMLDOMElementImpl(rootnode, mDoc);
	}
	else
		mRootElement = NULL;

	return mRootElement;
}

IDOMElement* RapidXMLDOMParserImpl::CreateDOMElement(const std::string& elementName)
{
	char * name = mDoc->allocate_string(elementName.c_str());
	rapidxml::xml_node<>* node = mDoc->allocate_node(rapidxml::node_element, name);
	return new RapidXMLDOMElementImpl(node, mDoc);
}

bool RapidXMLDOMParserImpl::Add(IDOMElement* element)
{
	rapidxml::xml_node<>* pDomElem = static_cast<RapidXMLDOMElementImpl*>(element)->GetElement();

	mDoc->append_node(pDomElem);

	return true;
}

bool RapidXMLDOMParserImpl::Save()
{
#if( RESTRICTED_VERSION==1 )
	assert( "This method is not allowed" );
	return false;
#endif


	if (mFileName.empty())
		return false;


	if (!mDoc)
		return false;
#ifdef WIN32
	std::ofstream out(Transcode(mFileName).c_str());
#else
	std::ofstream out(mFileName);
#endif
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" << std::endl;
	out << *mDoc;
	return out.good();
}

bool RapidXMLDOMParserImpl::SetCDATAContent(IDOMElement* element, const std::string& value)
{
	rapidxml::xml_node<>* cDATANode = mDoc->allocate_node(rapidxml::node_cdata);
	
	char *attr_value = mDoc->allocate_string(value.c_str());
	cDATANode->value(attr_value, value.size());

	rapidxml::xml_node<>* pDomElem = static_cast<RapidXMLDOMElementImpl*>(element)->GetElement();

	pDomElem->remove_all_nodes();

	pDomElem->append_node( cDATANode );

	return true;
}