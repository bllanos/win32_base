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
#include <exception>

HRESULT textProcessing::remove_ASCII_controlAndWhitespace(char* const str, const char* const ignore, const size_t nIgnore,
	const char delim, const char* const specialIgnore, const size_t nSpecialIgnore) {

	// Error checking
	if( str == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	} else if( (ignore == 0 && nIgnore != 0) || (ignore != 0 && nIgnore == 0) ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	} else if( (specialIgnore == 0 && nSpecialIgnore != 0) || (specialIgnore != 0 && nSpecialIgnore == 0) ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	} else if( delim == ESCAPE_CHAR ) {
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

		// Check for a special area of the string
		if( current == delim && specialIgnore != 0 && !escaped ) {

			++copyTo;

			// Search for the second delimiter
			bool foundEndSection = false;
			size_t endSection = 0;
			if( FAILED(findFirstNonEscaped(str, copyFrom + 1, delim, foundEndSection, endSection)) ) {
				return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);

			} else if(foundEndSection) {
				// Found a non-escaped second delimiter

				// Mark a substring
				str[endSection] = '\0';

				// Process the substring
				++copyFrom;
				result = remove_ASCII_controlAndWhitespace(str + copyFrom,
					specialIgnore, nSpecialIgnore, '\0', 0, 0);
				if( FAILED(result) ) {
					return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				}

				// Shift the substring
				size_t substrIndex = 0;
				while( str[substrIndex + copyFrom] != '\0' ) {
					str[substrIndex + copyTo] = str[substrIndex + copyFrom];
					++substrIndex;
				}
				str[substrIndex + copyTo] = str[substrIndex + copyFrom]; // Copy '\0'

				// Put back the delimiter
				str[endSection] = delim;

				// Jump ahead to the end of the processed substring
				while( copyTo < endSection && str[copyTo] != '\0' ) {
					++copyTo;
				}

				// Copy over and skip past the delimiter
				str[copyTo] = delim;
				++copyTo;
				copyFrom = endSection;
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

		// Track whether characters are espaced
		if( current == ESCAPE_CHAR ) {
			escaped = !escaped;
		} else {
			escaped = false;
		}
	}

	// Null-terminate the input string
	str[copyTo] = '\0';

	return result;
}

HRESULT textProcessing::findFirstNonEscaped(const char* const str, const size_t& startOffset,
	const char target, bool& found, size_t& foundOffset) {

	// Error checking
	if( str == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	}

	// Initialize result to "not found"
	foundOffset = 0;
	found = false;

	bool escaped = false;
	const char* testLocation = str + startOffset;
	while( *testLocation != '\0' ) {
		if( *testLocation == target && !escaped ) {
			foundOffset = static_cast<size_t>(testLocation - str);
			found = true;
			break;
		} else if( *testLocation == ESCAPE_CHAR ) {
			escaped = !escaped;
		} else {
			escaped = false;
		}
		++testLocation;
	}

	return ERROR_SUCCESS;
}

bool textProcessing::hasPrefix(const char* str, const char* prefix) {
	// Error checking
	if( str == 0 || prefix == 0 ) {
		// This is a Microsoft-specific constructor
		throw std::exception("hasPrefix() was passed null pointers.");
	}
	while( *prefix != '\0' ) {
		if( *str != *prefix ) {
			return false;
		}
		++str;
		++prefix;
	}
	return true;
}

HRESULT textProcessing::wStrLiteralToWString(std::wstring& out, const char* const in, size_t& index) {

	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	}

	// Check the start of the string
	if( in[index] == 'L' && in[index + 1] == '"' ) {

		// Find the end of the string literal
		bool foundEnd = false;
		size_t beginIndex = index + 2;
		size_t endIndex = 0;
		if( FAILED(findFirstNonEscaped(in, beginIndex, '"', foundEnd, endIndex)) ) {
			return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);

		} else if( foundEnd ) {

			// The string contains a string literal
			size_t wSize = endIndex - beginIndex;
			wchar_t* wCStr = new wchar_t[wSize];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wCStr, wSize, in + beginIndex, _TRUNCATE);

			if( convertedChars != wSize ) {
				return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_LIBRARY_CALL);
			}

			out = wCStr;
			delete[] wCStr;
			index = endIndex + 1;
		}
	}

	return ERROR_SUCCESS;
}

HRESULT textProcessing::wstringToWStrLiteral(std::wstring& out, const std::wstring& in) {
	out = L"L\"";
	out += in;
	out += L'"';
	return ERROR_SUCCESS;
}

HRESULT textProcessing::strToBool(bool& out, const char* const in, size_t& index) {

	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	}

	// Initialization
	const char* subStr = in + index;

	// Constants
	const char* const trueStr = "true";
	const size_t trueLen = 4;
	const char* const falseStr = "false";
	const size_t falseLen = 5;

	if( strncmp(subStr, trueStr, trueLen) == 0 ) {
		out = true;
		index += trueLen;
	} else if( strncmp(subStr, falseStr, falseLen) == 0 ) {
		out = false;
		index += falseLen;
	}
	
	return ERROR_SUCCESS;
}

HRESULT textProcessing::boolToWString(std::wstring& out, const bool& in) {
	if( in ) {
		out = L"true";
	} else {
		out = L"false";
	}
	return ERROR_SUCCESS;
}