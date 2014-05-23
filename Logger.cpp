/*
Logger.cpp
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: COMP2501A game design project code
-Adapted from Tutorial 4

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the Logger class.
*/

#include "Logger.h"
#include "defs.h"
#include "globals.h"
#include <ctime>
#include <exception>

// Using declarations
using std::wstring;
using std::basic_ofstream;

// Initialization of static members
unsigned int Logger::nConsoleWriters = 0;

Logger::Logger(bool allocLogFile, wstring filename, bool allocLogConsole) :
m_consoleOpen(allocLogConsole), m_defaultLogFileOpen(allocLogFile),
m_console(INVALID_HANDLE_VALUE), m_filename(filename), m_logfile()
{
	if (m_consoleOpen) {

		// Try to access the console output stream
		m_console = GetStdHandle(STD_OUTPUT_HANDLE);

		if (m_console == INVALID_HANDLE_VALUE) {
			// Perhaps no console is currently open
			BOOL success = true;
			if (nConsoleWriters == 0) {
				// Try to open a console
				success = AllocConsole();
				if (success == TRUE) {
					m_console = GetStdHandle(STD_OUTPUT_HANDLE);
					success = (m_console != INVALID_HANDLE_VALUE);
				}
			}
			if (success == FALSE) {
				// This is a Microsoft-specific constructor
				throw std::exception("Failed to open or access output console.");
			}
		}
		++nConsoleWriters;
	}
	
	if (m_defaultLogFileOpen) {
		m_logfile.open(m_filename, std::ios::out);
		if (!m_logfile.is_open()) {
			// This is a Microsoft-specific constructor
			throw std::exception("Failed to open output primary logging output file.");
		}
	}
}

Logger::~Logger(void) {
	// Dissociate this process from the console, if it is no longer being used
	--nConsoleWriters;
	if (nConsoleWriters == 0) {
		FreeConsole();
	}
	/* Note that the primary logging output file will be closed automatically
	by the basic_ofstream<wchar_t> class destructor.
	*/
}

HRESULT Logger::logMessage(const wstring& msg, bool toConsole, bool toFile, const wstring filename) {
	wstring fullMessage;
	if (FAILED(getDateAndTime(fullMessage))) {
		fullMessage = L"[Cannot get time]";
	}

	// Format the message
	fullMessage.back() = L' '; // Remove the newline character
	fullMessage += L"| ";
	fullMessage += msg;
	fullMessage += L"\n";

	HRESULT result = ERROR_SUCCESS;

	if (toConsole) {
		result = logMsgToConsole(fullMessage);
	}
	if (toFile) {
		HRESULT tempResult = logMsgToFile(fullMessage, filename);
		if ( FAILED(tempResult) && SUCCEEDED(result) ){
			result = tempResult;
		}
	}
	return result;
}

HRESULT Logger::getDateAndTime(wstring& timeStr) const {
	std::time_t timeVal;
	std::time(&timeVal); // Get the current timestamp
	struct std::tm timeStruct;

	// Convert to a local time
	if (localtime_s(&timeStruct, &timeVal)) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
	}

	// Following an example at http://msdn.microsoft.com/en-us/library/a442x3ye.aspx
	const size_t len = 26; // Length of the time string buffer
	char timeCStr[len];
	// Get a human-readable time string
	if (asctime_s(timeCStr, len, &timeStruct)) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
	}
	 
	wstring timeWStr;
	if (FAILED(toWString(timeWStr, timeCStr))) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
	}

	// Output the result
	timeStr = timeWStr;
	return ERROR_SUCCESS;
}

HRESULT Logger::logMsgToFile(const wstring& msg, wstring filename) {
	if (filename.length() > 0 && filename != m_filename) {
		// Open a custom logging file
		basic_ofstream<wchar_t> newFile(filename, std::ios::app);
		if (!newFile.is_open()) {
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FILE_NOT_FOUND);
		}
		newFile << msg;
	} else if (m_defaultLogFileOpen) {
		m_logfile << msg;
	}
	return ERROR_SUCCESS;
}

HRESULT Logger::logMsgToConsole(const std::wstring& msg) {
	const wchar_t* cStr = msg.c_str();
	if (m_consoleOpen) {
		WriteConsole(m_console, cStr, wcslen(cStr), NULL, NULL);
	}
	return ERROR_SUCCESS;
}