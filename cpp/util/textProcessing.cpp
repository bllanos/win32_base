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

HRESULT textProcessing::remove_ASCII_controlAndWhitespace(char* const str, const char* const ignore, const size_t nIgnore,
	const char delim, const char* const specialIgnore, const size_t nSpecialIgnore) {

	// Error checking
	if( str == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	} else if( (ignore == 0 && nIgnore != 0) || (ignore != 0 && nIgnore == 0) ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	} else if( (specialIgnore == 0 && nSpecialIgnore != 0) || (specialIgnore != 0 && nSpecialIgnore == 0) ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	} else if( delim == '\\' ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	HRESULT result = ERROR_SUCCESS;

	// Ranges of characters to delete
	char controlCharStart = 1;
	char controlCharEnd = 32; // Note: Inclusive endpoint
	char upperRangeStart = 127;

	// Process the input string
	size_t copyTo = 0;
	size_t copyFrom = 0;
	size_t ignoreIndex = 0;
	char current = 0;
	bool escaped = false;

	for( ; str[copyFrom] != '\0'; ++copyFrom ) {

		current = str[copyFrom];
		str[copyTo] = current;

		// Track whether characters are espaced
		if( current == '\\' ) {
			escaped = !escaped;
		} else {
			escaped = false;
		}

		// Check for a special area of the string
		if( current == delim && specialIgnore != 0 && !escaped ) {

			// Search for the second delimiter
			size_t endSection = copyFrom + 1;
			bool otherEscaped = false;
			while( str[endSection] != '\0' ) {
				if( str[endSection] == delim && !otherEscaped ) {
					// Second delimiter found and is not escaped
					break;
				} else if( str[endSection] == '\\' ) {
					otherEscaped = !otherEscaped;
				} else {
					otherEscaped = false;
				}
				++endSection;
			}

			++copyTo;

			if( str[endSection] != '\0' ) {
				// Found a non-escaped second delimiter

				// Mark a substring
				str[endSection] = '\0';

				// Process the substring
				result = remove_ASCII_controlAndWhitespace(str + copyTo,
					specialIgnore, nSpecialIgnore, '\0', 0, 0);
				if( FAILED(result) ) {
					return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				}

				// Put back the delimiter
				str[endSection] = delim;

				// Jump ahead to the end of the processed substring
				while( copyTo < endSection && str[copyTo] != '\0' ) {
					++copyTo;
				}

				// Copy over and skip past the delimiter
				str[copyTo] = delim;
				++copyTo;
				copyFrom = endSection + 1;
				escaped = false;
			}

		} else {

			// Check for a character to delete
			if( (current >= controlCharStart && current <= controlCharEnd) ||
				current >= upperRangeStart ) {

				// Check for a character to ignore (an exception)
				for( ignoreIndex = 0; ignoreIndex < nIgnore; ++ignoreIndex ) {
					if( ignore[ignoreIndex] == current ) {
						++copyTo;
						break;
					}
				}
			} else {
				++copyTo;
			}
		}
	}

	// Null-terminate the input string
	str[copyTo] = '\0';

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