/**
 * @file Passenger.h
 * @brief Deklaracja klasy Passenger - pasażer w systemie rezerwacji kolejowej
 */

#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <string>

/**
 * @class Passenger
 * @brief Klasa reprezentująca pasażera w systemie
 * 
 * Pasażer może przeglądać dostępne pociągi, rezerwować bilety,
 * przeglądać swoje rezerwacje, anulować je oraz modyfikować.
 * Dziedziczy po klasie User.
 */
class Passenger : public User
{
public:
	/**
	 * @brief Konstruktor klasy Passenger
	 * @param login Login pasażera
	 * @param password Hasło pasażera
	 * @param id Unikalny identyfikator pasażera
	 */
	Passenger(std::string login, std::string password, int id);

	/**
	 * @brief Wyświetla menu dostępne dla pasażera
	 */
	void showMenu() override;
	
	/**
	 * @brief Zwraca typ użytkownika
	 * @return Zawsze zwraca "PASSENGER"
	 */
	std::string getType() const override;

	/**
	 * @brief Wyszukuje i rezerwuje bilet na wybrany pociąg
	 * 
	 * Funkcja oferuje różne opcje wyszukiwania pociągów (po stacji, dacie),
	 * wyświetla dostępne miejsca i przeprowadza przez proces rezerwacji
	 * 
	 * @param trains Wektor pociągów dostępnych w systemie
	 * @param tickets Wektor biletów - nowy bilet zostanie do niego dodany
	 */
	void bookTicket(std::vector<Train>& trains, std::vector<Ticket>& tickets);
	
	/**
	 * @brief Wyświetla wszystkie aktywne rezerwacje pasażera
	 * @param tickets Wektor wszystkich biletów w systemie
	 */
	void listMyTickets(const std::vector<Ticket>& tickets) const;

	/**
	 * @brief Anuluje wybraną rezerwację pasażera
	 * 
	 * Funkcja usuwa bilet z systemu i zwalnia miejsce w pociągu
	 * 
	 * @param trains Wektor pociągów - miejsce zostanie zwolnione
	 * @param tickets Wektor biletów - bilet zostanie usunięty
	 */
	void cancelBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets);
	
	/**
	 * @brief Modyfikuje wybraną rezerwację pasażera
	 * 
	 * Pasażer może zmienić numer miejsca lub całkowicie przebookować na inny pociąg
	 * 
	 * @param trains Wektor pociągów
	 * @param tickets Wektor biletów
	 */
	void modifyBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets);
};