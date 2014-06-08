/*
testLogger_LogUser.h
---------------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
June 8, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Test functions for the Logger and LogUser classs
  -Boolean or HRESULT return values indicate success (true)
     or failure (false), where applicable.
*/

#include <Windows.h>

namespace testLogger_LogUser {

	/* Testing the bulk logging capabilities
	of the Logger class.
	*/
	HRESULT testBulkLogging(void);
}