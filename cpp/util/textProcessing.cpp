/*
textProcessing.cpp
------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
June 15, 2014

Primary basis: None
Other references: Horton, Ivor. Beginning Visual C++ 2008. Indianapolis: Wiley Publishing Inc., 2008
(Specifically the 'calculator' program in Chapter 6)

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of functions in the textProcessing namespace
*/

#include "textProcessing.h"
#include "defs.h"

HRESULT textProcessing::remove_ASCII_ControlAndWhitespace(char* const str, const char* const ignore, const size_t nIgnore) {

	// Error checking
	if( str == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	HRESULT result = ERROR_SUCCESS;

	// Create an initial list of characters to delete
	char start = 1;
	char end = 32;
	size_t nChar = end - start + 1 + 1; // One extra for the Delete character (127)
	char* const charToRemove = new char[nChar];
	for( char c = start; c <= end; ++c ) {
		charToRemove[c - start] = c;
	}
	charToRemove[nChar - 1] = static_cast<char>(127);

	// Remove any characters to ignore from this list
	if( ignore != 0 && nIgnore != 0 ) {
		size_t copyTo = 0;
		size_t copyFrom = 0;
		size_t ignoreIndex = 0;
		for( ; copyFrom < nChar; ++copyFrom ) {
			charToRemove[copyTo] = charToRemove[copyFrom];
			++copyTo;
			for( ignoreIndex = 0; ignoreIndex < nIgnore; ++ignoreIndex ) {
				if( charToRemove[copyFrom] == ignore[ignoreIndex] ) {
					--copyTo;
					break;
				}
			}
		}
		nChar = copyTo;
	}

	// Process the input string
	size_t copyTo = 0;
	size_t copyFrom = 0;
	size_t charToRemoveIndex = 0;
	for( ; str[copyFrom] != '\0'; ++copyFrom ) {
		str[copyTo] = str[copyFrom];
		++copyTo;
		for( charToRemoveIndex = 0; charToRemoveIndex < nChar; ++charToRemoveIndex ) {
			if( charToRemove[charToRemoveIndex] == str[copyFrom] ) {
				--copyTo;
				break;
			}
		}
	}

	// Null-terminate the input string
	str[copyTo] = '\0';

	delete [] charToRemove;
	return result;
}

HRESULT textProcessing::wStrLiteralToWString(std::wstring& out, const char* const in, size_t& index) {

	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	
}

HRESULT textProcessing::wstringToWStrLiteral(std::wstring& out, const std::wstring& in);

HRESULT textProcessing::strToBool(bool& out, const char* const in, size_t& index) {
	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	HRESULT result = ERROR_SUCCESS;

	return result;
}

HRESULT textProcessing::boolToWString(std::wstring& out, const bool& in);