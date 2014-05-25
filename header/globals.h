/*
globals.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: COMP2501A game design project code
  -Adapted from Tutorial 4

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A file containing globally-visible variable and function declarations
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

// Convert a string object to an equivalent wstring object
HRESULT toWString(std::wstring& wStr, const std::string& str);