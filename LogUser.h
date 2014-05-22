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
	bool m_loggingEnabled;
	Logger* m_logger;
	std::wstring m_msgPrefix; // All logging messages will be prefixed with this string

public:
	LogUser(bool enableLogging = true, const std::wstring msgPrefix = L"", Logger* logger = defaultLogger);
	virtual ~LogUser(void);

	// Getters and setters
public:
	Logger* getLogger(void) const;
	void setLogger(Logger* logger);
	void enableLogging();
	void disableLogging();

protected:
	void setMsgPrefix(const std::wstring& prefix);

	// Logging functions
protected:
	HRESULT logMessage(const std::wstring& msg,
		bool toConsole = true, bool toFile = true, const std::wstring filename = L"");
};

