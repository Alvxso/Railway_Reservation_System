/**
 * @file Passenger.h
 * @brief Klasa reprezentująca pasażera w systemie rezerwacji
 */

#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <string>

/**
 * @class Passenger
 * @brief Klasa pasażera dziedzicząca po User
 */
class Passenger : public User
{
public:
	// Konstruktor tworzący obiekt pasażera
	Passenger(std::string login, std::string password, int id);

	// Przesłonięcie metody wyświetlającej menu pasażera
	void showMenu() override;
	// Przesłonięcie metody zwracającej typ użytkownika ("PASSENGER")
	std::string getType() const override;

	// Wyszukuje pociągi i rezerwuje bilet
	void bookTicket(std::vector<Train>& trains, std::vector<Ticket>& tickets);
	// Wyświetla listę biletów należących do tego pasażera
	void listMyTickets(const std::vector<Ticket>& tickets) const;
};