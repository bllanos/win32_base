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
  -A namespace containing functions for processing filepaths and file/directory names
*/

#pragma once;

#include <windows.h> // For the HRESULT type
#include <string>

namespace fileUtil {

	/* Extract the (relative or absolute) path
	from a combined filepath and file/directory name string.
	To be exact, if successful, this function strips the last name
	on the path (either a file or a directory name).

	Note:
	  -As currently implemented, if the string ends in '\',
	   the entire string will be interpreted as a path
	   (by the Windows function PathRemoveFileSpec() ).
	  -See http://msdn.microsoft.com/en-us/library/windows/desktop/bb773748%28v=vs.85%29.aspx
	     -Paths should not contain forward slashes
	*/
	HRESULT extractPath(std::wstring& path, const std::wstring& nameAndPath);

	/* The input 'name' parameter is the name of a file or directory.
	   (The path of the file or directory can be included, but will be ignored.)

	   If the 'isFile' input parameter is true, the function will inspect
	   the name using its conventions for filenames. Otherwise,
	   the function will inspect the name using its conventions for directories.

	   Outputs a string to the 'msg' output parameter if the name
	   does not conform to the following format, and 'isFile' is true:
	     -All characters are either alphanumeric, '_', or '.'
	     -There is one occurrence of '.', and it is not at the beginning
		    or end.
	     -Occurrences of '_' are not at the beginning, the end,
		    immediately before '.', or anywhere after '.'

	   Outputs a string to the 'msg' output parameter if the name
	   does not conform to the following format, and 'isFile' is false:
	     -All characters are either alphanumeric or '_'
		 -'_' does not occur at the start or end

	   The intention is to try and validate a file or directory name.
	   I otherwise could not find an easy way to check for a valid name.
	   (See http://msdn.microsoft.com/en-us/library/aa365247.aspx for more
	    information on filenames in Windows.)

	   If there are no issues to report, 'msg' is assigned an empty string.

	   Notes:
	     -Nothing is reported for an empty string, if 'isFile' is false.
		 -If 'name' ends in '\', the function will take the file or directory
		  name to encompass the last item on the path before, '\', as well as '\'.
		  (This is done internally by calling the Windows function PathFindFileName().)
		  Therefore, the name will be found to be invalid.

	   The function will return a failure code only in case of an internal
	   error.
	 */
	HRESULT inspectFileOrDirName(const std::wstring& name, const bool& isFile, std::string& msg);

	/* Test if a file or directory exists or, if it does not exist,
	   if it can probably be created.
	
	   Parameters
	     -(in) 'filepath': The name of a file or directory,
		    optionally prefixed with its relative or absolute filepath.
			The file or directory does not need to exist.
		 -(in/out) 'isFile':
		    (in) Whether to treat the 'filepath' parameter
			   as a file (true) or a directory (false),
			   if the entity cannot be found in the file system
			   (i.e. when 'exists' is false).
			(out) If the 'filepath' string corresponds to an existing
			   file system entity,
			   whether it is a file (true) or a directory (false).
			   Meaningless when 'exists' is false.
		 -(out) 'hasPath': Whether the 'filepath' parameter contains a
		    filepath (true) or is simply a name without a path (false).
			The path does not need to exist.
			If 'exists' is true, then 'hasPath' will be true.
		 -(out) 'exists': Whether the 'filepath' parameter corresponds
		    to an existing file system entity.
		 -(out) 'msg': See below
	
	   This function writes messages to the 'msg' output parameter
	   if 'exists' is false and any of the following are true:
	     -The directory containing the file or directory does not exist.
		 -fileUtil::inspectFileOrDirName() writes a message to its output
		    parameter when called on the file/directory name.
			In this case, the 'msg' parameter is assigned
			the output of inspectFileOrDirName().
		 -There are internal errors.
		    In this case, the function will return a failure code.

	   If there are no issues to report, 'msg' is assigned an empty string.

	   The function will return a failure code only in case of an internal
	   error.

	   Note: Throws an exception of type std::exception
	     if 'filepath' points to an existing system folder,
		 an existing file in a system folder,
		 or a file or folder which does not exist, but where its
		 path is a system folder.

	     In this case, it also writes the message placed in the exception to the
	     'msg' output parameter.

		 The check for a 'system' file or folder is currently
		 done using the Windows function PathIsSystemFolder() (in Shlwapi.lib).
	 */
	HRESULT inspectFileOrDirNameAndPath(const std::wstring& filepath,
		bool& isFile, bool& hasPath, bool& exists, std::string& msg);
}