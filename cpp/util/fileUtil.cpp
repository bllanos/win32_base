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

/* For PathCchRemoveFileSpec() (Windows-specific)
Requires linking Pathcch.lib
*/
// #include <Pathcch.h> (Available only on Windows 8)

/* For PathRemoveFileSpec() (Deprecated, but available on Windows 7)
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