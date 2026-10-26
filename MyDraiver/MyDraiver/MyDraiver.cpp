// MyDraiver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "DevaiseProfil.h"
#include "DSL_Engin.h"

int main()
{

	DevaiseProfil devaise;
	DSL_Engin engin;
	while (true) {

		engin.Init();
		if(engin.IS_CONECT()){
			std::cout << "SET: " << '\n';
		}
		else {
			std::cout << "plaise conect: " << '\n';
		}
		std::string dal_text;
		std::cout << "> ";
		std::getline(std::cin, dal_text);
		engin.PrintScipt(dal_text);
		engin.SET_DSL(dal_text);

		// Вызвано исключение : нарушение доступа для чтения.

		// display name from D:\PillCaseProject\ProgrammSettingPillCase\ProgrammSettingPillCase\Configurenshen\API_GET_JSON.json
		
		// D:\PillCaseProject\ProgrammSettingPillCase\ProgrammSettingPillCase\Configurenshen\API_GET_JSON.json
		//engin.SET_DSL("display file D:\\PillCaseProject\\ProgrammSettingPillCase\\ProgrammSettingPillCase\\Configurenshen\\API_GET_JSON.json");
		//std::cout << "start..." << '\n';
		engin.Show();

		// std::cout << "You write: " << '\n';
		// engin.PRINT_FINAL_DSL();
		std::cout << '\n';
		engin.~DSL_Engin();
		std::cout << '\n' << '\n';
	}
	
	int i;
	std::cin >> i;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
