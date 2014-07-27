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

	/* Determines if the null-terminated string 'prefix'
	is a prefix of the null-terminated string 'str'
	*/
	bool hasPrefix(const char* str, const char* prefix);

	/* Determines if the null-terminated string 'sub'
	is a substring of the null-terminated string 'str',
	and if so, outputs the index at which 'sub'
	first occurs in 'str'.

	The 'startOffset' parameter is the index at which to start searching
	the string
	*/
	bool hasSubstr(const char* const str, const char* const sub, size_t& index,
		const size_t& startOffset = 0);

	/* Converts a wide character C++-style string literal stored
	as a substring of a null-terminated ASCII string to a wide character
	string object. The string literal must be prefixed by 'L"' and
	postfixed by '"' (not escaped) in order to be accepted.

	The index parameter is the point in the string at which to start
	parsing, and is moved to after the end of the parsed section if a
	string literal is found.
	(Otherwise, if a string literal is not found,
	the index parameter and output string parameter are not modified.)

	Failure codes are returned due to internal errors.

	NOTE: Character escape codes are not recognized in the string literal,
		except for the following: \", \t and \n
	*/
	HRESULT wStrLiteralToWString(std::wstring& out, const char* const in, size_t& index);

	/* Essentially the inverse of wStrLiteralToWString()

	   Character escape codes will be output for the following characters: ", \t and \n
	   Note that the output string is otherwise not 'sanitized' in any way.
	   In particular, nothing is done to ensure that it contains
	   only printable ASCII characters (as could be done by using remove_ASCII_controlAndWhitespace() ).
	 */
	HRESULT wstringToWStrLiteral(std::wstring& out, const std::wstring& in);

	/* Converts the null-terminated
	ASCII string containing the substrings 'true' or 'false' to bool values.
	(The first such substring encountered is assigned to the output parameter,
	at which point the function returns.)

	Otherwise behaves like wStrLiteralToWString()
	*/
	HRESULT strToBool(bool& out, const char* const in, size_t& index);

	/* Essentially the inverse of strToBool() */
	HRESULT boolToWString(std::wstring& out, const bool& in);

	/* Parses a double value from a null-terminated string,
	   using the istringstream class
	   (http://www.cplusplus.com/reference/sstream/istringstream/)

	   Otherwise behaves like wStrLiteralToWString()
	*/
	HRESULT strToDouble(double& out, const char* const in, size_t& index);

	/* The inverse of strToDouble(), using the wostringstream class */
	HRESULT doubleToWString(std::wstring& out, const double& in);

	/* Parses a float value from a null-terminated string,
	   essentially by calling strToDouble()

	   Otherwise behaves like wStrLiteralToWString()
	*/
	HRESULT strToFloat(float& out, const char* const in, size_t& index);

	/* The inverse of strToFloat(), using the wostringstream class */
	HRESULT floatToWString(std::wstring& out, const float& in);

	/* Parses an unsigned integer value from a null-terminated string,
	   using the istringstream class
	   (http://www.cplusplus.com/reference/sstream/istringstream/)

	   If the 'hex' parameter is true, the value will be parsed
	   as a hexadecimal number, rather than a decimal number.

	   Otherwise behaves like wStrLiteralToWString()
	*/
	HRESULT strToUInt(unsigned int& out, const char* const in, size_t& index, const bool hex = false);

	/* The inverse of strToUInt(), using the wostringstream class */
	HRESULT uIntToWString(std::wstring& out, const unsigned int& in, const bool hex = false);

	/* Parses a signed integer value from a null-terminated string,
	   essentially by calling strToUInt()

	   Otherwise behaves like wStrLiteralToWString()
	*/
	HRESULT strToInt(int& out, const char* const in, size_t& index);

	/* The inverse of strToInt(), using the wostringstream class */
	HRESULT intToWString(std::wstring& out, const int& in);
}