
#pragma once

#include <xercesc/sax/ErrorHandler.hpp>

// Predeclaration
class DOMTreeErrorReporter : public XERCES_CPP_NAMESPACE::ErrorHandler
{
public:
    DOMTreeErrorReporter();
	virtual ~DOMTreeErrorReporter();

    // -----------------------------------------------------------------------
    //  Implementation of the error handler interface
    // -----------------------------------------------------------------------
	void resetErrors();
	void warning(const XERCES_CPP_NAMESPACE::SAXParseException&);
	void error(const XERCES_CPP_NAMESPACE::SAXParseException&);
	void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException&);
    bool getSawErrors() const;
private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};