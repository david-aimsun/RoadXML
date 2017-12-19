
#define	_CRT_NON_CONFORMING_SWPRINTFS


#include <windows.h>
#include <wchar.h>
#include <direct.h>

#include <fstream>
#include <algorithm>

#include "stk/FileTools.h"
#include "stk/StringTools.h"

#define NOMINMAX
#include <Shlwapi.h>

namespace stk
{
	/* Utility to*/
	std::wstring FromUtf8(const std::string& input)
	{
		std::wstring result;

		if (input.empty())
			return result;

		size_t size = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), NULL, 0);
		// Allocate a buffer of that size plus one for the null and transcode
		wchar_t * buf = new wchar_t[size + 1];
		buf[size] = 0;
		::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), (LPWSTR)buf, (int)size + 1);
		result = buf;
		delete[] buf;

		return result;
	}

	std::string ToUtf8(const std::wstring& input)
	{
		std::string result;

		if (input.empty())
			return result;

		size_t size = ::WideCharToMultiByte(CP_UTF8, 0, input.c_str(), (int)input.size(), NULL, 0, NULL, NULL);
		// Allocate a buffer of that size plus one for the null and transcode
		char * buf = new char[size + 1];
		buf[size] = 0;
		::WideCharToMultiByte(CP_UTF8, 0, input.c_str(), (int)input.size(), buf, (int)size + 1, NULL, NULL);
		result = buf;
		delete[] buf;

		return result;
	}
}

bool stk::FileExist(const char* filename)
{
	return stk::FileExist(FromUtf8(filename).c_str());
}

bool stk::FileExist(const wchar_t* filename)
{
	return PathFileExistsW(filename) == TRUE;
}

const char* stk::JustDrive(const char *Name)
{
	if (Name == NULL)
		return "";

	static std::string res;
	res = ToUtf8(stk::JustDrive(FromUtf8(Name).c_str()));
	return res.c_str();
}

const wchar_t* stk::JustDrive(const wchar_t *Name)
{
	if (Name == NULL)
		return L"";

	static wchar_t drive[FILENAME_MAX];
	wchar_t dir[FILENAME_MAX];
	wchar_t fname[FILENAME_MAX];
	wchar_t ext[FILENAME_MAX];
	_wsplitpath(Name, drive, dir, fname, ext);
	return drive;
}

const char* stk::JustExtension(const char *Name)
{
	if (Name == NULL)
		return "";

	static std::string res;
	res = ToUtf8(stk::JustExtension(FromUtf8(Name).c_str()));
	return res.c_str();
}

const wchar_t* stk::JustExtension(const wchar_t *Name)
{
	if (Name == NULL)
		return L"";

	wchar_t drive[FILENAME_MAX];
	wchar_t dir[FILENAME_MAX];
	wchar_t fname[FILENAME_MAX];
	static wchar_t ext[FILENAME_MAX];
	_wsplitpath(Name, drive, dir, fname, ext);
	return ext;
}

std::string stk::GetApplicationPath()
{
	return ToUtf8(GetApplicationPathW());
}

std::wstring stk::GetApplicationPathW()
{
	wchar_t appName[1024];
	::GetModuleFileNameW(NULL, appName, 1024);

	wchar_t drive[32];
	wchar_t dir[1024];
	wchar_t fname[1024];
	wchar_t ext[32];
	_wsplitpath(appName, drive, dir, fname, ext);

	std::wstring applicationPath(drive);
	applicationPath += std::wstring(dir);

	return applicationPath;
}

std::string stk::GetTemporaryPath()
{
	wchar_t buffer[MAX_PATH+1] = { L'\0' };
	GetTempPathW(MAX_PATH, buffer);
	return ToUtf8(buffer);
}

const char* stk::JustFileName(const char *Name)
{
	if (Name == NULL)
		return "";

	static std::string res;
	res = ToUtf8(JustFileName(FromUtf8(Name).c_str()));
	return res.c_str();
}

