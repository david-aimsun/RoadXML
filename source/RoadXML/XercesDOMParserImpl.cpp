

#include "XercesDOMParserImpl.h"
#include "XercesDOMElementImpl.h"

#include "Transcode.h"

#include "RoadXML/RoadXMLTags.h"

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

XERCES_CPP_NAMESPACE_USE

#include <algorithm>

//! Parser's creator
/*! This is the only function known from outside the dll.
Everything is then handled through the interfaces IDOMParser, IDOMElement, ...
*/
IDOMParser* NewDOMParser(const char* fileName)
{
	XercesDOMParserImpl* result = new XercesDOMParserImpl();
	if (fileName)
		result->SetFileName(fileName);
	return result;
}

XercesDOMParserImpl::XercesDOMParserImpl() :
mDoc(NULL),
mVersionId(0),
ourParser(NULL)
{
	XMLPlatformUtils::Initialize();
}

void XercesDOMParserImpl::Clear()
{
	if (ourParser != NULL)
	{
		delete ourParser;
		ourParser = NULL;
		//the document belonged to the parser so we don't delete it
		mDoc = NULL;
	}

	if (mDoc != NULL)
	{
		delete mDoc;
		mDoc = 0;
	}

	mVersionId = 0;

	mRootElement = NULL;
}

XercesDOMParserImpl::~XercesDOMParserImpl()
{
	Clear();

	XMLPlatformUtils::Terminate();
}

IDOMElement* XercesDOMParserImpl::GetRootElement()
{
	DOMElement* pRootNode = mDoc->getDocumentElement();
	if (pRootNode)
		mRootElement = new XercesDOMElementImpl(pRootNode);
	else
		mRootElement = NULL;

	return mRootElement;
}

bool XercesDOMParserImpl::Load()
{
	Clear();

	//  Parse the XML file, catching any XML exceptions that might propogate
	//  out of it.
	try
	{
		ourParser = new XercesDOMParser();
		//  The parser will call back to methods of the ErrorHandler if it
		//  discovers errors during the course of parsing the XML document.
		ourParser->setStandardUriConformant(false);
		ourParser->setErrorHandler(&ourErrorHandler);
		ourParser->setValidationScheme(XercesDOMParser::Val_Auto);
		ourParser->setDoNamespaces(false);
		ourParser->setDoSchema(false);
		ourParser->setCreateEntityReferenceNodes(true);
		std::string file;
		std::replace(mFileName.begin(), mFileName.end(), '\\', '/');
#ifdef WIN32
		if (mFileName[0] == '/' && mFileName[1] == '/')
			file += "file://";
#endif
		file += mFileName;
		ourParser->parse(RoadXML::Transcode(file).c_str());
	}
	catch (const XMLException& e)
	{
		char* szMessage = XMLString::transcode(e.getMessage());
		printf("An error occured during parsing\n   Message: %s\n", szMessage);
		XMLString::release(&szMessage);
		Clear();
		return false;
	}
	catch (const DOMException& e)
	{
		printf("A DOM error occured during parsing\n   DOMException code: %d\n", e.code);
		Clear();
		return false;
	}
	catch (...)
	{
		printf("An error occured during parsing\n ");
		Clear();
		return false;
	}

	mDoc = ourParser->getDocument();
	if (!mDoc)
		return false;

	mDoc->normalize();

	DOMElement* pRootNode = mDoc->getDocumentElement();

	if (!pRootNode)
	{
		assert("Error : No root element node in document ");
		Clear();
		return false;
	}

	mRootElement = new XercesDOMElementImpl(pRootNode);

	return true;
}

IDOMElement* XercesDOMParserImpl::CreateDOMDocument(const char* rootElementName)
{
	Clear();

	DOMImplementation* pImplementation = DOMImplementation::getImplementation();
	mDoc = pImplementation->createDocument(NULL, RoadXML::Transcode(rootElementName).c_str(), NULL);

    mDoc->setXmlVersion(RoadXML::Transcode("1.0").c_str());
	mDoc->setXmlStandalone(true);

	DOMElement* pRootNode = mDoc->getDocumentElement();

	//pRootNode->setAttribute(L"xmlns:xsi",L"http://www.w3.org/2001/XMLSchema-instance");
	////pRootNode->setAttribute(L"xsi:noNamespaceSchemaLocation", CharToSTDWString(schemaName).c_str());
	//pRootNode->setAttribute(L"version", CharToSTDWString(VersionStr).c_str());

	if (pRootNode)
		mRootElement = new XercesDOMElementImpl(pRootNode);
	else
		mRootElement = NULL;

	return mRootElement;
}

IDOMElement* XercesDOMParserImpl::CreateDOMElement(const std::string& elementName)
{
	DOMElement* newDOMElem = mDoc->createElement(RoadXML::Transcode(elementName).c_str());
	return new XercesDOMElementImpl(newDOMElem);
}

bool XercesDOMParserImpl::Add(IDOMElement* element)
{
	DOMElement* pDomElem = static_cast<XercesDOMElementImpl*>(element)->GetElement();

	mDoc->getDocumentElement()->appendChild(pDomElem);

	return true;
}

bool XercesDOMParserImpl::Save()
{
#if( RESTRICTED_VERSION==1 )
	assert( "This method is not allowed" );
	return false;
#endif

	try
	{
        DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(RoadXML::Transcode("LS").c_str());
		DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();

		DOMConfiguration* dc = theSerializer->getDomConfig();
		if (dc->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			dc->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		// Specify the target for the XML output.
		XMLFormatTarget *formatTarget = new LocalFileFormatTarget(RoadXML::Transcode(mFileName).c_str());
		//XMLFormatTarget *myFormTarget = new StdOutFormatTarget();

		// Create a new empty output destination object.
		DOMLSOutput *output = ((DOMImplementationLS*)impl)->createLSOutput();

		// Set the stream to our target.
		output->setByteStream(formatTarget);

		// Write the serialized output to the destination.
		bool ret = theSerializer->write(mDoc, output);

		// Cleanup.
		theSerializer->release();
		delete formatTarget;
		output->release();

		return ret;
	}
	catch (XMLException& e)
	{
		char* szMessage = XMLString::transcode(e.getMessage());
		printf("An error occurred during creation of output transcoder. Msg is:\n %s\n", szMessage);
		XMLString::release(&szMessage);
		return false;
	}

	return true;
}

bool XercesDOMParserImpl::SetCDATAContent(IDOMElement* element, const std::string& value)
{
    RoadXML::XercesString nodeValue = RoadXML::Transcode(value);

	DOMCDATASection* cDATANode = mDoc->createCDATASection(nodeValue.c_str());

	DOMElement* pDomElem = static_cast<XercesDOMElementImpl*>(element)->GetElement();

	if( pDomElem->hasChildNodes() )
	{
		DOMNode* lastChild = pDomElem->getLastChild();
		while( lastChild != NULL )
		{
			pDomElem->removeChild( lastChild );

			lastChild = pDomElem->getLastChild();
		}
	}

	pDomElem->appendChild( cDATANode );

	return true;
}