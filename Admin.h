#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <memory>
#include <string>

class Admin : public User
{
public:
	Admin(std::string login, std::string password, int id);

	void showMenu() override;
	std::string getType() const override;

	void addTrain(std::vector<Train>& trains);

	void removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets);

	void generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users);
};