/**
 * @file Train.h
 * @brief Deklaracja klasy Train - reprezentuje pociąg w systemie rezerwacji
 */

#pragma once
#include <string>
#include <vector>

/**
 * @class Train
 * @brief Klasa reprezentująca pociąg w systemie rezerwacji kolejowej
 * 
 * Klasa Train przechowuje informacje o połączeniu kolejowym:
 * - ID pociągu, stacje początkową i końcową, datę kursu
 * - Pojemność (liczba miejsc) oraz stan zajętości miejsc
 * Obsługuje rezerwację i anulowanie miejsc oraz wyświetlanie mapy miejsc
 */
class Train
{
private:
	int Fid;                      ///< Unikalny identyfikator pociągu
	std::string Forigin;          ///< Stacja początkowa
	std::string Fdestination;     ///< Stacja końcowa
	std::string Fdate;            ///< Data kursu (format RRRR-MM-DD)
	int Fcapacity;                ///< Całkowita liczba miejsc w pociągu
	std::vector<bool> Fseats;     ///< Tablica zajętości miejsc (true = zajęte, false = wolne)

public:
	/**
	 * @brief Konstruktor klasy Train
	 * @param id Unikalny identyfikator pociągu
	 * @param Forigin Stacja początkowa
	 * @param Fdestination Stacja końcowa
	 * @param Fdate Data kursu
	 * @param Fcapacity Liczba miejsc w pociągu
	 */
	Train(int id, std::string Forigin, std::string Fdestination, std::string Fdate, int Fcapacity);

	/**
	 * @brief Zwraca ID pociągu
	 * @return Unikalny identyfikator pociągu
	 */
	int getID() const;
	
	/**
	 * @brief Zwraca nazwę stacji początkowej
	 * @return Nazwa stacji początkowej
	 */
	std::string getOrigin() const;
	
	/**
	 * @brief Zwraca nazwę stacji końcowej
	 * @return Nazwa stacji końcowej
	 */
	std::string getDestination() const;
	
	/**
	 * @brief Zwraca datę kursu
	 * @return Data w formacie RRRR-MM-DD
	 */
	std::string getDate() const;
	
	/**
	 * @brief Zwraca całkowitą liczbę miejsc w pociągu
	 * @return Pojemność pociągu
	 */
	int getCapacity() const;

	/**
	 * @brief Sprawdza czy dane miejsce jest wolne
	 * @param seatNumber Numer miejsca do sprawdzenia
	 * @return true jeśli miejsce jest wolne, false jeśli zajęte lub numer nieprawidłowy
	 */
	bool isSeatFree(int seatNumber) const;
	
	/**
	 * @brief Rezerwuje wybrane miejsce w pociągu
	 * @param seatNumber Numer miejsca do zarezerwowania
	 * @return true jeśli rezerwacja powiodła się, false jeśli miejsce było zajęte
	 */
	bool reserveSeat(int seatNumber);
	
	/**
	 * @brief Anuluje rezerwację wybranego miejsca
	 * @param seatNumber Numer miejsca do zwolnienia
	 */
	void cancelSeat(int seatNumber);

	/**
	 * @brief Zwraca liczbę zajętych miejsc w pociągu
	 * @return Liczba zarezerwowanych miejsc
	 */
	int getOccupiedSeatsCount() const;

	/**
	 * @brief Wyświetla wizualną mapę zajętości miejsc w pociągu
	 * 
	 * Pokazuje stan wszystkich miejsc - wolne z numerami, zajęte oznaczone 'X'
	 */
	void displaySeats() const;
};

