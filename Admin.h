#pragma once
#include "User.h"
class Admin : public User
{
public:
	Admin(std::string login, std::string password, int id);

	void showMenu() override;
	std::string getType() const override;

	void addTrain();
	void removeTrain();
	void generateReport();
};

