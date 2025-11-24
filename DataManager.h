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

	void saveTrains(const std::vector < std::unique_ptr<Train>>& train);
	void loadTrains(std::vector<std::unique_ptr<Train>>& train);

	void saveTickets(const std::vector < std::unique_ptr<Ticket>>& ticket);
	void loadTickets(std::vector<std::unique_ptr<Ticket>>& ticket);
};

