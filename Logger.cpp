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

Description
  -Implementation of the Logger class.
*/

#include "Logger.h"
#include "defs.h"
#include <ctime>
#include <exception>

// Using declarations
using std::wstring;
using std::basic_ofstream;

// Initialization of static members
unsigned int Logger::nConsoleWriters = 0;

Logger::Logger(bool allocLogConsole, bool allocLogFile, wstring filename) :
m_consoleOpen(allocLogConsole), m_defaultLogFileOpen(allocLogFile),
m_console(INVALID_HANDLE_VALUE), m_filename(filename), m_logfile()
{
	if (m_consoleOpen) {

		// Try to access the console output stream
		m_console = GetStdHandle(STD_OUTPUT_HANDLE);

		if (m_console == INVALID_HANDLE_VALUE) {
			// Perhaps no console is currently open
			bool success = true;
			if (nConsoleWriters == 0) {
				// Try to open a console
				success = AllocConsole();
				if (success) {
					m_console = GetStdHandle(STD_OUTPUT_HANDLE);
					success = (m_console != INVALID_HANDLE_VALUE);
				}
			}
			if (!success) {
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

HRESULT Logger::logMessage(const wstring msg, bool toConsole, bool toFile, wstring filename) {
	wstring fullMessage;
	if (FAILED(getDateAndTime(fullMessage))) {
		fullMessage = L"[Cannot get time]";
	}

	// Format the message
	fullMessage += L" | ";
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
	struct std::tm* pTimeStruct;
	// Note that the returned pointer is to an internal object
	pTimeStruct = std::localtime(&timeVal); // Convert to a local time

	char* timeCStr = std::asctime(pTimeStruct); // Get a human-readable time string

	/* Need to convert to a wide character string
	See http://msdn.microsoft.com/en-us/library/ms235631.aspx
	for more information on converting between string types.
	*/
	size_t wSize = strlen(timeCStr) + 1;
	wchar_t* timeWCStr = new wchar_t[wSize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, timeWCStr, wSize, timeCStr, _TRUNCATE);

	// Output a copy of the result
	timeStr = timeWCStr;
	delete[] timeWCStr;

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
}

HRESULT Logger::logMsgToConsole(const std::wstring& msg) {
	const wchar_t* cStr = msg.c_str();
	if (m_consoleOpen) {
		WriteConsole(m_console, cStr, wcslen(cStr), NULL, NULL);
	}
}