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

Description
  -The entry point of the application
  -Definition and initialization of global variables
*/

#include "defs.h"
#include "globals.h"
#include <windows.h>
#include <DirectXMath.h>

// Initialize global variables
Logger* defaultLogger = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Enable memory leak report output on program exit
	// See http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// This block is just to try and eliminate false positives in memory leak detection
	{
		HRESULT result;

		defaultLogger = new Logger(true, true);
		defaultLogger->logMessage(L"wWinMain() - wWinMain() has started.");

		/* Check that the DirectX Math library is supported
		on this system.
		See http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.utilities.xmverifycpusupport%28v=vs.85%29.aspx
		*/
		if (DirectX::XMVerifyCPUSupport()) {
			defaultLogger->logMessage(L"wWinMain() - System supports DirectX Math.");
		} else {
			defaultLogger->logMessage(L"wWinMain() - System does not support DirectX Math. Exiting.");
			delete defaultLogger;
			return 0;
		}

		defaultLogger->logMessage(L"wWinMain() - Exiting.");
		delete defaultLogger;

		// Create a memory leak, just to test that the memory leak check works
		int* leak = new int[4];
	}

	// Show any memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}