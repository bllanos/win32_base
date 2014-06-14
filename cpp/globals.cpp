/*
globals.cpp
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of the functions declared in globals.h
*/

#include "globals.h"
#include "defs.h"

/* For PathCchRemoveFileSpec() (Windows-specific)
   Requires linking Pathcch.lib
*/
// #include <Pathcch.h> (Available only on Windows 8)

/* For PathRemoveFileSpec() (Deprecated, but available on Windows 7)
   Requires linking Shlwapi.lib
*/
#include <Shlwapi.h>

HRESULT toWString(std::wstring& wStr, const std::string& str) {
	/* To convert an ASCII character string to a wide character string
	See http://msdn.microsoft.com/en-us/library/ms235631.aspx
	for more information on converting between string types.
	*/
	const char* cStr = str.c_str();
	size_t wSize = strlen(cStr) + 1;
	wchar_t* wCStr = new wchar_t[wSize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wCStr, wSize, cStr, _TRUNCATE);

	if (convertedChars != wSize) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	wStr = wCStr;
	delete[] wCStr;
	return ERROR_SUCCESS;
}

HRESULT extractPath(std::wstring& path, const std::wstring& filenameAndPath) {

	HRESULT result = ERROR_SUCCESS;

	// Get a copy of the combined filename and path
	size_t bufferLength = filenameAndPath.length() + 1;
	wchar_t* pathBuffer = new wchar_t[bufferLength];
	if( !wcscpy_s(pathBuffer, bufferLength, filenameAndPath.c_str()) ) {
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