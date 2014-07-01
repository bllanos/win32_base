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

#define ESCAPE_CHAR '\\'

namespace textProcessing {

	/* Removes characters from the character array (which must be null-terminated)
	as follows (exceptions are discussed afterwards):
		-Characters with decimal ASCII values in the
			closed interval [1, 32]
		-The Delete character (127)
		-Characters with decimal values greater than 127
	
	The array (first parameter) is output as a same length or shorter,
	null-terminated character string.

	Additional parameters:
		-The next two parameters are an array of characters to preserve
			in normal areas of the string and the number of these
			 these characters (i.e. length of the array), respectively.
		-Next, the client can pass in a delimiter character
			which will be interpreted as surrounding "special"
			areas of the input string.
			Delimiters are never stripped.
		-The following two parameters are an array of characters
			to preserve in "special" areas of the string
			and the number of these characters, respectively.

	Note that the list of exception characters for normal areas of the string
	is not applied to the special (delimited) areas of the string.

	The occurrence of an unmatched delimiter character
	will not be seen as the start of a "special" area of the string.
	Delimiters can be escaped by preceding them with '\'.
	'\' is not allowed as a delimiter.

	The function will do nothing and return an error code
	if any of the count/length parameters are nonzero,
	but the corresponding arrays are null pointers,
	or if the count parameters are zero, but the correpsonding
	arrays are not null pointers.

	If the function encounters an error, it may still have altered
	the input string before returning a failure code.
	*/
	HRESULT remove_ASCII_controlAndWhitespace(char* const str, const char* const ignore = 0, const size_t nIgnore = 0,
		const char delim = '"', const char* const specialIgnore = 0, const size_t nSpecialIgnore = 0);

	/* Finds the next occurrence of the 'target' character
	in the string that is not preceded by an odd number of '\'
	(i.e. the character is not "escaped"). The search begins
	at the 'startOffset' index.

	If the target character is found, the 'found' parameter
	will be set to true.

	The function outputs false to its 'found' parameter,
	but still returns a success code,
	if the 'target' non-escaped character is not found.
	(Failure codes are returned if there are faults.)
	*/
	HRESULT findFirstNonEscaped(const char* const str, const size_t& startOffset,
		const char target, bool& found, size_t& foundOffset);

	/* Converts a wide character string literal stored
	as a substring of a null-terminated ASCII string to a wide character
	string object. The string literal must be prefixed by 'L"' and
	postfixed by '"' (not escaped) in order to be accepted.

	The index parameter is the point in the string at which to start
	parsing, and is moved to the end of the parsed section if the
	call succeeds.

	Failure codes are returned due to internal errors, or if the input
	string does not contain a wide character string literal.
	*/
	HRESULT wStrLiteralToWString(std::wstring& out, const char* const in, size_t& index);

	/* Essentially the inverse of wStrLiteralToWString() */
	HRESULT wstringToWStrLiteral(std::wstring& out, const std::wstring& in);

	/* Converts the null-terminated
	ASCII string containing the substrings 'true' or 'false' to bool values.
	(The first such substring encountered is assigned to the output parameter,
	at which point the function returns.)

	The index parameter is the point in the string at which to start
	parsing, and is moved to the end of the parsed section if the
	call succeeds.

	Failure codes are returned due to internal errors, or if the input
	string does not contain a bool literal.
	*/
	HRESULT strToBool(bool& out, const char* const in, size_t& index);

	/* Essentially the inverse of strToBool() */
	HRESULT boolToWString(std::wstring& out, const bool& in);

}