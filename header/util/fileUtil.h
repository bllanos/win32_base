/*
fileUtil.h
----------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 3, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A namespace containing functions for processing filepaths and filenames
*/

#pragma once;

#include <windows.h> // For the HRESULT type
#include <string>

namespace fileUtil {

	/* Extract the (relative or absolute) path
	from a combined filepath and file/directory name string.
	To be exact, if successful, this function strips the last name
	on the path (either a file or a directory name).
	*/
	HRESULT extractPath(std::wstring& path, const std::wstring& filenameAndPath);

	/* The input 'filename' parameter is the name of a file or directory.
	   (The path of the file or directory can be included, but will be ignored.)

	   Outputs a string to the 'msg' output parameter if the name
	   does not conform to the following format:
	     -All characters are either alphanumeric, '_', or '.'
	     -There is one occurrence of '.', and it is not at the beginning
		    or end.
	     -Occurrences of '_' are not at the beginning,
		    immediately before '.', or anywhere after '.'

	   The intention is to try and validate a filename.
	   I otherwise could not find an easy way to check for a valid filename.
	   (See http://msdn.microsoft.com/en-us/library/aa365247.aspx for more
	    information on filenames in Windows.)

	   If there are no issues to report, 'msg' is assigned an empty string.

	   The function will return a failure code only in case of an internal
	   error.
	 */
	HRESULT inspectFilename(const std::wstring& filename, std::string& msg);

	/* The input parameter, 'filepath' is the name of a file or directory,
	   prefixed with its relative or absolute filepath.
	
	   This function writes messages to the 'msg' output parameter
	   if any of the following are true:
	     -The directory containing the file or directory does not exist.
		 -The call to fileUtil::inspectFilename() fails.
		  In this case, the function will return a failure code.
		 -fileUtil::inspectFilename() writes a message to its output
		    parameter when called on the file/directory name (stripped
			of the path). In this case, the 'msg' parameter is assigned
			 the output of inspectFilename().

	   If there are no issues to report, 'msg' is assigned an empty string.

	   The function will return a failure code only in case of an internal
	   error.
	 */
	HRESULT inspectFilenameAndPath(const std::wstring& filepath, std::string& msg);
}