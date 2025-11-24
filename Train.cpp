#include "Train.h"
#include <iostream>
#include <iomanip>

Train::Train(int id, std::string origin, std::string destination, std::string date, int capacity)
	: Fid(id), Forigin(origin), Fdestination(destination), Fdate(date), Fcapacity(capacity) {

	Fseats.resize(capacity + 1, false);
}

int Train::getID() const { return Fid; }
std::string Train::getOrigin() const { return Forigin; }
std::string Train::getDestination() const { return Fdestination; }
std::string Train::getDate() const { return Fdate; }
int Train::getCapacity() const { return Fcapacity; }

bool Train::isSeatFree(int seatNumber) const {
	if (seatNumber < 1 || seatNumber > Fcapacity) return false;
	return !Fseats[seatNumber];
}

bool Train::reserveSeat(int seatNumber) {
	if (!isSeatFree(seatNumber)) {
		return false;
	}
	Fseats[seatNumber] = true;
	return true;
}

void Train::cancelSeat(int seatNumber) {
	if (seatNumber >= 1 && seatNumber <= Fcapacity) {
		Fseats[seatNumber] = false;
	}
}

int Train::getOccupiedSeatsCount() const {
	int count = 0;
	for (size_t i = 1; i < Fseats.size(); i++) {
		if (Fseats[i]) count++;
	}
	return count;
}

void Train::displaySeats() const {
	std::cout << "\n=== MAPA MIEJSC (Pociag ID: " << Fid << "Relacji: " << Forigin << " --> " << Fdestination << " ===" << std::endl;
	std::cout << "Legenda: [ NR ] = Wolne, [ X ] = Zajete\n" << std::endl;

	for (auto i = 1; i <= Fcapacity; i++) {

		if (Fseats[i]) std::cout << "[  X ] ";
		else std::cout << "[ " << std::setw(2) << i << " ] ";

		if (i % 4 == 0) std::cout << "\n";
	}
	std::cout << "\n=============================================================" << std::endl;
}