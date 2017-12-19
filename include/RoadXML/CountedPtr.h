#pragma once
#include "RoadXML/RoadXML.h"

#include "CountedObject.h"
#include <stdio.h> // NULL define
#include <assert.h>

inline void ReleaseIfNotNull(void*& ptr)
{
	if (ptr)
	{
		(reinterpret_cast<CountedObject*>(ptr))->Release();
		ptr = NULL;
	}
}

template<class T> inline void ReleaseIfNotNull(T** ptr)
{
	assert(ptr);
	if (ptr && *ptr)
	{
		(*ptr)->Release();
		(*ptr) = NULL;
	}
}

template <class T> class CountedPtr
{
	public:

		CountedPtr() 
		{
			mObject = NULL;
		}
	
		CountedPtr(T* lp)
		{
			if ((mObject = lp) != NULL)
			{
				mObject->AddRef();
			}
		}
	
		CountedPtr(const CountedPtr& lp)
		{
			if ((mObject = lp.mObject) != NULL)
			{
				mObject->AddRef();
			}
		}

		~CountedPtr() 
		{
			if (mObject)
			{ 
				mObject->Release();
				mObject=NULL;
			}
		}

		size_t Release() 
		{
			size_t outRefs = 0;
			if (mObject)
			{
				outRefs = mObject->Release();
			}
			mObject=NULL;
			return outRefs;
		}
		
		operator T*() const
		{ return mObject; }
		
		T& operator*() const
		{ assert(mObject); return *mObject; }

		
		void Assign(T* lp)
		{
			if (lp == mObject) 
				return;

			if (lp != NULL)
				lp->AddRef();

			if (mObject)
				mObject->Release();

			mObject = lp;
		}

		T** operator&() 
		{ return &mObject; }

		T*const* operator&() const
		{ return &mObject; }
		
		CountedPtr* GetAddress()
		{
			return this;
		}

		T* operator->() const
		{ assert(mObject); return mObject; }
		
		CountedPtr& operator=(T* lp)
		{
			Assign(lp);
			return *this;
		}
		
		CountedPtr& operator=(const CountedPtr& lp)
		{
			Assign(lp.mObject);
			return *this;
		}

		bool operator==(const CountedPtr& rhs) const
		{ return (static_cast<T*>(rhs) == mObject); }

		bool operator==(const T* rhs) const
		{ return (rhs == mObject); }

		bool operator==(T* rhs) const
		{ return (rhs == mObject); }

		bool operator!() const {	return (mObject == NULL);}

		T*					Get() const {return mObject;}

		T*					mObject;
};

