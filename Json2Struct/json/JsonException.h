/*******************************************************************************
* @file     JsonException.h
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

class JsonException : public exception {
	string message;
	int code;
public:
	explicit JsonException(string message) {
		this->message = message;
	}
	
	virtual ~JsonException() throw() {}

	virtual const char* what() const throw () {
		if (message.length() == 0)
			return "Unknow Exception";
		return message.c_str();
	}
};

#endif