/*
testBasicWindow.cpp
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
  -Implementations of test functions for the BasicWindow class.
*/

#include <string>
#include "globals.h"
#include "BasicWindow.h"
#include "testBasicWindow.h"


void testBasicWindow::openNWindows(const unsigned int n, const unsigned int nExitAble, WPARAM& quit_wParam) {
	if (n == 0) return;
	BasicWindow** windowArray = new BasicWindow*[n];

	// Construction
	for (unsigned int i = 0; i < n; ++i) {
		if( i < nExitAble ) {
			windowArray[i] = new BasicWindow(
				ConfigUser::Usage::PRIVATE, false, L"Exitable - "+ std::to_wstring(i), true);
		} else {
			windowArray[i] = new BasicWindow(
				ConfigUser::Usage::PRIVATE, false, L"Non-Exitable - " + std::to_wstring(i), false);
		}
	}

	// Open each window
	for (unsigned int i = 0; i < n; ++i) {
		windowArray[i]->openWindow();
	}

	// Update loop
	g_defaultLogger->logMessage(L"TestBasicWindow::openNWindows() Entering update loop.");
	bool quit = false;
	quit_wParam = static_cast<WPARAM>(0);
	while (!quit) {
		if( FAILED(BasicWindow::updateAll(quit, quit_wParam)) ) {
			g_defaultLogger->logMessage(
				L"TestBasicWindow::openNWindows() BasicWindow updateAll() function returned an error.");
		}
		if( quit ) {
			break;
		}
	}
	g_defaultLogger->logMessage(L"TestBasicWindow::openNWindows() Leaving update loop.");

	// Destruction
	for (unsigned int i = 0; i < n; ++i) {
		delete windowArray[i];
		windowArray[i] = 0;
	}
	delete[] windowArray;

}