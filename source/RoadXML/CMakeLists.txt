
IF(WIN32)
 ADD_DEFINITIONS(-DROADXML_EXPORTS)
 INCLUDE_DIRECTORIES(${RAPIDXML_DIR})
 INCLUDE_DIRECTORIES(${XERCESC_DIR}/include) 
 LINK_DIRECTORIES(${XERCESC_DIR}/lib)
ENDIF(WIN32)

ADD_LIBRARY(RoadXML SHARED 
../../include/RoadXML/CountedObject.h
../../include/RoadXML/CountedPtr.h
../../include/RoadXML/Element.h
../../include/RoadXML/Encoding.h
../../include/RoadXML/IDOMElement.h
../../include/RoadXML/IDOMParser.h
../../include/RoadXML/RoadXML.h
../../include/RoadXML/RoadXMLClippedDataElements.h
../../include/RoadXML/RoadXMLCurveElements.h
../../include/RoadXML/RoadXMLElements.h
../../include/RoadXML/RoadXMLFunctionElements.h
../../include/RoadXML/RoadXMLGroundElements.h
../../include/RoadXML/RoadXMLMaterialElements.h
../../include/RoadXML/RoadXMLProfileElements.h
../../include/RoadXML/RoadXMLSignalElements.h
../../include/RoadXML/RoadXMLTags.h
../../include/RoadXML/RoadXMLTrackElements.h
../../include/RoadXML/RoadXMLTrajectoryElements.h
../../include/RoadXML/RoadXMLTrajectoryPartElements.h
../../include/RoadXML/RoadXMLVersion.h
../../include/RoadXML/RoadXMLModifierElement.h
Transcode.h
CountedObject.cpp
CountedPtr.cpp
DOMTreeErrorReporter.cpp
Element.cpp
Encoding.cpp
RoadXMLClippedDataElements.cpp
RoadXMLCurveElements.cpp
RoadXMLElements.cpp
RoadXMLFunctionElements.cpp
RoadXMLGroundElements.cpp
RoadXMLMaterialElements.cpp
RoadXMLProfileElements.cpp
RoadXMLSignalElements.cpp
RoadXMLTrackElements.cpp
RoadXMLTrajectoryElements.cpp
RoadXMLTrajectoryPartElements.cpp
RoadXMLVersion.cpp
Transcode.cpp
XercesDOMElementImpl.cpp
XercesDOMParserImpl.cpp
RapidXMLDOMElementImpl.h
RapidXMLDOMParserImpl.h
RapidXMLDOMElementImpl.cpp
RapidXMLDOMParserImpl.cpp
RoadXMLModifierElement.cpp
../../include/RoadXML/RoadXMLRollingSurfacePropsElement.h
RoadXMLRollingSurfacePropsElement.cpp
)

IF(WIN32)
	TARGET_LINK_LIBRARIES(RoadXML stk)
	TARGET_LINK_LIBRARIES(RoadXML optimized xerces-c_3.lib debug xerces-c_3${CMAKE_DEBUG_POSTFIX}.lib)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(RoadXML stk xerces-c)
ENDIF(WIN32)

install (TARGETS RoadXML DESTINATION lib)
install (DIRECTORY ../../include/RoadXML DESTINATION include)
