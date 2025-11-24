#pragma once
#include "User.h"
#include <vector>
#include <string>

class Passenger : public User
{
private:
	std::vector<std::string> reservations;

public:
	Passenger(std::string login, std::string password, int id);

	void showMenu() override;
	std::string getType() const override;

	void bookTicket();
	void listMyTickets();

};

