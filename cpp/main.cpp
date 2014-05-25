/*
main.cpp
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: COMP2501A game design project code
  -Adapted from Tutorial 4
Other references:
  -Luna, Frank D. 3D Game Programming with DirectX 11. Dulles: Mercury Learning and Information, 2012.

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -The entry point of the application
  -A helper function to start the application loop
  -Definition and initialization of global variables
*/

#include "defs.h"
#include "globals.h"
#include <windows.h>
#include <DirectXMath.h>
#include <string>
#include <exception>

// Initialize global variables
Logger* g_defaultLogger = 0;

// Application loop wrapper function
HRESULT applicationLoop(void);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdshow)
{
	// Enable memory leak report output on program exit
	// See http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// This block is just to try and eliminate false positives in memory leak detection
	{
		HRESULT error = ERROR_SUCCESS;

		try {

			// Set up the globally-visible Logger
			try {
				std::wstring logFilename = DEFAULT_LOG_PATH;
				logFilename += DEFAULT_LOG_FILENAME;
				g_defaultLogger = new Logger(true, logFilename, false);
			}
			catch (...) {
				// Message box documentation: http://msdn.microsoft.com/en-us/library/windows/desktop/ms645505%28v=vs.85%29.aspx
				MessageBox(NULL, L"Failed to initialize the default logger object.\nProgram will exit.",
					L"BL Project Error", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
				return 0;
			}

			g_defaultLogger->logMessage(L"wWinMain() - wWinMain() has started.");

			/* Check that the DirectX Math library is supported
			on this system.
			See http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.utilities.xmverifycpusupport%28v=vs.85%29.aspx
			*/
			if (DirectX::XMVerifyCPUSupport()) {
				g_defaultLogger->logMessage(L"wWinMain() - System supports DirectX Math.");
			} else {
				g_defaultLogger->logMessage(L"wWinMain() - System does not support DirectX Math. Exiting.");
				MessageBox(NULL, L"System does not support DirectX Math.\nProgram will exit.",
					L"BL Project Error", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
				delete g_defaultLogger;
				return 0;
			}

			/* The application loop goes here, as well as more task-specific
			 * initialization and shutdown
			 */
			// ------------------------------
			error = applicationLoop();
			// ------------------------------

			if( FAILED(error) ) {
				g_defaultLogger->logMessage(L"wWinMain() - Exiting with an error code reported from the application loop.\n"
					L"\tError (HRESULT type) value has severity " +
					std::to_wstring(static_cast<unsigned long>(HRESULT_SEVERITY(error))) + L", facility " +
					std::to_wstring(static_cast<unsigned long>(HRESULT_FACILITY(error))) + L", code " +
					std::to_wstring(static_cast<unsigned long>(HRESULT_CODE(error))) + L".");
			} else {
				g_defaultLogger->logMessage(L"wWinMain() - Exiting normally.");
			}

			// Create a memory leak, just to test that the memory leak check works
			// int* leak = new int[4];
		}
		catch (std::exception e) {
			std::wstring errorMsg = L"wWinMain() - An exception object was thrown: ";
			std::wstring exceptionMsg;
			if (FAILED(toWString(exceptionMsg, e.what()))) {
				errorMsg += L"Error retrieving exception message.";
			}
			errorMsg += exceptionMsg;
			g_defaultLogger->logMessage(errorMsg);
			error = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}
		catch (...) {
			g_defaultLogger->logMessage(L"wWinMain() - Exiting due to an unspecified exception.");
			error = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}

		delete g_defaultLogger;
		g_defaultLogger = 0;

		if (FAILED(error)) {
			MessageBox(NULL, L"Errors or exceptions occured. Program will now exit.\nPlease check the logs for details.",
				L"BL Project Error", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		}
	}

	// Show any memory leaks
	// _CrtDumpMemoryLeaks(); // This should be called automatically because of the call to _CrtSetDbgFlag() earlier

	return 0;
}

// Additional includes needed for test code
#include "testBasicWindow.h"

/*
Run the application, possibly through a top-level control object
*/
HRESULT applicationLoop(void) {

	TestBasicWindow::openNWindows(1, false);
	return ERROR_SUCCESS;
}