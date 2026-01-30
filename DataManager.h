/**
 * @file DataManager.h
 * @brief Deklaracja klasy DataManager - zarządzanie trwałością danych
 */

#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"
#include "Admin.h"
#include "Passenger.h"
#include "Train.h"
#include "Ticket.h"

/**
 * @class DataManager
 * @brief Klasa odpowiedzialna za zapisywanie i wczytywanie danych z plików YAML
 * 
 * DataManager obsługuje trwałość danych systemu rezerwacji:
 * - Zapisuje i wczytuje użytkowników (Admin i Passenger)
 * - Zapisuje i wczytuje pociągi wraz ze stanem zajętości miejsc
 * - Zapisuje i wczytuje bilety
 * 
 * Dane przechowywane są w plikach YAML dla łatwej edycji i przeglądania
 */
class DataManager
{
private:
	const std::string FusersFile = "users.yaml";      ///< Nazwa pliku z użytkownikami
	const std::string FtrainsFile = "trains.yaml";    ///< Nazwa pliku z pociągami
	const std::string FticketsFile = "tickets.yaml";  ///< Nazwa pliku z biletami

	/**
	 * @brief Usuwa białe znaki z początku i końca tekstu
	 * @param str Tekst do obróbki
	 * @return Tekst bez białych znaków na końcach
	 */
	std::string trim(const std::string& str);

public:
	/**
	 * @brief Konstruktor klasy DataManager
	 */
	DataManager();

	/**
	 * @brief Zapisuje wszystkich użytkowników do pliku YAML
	 * @param users Wektor użytkowników do zapisania
	 */
	void saveUsers(const std::vector < std::unique_ptr<User>>& users);
	
	/**
	 * @brief Wczytuje użytkowników z pliku YAML
	 * @param users Wektor, do którego zostaną wczytani użytkownicy
	 */
	void loadUsers(std::vector<std::unique_ptr<User>>& users);

	/**
	 * @brief Zapisuje wszystkie pociągi do pliku YAML
	 * 
	 * Zapisuje pełną informację o pociągach wraz ze stanem zajętości miejsc
	 * 
	 * @param trains Wektor pociągów do zapisania
	 */
	void saveTrains(const std::vector<Train>& trains);
	
	/**
	 * @brief Wczytuje pociągi z pliku YAML
	 * 
	 * Odtwarza pociągi wraz z ich stanem zajętości miejsc
	 * 
	 * @param trains Wektor, do którego zostaną wczytane pociągi
	 */
	void loadTrains(std::vector<Train>& trains);

	/**
	 * @brief Zapisuje wszystkie bilety do pliku YAML
	 * @param tickets Wektor biletów do zapisania
	 */
	void saveTickets(const std::vector<Ticket>& tickets);
	
	/**
	 * @brief Wczytuje bilety z pliku YAML
	 * @param tickets Wektor, do którego zostaną wczytane bilety
	 */
	void loadTickets(std::vector<Ticket>& tickets);
};

