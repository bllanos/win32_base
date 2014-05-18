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

Description
  -A class for setting up and managing a window, as well as handling messages for the window.
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
	*/
	BasicWindow(std::wstring name=L"No Name", bool exitAble = true,
		int width = BASICWINDOW_DEFAULT_WIDTH,
		int height = BASICWINDOW_DEFAULT_HEIGHT);

	virtual ~BasicWindow(void);

	// Setup and shutdown
public:
	HRESULT initializeWindow(void);

private:
	/* Closes this window
	*/
	HRESULT shutdownWindow(void);

	/* Closes all windows created by this class.
	To be called when an "exitAble" window is closed.
	*/
	static HRESULT shutdownAll(void);

public:
	/* Call this function once per application loop to update the internal state
	of the object and dispatch Windows messages

	Outputs true if the application must terminate
	*/
	HRESULT update(bool& quit);

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

