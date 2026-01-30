/**
 * @file Admin.h
 * @brief Deklaracja klasy Admin - administrator systemu rezerwacji kolejowej
 */

#pragma once
#include "User.h"
#include "Train.h"
#include "Ticket.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class Admin
 * @brief Klasa reprezentująca administratora systemu
 * 
 * Administrator ma uprawnienia do zarządzania pociągami (dodawanie, usuwanie)
 * oraz generowania raportów systemowych. Dziedziczy po klasie User.
 */
class Admin : public User
{
public:
	/**
	 * @brief Konstruktor klasy Admin
	 * @param login Login administratora
	 * @param password Hasło administratora
	 * @param id Unikalny identyfikator administratora
	 */
	Admin(std::string login, std::string password, int id);

	/**
	 * @brief Wyświetla menu dostępne dla administratora
	 */
	void showMenu() override;
	
	/**
	 * @brief Zwraca typ użytkownika
	 * @return Zawsze zwraca "ADMIN"
	 */
	std::string getType() const override;

	/**
	 * @brief Dodaje nowy pociąg do systemu
	 * @param trains Wektor pociągów, do którego zostanie dodany nowy pociąg
	 */
	void addTrain(std::vector<Train>& trains);

	/**
	 * @brief Usuwa pociąg z systemu oraz anuluje wszystkie związane z nim bilety
	 * @param trains Wektor pociągów
	 * @param tickets Wektor biletów - wszystkie bilety dla usuwanego pociągu zostaną anulowane
	 */
	void removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets);

	/**
	 * @brief Generuje szczegółowy raport systemowy
	 * @param trains Wektor pociągów w systemie
	 * @param tickets Wektor wszystkich biletów
	 * @param users Wektor wszystkich użytkowników systemu
	 */
	void generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users);
};