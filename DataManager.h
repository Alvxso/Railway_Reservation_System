/**
 * @file DataManager.h
 * @brief Klasa zarządzająca zapisem i odczytem danych z plików YAML
 */

#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"
#include "Admin.h"
#include "Passenger.h"
#include "Train.h"
#include "Ticket.h"

/**
 * @class DataManager
 * @brief Klasa odpowiedzialna za persystencję danych w plikach YAML
 */
class DataManager
{
private:
	const std::string FusersFile = "users.yaml";     // Nazwa pliku z użytkownikami
	const std::string FtrainsFile = "trains.yaml";   // Nazwa pliku z pociągami
	const std::string FticketsFile = "tickets.yaml"; // Nazwa pliku z biletami

	// Usuwa białe znaki z początku i końca ciągu
	std::string trim(const std::string& str);

public:
	// Konstruktor domyślny
	DataManager();

	// Zapisuje użytkowników do pliku
	void saveUsers(const std::vector < std::unique_ptr<User>>& users);
	// Wczytuje użytkowników z pliku
	void loadUsers(std::vector<std::unique_ptr<User>>& users);

	// Zapisuje pociągi do pliku
	void saveTrains(const std::vector<Train>& trains);
	// Wczytuje pociągi z pliku
	void loadTrains(std::vector<Train>& trains);

	// Zapisuje bilety do pliku
	void saveTickets(const std::vector<Ticket>& tickets);
	// Wczytuje bilety z pliku
	void loadTickets(std::vector<Ticket>& tickets);
};

