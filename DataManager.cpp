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
	std::cout << "Zapisano uzytkownukow do " << FusersFile << std::endl;
}

void DataManager::loadUsers(std::vector<std::unique_ptr<User>>& users) {
	std::ifstream file(FusersFile);
	if (!file.is_open()) {
		std::cout << "Plik " << FusersFile << " nie istnieje. Stworzono nowy." << std::endl;
		return;
	}

	std::string line;

	std::string type, login, password;
	int id = 0;

	while (std::getline(file, line)) {
		line = trim(line);
		if (line == "---") {
			if (!login.empty()) {
				users.push_back(std::make_unique<Admin>(login, password, id));
			}
			else if (type == "PASSANGER") {
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

	if (!login.empty()) {
		if (type == "ADMIN") users.push_back(std::make_unique<Admin>(login, password, id));
		else if (type == "PASSENGER") users.push_back(std::make_unique<Passenger>(login, password, id));
	}

	file.close();
	std::cout << "Wczytano " << users.size() << " uzytkownikow." << std::endl;
}
//DataManager::
//DataManager::
//DataManager::