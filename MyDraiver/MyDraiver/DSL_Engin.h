#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <chrono>

#include "API_JSON_VALID.h"
#include "EnginDslFunction.h"
#include "ErrorValidity.h"
#include "Variables.h"

using namespace std;



struct Statement
{
	int line;
	std::vector<std::string> words;
};


class DSL_Engin
{
private:
	string name;
	string password = "admin";
	bool trust = true;
	bool is_Connect = true;

	using Batch = std::vector<Statement>;
	using Script = std::vector<Batch>;


	// Одна функция: читает текст пользователя и возвращает готовый Script.
	// Разделители statement: ';' и '\n'.  GO отдельным словом закрывает батч.
	// Комментарии: --, #, // до конца строки.
	
	Script parseScript(const std::string& text) {
		Script script;
		Batch  currentBatch;

		std::string word;				  // текущий токен
		std::vector<std::string> words;   // токены текущего statement'а
		bool inQuotes = false;
		bool inComment = false;
		int  line = 1;
		int  stmtLine = 1;

		auto flushWord = [&]() {
			if (!word.empty()) { words.push_back(std::move(word)); word.clear(); }
			};

		auto flushStatement = [&]() {
			flushWord();
			inComment = false;
			if (words.empty()) { stmtLine = line; return; }

			if (words.size() == 1 && words[0] == "GO") {
				if (!currentBatch.empty()) {
					script.push_back(std::move(currentBatch));
					currentBatch.clear();
				}
			}
			else {
				currentBatch.push_back({ stmtLine, std::move(words) });
			}
			words.clear();
			stmtLine = line;
			};

		for (std::size_t i = 0; i < text.size(); ++i) {
			char c = text[i];

			if (inComment) {
				if (c == '\n') { inComment = false; flushStatement(); ++line; stmtLine = line; }
				continue;
			}

			// начало комментария
			if (c == '#' ||
				(c == '-' && i + 1 < text.size() && text[i + 1] == '-')) {
				flushWord();
				inComment = true;
				if (c != '#') ++i;          // съесть второй символ комментария
				continue;
			}

			if (c == '"') { inQuotes = !inQuotes; continue; }

			if (!inQuotes && (c == ' ' || c == '\t' || c == '\r')) {
				flushWord();
			}
			else if (!inQuotes && c == ';') {
				flushStatement();
			}
			else if (!inQuotes && c == '\n') {
				flushStatement();
				++line;
				stmtLine = line;
			}
			else {
				word.push_back(c);
			}
		}

		// хвост без завершающего разделителя
		flushStatement();
		if (!currentBatch.empty()) script.push_back(std::move(currentBatch));
		return script;
	}

	/// <summary>
	/// Обработка комнады Print
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	bool DlsPrint(const Statement& s) {
		if (s.words.size() < 2) {
			error_info.print_error(s.words[0], s.words[1], "usage: PRINT <text>");
			error_info.message = "usage: PRINT <text>";
			return false;
		}

		// проверяем, что это НЕ переменная
		for (int i = 0; i < variad.size(); i++) {
			Variables& v = variad[i];
			if (v.name == s.words[1]) {
				std::cout << v.values;
				return true;
			}
		}

		std::cout << s.words[1];
		return true;
	}
protected:

	ErrorValidity error_info;
	std::vector < Variables > variad;

	chrono::steady_clock::time_point timer;
	chrono::steady_clock::time_point request_timer;
	string dsl_text;
	vector<string> list_word_command;
	int word = -1;
	int line = -1;
	bool errors = false;
	string error;
	int code;
	const Statement s;
	

