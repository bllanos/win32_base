/*
testTextProcessing.h
---------------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
June 22, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Test functions for the textProcessing namespace
  -Boolean or HRESULT return values indicate success (true)
   or failure (false), where applicable.
*/

#include <Windows.h>

namespace testTextProcessing {

	/* Tests the remove_ASCII_controlAndWhitespace() function
	*/
	HRESULT testControlStrip(void);

}