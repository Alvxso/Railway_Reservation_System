/**
 * @file Train.h
 * @brief Klasa reprezentująca pociąg w systemie
 */

#pragma once
#include <string>
#include <vector>

/**
 * @class Train
 * @brief Klasa reprezentująca pociąg z informacjami o trasie i miejscach
 */
class Train
{
private:
	int Fid;                      // Unikalny identyfikator pociągu
	std::string Forigin;          // Stacja początkowa
	std::string Fdestination;     // Stacja docelowa
	std::string Fdate;            // Data odjazdu
	int Fcapacity;                // Maksymalna liczba miejsc
	std::vector<bool> Fseats;     // Wektor reprezentujący zajętość miejsc (true = zajęte)

public:
	// Konstruktor tworzący nowy pociąg
	Train(int id, std::string Forigin, std::string Fdestination, std::string Fdate, int Fcapacity);

	// Gettery zwracające informacje o pociągu
	int getID() const;
	std::string getOrigin() const;
	std::string getDestination() const;
	std::string getDate() const;
	int getCapacity() const;

	// Sprawdza czy dane miejsce jest wolne
	bool isSeatFree(int seatNumber) const;
	// Rezerwuje miejsce (zwraca true jeśli się udało)
	bool reserveSeat(int seatNumber);
	// Anuluje rezerwację miejsca
	void cancelSeat(int seatNumber);

	// Zwraca liczbę zajętych miejsc
	int getOccupiedSeatsCount() const;

	// Wyświetla mapę miejsc w pociągu
	void displaySeats() const;
};

