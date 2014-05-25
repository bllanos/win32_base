/*
LogUser.h
----------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A base class that manages the logging output of its descendents
  -Allows for logging to be turned on or off
  -Allows for the logging endpoint to be changed
  -Allows for logging output messages to have a common prefix added

Note that the globally-visible Logger object is initialized and destroyed in main.cpp,
  rather than being managed by this class.
*/

#pragma once

#include <Windows.h>
#include <string>
#include "globals.h"
#include "Logger.h"

class LogUser
{
	// Data members
private:
	bool m_loggingEnabled; // Switch for turning logging on or off
	Logger* m_logger; // Null if the global default Logger object is used instead of a custom Logger
	std::wstring m_msgPrefix; // All logging messages will be prefixed with this string

protected:
	/* The 'msgPrefix' parameter will become a prefix for all logged messages.
	It will automatically be separated from any following message text
	by a space.
	*/
	LogUser(bool enableLogging = true, const std::wstring msgPrefix = L"");

public:
	virtual ~LogUser(void);

	// Getters and setters
public:
	/* Arguments are forwarded to the Logger constructor
	If the call to the Logger constructor fails, this
	object's logger is not changed.
	 */
	HRESULT setLogger(bool allocLogFile = true, const std::wstring filename = L"customLog.txt", bool allocLogConsole = false);
	void enableLogging();
	void disableLogging();

protected:
	void setMsgPrefix(const std::wstring& prefix);

	// Logging functions
protected:
	// Arguments are forwarded to the Logger member function of the same name
	HRESULT logMessage(const std::wstring& msg,
		bool toConsole = true, bool toFile = true, const std::wstring filename = L"");

	// Currently not implemented - will cause linker errors if called
private:
	LogUser(const LogUser& other);
	LogUser& operator=(const LogUser& other);
};
