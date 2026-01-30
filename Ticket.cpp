/**
 * @file Ticket.cpp
 * @brief Implementacja klasy biletu kolejowego
 */

#include "Ticket.h"
#include <sstream>

// Konstruktor inicjalizujący bilet wszystkimi parametrami
Ticket::Ticket(int id, int tID, std::string login, int seat, double price) : 
	FticketId(id), trainId(tID), passengerLogin(login), seatNumber(seat), Fprice(price) {}

// Gettery - zwracają wartości pól prywatnych
int Ticket::getTicketId() const { return FticketId; }
int Ticket::getTrainId() const { return trainId; }
std::string Ticket::getPassengerLogin() const { return passengerLogin; }
int Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getPrice() const { return Fprice; }

// Tworzy i zwraca tekstową reprezentację biletu
std::string Ticket::toString() const {

	std::stringstream text;  // Strumień do budowania tekstu
	// Formatuj informacje o bilecie
	text << "Bilet #" << FticketId << " | Pociag ID: " << trainId << " | Miejsce " << seatNumber << " | Pasazer: " << passengerLogin;

	return text.str();  // Zwróć sformatowany ciąg
}