const wchar_t* stk::JustFileName(const wchar_t *Name)
{
	if (Name == NULL)
		return L"";

	static std::wstring Result;
	wchar_t drive[FILENAME_MAX];
	wchar_t dir[FILENAME_MAX];
	wchar_t fname[FILENAME_MAX];
	wchar_t ext[FILENAME_MAX];
	_wsplitpath(Name, drive, dir, fname, ext);
	Result = std::wstring(fname) + ext;
	return Result.c_str();
}


const char* stk::WithoutExtension(const char* Name)
{
	if (Name == NULL)
		return "";

	static std::string res;
	res = ToUtf8(WithoutExtension(FromUtf8(Name).c_str()));
	return res.c_str();
}

const wchar_t* stk::WithoutExtension(const wchar_t* Name)
{
	if (Name == NULL)
		return L"";

	static std::wstring Result;
	wchar_t drive[FILENAME_MAX];
	wchar_t dir[FILENAME_MAX];
	wchar_t fname[FILENAME_MAX];
	wchar_t ext[FILENAME_MAX];
#ifdef WIN32
	_wsplitpath(Name, drive, dir, fname, ext);
#endif
	Result = std::wstring(drive) + dir + fname;
	return Result.c_str();
}

const char* stk::JustDirName(const char *Name)
{
	if (Name == NULL)
		return "";

	static std::string res;
	res = ToUtf8(JustDirName(FromUtf8(Name).c_str())).c_str();
	return res.c_str();
}

//-------------------------------------------------------------------------------------------------------------------
const wchar_t* stk::JustDirName(const wchar_t *Name)
{
	if (Name == NULL)
		return L"";

	static std::wstring Result;
	wchar_t drive[FILENAME_MAX];
	wchar_t dir[FILENAME_MAX];
	wchar_t fname[FILENAME_MAX];
	wchar_t ext[FILENAME_MAX];
#ifdef WIN32
	_wsplitpath(Name, drive, dir, fname, ext);
#endif
	Result = std::wstring(drive) + dir;
	return Result.c_str();
}

//-------------------------------------------------------------------------------------------------------------------
//! Restore correct case for files
static std::wstring GetActualPathName(const wchar_t* path)
{
	// This is quite involved, but the meat is SHGetFileInfo
	const wchar_t kSeparator = L'\\';

	std::wstring str = path;
	std::replace(str.begin(), str.end(), '/', '\\');
	const std::wstring from = L"\\\\", to = L"\\";
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::wstring::npos)
		str.replace(start_pos, from.length(), to);

	// copy input string because we'll be temporary modifying it in place
	size_t length = wcslen(str.c_str());
	wchar_t buffer[MAX_PATH];
	memcpy(buffer, str.c_str(), (length + 1) * sizeof(path[0]));

	size_t i = 0;

	std::wstring result;

	// for network paths (\\server\share\RestOfPath), getting the display
	// name mangles it into unusable form (e.g. "\\server\share" turns
	// into "share on server (server)"). So detect this case and just skip
	// up to two path components
	if (length >= 2 && buffer[0] == kSeparator && buffer[1] == kSeparator)
	{
		int skippedCount = 0;
		i = 2; // start after '\\'
		while (i < length && skippedCount < 2)
		{
			if (buffer[i] == kSeparator)
				++skippedCount;
			++i;
		}

		result.append(buffer, i);
	}
	// for drive names, just add it uppercased
	else if (length >= 2 && buffer[1] == L':')
	{
		result += towupper(buffer[0]);
		result += L':';
		if (length >= 3 && buffer[2] == kSeparator)
		{
			result += kSeparator;
			i = 3; // start after drive, colon and separator
		}
		else
		{
			i = 2; // start after drive and colon
		}
	}

	size_t lastComponentStart = i;
	bool addSeparator = false;

	while (i < length)
	{
		// skip until path separator
		while (i < length && buffer[i] != kSeparator)
			++i;

		if (addSeparator)
			result += kSeparator;

		// if we found path separator, get real filename of this
		// last path name component
		bool foundSeparator = (i < length);
		buffer[i] = 0;
		SHFILEINFOW info;

		// nuke the path separator so that we get real name of current path component
		info.szDisplayName[0] = 0;
		if (SHGetFileInfoW(buffer, 0, &info, sizeof(info), SHGFI_DISPLAYNAME))
		{
			result += info.szDisplayName;
		}
		else
		{
			// most likely file does not exist.
			// So just append original path name component.
			result.append(buffer + lastComponentStart, i - lastComponentStart);
		}

		// restore path separator that we might have nuked before
		if (foundSeparator)
			buffer[i] = kSeparator;

		++i;
		lastComponentStart = i;
		addSeparator = true;
	}

	std::replace(result.begin(), result.end(), '\\', '/');

	return result;
}

