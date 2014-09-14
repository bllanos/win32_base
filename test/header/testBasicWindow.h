/*
testBasicWindow.h
-------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 24, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Test functions for the BasicWindow class.
  -Boolean or HRESULT return values indicate success (true) or failure (false), where applicable.
*/

#pragma once

#include <Windows.h>

namespace testBasicWindow {

	/* Opens the number of windows specified by the input parameter, 'n'
	The second parameter specifies how many windows are 'exitAble'.
	The 'exitAble' windows will be opened first.
	*/
	void openNWindows(const unsigned int n, const unsigned int nExitAble, WPARAM& quit_wParam);

}