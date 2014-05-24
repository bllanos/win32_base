/*
TestBasicWindow.cpp
-------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Test functions for the BasicWindow class.
  -Boolean return values indicate success (true) or failure (false), where applicable.
*/

#include <string>
#include "BasicWindow.h"

namespace TestBasicWindow {

	/* Opens the number of windows specified by the input parameter
	The second parameter specifies whether the windows are 'exitAble'
	*/
	void openNWindows(const unsigned int n, bool exitAble) {
		if (n == 0) return;
		BasicWindow** windowArray = new BasicWindow*[n];

		// Construction
		for (unsigned int i = 0; i < n; ++i) {
			windowArray[i] = new BasicWindow(std::to_wstring(i), exitAble);
		}

		// Open each window

		// Destruction
		for (unsigned int i = 0; i < n; ++i) {
			delete windowArray[i];
			windowArray[i] = 0;
		}
		delete[] windowArray;
	}

}