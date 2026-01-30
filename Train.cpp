/**
 * @file Train.cpp
 * @brief Implementacja klasy Train - zarządzanie pociągami i miejscami
 */

#include "Train.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Konstruktor klasy Train - inicjalizuje pociąg
 * 
 * Tworzy pociąg z podanymi parametrami i inicjalizuje wszystkie miejsca jako wolne.
 * Wektor miejsc ma rozmiar capacity+1 (indeks 0 nie jest używany dla wygody)
 * 
 * @param id Unikalny identyfikator pociągu
 * @param origin Stacja początkowa
 * @param destination Stacja końcowa
 * @param date Data kursu
 * @param capacity Liczba miejsc w pociągu
 */
Train::Train(int id, std::string origin, std::string destination, std::string date, int capacity)
	: Fid(id), Forigin(origin), Fdestination(destination), Fdate(date), Fcapacity(capacity) {

	// Inicjalizacja wektora miejsc - wszystkie miejsca wolne
	// Rozmiar capacity+1 aby numery miejsc zaczynały się od 1
	Fseats.resize(capacity + 1, false);
}

// Gettery - zwracają podstawowe informacje o pociągu
int Train::getID() const { return Fid; }
std::string Train::getOrigin() const { return Forigin; }
std::string Train::getDestination() const { return Fdestination; }
std::string Train::getDate() const { return Fdate; }
int Train::getCapacity() const { return Fcapacity; }

/**
 * @brief Sprawdza czy dane miejsce jest wolne
 * 
 * Weryfikuje czy numer miejsca jest prawidłowy (1 do capacity)
 * oraz czy miejsce nie jest już zarezerwowane
 * 
 * @param seatNumber Numer miejsca do sprawdzenia (1-indexed)
 * @return true jeśli miejsce jest wolne, false w przeciwnym razie
 */
bool Train::isSeatFree(int seatNumber) const {
	if (seatNumber < 1 || seatNumber > Fcapacity) return false;
	return !Fseats[seatNumber];
}

/**
 * @brief Rezerwuje wybrane miejsce w pociągu
 * 
 * Najpierw sprawdza czy miejsce jest wolne, następnie oznacza je jako zajęte
 * 
 * @param seatNumber Numer miejsca do zarezerwowania
 * @return true jeśli rezerwacja się powiodła, false jeśli miejsce już zajęte
 */
bool Train::reserveSeat(int seatNumber) {
	if (!isSeatFree(seatNumber)) {
		return false;
	}
	Fseats[seatNumber] = true;
	return true;
}

/**
 * @brief Anuluje rezerwację wybranego miejsca (zwalnia miejsce)
 * 
 * Sprawdza czy numer miejsca jest prawidłowy i oznacza miejsce jako wolne
 * 
 * @param seatNumber Numer miejsca do zwolnienia
 */
void Train::cancelSeat(int seatNumber) {
	if (seatNumber >= 1 && seatNumber <= Fcapacity) {
		Fseats[seatNumber] = false;
	}
}

/**
 * @brief Oblicza liczbę zajętych miejsc w pociągu
 * 
 * Iteruje przez wszystkie miejsca i zlicza te zarezerwowane
 * 
 * @return Liczba zarezerwowanych miejsc
 */
int Train::getOccupiedSeatsCount() const {
	int count = 0;
	for (size_t i = 1; i < Fseats.size(); i++) {
		if (Fseats[i]) count++;
	}
	return count;
}

/**
 * @brief Wyświetla wizualną mapę zajętości miejsc w pociągu
 * 
 * Pokazuje wszystkie miejsca w układzie 4 miejsca na rząd:
 * - Wolne miejsca: [ NR ] gdzie NR to numer miejsca
 * - Zajęte miejsca: [ X ]
 */
void Train::displaySeats() const {
	std::cout << "\n=== MAPA MIEJSC (Pociag ID: " << Fid << "Relacji: " << Forigin << " --> " << Fdestination << " ===" << std::endl;
	std::cout << "Legenda: [ NR ] = Wolne, [ X ] = Zajete\n" << std::endl;

	// Wyświetlanie miejsc w układzie 4 na rząd
	for (auto i = 1; i <= Fcapacity; i++) {

		if (Fseats[i]) std::cout << "[  X ] ";
		else std::cout << "[ " << std::setw(2) << i << " ] ";

		// Przejście do nowej linii co 4 miejsca
		if (i % 4 == 0) std::cout << "\n";
	}
	std::cout << "\n=============================================================" << std::endl;
}