/*
textProcessing.h
----------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 31, 2014

Primary basis: None
Other references: Horton, Ivor. Beginning Visual C++ 2008. Indianapolis: Wiley Publishing Inc., 2008
  (Specifically the 'calculator' program in Chapter 6)

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A namespace containing functions for processing text (stored in string objects or character arrays)
*/

#pragma once;

#include <windows.h> // For the HRESULT type
#include <string>

namespace textProcessing {

	/* Removes characters with decimal ASCII values in the
	closed interval [1, 32] and the Delete character (127)
	from the character array (which must be null-terminated).
	The array (first parameter) is output as a same length or shorter,
	null-terminated character string.

	However, an array of exceptions can be passed in as the second
	parameter. These characters will be preserved.
	The final parameter is the number of elements in the
	'ignore' array.

	If the function encounters an error, it may still have altered
	the input string before returning a failure code.
	*/
	HRESULT remove_ASCII_Control(char* const str, const char* const ignore, const int nIgnore);

	/* Converts a wide character string literal stored
	in an ASCII string to a wide character
	string object. The string literal must be prefixed by 'L"' and
	postfixed by '"' in order to be accepted.

	The index parameter is the point in the string at which to start
	parsing, and is moved to the end of the parsed section if the
	call succeeds.
	*/
	HRESULT strToWString(std::wstring& out, const char* const in, size_t& index);

	/* Essentially the inverse of strToWString() */
	HRESULT wstringToWStringLiteral(std::wstring& out, const std::wstring& in);

	/* Converts the ASCII strings 'true' and 'false' to bool values.

	The index parameter is the point in the string at which to start
	parsing, and is moved to the end of the parsed section if the
	call succeeds.
	*/
	HRESULT strToBool(bool& out, const char* const in, size_t& index);

	/* Essentially the inverse of strToBool() */
	HRESULT boolToWString(std::wstring& out, const bool& in);

}