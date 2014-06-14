/*
Logger.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 19, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A class for outputting log messages to a console window or to files
  -Multiple instances of this class can use the console, but cannot
   write to the same log file.

Issues
  -Currently, the log file may not receive all logging messages if the application
   terminates in an abnormal way. Perhaps the output file stream could be flushed after
   each message to eliminate this issue, but there might be a performance cost?
*/

#pragma once

#include <windows.h>
#include <string>
#include <list>
#include <iterator>
#include <fstream>

class Logger
{
private:
	// Counts the number of instances of this class using the console
	static unsigned int s_nConsoleWriters;

	// Data members
private:
	bool m_consoleOpen; // A console has been created

	bool m_defaultLogFileOpen; // A text file has been opened, or can be opened

	/* True if the primary log file is opened in the absence of message logging,
	 as discussed further in the constructor documentation.
	 */
	bool m_lockAndReplaceFile;

	HANDLE m_console; // The handle to the output console

	std::wstring m_filename; // The name and path of the primary log file

	std::basic_ofstream<wchar_t> m_logfile; // The primary log file output stream

public:
	/*
	If 'allocLogConsole' is true, a console window will be created.
	If the 'allocLogFile' parameter is true, an output text file will be created.
	The 'filename' parameter sets the name of the primary log file.

	If the 'lockAndReplaceFile' parameter is true:
	  The primary log file is opened by the constructor and closed
	  by the destructor. The previous contents of the file are overwritten.
	  If the logging file or console cannot be opened, an exception will be thrown.
	otherwise:
	  The primary log file is opened and appended to only when messages
	  are being written to it. The original contents of the file are added to,
	  not overwritten.
	  If the directory in which the logging file is to be found/created does not
	  exist, an exception will be thrown.

	This ensures that pointers to Logger objects will either be null
	or will refer to functional objects.
	*/
	Logger(bool allocLogFile = true, const std::wstring filename = L"log.txt", bool lockAndReplaceFile = false, bool allocLogConsole = false);

	virtual ~Logger(void);

public:
	/*
	Outputs the specified message to the debugging console window
	(if the console window exists and the second parameter is true)
	and to the primary log file (if the third parameter is true and
	the fourth parameter is an empty string).
	
	If the fourth parameter
	has a non-empty value other than the name of the primary log file,
	and the third parameter is true, the message will be appended to
	the specified file instead of the primary log file. (The specified
	file will not be overwritten.)

	All messages will be prefixed with the current date and time,
	and postfixed with a newline.
	*/
	HRESULT logMessage(const std::wstring& msg,
		bool toConsole = true, bool toFile = true, const std::wstring filename = L"");

	/* This function is intended for bulk logging operations,
	 and will be more efficient than calling the version for single messages
	 repeatedly.

	 All messages in the stream will be given the same time prefix, followed
	 by the optional prefix passed in as the third argument. No separation is
	 added between the prefix and the rest of each message.

	 Messages will be logged in the order that they are provided by the iterators.
	 */
	HRESULT logMessage(std::list<std::wstring>::const_iterator start,
		std::list<std::wstring>::const_iterator end, const std::wstring& prefix = L"",
		bool toConsole = true, bool toFile = true, const std::wstring filename = L"");

private:
	// Retrieves the current local time
	HRESULT getDateAndTime(std::wstring& timeStr) const;

	// Outputs a message to a file
	HRESULT logMsgToFile(const std::wstring& msg,
		const std::wstring filename = L"");

	// Outputs multiple messages to a file
	HRESULT logMsgToFile(std::list<std::wstring>::const_iterator start,
		std::list<std::wstring>::const_iterator end,
		const std::wstring& prefix,
		const std::wstring filename = L"");

	// Outputs a message to the debugging console
	HRESULT logMsgToConsole(const std::wstring& msg);

	// Outputs multiple messages to the debugging console
	HRESULT logMsgToConsole(std::list<std::wstring>::const_iterator start,
		std::list<std::wstring>::const_iterator end,
		const std::wstring& prefix);

	// Currently not implemented - will cause linker errors if called
private:
	Logger(const Logger& other);
	Logger& operator=(const Logger& other);
};

