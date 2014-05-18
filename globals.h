/*
globals.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: No reference
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit

Description
  A file containing globally-visible variable and function declarations
*/

#pragma once;
#include <string>

/* Set to true to get a console window for writing debug info to
*/
extern bool allocDebugConsole;

/* Function for safe writing to the debugging console
(Safe means it checks whether the debug console exists.)
*/
void writeToDebugConsole(std::wstring str);