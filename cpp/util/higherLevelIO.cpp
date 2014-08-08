/*
higherLevelIO.cpp
------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 4, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of functions in the higherLevelIO namespace
*/

#include "higherLevelIO.h"
#include "textProcessing.h"
#include "defs.h"

using DirectX::XMFLOAT4;
using std::wstring;

#define FLOAT4_SIZE 4
#define START_CH '('
#define START_CH_W L'('
#define END_CH ')'
#define END_CH_W L')'

HRESULT strToXMFLOAT4(XMFLOAT4& out, const char* const in, size_t& index) {

	// Error checking
	if( in == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NULL_INPUT);
	}

	float* tempOut = 0;
	size_t tempIndex = index;
	if( FAILED(textProcessing::strToNumberArray(
		tempOut, in, tempIndex, FLOAT4_SIZE, START_CH, END_CH)) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);

	} else if( tempIndex != index ) {
		// Valid data literal found
		out.x = tempOut[0];
		out.y = tempOut[1];
		out.z = tempOut[2];
		out.w = tempOut[3];
		index = tempIndex;
	}
	return ERROR_SUCCESS;
}

HRESULT XMFLOAT4ToWString(wstring& out, const XMFLOAT4& in) {
	wstring tempOut;
	float inAsArray[] = { in.x, in.y, in.z, in.w };
	const float* pInAsArray = inAsArray;
	if( FAILED(textProcessing::numberArrayToWString(
		tempOut, pInAsArray, static_cast<size_t>(FLOAT4_SIZE), START_CH_W, END_CH_W)) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
	} else {
		out = tempOut;
		return ERROR_SUCCESS;
	}
}

HRESULT strToColorRGBA(XMFLOAT4& out, const char* const in, size_t& index);

HRESULT colorRGBAToWString(wstring& out, const XMFLOAT4& in);