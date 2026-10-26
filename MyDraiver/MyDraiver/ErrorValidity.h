#pragma once
#include <string>
#include <iostream>

enum ErrorCodeName
{
	Synthesis,
	Validity,
	Api,
	Dicase,
	Version,
	File,
	Untreated,
};

class ErrorValidity
{

public: 
	int ERROR_CODE;
	ErrorCodeName code_name = ErrorCodeName::Untreated;
	std::string message;
	bool is_error;
	int line, word;

	void register_fotal_error(int int_code, std::string error_text, int line, int word) {
		is_error = true;
		message = error_text;
	}

	void print_error() {
		std::cout << '\t' << "code: " << ERROR_CODE << '\n';
		std::cout << '\t' << "error: " << message << '\n';
		std::cout << '\t' << "Type Error: " << code_name << '\n';
		std::cout << '\t' << "line: " << line << '\n';
		std::cout << '\t' << "word: " << word << '\n';
	}

	void print_error(const std::string& main_text, const std::string& token, const std::string& message) {

		register_fotal_error(3, message, -1, -1);

		std::cout << main_text << token << '\t' << message << '\n';
		std::cout << std::string(token.size(), ' ') << std::string(token.size(), '^') << '\n';
	}

	void print_error(const std::string& token, const std::string& message) {
		std::cout << token << '\t' << message << '\n';
		std::cout << std::string(token.size(), '^') << '\n';
	}

};

