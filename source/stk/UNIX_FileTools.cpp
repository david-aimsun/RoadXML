#include "stk/FileTools.h"
#include "stk/StringTools.h"

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>
#include <fstream>

void string_printf(std::string& s, const char* format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	s = buffer;
}

bool stk::FileExist(const char* filename)
{
        std::ifstream in(filename);
        return in.good();
}


const char* stk::JustDrive(const char *Name)
{
	return "";
}

const char* stk::JustExtension(const char *Name)
{
	if (Name==NULL)
		return "";

	static	char result[1024];
	strncpy(result, Name, 1024);
	char * res = strrchr(result,'.');

	if (res)
        return res;
	else
        return "";
}

std::string stk::GetApplicationPath()
{
    std::string path;

    char p[PATH_MAX];
    ssize_t res = readlink("/proc/self/exe", p, PATH_MAX);

    if (res != -1)
    {
        p[res] = 0;
        path = dirname(p);
    }
    else
        path = getcwd(p, PATH_MAX);

    return path + "/";
}

std::string stk::GetTemporaryPath()
{
    char * tmp = getenv("TMPDIR");
    if (!tmp)
        return "/tmp/";
    else
        return tmp;
}

//  Renvoie le nom de fichier avec extension                        
const char* stk::JustFileName(const char *Name)
{
	if (Name==NULL)
		return "";
	else
	{
		static	char result[1024];
		strncpy(result, Name, 1024);
		return basename(result);
	}
}

// Renvoie le nom de fichier sans extention
const char* stk::WithoutExtension(const char* Name)
{
    if (Name==NULL || Name[0] == 0)
		return "";
	else
	{
		static	char result[1024];
		strncpy(result, Name, 1024);
		char * temp = basename(result);
		char * res = strrchr(result,'.');
        if (res)
        {
            *res = '\0';
            return temp;
        }
        else
            return Name;
	}
}

// Renvoie le path d'un nom de fichier
const char* stk::JustDirName(const char *Name)
{
	if (Name==NULL)
		return "";
	else
	{
		static	char result[1024];
		strncpy(result, Name, 1024);
		return dirname(result);
	}
}

//-------------------------------------------------------------------------------------------------------------------
std::string stk::RestoreCase(const std::string& fileName)
{
	return fileName; // Do nothing
}

std::string stk::AbsoluteFilename(const std::string& currentDirectory, const std::string& relativeOrAbsoluteFilename, bool fixCase)
{
    if (relativeOrAbsoluteFilename.empty() || (relativeOrAbsoluteFilename[0] == '/'))
    {	// It's an absolute path
        return relativeOrAbsoluteFilename;
    }
    else
    {
        std::string fullPath= currentDirectory + "/" + relativeOrAbsoluteFilename;
        char * cleanpath = realpath(fullPath.c_str(), NULL);
        std::string result;
        if (cleanpath)
        {
            result = cleanpath;
            free(cleanpath);
        }
        else
        {
            result = fullPath;
        }

        return result;
    }
}

bool stk::IsAbsolutePath(const std::string& path)
{
	return (!path.empty()) && (path[1] == '/');
}

std::string stk::RelativeFilename(const std::string& absoluteDirectory, const std::string& absoluteFilename)
{
#ifndef _RTX
	// Clean up the path (remove the useless ../)
	std::string cleanAbsoluteFilename = NormalizePath(absoluteFilename);
	std::string cleanAbsoluteDirectory = NormalizePath(absoluteDirectory);
#else
	std::string cleanAbsoluteFilename(absoluteFilename);
	std::string cleanAbsoluteDirectory(absoluteDirectory);
#endif

	if ((!IsAbsolutePath(cleanAbsoluteDirectory))
		|| (!IsAbsolutePath(cleanAbsoluteFilename)))
	{
		// If the input path is not absolute, it should already be relative, so return it
		return absoluteFilename;
	}

	// declarations - put here so this should work in a C compiler
	size_t afMarker = 0;
	size_t cdLen = 0, afLen = 0;
	size_t i = 0;
	size_t levels = 0;
	std::string relativeFilename;

	cdLen = cleanAbsoluteDirectory.length();
	afLen = cleanAbsoluteFilename.length();

	// Handle DOS names that are on different drives:
	if (cleanAbsoluteDirectory[0] != cleanAbsoluteFilename[0])
	{
		// not on the same drive, so only absolute filename will do
		return absoluteFilename;
	}

	// they are on the same drive, find out how much of the current directory
	// is in the absolute filename
	i = 3;
	while (i < afLen && i < cdLen && cleanAbsoluteDirectory[i] == cleanAbsoluteFilename[i])
	{
		i++;
	}

	if (i == cdLen && (IsSeparator(cleanAbsoluteFilename[i]) || IsSeparator(cleanAbsoluteFilename[i - 1])))
	{
		// the whole current directory name is in the file name,
		// so we just trim off the current directory name to get the
		// current file name.
		if (IsSeparator(cleanAbsoluteFilename[i]))
		{
			// a directory name might have a trailing slash but a relative
			// file name should not have a leading one...
			i++;
		}

		relativeFilename.assign(cleanAbsoluteFilename.begin() + i, cleanAbsoluteFilename.end());
		return relativeFilename;
	}


	// The file is not in a child directory of the current directory, so we
	// need to step back the appropriate number of parent directories by
	// using "..\"s.  First find out how many levels deeper we are than the
	// common directory
	afMarker = i;
	levels = 1;

	// count the number of directory levels we have to go up to get to the
	// common directory
	while (i < cdLen)
	{
		i++;
		if (IsSeparator(cleanAbsoluteDirectory[i]))
		{
			// make sure it's not a trailing slash
			i++;
			if (cleanAbsoluteDirectory[i] != '\0')
			{
				levels++;
			}
		}
	}

	// move the absolute filename marker back to the start of the directory name
	// that it has stopped in.
	while (afMarker > 0 && !IsSeparator(cleanAbsoluteFilename[afMarker - 1]))
	{
		afMarker--;
	}

	// add the appropriate number of "..\"s.
	for (i = 0; i < levels; i++)
	{
		relativeFilename += "../";
	}

	// copy the rest of the filename into the result string
	relativeFilename.append(cleanAbsoluteFilename.begin() + afMarker, cleanAbsoluteFilename.end());

	return relativeFilename;
}