//-------------------------------------------------------------------------------------------------------------------
std::string stk::RestoreCase(const std::string& fileName)
{
	std::wstring fnamew = FromUtf8(fileName);
	return ToUtf8(GetActualPathName(fnamew.c_str()));
}

//-------------------------------------------------------------------------------------------------------------------
std::string stk::AbsoluteFilename(const std::string& currentDirectory, const std::string& relativeOrAbsoluteFilename, bool fixCase)
{
	std::wstring currentDirectoryW = FromUtf8(currentDirectory);
	std::wstring relativeOrAbsoluteFilenameW = FromUtf8(relativeOrAbsoluteFilename);
	if (PathIsRelativeW(relativeOrAbsoluteFilenameW.c_str()) == FALSE)
	{	// It's an absolute path
		return relativeOrAbsoluteFilename;
	}
	else
	{	
		wchar_t fullPath[MAX_PATH];
		wchar_t cleanPath[MAX_PATH];

		std::replace(currentDirectoryW.begin(), currentDirectoryW.end(), '/', '\\');
		std::replace(relativeOrAbsoluteFilenameW.begin(), relativeOrAbsoluteFilenameW.end(), '/', '\\');

		if (PathCombineW(fullPath, currentDirectoryW.c_str(), relativeOrAbsoluteFilenameW.c_str()) == NULL)
			return "";

		if (!PathCanonicalizeW(cleanPath, fullPath))
			return "";

#ifdef WIN32
		if (fixCase)
			return ToUtf8(GetActualPathName(cleanPath)); 			// Restore Case
		else
			return ToUtf8(cleanPath);
#else
		return ToUtf8(cleanPath);
#endif
	}
}

bool stk::IsAbsolutePath(const std::string& path)
{
	return ((path.size() >= 3)
		&& (path[1] == ':')
		&& (IsSeparator(path[2])));
}

std::string stk::RelativeFilename(const std::string& absoluteDirectory, const std::string& absoluteFilename)
{
	// Clean up the path (remove the useless ../)
	std::string cleanAbsoluteFilename = NormalizePath(absoluteFilename);
	std::string cleanAbsoluteDirectory = NormalizePath(absoluteDirectory);

	if ((!IsAbsolutePath(cleanAbsoluteDirectory))
		|| (!IsAbsolutePath(cleanAbsoluteFilename)))
	{
		// If the input path is not absolute, it should already be relative, so return it
		return absoluteFilename;
	}

	wchar_t szOut[MAX_PATH] = L"";

	std::replace(cleanAbsoluteDirectory.begin(), cleanAbsoluteDirectory.end(), '/', '\\');
	std::replace(cleanAbsoluteFilename.begin(), cleanAbsoluteFilename.end(), '/', '\\');

	BOOL res = PathRelativePathToW(szOut, FromUtf8(cleanAbsoluteDirectory).c_str(), FILE_ATTRIBUTE_DIRECTORY, FromUtf8(cleanAbsoluteFilename).c_str(), FILE_ATTRIBUTE_NORMAL);

	if (res == TRUE)
	{
		std::string result = ToUtf8(szOut);
		std::replace(result.begin(), result.end(), '\\', '/');

		// Remove useless "./"
		if (result[0] == '.' && result[1] == '/')
			result.erase(0, 2);
		return result;
	}
	else
	{
		return absoluteFilename;
	}

}
