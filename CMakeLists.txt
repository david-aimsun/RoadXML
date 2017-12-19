
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0 FATAL_ERROR)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


set (ROADXML_VERSION 242)

PROJECT(RoadXML)

IF(WIN32)
	IF(MSVC)
		ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ENDIF(MSVC)

	ADD_DEFINITIONS(-DWIN32)

# Change this to your xerces installation folder
	SET(XERCESC_DIR "C:\\Xerces" CACHE PATH "Choose xercesc path")
	ELSE(WIN32)
	SET(XERCESC_DIR "/usr")
ENDIF(WIN32)


INCLUDE_DIRECTORIES(include) 

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

ADD_SUBDIRECTORY(source)
ADD_SUBDIRECTORY(examples)

set_target_properties(RoadXML PROPERTIES SUFFIX "${ROADXML_VERSION}.dll")