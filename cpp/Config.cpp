/*
Config.cpp
----------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 25, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the Config class and its nested classes

Usage Notes: See Config.h
*/

#include "Config.h"
#include <stdexcept>

using std::map;

Config::Value::Value(const DataType type, const void* const value) :
m_type(type), m_value(value)
{
	if( value == 0 ) {
		throw std::invalid_argument("Config::Value constructor passed a null pointer.");
	}
}

Config::Value::~Value(void) {

}

const void* Config::Value::getValue(const DataType type) {

}

Config::Key::Key(const std::wstring& scope, const std::wstring& field) {

}

bool Config::Key::operator==(const Key& other) const {

}

bool Config::Key::operator<(const Key& other) const {

}

HRESULT Config::insert(const std::wstring& scope, const std::wstring& field,
	const DataType type, const void* const value) {

}

const void* Config::retrieve(const std::wstring& scope, const std::wstring& field,
	const DataType type) const {

}

map<Config::Key, Config::Value>::const_iterator Config::cbegin(void) const {

}

map<Config::Key, Config::Value>::const_iterator Config::cend(void) const {

}

HRESULT Config::insert(const std::wstring& scope, const std::wstring& field, const std::wstring* const value) {

}

HRESULT Config::retrieve(const std::wstring& scope, const std::wstring& field, const std::wstring*& value) const {

}