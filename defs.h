/*
defs.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: No reference
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  A file containing global definitions and preprocessor constants
*/

#pragma once

// For finding memory leaks in the debug build configuration
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
// See http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/* This allows the memory leak check to report more information
(e.g. file and line number) when it detects memory leaks resulting
from the use of the 'new' operator.
*/
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#endif

// Used for making custom HRESULT values
#define FACILITY_BL_ENGINE 0

// Custom error codes
#define ERROR_FUNCTION_CALL 1800 // Error code returned by a function call