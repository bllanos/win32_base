/*
globals.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A file containing globally-visible variable and function declarations
     -Groups of related functions that are not members of classes should
	  be placed in their own namespaces. Refer to the examples in the 'header\util' folder.
  -Global variable names will be prefixed with 'g_' for ease of identification
  -Definitions and initializations of global variables are contained in main.cpp
*/

#pragma once;

#include <Windows.h>
#include <string>
#include "Logger.h"

// Global variables
// ----------------

extern Logger* g_defaultLogger;

// Global functions
// ----------------

/* Converts a string object to an equivalent wstring object.
   This function expects ASCII characters (specifically,
   that all characters in the string object are represented
   by one-byte-length code points). The function will return
   a failure code if this is not the case.
 */
HRESULT toWString(std::wstring& wStr, const std::string& str);

/* The inverse of toWString(), which also expects
   ASCII characters only.
 */
HRESULT toString(std::string& str, const std::wstring& wStr);

/* Convert an HRESULT value to a more readable form,
   with each of its components identified

   The return value indicates whether the function
   is successful.
 */
HRESULT prettyPrintHRESULT(std::wstring& out, HRESULT in);