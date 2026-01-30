/**
 * @file Admin.cpp
 * @brief Implementacja funkcjonalności administratora systemu
 */

#include "Admin.h"
#include "InputValidation.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype> 

// Konstruktor wywołuje konstruktor klasy bazowej User
Admin::Admin(std::string login, std::string password, int id)
	: User(login, password, id) {
}

// Zwraca typ użytkownika jako "ADMIN"
std::string Admin::getType() const { return "ADMIN"; }

// Wyświetla menu opcji dostępnych dla administratora
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
	if (str.empty()) return str;  // Jeśli pusty ciąg, zwróć bez zmian
	str[0] = std::toupper(str[0]);  // Pierwsza litera na wielką
	for (size_t i = 1; i < str.length(); i++) {  // Pozostałe litery na małe
		str[i] = std::tolower(str[i]);
	}
	return str;  // Zwróć sformatowany ciąg
}

// Dodaje nowy pociąg do systemu
void Admin::addTrain(std::vector<Train>& trains) {
	std::cout << "\n--- KREATOR POCIAGU ---\n";
	int id, capacity;
	std::string origin, destination, date;

	// Pętla sprawdzająca unikalność ID pociągu
	while (true) {
		readInt("Podaj unikalne ID pociagu: ", id);  // Wczytaj ID od użytkownika
		// Szukaj pociągu o takim ID w istniejących pociągach
		auto it = std::find_if(trains.begin(), trains.end(),
			[id](const Train& t) { return t.getID() == id; });
		if (it == trains.end()) break;  // Jeśli nie znaleziono, ID jest unikalne
		std::cout << "Blad: Pociag o ID " << id << " juz istnieje!\n";
	}

	// Wczytaj stację początkową i sformatuj nazwę
	readLine("Stacja poczatkowa: ", origin);
	origin = formatCity(origin); 

	// Wczytaj stację końcową i sformatuj nazwę
	readLine("Stacja koncowa: ", destination);
	destination = formatCity(destination); 
	// Wczytaj datę odjazdu
	readLine("Data (RRRR-MM-DD): ", date);

	// Wczytaj liczbę miejsc i ogranicz do 1-100
	readInt("Liczba miejsc (max 100): ", capacity);
	if (capacity > 100) capacity = 100;  // Nie więcej niż 100 miejsc
	if (capacity < 1) capacity = 1;      // Co najmniej 1 miejsce

	// Dodaj nowy pociąg do wektora
	trains.emplace_back(id, origin, destination, date, capacity);
	std::cout << "SUKCES: Pociag relacji " << origin << " -> " << destination << " zostal dodany.\n";
}

// Usuwa pociąg oraz wszystkie bilety powiązane z tym pociągiem
void Admin::removeTrain(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- USUWANIE POCIAGU ---\n";
	// Sprawdź czy są jakieś pociągi w bazie
	if (trains.empty()) {
		std::cout << "Baza pociagow jest pusta.\n";
		return;
	}

	int idToRemove;
	readInt("Podaj ID pociagu do usuniecia: ", idToRemove);  // Wczytaj ID pociągu do usunięcia

	// Szukaj pociągu o podanym ID
	auto it = std::find_if(trains.begin(), trains.end(),
		[idToRemove](const Train& t) { return t.getID() == idToRemove; });

	// Jeśli nie znaleziono pociągu, wyświetl błąd
	if (it == trains.end()) {
		std::cout << "BLAD: Nie znaleziono pociagu o takim ID.\n";
		return;
	}

	// Zapamiętaj liczbę biletów przed usunięciem
	auto initialTicketCount = tickets.size();
	// Usuń wszystkie bilety powiązane z tym pociągiem
	tickets.erase(
		std::remove_if(tickets.begin(), tickets.end(),
			[idToRemove](const Ticket& t) { return t.getTrainId() == idToRemove; }),
		tickets.end()
	);
	// Oblicz ile biletów zostało usuniętych
	int deletedTickets = initialTicketCount - tickets.size();

	// Usuń pociąg z listy
	trains.erase(it);

	std::cout << "SUKCES: Usunieto pociag ID " << idToRemove << ".\n";
	// Jeśli usunięto jakieś bilety, poinformuj użytkownika
	if (deletedTickets > 0) {
		std::cout << "UWAGA: Anulowano rowniez " << deletedTickets << " biletow.\n";
	}
}

// Generuje i wyświetla raport systemowy ze statystykami
void Admin::generateReport(const std::vector<Train>& trains, const std::vector<Ticket>& tickets, const std::vector<std::unique_ptr<User>>& users) {
	std::cout << "\n================ RAPORT SYSTEMOWY ================\n";
	std::cout << "Liczba uzytkownikow: " << users.size() << "\n";      // Wyświetl liczbę użytkowników
	std::cout << "Liczba pociagow: " << trains.size() << "\n";         // Wyświetl liczbę pociągów
	std::cout << "Liczba sprzedanych biletow: " << tickets.size() << "\n";  // Wyświetl liczbę biletów
	std::cout << "==================================================\n";
}