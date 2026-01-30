/**
 * @file Admin.cpp
 * @brief Implementacja funkcjonalności administratora systemu rezerwacji kolejowej
 */

#include "Admin.h"
#include "InputValidation.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype> 

/**
 * @brief Konstruktor klasy Admin - inicjalizuje administratora
 * @param login Login administratora
 * @param password Hasło administratora  
 * @param id Unikalny identyfikator administratora
 */
Admin::Admin(std::string login, std::string password, int id)
	: User(login, password, id) {
}

/**
 * @brief Zwraca typ użytkownika
 * @return Zawsze zwraca "ADMIN"
 */
std::string Admin::getType() const { return "ADMIN"; }

/**
 * @brief Wyświetla menu administracyjne z dostępnymi opcjami
 */
void Admin::showMenu() {
	std::cout << "\n=== PANEL ADMINA: " << get_Flogin() << " ===" << std::endl;
	std::cout << "1. Dodaj nowy pociag" << std::endl;
	std::cout << "2. Usun pociag" << std::endl;
	std::cout << "3. Generuj raport systemowy" << std::endl;
	std::cout << "4. Wroc do menu glownego (Wyloguj)" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
}

/**
 * @brief Formatuje nazwę miasta - pierwsza litera wielka, pozostałe małe
 * @param str Nazwa miasta do sformatowania
 * @return Sformatowana nazwa miasta
 */
std::string formatCity(std::string str) {
	if (str.empty()) return str;
	str[0] = std::toupper(str[0]);
	for (size_t i = 1; i < str.length(); i++) {
		str[i] = std::tolower(str[i]);
	}
	return str;
}

/**
 * @brief Dodaje nowy pociąg do systemu
 * 
 * Funkcja przeprowadza użytkownika przez proces tworzenia nowego pociągu:
 * - Sprawdza unikalność ID pociągu
 * - Pobiera dane: stację początkową, końcową, datę i liczbę miejsc
 * - Automatycznie formatuje nazwy stacji
 * - Ogranicza liczbę miejsc do maksymalnie 100
 * 
 * @param trains Wektor pociągów, do którego zostanie dodany nowy pociąg
 */
void Admin::addTrain(std::vector<Train>& trains) {
	std::cout << "\n--- KREATOR POCIAGU ---\n";
	int id, capacity;
	std::string origin, destination, date;

	// Zapewnienie unikalności ID pociągu
	while (true) {
		readInt("Podaj unikalne ID pociagu: ", id);
		auto it = std::find_if(trains.begin(), trains.end(),
			[id](const Train& t) { return t.getID() == id; });
		if (it == trains.end()) break;
		std::cout << "Blad: Pociag o ID " << id << " juz istnieje!\n";
	}

	// Wczytanie i formatowanie nazw stacji
	readLine("Stacja poczatkowa: ", origin);
	origin = formatCity(origin); 

	readLine("Stacja koncowa: ", destination);
	destination = formatCity(destination); 
	readLine("Data (RRRR-MM-DD): ", date);

	// Wczytanie i walidacja liczby miejsc
	readInt("Liczba miejsc (max 100): ", capacity);
	if (capacity > 100) capacity = 100;
	if (capacity < 1) capacity = 1;

	// Utworzenie nowego pociągu
	trains.emplace_back(id, origin, destination, date, capacity);
	std::cout << "SUKCES: Pociag relacji " << origin << " -> " << destination << " zostal dodany.\n";
}

/**
 * @brief Usuwa pociąg z systemu i anuluje wszystkie związane z nim bilety
 * 
 * Funkcja:
 * - Wyszukuje pociąg o podanym ID
 * - Usuwa wszystkie bilety związane z tym pociągiem
 * - Usuwa pociąg z systemu
 * - Informuje o liczbie anulowanych biletów
 * 
 * @param trains Wektor pociągów
 * @param tickets Wektor biletów - wszystkie bilety dla usuwanego pociągu zostaną automatycznie anulowane
 */
void Admin::removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- USUWANIE POCIAGU ---\n";
	if (trains.empty()) {
		std::cout << "Baza pociagow jest pusta.\n";
		return;
	}

	int idToRemove;
	readInt("Podaj ID pociagu do usuniecia: ", idToRemove);

	// Wyszukanie pociągu
	auto it = std::find_if(trains.begin(), trains.end(),
		[idToRemove](const Train& t) { return t.getID() == idToRemove; });

	if (it == trains.end()) {
		std::cout << "BLAD: Nie znaleziono pociagu o takim ID.\n";
		return;
	}

	// Usunięcie wszystkich biletów związanych z pociągiem
	auto initialTicketCount = tickets.size();
	tickets.erase(
		std::remove_if(tickets.begin(), tickets.end(),
			[idToRemove](const Ticket& t) { return t.getTrainId() == idToRemove; }),
		tickets.end()
	);
	int deletedTickets = initialTicketCount - tickets.size();

	// Usunięcie pociągu
	trains.erase(it);

	std::cout << "SUKCES: Usunieto pociag ID " << idToRemove << ".\n";
	if (deletedTickets > 0) {
		std::cout << "UWAGA: Anulowano rowniez " << deletedTickets << " biletow.\n";
	}
}

/**
 * @brief Generuje i wyświetla szczegółowy raport systemowy
 * 
 * Raport zawiera:
 * - Liczbę zarejestrowanych użytkowników
 * - Liczbę pociągów w systemie
 * - Liczbę sprzedanych biletów
 * - Całkowity przychód ze sprzedaży biletów
 * 
 * @param trains Wektor wszystkich pociągów w systemie
 * @param tickets Wektor wszystkich biletów
 * @param users Wektor wszystkich użytkowników
 */
void Admin::generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users) {
	// Obliczenie całkowitego przychodu
	double totalRevenue = 0.0;
	for (const auto& t : tickets) {
		totalRevenue += t.getPrice();
	}
	
	// Wyświetlenie raportu
	std::cout << "\n================ RAPORT SYSTEMOWY ================\n";
	std::cout << "Liczba uzytkownikow: " << users.size() << "\n";
	std::cout << "Liczba pociagow: " << trains.size() << "\n";
	std::cout << "Liczba sprzedanych biletow: " << tickets.size() << "\n";
	std::cout << "Calkowity przychod z biletow: " << std::fixed << std::setprecision(2) << totalRevenue << " PLN\n";
	std::cout << "==================================================\n";
}