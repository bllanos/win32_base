/*
BasicWindow.h
-------------

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
  -A class for setting up and managing a window, as well as handling messages for the window.

Issues
  -This class is not safe for use by multiple threads because it makes use of shared static members
  -The static window procedure relies on the uniqueness of HWND values to call
     the window procedures for specific windows. The uniqueness of HWND values
	 is probably not guaranteed, however.
*/

#pragma once

// Dependencies
#include <windows.h>
#include <string>
#include <vector>

// Preprocessor Definitions
#define WIN32_LEAN_AND_MEAN
#define BASICWINDOW_DEFAULT_WIDTH 600
#define BASICWINDOW_DEFAULT_HEIGHT 400

class BasicWindow
{
	// Data members
private:
	const std::wstring	m_applicationName;
	HINSTANCE		m_hinstance;
	HWND			m_hwnd;  //handle to the client window for the application
	bool			m_exitAble; // True if closing this window will cause the application to quit
	unsigned int	m_width; // Pixel width of the window
	unsigned int	m_height; // Pixel height of the window
	std::vector<BasicWindow>::size_type	m_id; // Unique id of this window, managed by this class
	bool			m_opened; // True if this window has been opened at least once

	// Constructors, destructors
public:
	/*
	The window width and height will be reduced if the screen size
	turns out to be smaller.

	The "exitAble" parameter, when true, causes window procedure to post a quit message
	when the window is closed. A quit message will shortly cause shutdownAll() to be called,
	and Windows will probably expect the thread to terminate.
	*/
	BasicWindow(const std::wstring name=L"No Name", bool exitAble = true,
		int width = BASICWINDOW_DEFAULT_WIDTH,
		int height = BASICWINDOW_DEFAULT_HEIGHT);

	virtual ~BasicWindow(void);

	// Setup and shutdown
public:
	HRESULT initializeWindow(void);

	/* Closes this window

	If this is the last window to be closed that was
	created by this class, a quit message will be posted to Windows.
	*/
	HRESULT shutdownWindow(void);

	/* Closes all windows created by this class.
	To be called when an "exitAble" window is closed.
	*/
	static HRESULT shutdownAll(void);

public:
	/* Call this function once per application loop to update the internal state
	of the object and dispatch Windows messages for the window.

	The output parameter is true if the application must terminate
	(e.g. due to the user closing an "exitAble" window, or closing all windows).

	This function will close all windows if the application must terminate.

	(Note that "all windows" refers to all windows created by this class.)
	*/
	HRESULT update(bool& quit);

private:
	/* It seems that only static functions can be registered as window procedures.
	This function will be called by the static window procedure.
	*/
	LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);

	// Getters and setters
public:
	HWND getHWND(void) const;
	unsigned int getWidth(void) const;
	unsigned int getHeight(void) const;

private:
	/*
	This window procedure will direct messages to all open windows,
	using the list of window procedures declared below.
	*/
	static LRESULT CALLBACK appProc(HWND, UINT, WPARAM, LPARAM);

	// Static data members
	static std::vector<BasicWindow*>* winProcList;
	static std::vector<BasicWindow>::size_type currentId; // The ID to be assigned to the next window opened

	// Currently not implemented - will cause linker errors if called
private:
	BasicWindow(const BasicWindow& other);
	BasicWindow& operator=(const BasicWindow& other);
};

