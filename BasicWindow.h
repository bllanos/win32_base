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
	const unsigned int	m_id; // Unique id of this window, managed by this class

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
	HRESULT shutdownWindow(void);

public:
	/* Call this function once per application loop to update the internal state
	of the object
	*/
	HRESULT run(void);

	/* It seems that only static functions can be registered as window procedures.
	This function will be called by the static window procedure.
	*/
	LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);

	// Getters and setters
public:
	HWND getHWND(void) const;
	int getWidth(void) const;
	int getHeight(void) const;

	// Static members
private:
	/*
	This window procedure will direct messages to all open windows,
	using the list of window procedures declared below.
	*/
	static LRESULT CALLBACK appProc(HWND, UINT, WPARAM, LPARAM);
	static std::vector<LRESULT (CALLBACK BasicWindow::*)(HWND, UINT, WPARAM, LPARAM)>* winProcList;
	static unsigned int currentId; // The ID to be assigned to the next window created

	// Static 

	// Currently not implemented - will cause linker errors if called
private:
	BasicWindow(const BasicWindow& other);
	BasicWindow& operator=(const BasicWindow& other);
};

