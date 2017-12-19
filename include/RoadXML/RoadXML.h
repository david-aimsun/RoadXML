#pragma once

// disable VisualStudio warnings 
#if defined(_MSC_VER)
    #pragma warning( disable : 4251 )
#endif

#if defined(_MSC_VER)
    #  if defined( ROADXML_EXPORTS )
    #    define ROADXML_API   __declspec(dllexport)
    #  else
    #    define ROADXML_API   __declspec(dllimport)
    #  endif
#else
    #  define ROADXML_API
#endif

// set up define for whether member templates are supported by VisualStudio compilers.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif