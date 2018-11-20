#pragma once
#include "RoadXML/RoadXML.h"

#include <assert.h>

#ifndef WIN32
#include <unistd.h>
#endif

//! Base class for the counted object.
/*! Inherit from this class to be ablme to use CountedPtr on your own classes.
*/
class ROADXML_API CountedObject
{
protected:
	CountedObject(): mRefCount(0) {}
	virtual ~CountedObject(){assert(mRefCount == 0);}
	
public:

	//! AddRef is called when a CountedPtr<> is taken on a 
	virtual size_t	AddRef(){ return ++mRefCount; }
	virtual size_t	Release()
	{
		if (mRefCount == 0)
		{
			//CTNotify("ERROR: TComponent::Release() called with ref count already 0");
			delete this;
			return 0;
		}

		size_t unreleasedCount = --mRefCount;

		if (unreleasedCount == 0)
		{	// No more CountedPtr on this object, it can be deleted
			delete this;
		}
		return unreleasedCount;
	}

	//! Direct access to the counter. 
	virtual void SetRefCount(size_t value) {mRefCount=value;}

protected:

	size_t	mRefCount;
};
