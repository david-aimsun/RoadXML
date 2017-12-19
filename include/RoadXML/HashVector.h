#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

namespace RoadXML
{
	/**
	* HashVector is a multi index container. It's a mix between a Hash table and an ordered sequence (like std::vector).
	* That is, access to an element is o(1) constant time (plus the key computation time) and unlike an std::map,
	* elements are ordered at the insertion procedure (when using push_back())
	*
	* @tparam KEY_TYPE the type of the key (eg: a std::string or an int etc...)
	* @tparam VALUE_TYPE the type of the stored value (any type)
	*
	*/
	template<typename VALUE_TYPE>
	class HashVector {

	public:
		typedef std::vector<VALUE_TYPE> VectorType;
        typedef typename VectorType::iterator	iterator;
        typedef typename VectorType::const_iterator	const_iterator;

	private:
		
		VectorType myVector;

		typedef std::multimap<std::string, size_t>			MapType;
        typedef typename MapType::iterator			keyiterator;
        typedef typename MapType::const_iterator	const_keyiterator;
		MapType myMap;

		struct InvalidKey : public std::runtime_error { InvalidKey() :std::runtime_error("The key does not exist") {} };
		typedef std::string KeyType;
		typedef VALUE_TYPE ValueType;

	public:
		// STL like interface
		size_t size() const				{ return myVector.size(); }
		const_iterator begin() const	{ return myVector.begin();	}
		const_iterator end() const		{ return myVector.end(); }
		iterator begin()				{ return myVector.begin(); }
		iterator end()					{ return myVector.end(); }
		bool empty() const				{ return myVector.empty(); }
		void clear()					{ myVector.clear();	myMap.clear(); }

		iterator erase(iterator position) 
		{
			size_t idx = position - begin();
			keyiterator it = myMap.begin();
			while (it != myMap.end())
			{
				if (it->second == idx)
				{
					myMap.erase(it++);
				}
				else
				{
					if( it->second > idx )
					{
						it->second = it->second - 1;
					}
					++it;
				}
			}

			return myVector.erase(position);
		}


		iterator erase(iterator first, iterator last)
		{
			size_t firstidx = first - begin();
			size_t lastidx = last - begin();
			size_t decrease = lastidx - firstidx;
			keyiterator it = myMap.begin();
			while (it != myMap.end()) 
			{
				if ((it->second >= firstidx) && (it->second < lastidx))	
				{
					myMap.erase(it++);
				} 
				else 
				{
					if( it->second >= lastidx )
					{
						it->second = it->second - decrease;
					}
					++it;
				}
			}

			return myVector.erase(first, last);
		}

		void append(const_iterator first, const_iterator last)
		{
			const_iterator it = first;
			while (it != last)
			{
				push_back(*it);
				++it;
			}
		}

		// Insertion
		void push_back(const VALUE_TYPE& val)
		{
			myVector.push_back(val);
			myMap.insert(std::make_pair(val->GetName(), myVector.size() - 1));
		}

		VALUE_TYPE& operator[](size_t idx)
		{
			return myVector[idx];
		}

		const VALUE_TYPE& operator[](size_t idx) const
		{
			return myVector[idx];
		}

		const VALUE_TYPE& operator[](const std::string& key)
		{
			typename MapType::iterator it = myMap.find(key);
			if (it != myMap.end())
			{
				return myVector[it->second];
			}
			else
			{
				push_back(key, VALUE_TYPE());
				return myVector[myVector.size() - 1];
			}
		}

		// Find value or iterator
		VALUE_TYPE  find(const std::string& key) const
		{
			static VALUE_TYPE empty;
			typename MapType::const_iterator it = myMap.find(key);
			if (it != myMap.end())
                return myVector[it->second];
			else
				return empty;
		}

		const_iterator findit(const std::string& key) const
		{
			typename MapType::const_iterator it = myMap.find(key);
			if (it != myMap.end())
				return myVector.begin() + it->second;
			else
				return myVector.end();
		}

		iterator findit(const std::string& key)
		{
			typename MapType::const_iterator it = myMap.find(key);
			if (it != myMap.end())
				return myVector.begin() + it->second;
			else
				return myVector.end();
		}

		template <typename T> 
		void updateKey( const T* item, const std::string& oldKey, const std::string& newKey)
		{
			typedef typename HashVector<CountedPtr<T> >::keyiterator localInterator;

			std::pair< localInterator, localInterator > res = myMap.equal_range(oldKey);
			
			bool found = false;
			localInterator itToChange = myMap.end();
			for (localInterator it=res.first; it!=res.second && !found; ++it)
			{
				size_t index = it->second;
				if( myVector[index] == item )
				{
					itToChange = it;
					found = true;
				}
			}

			if( itToChange != myMap.end() )
			{
				size_t index = itToChange->second;

				myMap.erase( itToChange );
				myMap.insert( std::make_pair(newKey, index ) );
			}
		}

	};
	
}
