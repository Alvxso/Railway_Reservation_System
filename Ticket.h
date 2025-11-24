#pragma once
#include <string>

class Ticket
{
private:
	int FticketId;
	int trainId;
	std::string passengerLogin;
	int seatNumber;
	double Fprice;
	
public:
	Ticket(int id, int tID, std::string login, int seat, double price);

	int getTicketId() const;
	int getTrainId() const;
	std::string getPassengerLogin() const;
	int getSeatNumber() const;
	double getPrice() const;

	std::string toString() const;
};

