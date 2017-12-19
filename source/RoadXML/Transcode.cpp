#include "Transcode.h"
#include "RoadXML/Encoding.h"

#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_USE

std::string RoadXML::Transcode(const RoadXML::XercesString& input)
{
	std::string result;

	if (input.empty())
		return result;

	if (RoadXML::Encoding::GetMode() == Encoding::LOCAL8)
	{
		char * str = XMLString::transcode(input.c_str());
		result = str;
		XMLString::release(&str);
	}
	else if (RoadXML::Encoding::GetMode() == Encoding::UTF8)
	{
		TranscodeToStr tts(input.c_str(), "UTF-8");
		result = (const char*)tts.str();
	}

	return result;
}

RoadXML::XercesString RoadXML::Transcode(const std::string& input)
{
	RoadXML::XercesString result;

	if (input.empty())
		return result;

	if (RoadXML::Encoding::GetMode() == Encoding::LOCAL8)
	{
		XMLCh * str = XMLString::transcode(input.c_str());
		result = str;
		XMLString::release(&str);
	}
	else if (RoadXML::Encoding::GetMode() == Encoding::UTF8)
	{
		TranscodeFromStr tts((const XMLByte*)input.c_str(), input.size(), "UTF-8");
		result = tts.str();
	}

	return result;
}
