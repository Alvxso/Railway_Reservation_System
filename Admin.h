/**
 * @file Admin.h
 * @brief Klasa reprezentująca administratora systemu
 */

#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class Admin
 * @brief Klasa administratora dziedzicząca po User
 */
class Admin : public User
{
public:
	// Konstruktor tworzący obiekt administratora
	Admin(std::string login, std::string password, int id);

	// Przesłonięcie metody wyświetlającej menu administratora
	void showMenu() override;
	// Przesłonięcie metody zwracającej typ użytkownika ("ADMIN")
	std::string getType() const override;

	// Dodaje nowy pociąg do systemu
	void addTrain(std::vector<Train>& trains);

	// Usuwa pociąg oraz wszystkie powiązane z nim bilety
	void removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets);

	// Generuje raport systemowy z statystykami
	void generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users);
};