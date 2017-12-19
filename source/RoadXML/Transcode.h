#pragma once

#include <string>
#include <xercesc/util/XMLString.hpp>

namespace RoadXML {

	typedef std::basic_string<XMLCh> XercesString;

	//! Transcode strings according to current mode
	std::string Transcode(const XercesString& input);
	XercesString Transcode(const std::string& input);

}