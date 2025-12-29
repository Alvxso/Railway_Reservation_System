#include "Admin.h"
#include "InputValidation.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype> 

Admin::Admin(std::string login, std::string password, int id)
	: User(login, password, id) {
}

std::string Admin::getType() const { return "ADMIN"; }

void Admin::showMenu() {
	std::cout << "\n=== PANEL ADMINA: " << get_Flogin() << " ===" << std::endl;
	std::cout << "1. Dodaj nowy pociag" << std::endl;
	std::cout << "2. Usun pociag" << std::endl;
	std::cout << "3. Generuj raport systemowy" << std::endl;
	std::cout << "4. Wroc do menu glownego (Wyloguj)" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
}

std::string formatCity(std::string str) {
	if (str.empty()) return str;
	str[0] = std::toupper(str[0]);
	for (size_t i = 1; i < str.length(); i++) {
		str[i] = std::tolower(str[i]);
	}
	return str;
}

void Admin::addTrain(std::vector<Train>& trains) {
	std::cout << "\n--- KREATOR POCIAGU ---\n";
	int id, capacity;
	std::string origin, destination, date;

	while (true) {
		readInt("Podaj unikalne ID pociagu: ", id);
		auto it = std::find_if(trains.begin(), trains.end(),
			[id](const Train& t) { return t.getID() == id; });
		if (it == trains.end()) break;
		std::cout << "Blad: Pociag o ID " << id << " juz istnieje!\n";
	}

	readLine("Stacja poczatkowa: ", origin);
	origin = formatCity(origin); 

	readLine("Stacja koncowa: ", destination);
	destination = formatCity(destination); 
	readLine("Data (RRRR-MM-DD): ", date);

	readInt("Liczba miejsc (max 100): ", capacity);
	if (capacity > 100) capacity = 100;
	if (capacity < 1) capacity = 1;

	trains.emplace_back(id, origin, destination, date, capacity);
	std::cout << "SUKCES: Pociag relacji " << origin << " -> " << destination << " zostal dodany.\n";
}

void Admin::removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- USUWANIE POCIAGU ---\n";
	if (trains.empty()) {
		std::cout << "Baza pociagow jest pusta.\n";
		return;
	}

	int idToRemove;
	readInt("Podaj ID pociagu do usuniecia: ", idToRemove);

	auto it = std::find_if(trains.begin(), trains.end(),
		[idToRemove](const Train& t) { return t.getID() == idToRemove; });

	if (it == trains.end()) {
		std::cout << "BLAD: Nie znaleziono pociagu o takim ID.\n";
		return;
	}

	auto initialTicketCount = tickets.size();
	tickets.erase(
		std::remove_if(tickets.begin(), tickets.end(),
			[idToRemove](const Ticket& t) { return t.getTrainId() == idToRemove; }),
		tickets.end()
	);
	int deletedTickets = initialTicketCount - tickets.size();

	trains.erase(it);

	std::cout << "SUKCES: Usunieto pociag ID " << idToRemove << ".\n";
	if (deletedTickets > 0) {
		std::cout << "UWAGA: Anulowano rowniez " << deletedTickets << " biletow.\n";
	}
}

void Admin::generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users) {
	std::cout << "\n================ RAPORT SYSTEMOWY ================\n";
	std::cout << "Liczba uzytkownikow: " << users.size() << "\n";
	std::cout << "Liczba pociagow: " << trains.size() << "\n";
	std::cout << "Liczba sprzedanych biletow: " << tickets.size() << "\n";
	std::cout << "==================================================\n";
}