#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

DataManager::DataManager(){}

std::string DataManager::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (std::string::npos == first) return str;
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

void DataManager::saveUsers(const std::vector<std::unique_ptr<User>>& users) {
	std::ofstream file(FusersFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FusersFile << " do zapisu" << std::endl;
		return;
	}

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

void DataManager::loadUsers(std::vector<std::unique_ptr<User>>& users) {
	std::ifstream file(FusersFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FusersFile << " nie istnieje. Tworze nowy startowy." << std::endl;
		return;
	}

	std::string line;
	std::string type, login, password;
	int id = 0;

	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
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

	if (!login.empty()) {
		if (type == "ADMIN") users.push_back(std::make_unique<Admin>(login, password, id));
		else if (type == "PASSENGER") users.push_back(std::make_unique<Passenger>(login, password, id));
	}

	file.close();
	std::cout << "Wczytano " << users.size() << " uzytkownikow." << std::endl;
}

void DataManager::saveTrains(const std::vector<Train>& trains) {
	std::ofstream file(FtrainsFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FtrainsFile << " do zapisu" << std::endl;
		return;
	}

	for (const auto& train : trains) {
		file << "---\n";
		file << "id: " << train.getID() << "\n";
		file << "origin: " << train.getOrigin() << "\n";
		file << "destination: " << train.getDestination() << "\n";
		file << "date: " << train.getDate() << "\n";
		file << "capacity: " << train.getCapacity() << "\n";

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

void DataManager::loadTrains(std::vector<Train>& trains) {
	std::ifstream file(FtrainsFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FtrainsFile << " nie istnieje. Stworzono nowy." << std::endl;
		return;
	}

	std::string line;
	int id = 0, capacity = 0;
	std::string origin, destination, date, occupiedStr;


	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			if (capacity > 0) {
				Train t(id, origin, destination, date, capacity);

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

				id = 0; capacity = 0; origin = ""; destination = ""; date = ""; occupiedStr = "";
			}
			continue;
		}

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

void DataManager::saveTickets(const std::vector<Ticket>& tickets) {
	std::ofstream file(FticketsFile);
	if (!file.is_open()) {
		std::cerr << "Blad: Nie mozna otworzyc pliku " << FticketsFile << "do zapisu" << std::endl;
	}

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

	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			if (id > 0 && !passengerLogin.empty()) {
				Ticket t(id, trainId, passengerLogin, seat, price);
				tickets.push_back(t);

				id = 0; trainId = 0; seat = 0; price = 0.0; passengerLogin = "";
			}
			continue;
		}

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

	if (id > 0 && !passengerLogin.empty()) {
		Ticket t(id, trainId, passengerLogin, seat, price);
		tickets.push_back(t);
	}

	file.close();
	std::cout << "Wczytano " << tickets.size() << " biletow" << std::endl;
}