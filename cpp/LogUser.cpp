/*
LogUser.cpp
-----------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the LogUser class.
*/

#include "defs.h"
#include "LogUser.h"

LogUser::LogUser(bool enableLogging, std::wstring msgPrefix) :
m_loggingEnabled(enableLogging), m_logger(0), m_msgPrefix(msgPrefix)
{}

LogUser::~LogUser(void) {
	if (m_logger != 0) {
		delete m_logger;
		m_logger = 0;
	}
}

HRESULT LogUser::setLogger(bool allocLogFile, const std::wstring filename, bool allocLogConsole) {
	try {
		Logger* newLogger;
		newLogger = new Logger(allocLogFile, filename, allocLogConsole);
		if (m_logger != 0) {
			delete m_logger;
			m_logger = newLogger;
		}
		return ERROR_SUCCESS;
	}
	catch (...) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
	}
}

void LogUser::enableLogging() {
	m_loggingEnabled = true;
}

void LogUser::disableLogging() {
	m_loggingEnabled = false;
}

void LogUser::setMsgPrefix(const std::wstring& prefix) {
	m_msgPrefix = prefix;
}

HRESULT LogUser::logMessage(const std::wstring& msg,
	bool toConsole, bool toFile, const std::wstring filename) {
	if (m_loggingEnabled) {
		std::wstring fullMsg = m_msgPrefix + L" " + msg;
		if (m_logger != 0) {
			return m_logger->logMessage(fullMsg, toConsole, toFile, filename);
		}
		else if (g_defaultLogger != 0){
			return g_defaultLogger->logMessage(fullMsg, toConsole, toFile, filename);
		}
		else {
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
		}
	} else {
		return ERROR_SUCCESS;
	}
}