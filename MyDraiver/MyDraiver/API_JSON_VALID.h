#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <chrono>
#include "DevaiseProfil.h"


using json = nlohmann::json;

class API_JSON_VALID
{
private:
	json data;
	DevaiseProfil deveise;
	std::string DivaiseName;
public: 

	API_JSON_VALID(std::string name, std::string password) {
		if (deveise.dName != name || (password != deveise.password)) {
			message = "without access";
			return;
		}
	}

	bool allow_exceptions = true;
	bool ignore_comments = true;
	bool ignore_trailing_commas = true;

	std::string message;
	std::vector<string> woid;

	void TEXT_JSON(std::string json,bool path = false) {
		if (!path) {
			data = json::parse(json, nullptr, true, true);
		}
		else {
			std::ifstream f(json);
			data = json::parse(f, nullptr, true, true);
		}
	}

	std::string GET(const std::string& key) {
		try {



			const json& value = data.at(deveise.dName).at(key);

			if (value.is_string())
				return value.get<std::string>();

			return value.dump();                    
		}
		catch (const json::out_of_range&) {
			message = "Key '" + key + "' not found";
		}
		catch (const json::type_error& e) {
			message = "Incorrect type: " + std::string(e.what());
		}
		return "not found";
	}
};

