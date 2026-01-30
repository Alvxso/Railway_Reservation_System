/**
 * Klasa DataManager - Zarządzanie zapisem i odczytem danych
 * 
 * Odpowiada za:
 * - Zapisywanie danych do plików YAML (użytkownicy, pociągi, bilety)
 * - Wczytywanie danych z plików YAML przy starcie programu
 * - Zapewnienie trwałości danych między uruchomieniami programu
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

class DataManager
{
private:
	const std::string FusersFile = "users.yaml";
	const std::string FtrainsFile = "trains.yaml";
	const std::string FticketsFile = "tickets.yaml";

	std::string trim(const std::string& str);

public:
	DataManager();

	void saveUsers(const std::vector < std::unique_ptr<User>>& users);
	void loadUsers(std::vector<std::unique_ptr<User>>& users);

	void saveTrains(const std::vector<Train>& trains);
	void loadTrains(std::vector<Train>& trains);

	void saveTickets(const std::vector<Ticket>& tickets);
	void loadTickets(std::vector<Ticket>& tickets);
};

