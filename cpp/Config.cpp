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
#include "globals.h"
#include "defs.h"
#include <stdexcept>

using std::map;

Config::Value::Value(const DataType type, const void* const value) :
m_type(type), m_value(value)
{
	if( m_value == 0 ) {
		throw std::invalid_argument("Config::Value constructor passed a null pointer.");
	}
}

Config::Value::~Value(void) {
	switch( m_type ) {
	case DataType::WSTRING:
		delete static_cast<const std::wstring* const>(m_value);
		break;
	default:
		// This is a Microsoft-specific constructor
		throw std::exception("Config::Value class destructor is not designed to"
			"delete this type of data.");
		return;
	}
}

const void* const Config::Value::getValue(const DataType type) const {
	if( type == m_type ) {
		return m_value;
	} else {
		return 0;
	}
}

Config::Key::Key(const std::wstring& scope, const std::wstring& field) :
m_scope(scope), m_field(field)
{
	if( m_field.length == 0 ) {
		throw std::invalid_argument("Config::Key constructor passed an empty field value."
			" Only the scope string can be empty");
	}
}

bool Config::Key::operator==(const Key& other) const {
	return (m_field == other.m_field) && (m_scope == other.m_scope);
}

bool Config::Key::operator<(const Key& other) const {
	if( m_scope < other.m_scope ) {
		return true;
	} else if( m_scope == other.m_scope ) {
		return m_field < other.m_field;
	} else {
		return false;
	}
}

const std::wstring& Config::Key::getScope(void) const {
	return m_scope;
}

const std::wstring& Config::Key::getField(void) const {
	return m_field;
}

Config::Key::Key(const Key& other) :
m_scope(other.m_scope), m_field(other.m_field)
{}

Config::~Config(void) {
	// Delete all dynamically-allocated Value objects referred to by the map
	map<Key, Value*>::iterator end = m_map.end();
	for( map<Key, Value*>::iterator it = m_map.begin(); it != end; ++it ) {
		delete it->second;
		it->second = 0;
	}
}

HRESULT Config::insert(const std::wstring& scope, const std::wstring& field,
	const DataType type, const void* const value) {

	if( field.length == 0 || value == 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_DATA);
	}

	// Check for existing elements
	Key key(scope, field);
	if( m_map.count(key) != 0 ) {
		return 	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_ALREADY_ASSIGNED);
	} else {
		Value* value = new Value(type, value);
		m_map[key] = value;
	}
}

const void* Config::retrieve(const std::wstring& scope, const std::wstring& field,
	const DataType type) const {

	if( field.length == 0 ) {
		// Invalid input argument
		return 	0;
	}

	Key key(scope, field);
	map<Key, Value*>::const_iterator mapping = m_map.find(key);

	// Check if there is a value associated with the key parameters
	if( mapping != m_map.cend() ) {
		Value* value = mapping->second;
		// Checks for wrong data type
		return value->getValue(type);
	} else {
		return 0;
	}
}

map<Config::Key, Config::Value*>::const_iterator Config::cbegin(void) const {

}

map<Config::Key, Config::Value*>::const_iterator Config::cend(void) const {

}

HRESULT Config::insert(const std::wstring& scope, const std::wstring& field, const std::wstring* const value) {

}

HRESULT Config::retrieve(const std::wstring& scope, const std::wstring& field, const std::wstring*& value) const {

}