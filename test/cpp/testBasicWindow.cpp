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
#include "fileUtil.h"
#include "FlatAtomicConfigIO.h"
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
	g_defaultLogger->logMessage(L"testBasicWindow::openNWindows() Entering update loop.");
	bool quit = false;
	quit_wParam = static_cast<WPARAM>(0);
	while (!quit) {
		if( FAILED(BasicWindow::updateAll(quit, quit_wParam)) ) {
			g_defaultLogger->logMessage(
				L"testBasicWindow::openNWindows() BasicWindow updateAll() function returned an error.");
		}
		if( quit ) {
			break;
		}
	}
	g_defaultLogger->logMessage(L"testBasicWindow::openNWindows() Leaving update loop.");

	// Destruction
	for (unsigned int i = 0; i < n; ++i) {
		delete windowArray[i];
		windowArray[i] = 0;
	}
	delete[] windowArray;

}

HRESULT testBasicWindow::testGlobalBasicWindowConfig(WPARAM& quit_wParam) {

	// Create a file for logging the test results
	Logger* logger = 0;
	std::wstring logFilename;
	try {
		logFilename;
		fileUtil::combineAsPath(logFilename, DEFAULT_LOG_PATH_TEST, L"testGlobalBasicWindowConfig.txt");
		logger = new Logger(true, logFilename, false, false);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	std::wstring errorStr;
	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	BasicWindow basicWindow(ConfigUser::Usage::GLOBAL, true);

	// Open the window
	result = basicWindow.openWindow();
	if( FAILED(result) ) {
		logger->logMessage(L"Failed to open the window.");
		prettyPrintHRESULT(errorStr, result);
		logger->logMessage(errorStr);
		finalResult = result;
	}

	// Update loop
	logger->logMessage(L"testBasicWindow::testGlobalBasicWindowConfig() Entering update loop.");
	bool quit = false;
	quit_wParam = static_cast<WPARAM>(0);
	while( !quit ) {
		result = BasicWindow::updateAll(quit, quit_wParam);
		if( FAILED(result) ) {
			logger->logMessage(
				L"testBasicWindow::testGlobalBasicWindowConfig() BasicWindow updateAll() function returned an error.");
			prettyPrintHRESULT(errorStr, result);
			logger->logMessage(errorStr);
			finalResult = result;
		}
		if( quit ) {
			break;
		}
	}
	logger->logMessage(L"testBasicWindow::testGlobalBasicWindowConfig() Leaving update loop.");

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests passed.");
	} else {
		logger->logMessage(L"Some or all tests failed.");
	}

	delete logger;

	return finalResult;
}