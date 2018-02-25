#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <sstream>

using namespace std;


class InvalidExpressionException : public std::exception {

public:
	InvalidExpressionException(const string m) : message(m) { }

	virtual const char * what() const throw() {
		return message.c_str();
	}

private:
	int lineNumber;
	string message;
};