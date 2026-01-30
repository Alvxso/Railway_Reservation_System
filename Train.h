/**
 * Klasa Train - Reprezentuje pociąg w systemie
 * 
 * Przechowuje informacje o:
 * - Trasie pociągu (stacja początkowa i końcowa)
 * - Dacie i godzinie odjazdu
 * - Liczbie miejsc i ich dostępności
 * - Rezerwacjach miejsc
 */

#pragma once
#include <string>
#include <vector>

class Train
{
private:
	int Fid;
	std::string Forigin;
	std::string Fdestination;
	std::string Fdate;
	int Fcapacity;
	std::vector<bool> Fseats;

public:
	Train(int id, std::string Forigin, std::string Fdestination, std::string Fdate, int Fcapacity);

	int getID() const;
	std::string getOrigin() const;
	std::string getDestination() const;
	std::string getDate() const;
	int getCapacity() const;

	bool isSeatFree(int seatNumber) const;
	bool reserveSeat(int seatNumber);
	void cancelSeat(int seatNumber);

	int getOccupiedSeatsCount() const;

	void displaySeats() const;
};

