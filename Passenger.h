#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <string>

class Passenger : public User
{
public:
	Passenger(std::string login, std::string password, int id);

	void showMenu() override;
	std::string getType() const override;

	void bookTicket(std::vector<Train>& trains, std::vector<Ticket>& tickets);
	void listMyTickets(const std::vector<Ticket>& tickets) const;
};