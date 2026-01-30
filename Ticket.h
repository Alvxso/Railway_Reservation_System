/**
 * @file Ticket.h
 * @brief Deklaracja klasy Ticket - reprezentuje bilet w systemie rezerwacji
 */

#pragma once
#include <string>

/**
 * @class Ticket
 * @brief Klasa reprezentująca bilet w systemie rezerwacji kolejowej
 * 
 * Przechowuje informacje o bilecie:
 * - ID biletu i pociągu
 * - Login pasażera
 * - Numer zarezerwowanego miejsca
 * - Cenę biletu
 */
class Ticket
{
private:
	int FticketId;                ///< Unikalny identyfikator biletu
	int trainId;                  ///< ID pociągu, na który wystawiony jest bilet
	std::string passengerLogin;   ///< Login pasażera będącego właścicielem biletu
	int seatNumber;               ///< Numer zarezerwowanego miejsca
	double Fprice;                ///< Cena biletu w PLN

public:
	/**
	 * @brief Konstruktor klasy Ticket
	 * @param id Unikalny identyfikator biletu
	 * @param tID ID pociągu
	 * @param login Login pasażera
	 * @param seat Numer miejsca
	 * @param price Cena biletu
	 */
	Ticket(int id, int tID, std::string login, int seat, double price);

	/**
	 * @brief Zwraca ID biletu
	 * @return Unikalny identyfikator biletu
	 */
	int getTicketId() const;
	
	/**
	 * @brief Zwraca ID pociągu
	 * @return ID pociągu, na który wystawiony jest bilet
	 */
	int getTrainId() const;
	
	/**
	 * @brief Zwraca login pasażera
	 * @return Login właściciela biletu
	 */
	std::string getPassengerLogin() const;
	
	/**
	 * @brief Zwraca numer miejsca
	 * @return Numer zarezerwowanego miejsca
	 */
	int getSeatNumber() const;
	
	/**
	 * @brief Zwraca cenę biletu
	 * @return Cena biletu w PLN
	 */
	double getPrice() const;

	/**
	 * @brief Ustawia nowy numer miejsca (używane przy modyfikacji rezerwacji)
	 * @param newSeat Nowy numer miejsca
	 */
	void setSeatNumber(int newSeat);

	/**
	 * @brief Konwertuje dane biletu na sformatowany tekst
	 * @return Tekstowa reprezentacja biletu ze wszystkimi szczegółami
	 */
	std::string toString() const;
};