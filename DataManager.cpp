/**
 * @file DataManager.cpp
 * @brief Implementacja klasy DataManager - obsługa trwałości danych w plikach YAML
 */

#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

/**
 * @brief Konstruktor DataManager
 */
DataManager::DataManager(){}

/**
 * @brief Usuwa białe znaki (spacje, tabulatory, nowe linie) z początku i końca tekstu
 * 
 * Pomocnicza funkcja używana przy parsowaniu plików YAML
 * 
 * @param str Tekst do obróbki
 * @return Tekst bez białych znaków na końcach
 */
std::string DataManager::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (std::string::npos == first) return str;
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

/**
 * @brief Zapisuje wszystkich użytkowników do pliku YAML
 * 
 * Format YAML dla każdego użytkownika:
 * ---
 * type: ADMIN/PASSENGER
 * id: [numer]
 * login: [login]
 * password: [hasło]
 * 
 * @param users Wektor użytkowników do zapisania
 */
void DataManager::saveUsers(const std::vector<std::unique_ptr<User>>& users) {
	std::ofstream file(FusersFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FusersFile << " do zapisu" << std::endl;
		return;
	}

	// Zapisz każdego użytkownika w formacie YAML
	for (const auto& user : users) {
		file << "---\n";
		file << "type: " << user->getType() << "\n";
		file << "id: " << user->get_Fid() << "\n";
		file << "login: " << user->get_Flogin() << "\n";
		file << "password: " << user->get_Fpassword() << "\n";
	}
	file.close();
	std::cout << "Zapisano uzytkownikow do " << FusersFile << std::endl;
}

/**
 * @brief Wczytuje użytkowników z pliku YAML
 * 
 * Parsuje plik YAML i tworzy obiekty Admin lub Passenger w zależności od typu.
 * Jeśli plik nie istnieje, zwraca pusty wektor (pierwsze uruchomienie)
 * 
 * @param users Wektor, do którego zostaną wczytani użytkownicy
 */
void DataManager::loadUsers(std::vector<std::unique_ptr<User>>& users) {
	std::ifstream file(FusersFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FusersFile << " nie istnieje. Tworze nowy startowy." << std::endl;
		return;
	}

	std::string line;
	std::string type, login, password;
	int id = 0;

	// Parsowanie pliku YAML linia po linii
	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			// Separator - jeśli mamy zebrane dane, utwórz użytkownika
			if (!login.empty()) {
				if (type == "ADMIN") {
					users.push_back(std::make_unique<Admin>(login, password, id));
				}
				else if (type == "PASSENGER") { 
					users.push_back(std::make_unique<Passenger>(login, password, id));
				}
				login = ""; password = ""; type = ""; id = 0;
			}
			continue; 
		}

		// Parsowanie pary klucz:wartość
		size_t colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));
			std::string value = trim(line.substr(colonPos + 1));

			if (key == "type") type = value;
			else if (key == "id") id = std::stoi(value);
			else if (key == "login") login = value;
			else if (key == "password") password = value;
		}
	}

	// Utwórz ostatniego użytkownika jeśli dane są kompletne
	if (!login.empty()) {
		if (type == "ADMIN") users.push_back(std::make_unique<Admin>(login, password, id));
		else if (type == "PASSENGER") users.push_back(std::make_unique<Passenger>(login, password, id));
	}

	file.close();
	std::cout << "Wczytano " << users.size() << " uzytkownikow." << std::endl;
}

/**
 * @brief Zapisuje wszystkie pociągi do pliku YAML
 * 
 * Format YAML dla każdego pociągu:
 * ---
 * id: [numer]
 * origin: [stacja początkowa]
 * destination: [stacja końcowa]
 * date: [data w formacie RRRR-MM-DD]
 * capacity: [liczba miejsc]
 * occupied: [lista numerów zajętych miejsc oddzielonych przecinkami]
 * 
 * @param trains Wektor pociągów do zapisania
 */
void DataManager::saveTrains(const std::vector<Train>& trains) {
	std::ofstream file(FtrainsFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FtrainsFile << " do zapisu" << std::endl;
		return;
	}

	// Zapisz każdy pociąg w formacie YAML
	for (const auto& train : trains) {
		file << "---\n";
		file << "id: " << train.getID() << "\n";
		file << "origin: " << train.getOrigin() << "\n";
		file << "destination: " << train.getDestination() << "\n";
		file << "date: " << train.getDate() << "\n";
		file << "capacity: " << train.getCapacity() << "\n";

		// Zapisz listę zajętych miejsc jako liczby oddzielone przecinkami
		file << "occupied: ";

		bool first = true;

		for (auto i = 1; i <= train.getCapacity(); i++) {
			if (!train.isSeatFree(i)) {
				if (!first) file << ",";
				file << i;
				first = false;
			}
		}
		file << "\n";
	}
	file.close();
	std::cout << "Zapisano pociagi do " << FtrainsFile << std::endl;
}

/**
 * @brief Wczytuje pociągi z pliku YAML
 * 
 * Parsuje plik YAML i odtwarza obiekty Train wraz z ich stanem zajętości miejsc.
 * Jeśli plik nie istnieje, zwraca pusty wektor
 * 
 * @param trains Wektor, do którego zostaną wczytane pociągi
 */
