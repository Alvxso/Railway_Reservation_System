#include "Passenger.h"
#include <iostream>

Passenger::Passenger(std::string login, std::string password, int id) : User(login, password, id) {}

std::string Passenger::getType() const { return "PASSENGER"; }

void Passenger::showMenu() {
	std::cout << "\n=== Panel Pasazera: " << get_Flogin() << "===" << std::endl;
	std::cout << "1. Wyszukaj pociag i zarezerwuj bilet" << std::endl;
	std::cout << "2. Moje rezerwacje" << std::endl;
	std::cout << "3. Wyloguj" << std::endl;
	std::cout << "Wybierz opcje: " << std::endl;
}

void Passenger::bookTicket() {
	//TO DO LOGIKA REZERWACJI
}

void Passenger::listMyTickets() {
	std::cout << "\nTwoje rezerwacje:" << std::endl;
	if (reservations.empty()) {
		std::cout << "Brak rezerwacji" << std::endl;
	}
	else {
		for (const auto& ticket : reservations) {
			std::cout << "- " << ticket << std::endl;
		}
	}
}
