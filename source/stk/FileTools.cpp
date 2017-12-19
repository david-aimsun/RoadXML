#include "stk/FileTools.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <algorithm>

bool stk::IsSeparator(char c)
{
	return c=='\\' || c=='/';
}

void stk::MoveIfExists(const std::string& fileName )
{
	std::string cleanFileName = stk::NormalizePath( fileName );

	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	std::stringstream savedName;
	savedName << stk::WithoutExtension( cleanFileName.c_str() ) 
		<< "_" << timeinfo->tm_year + 1900
		<< "_" << timeinfo->tm_mon
		<< "_" << timeinfo->tm_mday
		<< "_" << timeinfo->tm_hour
		<< "." << timeinfo->tm_min
		<< "." << timeinfo->tm_sec
		<< stk::JustExtension( cleanFileName.c_str() );

	rename(cleanFileName.c_str(), savedName.str().c_str());
}

std::string stk::FixPathSeparators(const std::string & path)
{
	std::string fixedPath = path;

	// replace all "\" by "/" for system compatibility.

	for(unsigned int i = 0; i < fixedPath.length(); i++)
	{
		if(path[i] == '\\')
			fixedPath[i] = '/';
	}

	// replace ":/" by ":\" to restore windows drive letter if necessary.

	size_t loc = fixedPath.find( ":/", 0 );
	if(loc != std::string::npos)
		fixedPath.replace( loc, 2, ":\\", 0, 2);

	return fixedPath;
}

size_t Biggest( size_t A, size_t B )
{
	if( A==std::string::npos )
		return B;
	if( B==std::string::npos )
		return A;

	return std::max( A, B );
}

size_t Smallest( size_t A, size_t B )
{
	if( A==std::string::npos )
		return B;
	if( B==std::string::npos )
		return A;

	return std::min( A, B );
}

// Clean up the path (remove the useless ../)
std::string RemoveUselessDoubleDot( const std::string& path )
{
	std::string result(path);

	size_t searchFrom = 0; 
	size_t found = result.find( "..", searchFrom );
	while( found!=std::string::npos )
	{	
		if(  found>1 )
		{
			// Remove the .. and the /xxx/ before
			size_t secondPoint = found+1;;
			size_t firstSeparator = Biggest( result.rfind('/',found-2), result.rfind('\\',found-2) );

			size_t prevDoubleDot = result.rfind( "..", found-1 );
			if( prevDoubleDot == found-3 )
			{	// Ignore it and move further
				searchFrom = found+2;
			}
			else
			{
				if( firstSeparator==std::string::npos )
				{	// Remove the begining of the string
					result.erase( 0, secondPoint+1 );
				}
				else
				{	// Remove the part between the /xxx/ if it's not /../
						size_t nbChars = secondPoint-firstSeparator+1;
						result.erase( firstSeparator, nbChars );
				}
			}
		}
		else
		{
			searchFrom = found+2;
		}

		found = result.find( "..", searchFrom );
	}

	return result;
}

// Clean up the path (remove the useless ../)
std::string RemoveUselessDash( const std::string& path )
{
	std::string result(path);

	size_t found = Smallest( result.find( '/', 0 ), result.find( '\\', 0 ) );
	while( found!=std::string::npos && found<result.size()-1 )
	{	
		// Check the next character
		while( found+1<result.size() && stk::IsSeparator( result[found+1] ) )
		{
			result.erase( found+1, 1 );
		}

		found = Smallest( result.find( '/', found+1 ), result.find( '\\', found+1 ) );
	}

	return result;
}


std::string CleanUpPath( const std::string& path )
{
	std::string clean = stk::FixPathSeparators( path );
	bool addDash = false;
	if(clean.size() >= 2 && clean[0] == '/' && clean[1] == '/')
		addDash = true;
	clean = RemoveUselessDash( clean );
	if(addDash)
		clean = '/' + clean;
	clean = RemoveUselessDoubleDot( clean );

	return clean;
}

std::string stk::NormalizePath( const std::string& path )
{
	std::string clean = CleanUpPath( path );
	return clean;
}



// See comment in .h

