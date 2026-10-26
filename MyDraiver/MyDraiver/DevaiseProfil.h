#pragma once
#include <string>
#include <iostream>
#include "EMENS.h"
using namespace std;



/// <summary>
/// Профиль девайса
/// </summary>
class DevaiseProfil
{
public:
	int host = 4378;
	string addres = "0.0.0.0";
	string dName = "pillcase";
	string password = "admin";
	string devaise_name = "My Pill Case";
	string devaise_descript = "null";
	Roles rele = Roles::Devaise;


	void Info() {
		cout << "Devaise name: " << devaise_name << '\n';
		cout << "Devaise host: " << host << '\n';
		cout << "Devaise addres: " << addres << '\n';
		cout << "Role: " << rele;
	}
};

