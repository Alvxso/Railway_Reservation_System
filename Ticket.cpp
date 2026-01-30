/**
 * @file Ticket.cpp
 * @brief Implementacja klasy Ticket - zarządzanie biletami
 */

#include "Ticket.h"
#include <sstream>
#include <iomanip>

/**
 * @brief Konstruktor klasy Ticket - inicjalizuje bilet
 * @param id Unikalny identyfikator biletu
 * @param tID ID pociągu
 * @param login Login pasażera
 * @param seat Numer miejsca
 * @param price Cena biletu w PLN
 */
Ticket::Ticket(int id, int tID, std::string login, int seat, double price) : 
	FticketId(id), trainId(tID), passengerLogin(login), seatNumber(seat), Fprice(price) {}

// Gettery - zwracają podstawowe informacje o bilecie
int Ticket::getTicketId() const { return FticketId; }
int Ticket::getTrainId() const { return trainId; }
std::string Ticket::getPassengerLogin() const { return passengerLogin; }
int Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getPrice() const { return Fprice; }

/**
 * @brief Ustawia nowy numer miejsca
 * 
 * Używane przy modyfikacji rezerwacji, gdy pasażer zmienia miejsce
 * 
 * @param newSeat Nowy numer miejsca
 */
void Ticket::setSeatNumber(int newSeat) {
	seatNumber = newSeat;
}

/**
 * @brief Konwertuje dane biletu na sformatowany tekst
 * 
 * Tworzy czytelną reprezentację biletu zawierającą wszystkie informacje:
 * - Numer biletu
 * - ID pociągu
 * - Numer miejsca
 * - Cenę (sformatowaną do 2 miejsc po przecinku)
 * - Login pasażera
 * 
 * @return Tekstowa reprezentacja biletu
 */
std::string Ticket::toString() const {

	std::stringstream text;
	text << std::fixed << std::setprecision(2);

	text << "Bilet #" << FticketId << " | Pociag ID: " << trainId << " | Miejsce " << seatNumber << " | Cena: " << Fprice << " PLN" << " | Pasazer: " << passengerLogin;

	return text.str();
}