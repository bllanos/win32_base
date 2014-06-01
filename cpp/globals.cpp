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