void DataManager::loadTrains(std::vector<Train>& trains) {
	std::ifstream file(FtrainsFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FtrainsFile << " nie istnieje. Stworzono nowy." << std::endl;
		return;
	}

	std::string line;
	int id = 0, capacity = 0;
	std::string origin, destination, date, occupiedStr;


	// Parsowanie pliku YAML linia po linii
	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			// Separator - jeśli mamy zebrane dane, utwórz pociąg
			if (capacity > 0) {
				Train t(id, origin, destination, date, capacity);

				// Parsuj listę zajętych miejsc i zarezerwuj je
				if (!occupiedStr.empty()) {
					std::stringstream ss(occupiedStr);
					std::string segment;
					while (std::getline(ss, segment, ',')) {
						try {
							int seatNum = std::stoi(segment);
							t.reserveSeat(seatNum);
						} catch (...) {}
					}
				}
				trains.push_back(t);

				// Reset zmiennych dla następnego pociągu
				id = 0; capacity = 0; origin = ""; destination = ""; date = ""; occupiedStr = "";
			}
			continue;
		}

		// Parsowanie pary klucz:wartość
		size_t colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));
			std::string value = trim(line.substr(colonPos + 1));

			if (key == "id") id = std::stoi(value);
			else if (key == "origin") origin = value;
			else if (key == "destination") destination = value;
			else if (key == "date") date = value;
			else if (key == "capacity") capacity = std::stoi(value);
			else if (key == "occupied") occupiedStr = value;
		}
	}
	
	// Utwórz ostatni pociąg jeśli dane są kompletne
	if (capacity > 0) {
		Train t(id, origin, destination, date, capacity);
		if (!occupiedStr.empty()) {
			std::stringstream ss(occupiedStr);
			std::string segment;
			while (std::getline(ss, segment, ',')) {
				try { t.reserveSeat(std::stoi(segment)); } catch(...) {}
			}
		}
		trains.push_back(t);
	}
	file.close();
	std::cout << "Wczytano " << trains.size() << "pociągów" << std::endl;

}

/**
 * @brief Zapisuje wszystkie bilety do pliku YAML
 * 
 * Format YAML dla każdego biletu:
 * ---
 * id: [numer biletu]
 * trainId: [ID pociągu]
 * passenger: [login pasażera]
 * seat: [numer miejsca]
 * price: [cena]
 * 
 * @param tickets Wektor biletów do zapisania
 */
void DataManager::saveTickets(const std::vector<Ticket>& tickets) {
	std::ofstream file(FticketsFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FticketsFile << "do zapisu" << std::endl;
	}

	// Zapisz każdy bilet w formacie YAML
	for (const auto& ticket : tickets) {
		file << "---\n";
		file << "id: " << ticket.getTicketId() << "\n";
		file << "trainId: " << ticket.getTrainId() << "\n";
		file << "passenger: " << ticket.getPassengerLogin() << "\n";
		file << "seat: " << ticket.getSeatNumber() << "\n";
		file << "price: " << ticket.getPrice() << "\n";
	}
	file.close();
	std::cout << "Zapisano bilety do " << FticketsFile << std::endl;
}

/**
 * @brief Wczytuje bilety z pliku YAML
 * 
 * Parsuje plik YAML i tworzy obiekty Ticket.
 * Jeśli plik nie istnieje, zwraca pusty wektor
 * 
 * @param tickets Wektor, do którego zostaną wczytane bilety
 */
void DataManager::loadTickets(std::vector<Ticket>& tickets) {
	std::ifstream file(FticketsFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FticketsFile << " nie istnieje, Stworzono nowy" << std::endl;
		return;
	}

	std::string line;
	int id = 0, trainId = 0, seat = 0;
	double price = 0.0;
	std::string passengerLogin;

	// Parsowanie pliku YAML linia po linii
	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			// Separator - jeśli mamy zebrane dane, utwórz bilet
			if (id > 0 && !passengerLogin.empty()) {
				Ticket t(id, trainId, passengerLogin, seat, price);
				tickets.push_back(t);

				// Reset zmiennych dla następnego biletu
				id = 0; trainId = 0; seat = 0; price = 0.0; passengerLogin = "";
			}
			continue;
		}

		// Parsowanie pary klucz:wartość
		auto colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));
			std::string value = trim(line.substr(colonPos + 1));
			if (key == "id") id = std::stoi(value);
			else if (key == "trainId") trainId = std::stoi(value);
			else if (key == "passenger") passengerLogin = value;
			else if (key == "seat") seat = std::stoi(value);
			else if (key == "price") {
				try {
					price = std::stod(value);
				}
				catch (...) { price = 0.0; }
			}
		}
	}

	// Utwórz ostatni bilet jeśli dane są kompletne
	if (id > 0 && !passengerLogin.empty()) {
		Ticket t(id, trainId, passengerLogin, seat, price);
		tickets.push_back(t);
	}

	file.close();
	std::cout << "Wczytano " << tickets.size() << " biletow" << std::endl;
}