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

#define ESCAPE_CHAR '\\'
#define W_ESCAPE_CHAR L'\\'

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
		throw std::exception("hasPrefix() was passed null pointer(s).");
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

bool textProcessing::hasSubstr(const char* const str, const char* const sub, size_t& index,
	const size_t& startOffset) {

	// Error checking
	if( str == 0 || sub == 0 ) {
		// This is a Microsoft-specific constructor
		throw std::exception("hasSubstr() was passed null pointer(s).");
	}
	if( startOffset >= strlen(str) ) {
		// This is a Microsoft-specific constructor
		throw std::exception("hasSubstr() was passed a starting index greater than or equal to the length of the string.");
	}

	if( *str == '\0' && sub == '\0' ) {
		index = 0;
		return true;
	}
	const char* suffix = str + startOffset;
	while( *suffix != '\0' ) {
		if( hasPrefix(suffix, sub) ) {
			index = static_cast<size_t>(suffix - str);
			return true;
		}
		++suffix;
	}
	return false;
}

static const wchar_t s_escapeSequenceEnds[] = {
	L'"',
	L't',
	L'n'
};

static const wchar_t s_escapeSequenceResults[] = {
	L'"',
	L'\t',
	L'\n'
};

static const size_t s_nEscapeSequences = sizeof(s_escapeSequenceEnds) / sizeof(s_escapeSequenceEnds[0]);

HRESULT textProcessing::wStrLiteralToWString(std::wstring& out, const char* const in, size_t& index) {

	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	}

	/* Check the start of the string
	   Note: This condition statement screens out the empty string
	 */
	if( in[index] == 'L' && in[index + 1] == '"' ) {

		// Find the end of the string literal
		bool foundEnd = false;
		size_t beginIndex = index + 2;
		size_t endIndex = 0;
		if( FAILED(findFirstNonEscaped(in, beginIndex, '"', foundEnd, endIndex)) ) {
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);

		} else if( foundEnd ) {

			// The string contains a string literal
			// ------------------------------------

			size_t wSize = endIndex - beginIndex + 1; // Buffer length, not string length!

			// The string is not empty
			if( wSize > 1 ) {

				// Convert to a wide-character string
				wchar_t* wCStr = new wchar_t[wSize];
				size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, wCStr, wSize, in + beginIndex, _TRUNCATE);

				if( convertedChars != wSize ) {
					delete[] wCStr;
					return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_LIBRARY_CALL);
				}

				// Convert recognized escape codes to characters
				wchar_t* copyTo = wCStr;
				wchar_t* copyFrom = wCStr;
				bool escaped = false;
				bool isEscapeSeq = false;
				size_t escapeSeqIndex = 0;
				while( *copyFrom != L'\0' ) {
					if( !escaped ) {
						if( *copyFrom == W_ESCAPE_CHAR ) {
							escaped = true;
						} else {
							*copyTo = *copyFrom;
							++copyTo;
						}
					} else {
						escaped = false;
						// Check for a recognized escape sequence
						for( escapeSeqIndex = 0; escapeSeqIndex < s_nEscapeSequences; ++escapeSeqIndex ) {
							if( *copyFrom == s_escapeSequenceEnds[escapeSeqIndex] ) {
								isEscapeSeq = true;
								break;
							}
						}
						if( isEscapeSeq ) {
							// Convert the escape sequence
							*copyTo = s_escapeSequenceResults[escapeSeqIndex];
						} else {
							// Copy the unrecognized escape sequence
							*copyTo = *(copyFrom - 1);
							++copyTo;
							*copyTo = *copyFrom;
						}
						++copyTo;
					}
					++copyFrom;
				}

				if( escaped ) {
					*copyTo = *(copyFrom - 1);
					++copyTo;
				}
				*copyTo = *copyFrom; // Copy the null-terminating character

				out = wCStr;
				delete[] wCStr;
			}

			// The string is empty
			else if( wSize == 1 ) {
				out = L"";
			}

			// Something went wrong
			else {
				return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_BROKEN_CODE);
			}

			index = endIndex + 1;
		}
	}

	return ERROR_SUCCESS;
}

HRESULT textProcessing::wstringToWStrLiteral(std::wstring& out, const std::wstring& in) {
	out = L"L\"";

	// Convert recognized special characters to escape sequences

	// The result can at most double in size
	wchar_t* const wCStr_out = new wchar_t[in.length() * 2 + 1];
	const wchar_t* const wCStr_in = in.c_str();

	const wchar_t* copyFrom = wCStr_in;
	wchar_t* copyTo = wCStr_out;
	size_t escapeSeqIndex = 0;
	while( *copyFrom != L'\0' ) {
		for( escapeSeqIndex = 0; escapeSeqIndex < s_nEscapeSequences; ++escapeSeqIndex ) {
			if( *copyFrom == s_escapeSequenceResults[escapeSeqIndex] ) {
				// Found a recognized special character
				*copyTo = W_ESCAPE_CHAR;
				++copyTo;
				break;
			}
		}
		*copyTo = *copyFrom;
		++copyFrom;
		++copyTo;
	}

	// Null-terminate the result
	*copyTo = *copyFrom;

	out += wCStr_out;
	delete[] wCStr_out;

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