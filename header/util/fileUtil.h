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

}