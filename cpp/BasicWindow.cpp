/*
BasicWindow.cpp
---------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: COMP2501A game design project code
-Adapted from Tutorial 4
-Probably originally based on the Rastertek tutorials (http://www.rastertek.com/index.html)

Other references:
-Luna, Frank D. 3D Game Programming with DirectX 11. Dulles: Mercury Learning and Information, 2012.

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the BasicWindow class
*/

#include "BasicWindow.h"
#include "defs.h"

// Using declarations
using std::wstring;
using std::vector;

// Static member initialization
vector<BasicWindow*>* BasicWindow::winProcList = 0;
std::vector<BasicWindow>::size_type BasicWindow::currentId = 0;

BasicWindow::BasicWindow(std::wstring name, bool exitAble, int width, int height) :
LogUser(),
m_applicationName(name), m_hinstance(0), m_hwnd(0), m_exitAble(exitAble),
m_width(width), m_height(height), m_id(0), m_opened(false)
{
	if (name.length() == 0) {
		m_applicationName = BASICWINDOW_DEFAULT_NAME;
	}

	// Set the logging message prefix
	setMsgPrefix(L"BasicWindow '" + m_applicationName + L"'");

	// Determine the resolution of the client's screen and adapt if necessary
	unsigned int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	unsigned int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (m_width > screenWidth) {
		m_width = screenWidth;
	}
	if (m_height > screenHeight) {
		m_height = screenHeight;
	}
}

BasicWindow::~BasicWindow(void) {}

HRESULT BasicWindow::openWindow(void) {

	// Update static data members
	if (winProcList == 0) {
		winProcList = new vector < BasicWindow* > ;
	}
	if (!m_opened) {
		// Need to create a new entry for this window
		m_opened = true;
		m_id = currentId;
		++currentId;
		winProcList->push_back(this);
	} else if( (*winProcList)[m_id] == 0 ) {
		// This window is reopening
		(*winProcList)[m_id] = this;
	} else {
		logMessage(L"Attempt to open a window that is already open.");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	}

	WNDCLASSEX wc; // struct which describes the window class (properties of the window)
	// DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // See http://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = appProc; // our procedure to call back on window events
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance; // set window's application to this application
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // use standard arrow cursor for window (when shown)
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)); // brush for painting background colour
	wc.lpszMenuName = NULL; // we have no menus with our window
	wc.lpszClassName = m_applicationName.c_str(); // name of our application (Note: Project properties need to be set to use the Unicode character set for this line to compile)
	wc.cbSize = sizeof(WNDCLASSEX);  //size of this structure in bytes

	// Register the window class.
	// The window class must be registered with Window's OS before the window
	// can actually be created.
	RegisterClassEx(&wc);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	// Create the window with the screen settings and get the handle to it.
	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679(v=vs.85).aspx
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		m_applicationName.c_str(),
		m_applicationName.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		posX,  // screen X of window's top left corner
		posY, // screen Y of window's top left corner
		m_width,  // width of screen
		m_height, // height of screen
		NULL,
		NULL,
		m_hinstance, // the application instance
		NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW); // show  the window
	SetForegroundWindow(m_hwnd); // make window the foreground window
	SetFocus(m_hwnd);            // give window input focus

	// Set the logging message prefix to include more information
	setMsgPrefix(L"BasicWindow '" + m_applicationName +
		L"', id = " + std::to_wstring(m_id) + L":");

	logMessage(L"Window opened.");

	return ERROR_SUCCESS;
}

HRESULT BasicWindow::shutdownWindow(const bool exitIfLast) {

	// Check if this window has ever been opened
	if (!m_opened) {
		logMessage(L"Attempt to close a window that has never been opened.");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	}

	// Check if the window is not already closed
	if ((*winProcList)[m_id] != 0) {

		// Remove the window.
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		// Remove the application instance.
		UnregisterClass(m_applicationName.c_str(), m_hinstance);
		m_hinstance = NULL;

		// Remove the reference to this object
		(*winProcList)[m_id] = 0;
		logMessage(L"window closed.");

		// Check if all windows are now closed
		if (exitIfLast) {
			bool allClosed = true;
			for (std::vector<BasicWindow>::size_type i = 0; i < currentId; ++i) {
				if ((*winProcList)[i] != 0) {
					allClosed = false;
					break;
				}
			}
			if (allClosed) {
				PostQuitMessage(0);
				logMessage(L"Posted quit message because all windows are closed.");
			}
		}
	} else {
		logMessage(L"Attempt to close a window that is already closed.");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	}

	return ERROR_SUCCESS;
}

HRESULT BasicWindow::shutdownAll(void) {

	// Close all open windows
	for (std::vector<BasicWindow>::size_type i = 0; i < currentId; ++i) {
		if ((*winProcList)[i] != 0) {
			(*winProcList)[i]->shutdownWindow(false);
		}
	}

	// Cleanup static members
	delete winProcList;
	winProcList = 0;
	return ERROR_SUCCESS;
}

HRESULT BasicWindow::updateAll(bool& quit) {

	quit = false;

	// Initialize the message structure.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	/* Dispatch Windows messages for this thread
	   Note that passing in a HWND value for the second
	   parameter might seem like a good idea
	   (e.g. for an instance
	   member function where each window can check its own messages),
	   but will miss WM_QUIT messages.
	 */
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		// The application must terminate
		quit = true;
		shutdownAll();
	}

	return ERROR_SUCCESS;
}

LRESULT CALLBACK BasicWindow::winProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

	// Check if the window is being destroyed or closed
	if (umsg == WM_DESTROY || umsg == WM_CLOSE) {
		
		if (m_exitAble) {
			/* Closing this window means that the entire application must shut down
			shortly. The quit message will be encountered by any update() function call.
			*/
			PostQuitMessage(0);
			logMessage(L"Posted quit message because this 'exitAble' window received a WM_CLOSE (2) or WM_DESTROY (16) message."
				L"\n\tThe message value is "+std::to_wstring(umsg));
		} else {
			logMessage(L"Non-'exitAble' window received a WM_CLOSE (2) or WM_DESTROY (16) message."
				L"\n\tThe message value is " + std::to_wstring(umsg));
			shutdownWindow(true); // Close only this window, unless it is the only open window
		}
		return 0;
	}

	// Any unprocessed messages are sent to the default message handler
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

HWND BasicWindow::getHWND(void) const {
	return m_hwnd;
}

unsigned int BasicWindow::getWidth(void) const {
	return m_width;
}

unsigned int BasicWindow::getHeight(void) const {
	return m_height;
}

std::vector<BasicWindow>::size_type	BasicWindow::getID(void) const {
	return m_id;
}

LRESULT CALLBACK BasicWindow::appProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

	for (std::vector<BasicWindow>::size_type i = 0; i < currentId; ++i) {
		if ((*winProcList)[i] != 0 && (*winProcList)[i]->getHWND() == hwnd) {
			return (*winProcList)[i]->winProc(hwnd, umsg, wparam, lparam);
		}
	}

	// Any unprocessed messages are sent to the default message handler
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}