/*
fileUtil.cpp
------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 3, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of functions in the fileUtil namespace
*/

#include "fileUtil.h"
#include "defs.h"
#include <cctype>

/* For PathCchRemoveFileSpec() (Windows-specific)
   Requires linking Pathcch.lib
*/
// #include <Pathcch.h> (Available only on Windows 8)

/* For PathRemoveFileSpec() (deprecated, but available on Windows 7),
   as well as for directory/file existence checks (Windows-specific)

   Requires linking Shlwapi.lib
*/
#include <Shlwapi.h>

using std::wstring;

HRESULT fileUtil::extractPath(std::wstring& path, const std::wstring& filenameAndPath) {

	HRESULT result = ERROR_SUCCESS;

	// Get a copy of the combined filename and path
	size_t bufferLength = filenameAndPath.length() + 1;
	wchar_t* pathBuffer = new wchar_t[bufferLength];
	if( wcscpy_s(pathBuffer, bufferLength, filenameAndPath.c_str()) ) {
		result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_LIBRARY_CALL);

		// Copy operation was successful
	} else {

		/* (Works only on Windows 8 and above, it seems)
		// Remove the last directory or filename from the string
		if(FAILED(PathCchRemoveFileSpec(pathBuffer, bufferLength))) {
		result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_LIBRARY_CALL);
		} else {
		path = pathBuffer;
		}
		*/

		// (Windows 7 - compatible version)
		// Remove the last directory or filename from the string
		PathRemoveFileSpec(pathBuffer);
		path = pathBuffer;
	}
	delete[] pathBuffer;
	return result;
}

// Constants used by inspectFilename()
#define FILEUTIL_DOT L'.'
#define FILEUTIL_USCORE L'_'
#define FILEUTIL_DOT_STR "'.'"
#define FILEUTIL_USCORE_STR "'_'"

HRESULT fileUtil::inspectFilename(const std::wstring& filename, std::string& msg) {

	msg.clear();

	const wchar_t* const filenameCStr = PathFindFileName(filename.c_str());
	size_t filenameCStrSize = wcslen(filenameCStr);

	if( filenameCStr[0] == FILEUTIL_DOT || filenameCStr[filenameCStrSize - 1] == FILEUTIL_DOT ) {
		msg = "Filename starts or ends with ";
		msg += FILEUTIL_DOT_STR;
	} else if( filenameCStr[0] == FILEUTIL_USCORE ) {
		msg = "Filename starts with ";
		msg += FILEUTIL_USCORE_STR;
	} else {

		size_t dotPos = static_cast<size_t>(0);
		for( size_t i = 0; i < filenameCStrSize; ++i ) {
			if( filenameCStr[i] == FILEUTIL_DOT ) {
				if( dotPos != static_cast<size_t>(0) ) {
					msg = "Filename has multiple occurrences of ";
					msg += FILEUTIL_DOT_STR;
					break;
				} else {
					dotPos = i;
				}
			} else if( !isalnum(filenameCStr[i]) ) {
				if( filenameCStr[i] != FILEUTIL_USCORE ) {
					msg = "Filename has a non-alphanumeric character other than ";
					msg += FILEUTIL_DOT_STR;
					msg += " or ";
					msg += FILEUTIL_USCORE_STR;
					break;
				} else if( dotPos != static_cast<size_t>(0) ) {
					msg = "Filename has ";
					msg += FILEUTIL_USCORE_STR;
					msg += " after ";
					msg += FILEUTIL_DOT_STR;
					break;
				}
			}
		}

		// If no problems have been reported yet, perform the remaining checks.
		if( msg.empty() ) {
			if( dotPos == static_cast<size_t>(0) ) {
				msg = "Filename does not contain ";
				msg += FILEUTIL_DOT_STR;
			} else if( filenameCStr[dotPos - static_cast<size_t>(1)] == FILEUTIL_USCORE ) {
				msg = "Filename has ";
				msg += FILEUTIL_USCORE_STR;
				msg += " immediately before ";
				msg += FILEUTIL_DOT_STR;
			}
		}
	}
	return ERROR_SUCCESS;
}

HRESULT fileUtil::inspectFilenameAndPath(const std::wstring& filepath, std::string& msg) {

	HRESULT result = ERROR_SUCCESS;
	msg.clear();

	// Check if the directory where the file will be created is valid
	wstring path;
	if( FAILED(extractPath(path, filepath)) ) {
		msg = "Failure retrieving the file's path.";
	} else if( !path.empty() && !PathIsDirectory(path.c_str()) ) {
		// The location of the file is invalid
		msg = "File's path is invalid.";
	} else {
		// Validate the filename
		if( FAILED(inspectFilename(filepath, msg)) ) {
			msg = "Call to fileUtil::inspectFilename() failed.";
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}
	}
	return result;
}