/**
 * @file DataManager.cpp
 * @brief Implementacja funkcjonalności zapisu i odczytu danych
 */

#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// Konstruktor domyślny
DataManager::DataManager(){}

// Funkcja pomocnicza usuwająca białe znaki z początku i końca ciągu
std::string DataManager::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");  // Znajdź pierwszy znak niebędący białym znakiem
	if (std::string::npos == first) return str;  // Jeśli cały ciąg to białe znaki, zwróć go bez zmian
	size_t last = str.find_last_not_of(" \t\r\n");   // Znajdź ostatni znak niebędący białym znakiem
	return str.substr(first, (last - first + 1));     // Zwróć wycięty fragment
}

// Zapisuje użytkowników do pliku YAML
void DataManager::saveUsers(const std::vector<std::unique_ptr<User>>& users) {
	std::ofstream file(FusersFile);  // Otwórz plik do zapisu
	if (!file.is_open()) {  // Sprawdź czy udało się otworzyć plik
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FusersFile << " do zapisu" << std::endl;
		return;
	}

	// Zapisz każdego użytkownika w formacie YAML
	for (const auto& user : users) {
		file << "---\n";  // Separator dokumentu YAML
		file << "type: " << user->getType() << "\n";           // Typ użytkownika (ADMIN/PASSENGER)
		file << "id: " << user->get_Fid() << "\n";             // ID użytkownika
		file << "login: " << user->get_Flogin() << "\n";       // Login
		file << "password: " << user->get_Fpassword() << "\n"; // Hasło
	}
	file.close();  // Zamknij plik
	std::cout << "Zapisano uzytkownikow do " << FusersFile << std::endl;
}

// Wczytuje użytkowników z pliku YAML
void DataManager::loadUsers(std::vector<std::unique_ptr<User>>& users) {
	std::ifstream file(FusersFile);  // Otwórz plik do odczytu
	if (!file.is_open()) {  // Jeśli plik nie istnieje
		std::cout << "Plik " << FusersFile << " nie istnieje. Tworze nowy startowy." << std::endl;
		return;
	}

	std::string line;
	std::string type, login, password;
	int id = 0;

	// Wczytuj plik linia po linii
	while (std::getline(file, line)) {
		line = trim(line);  // Usuń białe znaki z początku i końca
		if (line == "---") {  // Separator dokumentu - zapisz poprzedniego użytkownika
			if (!login.empty()) {  // Jeśli mamy dane użytkownika
				if (type == "ADMIN") {  // Utwórz administratora
					users.push_back(std::make_unique<Admin>(login, password, id));
				}
				else if (type == "PASSENGER") {  // Utwórz pasażera
					users.push_back(std::make_unique<Passenger>(login, password, id));
				}
				login = ""; password = ""; type = ""; id = 0;  // Wyczyść zmienne
			}
			continue;  // Przejdź do następnej linii
		}

		size_t colonPos = line.find(":");  // Znajdź separator klucz:wartość
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));      // Wyodrębnij klucz
			std::string value = trim(line.substr(colonPos + 1));   // Wyodrębnij wartość

			// Przypisz wartość do odpowiedniej zmiennej
			if (key == "type") type = value;
			else if (key == "id") id = std::stoi(value);  // Konwertuj string na int
			else if (key == "login") login = value;
			else if (key == "password") password = value;
		}
	}

	// Zapisz ostatniego użytkownika (jeśli plik nie kończy się separatorem)
	if (!login.empty()) {
		if (type == "ADMIN") users.push_back(std::make_unique<Admin>(login, password, id));
		else if (type == "PASSENGER") users.push_back(std::make_unique<Passenger>(login, password, id));
	}

	file.close();  // Zamknij plik
	std::cout << "Wczytano " << users.size() << " uzytkownikow." << std::endl;
}

// Zapisuje pociągi do pliku YAML
void DataManager::saveTrains(const std::vector<Train>& trains) {
	std::ofstream file(FtrainsFile);  // Otwórz plik do zapisu
	if (!file.is_open()) {  // Sprawdź czy udało się otworzyć
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FtrainsFile << " do zapisu" << std::endl;
		return;
	}

	// Zapisz każdy pociąg w formacie YAML
	for (const auto& train : trains) {
		file << "---\n";  // Separator dokumentu
		file << "id: " << train.getID() << "\n";                    // ID pociągu
		file << "origin: " << train.getOrigin() << "\n";            // Stacja początkowa
		file << "destination: " << train.getDestination() << "\n";  // Stacja końcowa
		file << "date: " << train.getDate() << "\n";                // Data
		file << "capacity: " << train.getCapacity() << "\n";        // Pojemność

		file << "occupied: ";  // Lista zajętych miejsc

		bool first = true;  // Flaga do zarządzania przecinkami

		// Przejdź przez wszystkie miejsca i zapisz zajęte
		for (auto i = 1; i <= train.getCapacity(); i++) {
			if (!train.isSeatFree(i)) {  // Jeśli miejsce zajęte
				if (!first) file << ",";  // Dodaj przecinek przed kolejnym numerem
				file << i;  // Zapisz numer miejsca
				first = false;
			}
		}
		file << "\n";
	}
	file.close();  // Zamknij plik
	std::cout << "Zapisano pociagi do " << FtrainsFile << std::endl;
}

