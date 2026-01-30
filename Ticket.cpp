#include "Ticket.h"
#include <sstream>
#include <iomanip>

Ticket::Ticket(int id, int tID, std::string login, int seat, double price) : 
	FticketId(id), trainId(tID), passengerLogin(login), seatNumber(seat), Fprice(price) {}

int Ticket::getTicketId() const { return FticketId; }
int Ticket::getTrainId() const { return trainId; }
std::string Ticket::getPassengerLogin() const { return passengerLogin; }
int Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getPrice() const { return Fprice; }

void Ticket::setSeatNumber(int newSeat) {
	seatNumber = newSeat;
}

std::string Ticket::toString() const {

	std::stringstream text;
	text << std::fixed << std::setprecision(2);

	text << "Bilet #" << FticketId << " | Pociag ID: " << trainId << " | Miejsce " << seatNumber << " | Cena: " << Fprice << " PLN" << " | Pasazer: " << passengerLogin;

	return text.str();
}