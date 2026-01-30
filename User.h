/**
 * @file User.h
 * @brief Abstrakcyjna klasa bazowa reprezentująca użytkownika systemu
 */

#pragma once

#include <string>
#include <iostream>

/**
 * @class User
 * @brief Bazowa klasa dla wszystkich typów użytkowników (Admin, Passenger)
 */
class User
{
private:
	std::string Flogin;     // Login użytkownika
	std::string Fpassword;  // Hasło użytkownika
	int Fid;                // Unikalny identyfikator użytkownika
protected:
	// Konstruktor chroniony - tylko klasy pochodne mogą go użyć
	User(std::string login, std::string password, int id) : Flogin(login), Fpassword(password), Fid(id) {}
public:
	// Wirtualny destruktor dla poprawnego dziedziczenia
	virtual ~User() = default;

	// Czysto wirtualna metoda wyświetlająca menu - implementowana w klasach pochodnych
	virtual void showMenu() = 0;

	// Czysto wirtualna metoda zwracająca typ użytkownika
	virtual std::string getType() const = 0;

	// Getter zwracający login użytkownika
	std::string get_Flogin() const {return Flogin;}
	// Getter zwracający hasło użytkownika
	std::string get_Fpassword() const { return Fpassword; }
	// Getter zwracający ID użytkownika
	int get_Fid() const { return Fid; }
};

