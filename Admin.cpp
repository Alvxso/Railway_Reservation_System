#include "Admin.h"
#include <iostream>


Admin::Admin(std::string login, std::string password, int id) : User(login, password, id) {}

std::string Admin::getType() const { return "ADMIN"; }

void Admin::showMenu() {
	std::cout << "\n=== Panel Admina: " << get_Flogin() << "===" << std::endl;
	std::cout << "1. Zarzadzaj pociagami" << std::endl;
	std::cout << "2. Generuj raporty sprzedazy" << std::endl;
	std::cout << "3. Zarzadzaj uzytkownikami" << std::endl;
	std::cout << "4. Wyloguj" << std::endl;
	std::cout << "Wybierz opcje: " << std::endl;
}

void Admin::addTrain() {} //TO DO
void Admin::removeTrain() {} //TO DO
void Admin::generateReport() {} //TO DO