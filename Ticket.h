/**
 * @file Ticket.h
 * @brief Klasa reprezentująca bilet kolejowy
 */

#pragma once
#include <string>

/**
 * @class Ticket
 * @brief Klasa reprezentująca bilet na pociąg
 */
class Ticket
{
private:
	int FticketId;              // Unikalny identyfikator biletu
	int trainId;                // ID pociągu
	std::string passengerLogin; // Login pasażera, który kupił bilet
	int seatNumber;             // Numer zarezerwowanego miejsca
	double Fprice;              // Cena biletu
	
public:
	// Konstruktor tworzący nowy bilet
	Ticket(int id, int tID, std::string login, int seat, double price);

	// Gettery zwracające informacje o bilecie
	int getTicketId() const;
	int getTrainId() const;
	std::string getPassengerLogin() const;
	int getSeatNumber() const;
	double getPrice() const;

	// Zwraca tekstową reprezentację biletu
	std::string toString() const;
};