	void DSLEngin(vector<string> word_command) {
		int local_woid = -1, local_line =  -1;
		try {
			if (word_command.size() < 1) { code = 1; error = "element 'list_word_command' is null"; return; }
			int index = 1;
			if (is_Connect) {
				if (word_command[0] == "display") {
					local_woid = 0;
					if (word_command.size() < 3) {
						code = 1;
						error = "The entered query had an incorrect format or an incorrect number of words.";
						return;
					}
					else {
						if (word_command[1] == "file") {
							local_woid = 1;
							cout << SET_FILE_DSL(word_command[2]);
						}
						else if (word_command[2] == "from")
						{
							API_JSON_VALID api = API_JSON_VALID(name, password);
							api.TEXT_JSON(word_command[3], true);
							cout << "GET: " << api.GET(word_command[1]) << '\n';
							valide_errors.register_fotal_error(300, api.message, -1, -1);
						}
						else {
							valide_errors.print_error(word_command[0], word_command[1], "no correct comande");
						}
					}
				}
				else if (word_command[0] == "PRINT") {
					// s.words = word_command[1];
					DlsPrint(s);
					return;
				}
				else if (word_command[0] == "diconect") {
					is_Connect = false;
				}
				else if (word_command[0] == "list") {
					cout << "viwe comand" << ':' << '\n';
					for (int i = 1; i < word_command.size(); i++) {
						cout << i - 1 << " ) " << word_command[i] << '.' << '\n';
					}
				}
				else {
					valide_errors.print_error(word_command[0], "no correct comande");
				}
			}
			else {

				// conect pillcase password admin trust true
				if(word_command[0] == "conect") {
					name = word_command[1];
					if(word_command[2] == "password") password = word_command[3];
					else valide_errors.print_error(word_command[0] + " " + word_command[1] + " ", word_command[2], "no correct");
					if (word_command[4] == "trust") {
						if (word_command[5] == "true") { trust = false; }
						else { trust = false; }
					}
					else {
						valide_errors.print_error(word_command[0]+" "+ word_command[1]+" "+ word_command[2]+" "+ word_command[3], word_command[4]+ " ", "no correct");
					}
					is_Connect = trust;
				}
				else {
					valide_errors.register_fotal_error(0,"no trust or conect",1,0);
				}
			}
		}
		catch (std::exception e) {
			valide_errors.register_fotal_error(10, e.what(), local_line, local_woid);
			return;
		}
	}

	void ReadDSLEngin() {
		request_timer = std::chrono::steady_clock::now();
		int local_woid;
		try {
			string text;
			for (int i = 0; i < dsl_text.size(); i++)
			{
				if (dsl_text[i] == ' ') {
					//cout << "register: " << text << '\n';
					list_word_command.push_back(text);
					text = "";
					local_woid = i;
				}
				else {
					text += dsl_text[i];
				}
			}

			list_word_command.push_back(text);
			text = "";
			
			DSLEngin(list_word_command);
		}
		catch (std::exception e) 
		{
			valide_errors.register_fotal_error(1, e.what(), 0, local_woid);
		}
	}

public:
	ErrorValidity valide_errors;

#pragma region IS_VALUE

	bool IS_VALUE(bool value1, bool value2) {
		if (value1 == value2) return true;
		else return false;
	}

	bool IS_VALUE(double value1, double value2) {
		if (value1 == value2) return true;
		else return false;
	}

	bool IS_VALUE(float value1, float value2) {
		if (value1 == value2) return true;
		else return false;
	}

	bool IS_VALUE(string value1, string value2) {
		if (value1 == value2) return true;
		else return false;
	}

	bool IS_VALUE(int value1, int value2) {
		if (value1 == value2) return true;
		else return false;
	}

	bool IS_VALUE(bool value1, int value2) {
		if (value1 == true && value2 == 1) {
			return true;
		}
		else if (value1 == false && (value2 == 0 || value2 == -1)) {
			return true;
		}
		else {
			return false;
		}
	}

#pragma endregion

	void SET_LENGEG(const char *name) {
		setlocale(LC_ALL, name);
	}

	void PrintScipt(std::string text) {
		Script scp = (parseScript(text));
	}

	bool IS_CONECT() {
		return is_Connect;
	}

	void Init() {
		timer = std::chrono::steady_clock::now();
	}

	DSL_Engin()
	{
		Init();
	}

	~DSL_Engin() {

			cout << "FINAL:" << '\n';
			if (errors) {
				valide_errors.print_error();
			}
			cout << "code: " << code << '\n';
			cout << "request_timer: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - request_timer).count() << " ms" << '\n';

			cout << "timer: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer).count() << " ms" << '\n';

			timer = std::chrono::steady_clock::now();
			list_word_command.clear();
	}


	void Show() {
		try {
			ReadDSLEngin();
		}
		catch (std::exception e)
		{
			// errors = 10;
			error = e.what();
			std::cout << e.what() << '\n';
			return;
		}
	}

	void PRINT_FINAL_DSL() {
		try 
		{
			if (list_word_command.size() <= 0) { code = 1; error = "element 'list_word_command' is null"; return; }
			
			for (int i = 0; i < list_word_command.size(); i++) {
				std::cout << i << ") " << list_word_command[i] << '\n';
			}
		}
		catch (std::out_of_range e)
		{
			valide_errors.register_fotal_error(1, e.what(), 0, -1);
		}
	}
	
	void SET_DSL(string text) {
		dsl_text = text;
	}

	string SET_FILE_DSL(string path) {
		std::ifstream inFile(path); // создаём/открываем файл для записи
		std::string line = "not font";

		if (inFile.is_open()) {
			while (std::getline(inFile, line)) {
				
			}
		}
		else {
			valide_errors.register_fotal_error(2, "The file could not be opened for writing", 0, -1);
			return "";
		}
		inFile.close(); 
		return line;
	}

};