// Wczytuje pociągi z pliku YAML
void DataManager::loadTrains(std::vector<Train>& trains) {
	std::ifstream file(FtrainsFile);  // Otwórz plik do odczytu
	if (!file.is_open()) {  // Jeśli plik nie istnieje
		std::cout << "Plik " << FtrainsFile << " nie istnieje. Stworzono nowy." << std::endl;
		return;
	}

	std::string line;
	int id = 0, capacity = 0;
	std::string origin, destination, date, occupiedStr;


	// Wczytuj plik linia po linii
	while (std::getline(file, line)) {
		line = trim(line);  // Usuń białe znaki
		if (line == "---") {  // Separator - zapisz poprzedni pociąg
			if (capacity > 0) {  // Jeśli mamy dane pociągu
				Train t(id, origin, destination, date, capacity);  // Utwórz nowy pociąg

				// Zarezerwuj miejsca z listy zajętych
				if (!occupiedStr.empty()) {
					std::stringstream ss(occupiedStr);  // Strumień do parsowania listy
					std::string segment;
					while (std::getline(ss, segment, ',')) {  // Podziel po przecinkach
						try {
							int seatNum = std::stoi(segment);  // Konwertuj string na int
							t.reserveSeat(seatNum);  // Zarezerwuj miejsce
						} catch (...) {}  // Ignoruj błędy konwersji
					}
				}
				trains.push_back(t);  // Dodaj pociąg do listy

				// Wyczyść zmienne dla następnego pociągu
				id = 0; capacity = 0; origin = ""; destination = ""; date = ""; occupiedStr = "";
			}
			continue;
		}

		size_t colonPos = line.find(":");  // Znajdź separator klucz:wartość
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));      // Klucz
			std::string value = trim(line.substr(colonPos + 1));   // Wartość

			// Przypisz wartość do odpowiedniej zmiennej
			if (key == "id") id = std::stoi(value);
			else if (key == "origin") origin = value;
			else if (key == "destination") destination = value;
			else if (key == "date") date = value;
			else if (key == "capacity") capacity = std::stoi(value);
			else if (key == "occupied") occupiedStr = value;
		}
	}
	// Zapisz ostatni pociąg (jeśli plik nie kończy się separatorem)
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
	file.close();  // Zamknij plik
	std::cout << "Wczytano " << trains.size() << " pociągów" << std::endl;

}

// Zapisuje bilety do pliku YAML
void DataManager::saveTickets(const std::vector<Ticket>& tickets) {
	std::ofstream file(FticketsFile);  // Otwórz plik do zapisu
	if (!file.is_open()) {  // Sprawdź czy udało się otworzyć
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FticketsFile << " do zapisu" << std::endl;
	}

	// Zapisz każdy bilet w formacie YAML
	for (const auto& ticket : tickets) {
		file << "---\n";  // Separator dokumentu
		file << "id: " << ticket.getTicketId() << "\n";              // ID biletu
		file << "trainId: " << ticket.getTrainId() << "\n";          // ID pociągu
		file << "passenger: " << ticket.getPassengerLogin() << "\n"; // Login pasażera
		file << "seat: " << ticket.getSeatNumber() << "\n";          // Numer miejsca
		file << "price: " << ticket.getPrice() << "\n";              // Cena
	}
	file.close();  // Zamknij plik
	std::cout << "Zapisano bilety do " << FticketsFile << std::endl;
}

// Wczytuje bilety z pliku YAML
void DataManager::loadTickets(std::vector<Ticket>& tickets) {
	std::ifstream file(FticketsFile);  // Otwórz plik do odczytu
	if (!file.is_open()) {  // Jeśli plik nie istnieje
		std::cout << "Plik " << FticketsFile << " nie istnieje, Stworzono nowy" << std::endl;
		return;
	}

	std::string line;
	int id = 0, trainId = 0, seat = 0;
	double price = 0.0;
	std::string passengerLogin;

	// Wczytuj plik linia po linii
	while (std::getline(file, line)) {
		line = trim(line);  // Usuń białe znaki
		if (line == "---") {  // Separator - zapisz poprzedni bilet
			if (id > 0 && !passengerLogin.empty()) {  // Jeśli mamy dane biletu
				Ticket t(id, trainId, passengerLogin, seat, price);  // Utwórz bilet
				tickets.push_back(t);  // Dodaj do listy

				// Wyczyść zmienne dla następnego biletu
				id = 0; trainId = 0; seat = 0; price = 0.0; passengerLogin = "";
			}
			continue;
		}

		auto colonPos = line.find(":");  // Znajdź separator klucz:wartość
		if (colonPos != std::string::npos) {
			std::string key = trim(line.substr(0, colonPos));      // Klucz
			std::string value = trim(line.substr(colonPos + 1));   // Wartość
			// Przypisz wartość do odpowiedniej zmiennej
			if (key == "id") id = std::stoi(value);
			else if (key == "trainId") trainId = std::stoi(value);
			else if (key == "passenger") passengerLogin = value;
			else if (key == "seat") seat = std::stoi(value);
			else if (key == "price") {
				try {
					price = std::stod(value);  // Konwertuj string na double
				}
				catch (...) { price = 0.0; }  // W przypadku błędu ustaw 0
			}
		}
	}

	// Zapisz ostatni bilet (jeśli plik nie kończy się separatorem)
	if (id > 0 && !passengerLogin.empty()) {
		Ticket t(id, trainId, passengerLogin, seat, price);
		tickets.push_back(t);
	}

	file.close();  // Zamknij plik
	std::cout << "Wczytano " << tickets.size() << " biletow" << std::endl;
}