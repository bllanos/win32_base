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
  -Definition and initialization of global variables
*/

#include "defs.h"
#include "globals.h"
#include <windows.h>
#include <DirectXMath.h>
#include <string>
#include <exception>

// Initialize global variables
Logger* defaultLogger = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdshow)
{
	// Enable memory leak report output on program exit
	// See http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// This block is just to try and eliminate false positives in memory leak detection
	{
		try {

			// Set up the globally-visible Logger
			try {
				defaultLogger = new Logger(true, L".\\logger\\log.txt", true);
			}
			catch (...) {
				return 0;
			}

			defaultLogger->logMessage(L"wWinMain() - wWinMain() has started.");

			/* Check that the DirectX Math library is supported
			on this system.
			See http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.utilities.xmverifycpusupport%28v=vs.85%29.aspx
			*/
			if (DirectX::XMVerifyCPUSupport()) {
				defaultLogger->logMessage(L"wWinMain() - System supports DirectX Math.");
			}
			else {
				defaultLogger->logMessage(L"wWinMain() - System does not support DirectX Math. Exiting.");
				delete defaultLogger;
				return 0;
			}

			defaultLogger->logMessage(L"wWinMain() - Exiting.");

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
			defaultLogger->logMessage(errorMsg);
		}
		catch (...) {
			defaultLogger->logMessage(L"wWinMain() - Exiting due to an unspecified exception.");
		}

		delete defaultLogger;
		defaultLogger = 0;
	}

	// Show any memory leaks
	// _CrtDumpMemoryLeaks(); // This should be called automatically because of the call to _CrtSetDbgFlag() earlier

	return 0;
}