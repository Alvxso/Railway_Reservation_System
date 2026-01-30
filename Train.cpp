/**
 * @file Train.cpp
 * @brief Implementacja funkcjonalności pociągu
 */

#include "Train.h"
#include <iostream>
#include <iomanip>

// Konstruktor inicjalizujący pociąg z podanymi parametrami
Train::Train(int id, std::string origin, std::string destination, std::string date, int capacity)
	: Fid(id), Forigin(origin), Fdestination(destination), Fdate(date), Fcapacity(capacity) {

	// Tworzenie wektora miejsc, indeksy 1-capacity (indeks 0 nie jest używany)
	Fseats.resize(capacity + 1, false);
}

// Gettery - zwracają wartości pól prywatnych
int Train::getID() const { return Fid; }
std::string Train::getOrigin() const { return Forigin; }
std::string Train::getDestination() const { return Fdestination; }
std::string Train::getDate() const { return Fdate; }
int Train::getCapacity() const { return Fcapacity; }

// Sprawdza czy miejsce o podanym numerze jest wolne
bool Train::isSeatFree(int seatNumber) const {
	// Sprawdź czy numer miejsca jest w poprawnym zakresie
	if (seatNumber < 1 || seatNumber > Fcapacity) return false;
	return !Fseats[seatNumber];  // Zwróć true jeśli miejsce nie jest zajęte (false w wektorze)
}

// Próbuje zarezerwować miejsce
bool Train::reserveSeat(int seatNumber) {
	// Sprawdź czy miejsce jest wolne
	if (!isSeatFree(seatNumber)) {
		return false;  // Miejsce zajęte lub niepoprawny numer
	}
	Fseats[seatNumber] = true;  // Oznacz miejsce jako zajęte
	return true;  // Rezerwacja udana
}

// Anuluje rezerwację miejsca
void Train::cancelSeat(int seatNumber) {
	// Sprawdź czy numer miejsca jest poprawny
	if (seatNumber >= 1 && seatNumber <= Fcapacity) {
		Fseats[seatNumber] = false;  // Oznacz miejsce jako wolne
	}
}

// Liczy ile miejsc jest zajętych
int Train::getOccupiedSeatsCount() const {
	int count = 0;
	// Przejdź przez wszystkie miejsca (zaczynamy od 1)
	for (size_t i = 1; i < Fseats.size(); i++) {
		if (Fseats[i]) count++;  // Jeśli miejsce zajęte, zwiększ licznik
	}
	return count;
}

// Wyświetla wizualną mapę miejsc w pociągu
void Train::displaySeats() const {
	std::cout << "\n=== MAPA MIEJSC (Pociag ID: " << Fid << " Relacji: " << Forigin << " --> " << Fdestination << " ===" << std::endl;
	std::cout << "Legenda: [ NR ] = Wolne, [ X ] = Zajete\n" << std::endl;

	// Wyświetl wszystkie miejsca w formacie 4 w wierszu
	for (auto i = 1; i <= Fcapacity; i++) {

		if (Fseats[i]) std::cout << "[  X ] ";  // Miejsce zajęte
		else std::cout << "[ " << std::setw(2) << i << " ] ";  // Miejsce wolne z numerem

		if (i % 4 == 0) std::cout << "\n";  // Nowy wiersz co 4 miejsca
	}
	std::cout << "\n=============================================================" << std::endl;
}