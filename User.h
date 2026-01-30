/**
 * @file User.h
 * @brief Bazowa klasa abstrakcyjna dla wszystkich użytkowników systemu rezerwacji kolejowej
 */

#pragma once

#include <string>
#include <iostream>

/**
 * @class User
 * @brief Abstrakcyjna klasa bazowa reprezentująca użytkownika systemu
 * 
 * Klasa User jest podstawą dla wszystkich typów użytkowników w systemie.
 * Zawiera podstawowe informacje o użytkowniku takie jak login, hasło i ID.
 * Dziedziczy po niej klasy Admin i Passenger.
 */
class User
{
private:
	std::string Flogin;      ///< Login użytkownika
	std::string Fpassword;   ///< Hasło użytkownika
	int Fid;                 ///< Unikalny identyfikator użytkownika
protected:
	/**
	 * @brief Konstruktor chroniony - klasa może być tworzona tylko przez klasy dziedziczące
	 * @param login Login użytkownika
	 * @param password Hasło użytkownika
	 * @param id Unikalny identyfikator użytkownika
	 */
	User(std::string login, std::string password, int id) : Flogin(login), Fpassword(password), Fid(id) {}
public:
	/**
	 * @brief Wirtualny destruktor - pozwala na poprawne usuwanie obiektów klas pochodnych
	 */
	virtual ~User() = default;

	/**
	 * @brief Czysto wirtualna metoda wyświetlająca menu dla danego typu użytkownika
	 */
	virtual void showMenu() = 0;

	/**
	 * @brief Czysto wirtualna metoda zwracająca typ użytkownika
	 * @return Typ użytkownika jako tekst (np. "ADMIN", "PASSENGER")
	 */
	virtual std::string getType() const = 0;

	/**
	 * @brief Getter dla loginu użytkownika
	 * @return Login użytkownika
	 */
	std::string get_Flogin() const {return Flogin;}
	
	/**
	 * @brief Getter dla hasła użytkownika
	 * @return Hasło użytkownika
	 */
	std::string get_Fpassword() const { return Fpassword; }
	
	/**
	 * @brief Getter dla ID użytkownika
	 * @return Unikalny identyfikator użytkownika
	 */
	int get_Fid() const { return Fid; }
};

