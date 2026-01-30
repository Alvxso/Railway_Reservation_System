/**
 * @file Passenger.cpp
 * @brief Implementacja funkcjonalności pasażera systemu
 */

#include "Passenger.h"
#include "InputValidation.h"
#include <iostream>
#include <algorithm> 
#include <iterator>  
#include <iomanip>   
#include <string>
#include <limits>    
#include <cctype>    

// Konstruktor wywołuje konstruktor klasy bazowej User
Passenger::Passenger(std::string login, std::string password, int id)
	: User(login, password, id) {
}

// Zwraca typ użytkownika jako "PASSENGER"
std::string Passenger::getType() const { return "PASSENGER"; }

// Wyświetla menu opcji dostępnych dla pasażera
void Passenger::showMenu() {
	std::cout << "\n=== PANEL PASAZERA: " << get_Flogin() << " ===" << std::endl;
	std::cout << "1. Wyszukiwarka pociagow i rezerwacja" << std::endl;
	std::cout << "2. Moje rezerwacje" << std::endl;
	std::cout << "3. Wyloguj" << std::endl;
	std::cout << "Wybierz opcje: ";
}


/**
 * @brief Konwertuje ciąg znaków na małe litery
 * @param str Ciąg do konwersji
 * @return Ciąg z małymi literami
 */
std::string toLowerCase(std::string str) {
	// Zamienia wszystkie znaki na małe litery
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

/**
 * @brief Sprawdza czy tekst zawiera podany termin (bez rozróżniania wielkości liter)
 * @param text Tekst do przeszukania
 * @param term Szukany termin
 * @return true jeśli termin został znaleziony
 */
bool containsString(const std::string& text, const std::string& term) {
	if (term.empty()) return true;  // Pusty termin zawsze pasuje
	// Szukaj terminu bez rozróżniania wielkości liter
	return toLowerCase(text).find(toLowerCase(term)) != std::string::npos;
}


// Wyszukuje pociągi według kryteriów i pozwala zarezerwować bilet
void Passenger::bookTicket(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	// Sprawdź czy są jakieś pociągi w systemie
	if (trains.empty()) {
		std::cout << "\n[INFO] Aktualnie brak pociagow w bazie.\n";
		return;
	}

	// Wyświetl opcje wyszukiwania
	std::cout << "\n--- WYSZUKIWARKA POLACZEN ---\n";
	std::cout << "1. Pokaz wszystkie pociagi\n";
	std::cout << "2. Szukaj po stacji poczatkowej (Skad)\n";
	std::cout << "3. Szukaj po stacji koncowej (Dokad)\n";
	std::cout << "4. Wyszukiwanie zaawansowane (Filtruj dowolne pola)\n";

	int searchOption;
	readInt("Wybierz opcje filtrowania: ", searchOption);

	std::vector<Train> displayList;  // Lista pociągów do wyświetlenia

	// Obsługa opcji 1 - pokaż wszystkie pociągi
	if (searchOption == 1) {
		displayList = trains;
	}
	// Obsługa opcji 2 i 3 - filtrowanie po jednej stacji
	else if (searchOption == 2 || searchOption == 3) {
		std::string searchTerm;
		readLine("Wpisz nazwe stacji: ", searchTerm);

		// Opcja 2 - filtruj po stacji początkowej
		if (searchOption == 2) {
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getOrigin(), searchTerm); });
		}
		// Opcja 3 - filtruj po stacji końcowej
		else {
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getDestination(), searchTerm); });
		}
	}
	// Obsługa opcji 4 - zaawansowane wyszukiwanie
	else if (searchOption == 4) {
		std::string from, to, date;

		std::cout << "\n--- Planowanie Podrozy ---\n";
		std::cout << "(Wcisnij ENTER aby pominac dany filtr)\n";

		// Wczytaj kryteria wyszukiwania (każde może być puste)
		std::cout << "Skad: ";
		std::getline(std::cin, from);

		std::cout << "Dokad: ";
		std::getline(std::cin, to);

		std::cout << "Data (RRRR-MM-DD): ";
		std::getline(std::cin, date);

		// Filtruj pociągi według wszystkich kryteriów jednocześnie
		std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
			[&from, &to, &date](const Train& t) {
				bool matchOrigin = containsString(t.getOrigin(), from);      // Sprawdź stację początkową
				bool matchDest = containsString(t.getDestination(), to);     // Sprawdź stację końcową
				bool matchDate = containsString(t.getDate(), date);          // Sprawdź datę
				return matchOrigin && matchDest && matchDate;  // Wszystkie kryteria muszą być spełnione
			});
	}
	else {
		std::cout << "Niepoprawna opcja wyszukiwania.\n";
		return;
	}

	// Sprawdź czy znaleziono jakieś pociągi
	if (displayList.empty()) {
		std::cout << "\n[INFO] Nie znaleziono polaczen spelniajacych kryteria.\n";
		return;
	}

	// Wyświetl informację o liczbie znalezionych pociągów
	std::cout << "\nZnaleziono " << displayList.size() << " pasujacych polaczen:\n";
	std::cout << "-----------------------------------------------------------------------\n";
	// Wyświetl nagłówki kolumn tabeli
	std::cout << std::left << std::setw(6) << "ID"
		<< std::setw(20) << "Odjazd"
		<< std::setw(20) << "Przyjazd"
		<< std::setw(12) << "Data"
		<< "Wolne" << "\n";
	std::cout << "-----------------------------------------------------------------------\n";

	// Wyświetl każdy pociąg z listy w formie tabeli
	for (const auto& t : displayList) {
		std::cout << std::left << std::setw(6) << t.getID()
			<< std::setw(20) << t.getOrigin()
			<< std::setw(20) << t.getDestination()
			<< std::setw(12) << t.getDate()
			<< (t.getCapacity() - t.getOccupiedSeatsCount())  // Oblicz liczbę wolnych miejsc
			<< "\n";
	}
	std::cout << "-----------------------------------------------------------------------\n";

	int tId;
	readInt("\nPodaj ID pociagu, aby zarezerwowac (0 aby anulowac): ", tId);

	if (tId == 0) return;  // Użytkownik anulował rezerwację

	// Znajdź wybrany pociąg w pełnej liście pociągów
	auto it = std::find_if(trains.begin(), trains.end(), [tId](const Train& t) {
		return t.getID() == tId;
		});

	// Jeśli znaleziono pociąg
	if (it != trains.end()) {
		it->displaySeats();  // Wyświetl mapę miejsc w pociągu

		int seatNum;
		readInt("Wybierz numer miejsca: ", seatNum);

		// Próbuj zarezerwować wybrane miejsce
		if (it->reserveSeat(seatNum)) {
			// Wygeneruj nowe ID biletu (kolejny numer)
			int newTicketId = (tickets.empty() ? 1 : tickets.back().getTicketId() + 1);
			double price = 50.0;  // Stała cena biletu

			// Utwórz nowy bilet i dodaj do listy
			tickets.emplace_back(newTicketId, tId, get_Flogin(), seatNum, price);
			std::cout << "\n[SUKCES] Zarezerwowano bilet!\n";
		}
		else {
			std::cout << "\n[BLAD] Miejsce zajete lub nie istnieje.\n";
		}
	}
	else {
		std::cout << "\n[BLAD] Pociag o ID " << tId << " nie istnieje.\n";
	}
}

// Wyświetla listę biletów należących do zalogowanego pasażera
void Passenger::listMyTickets(const std::vector<Ticket>& tickets) const {
	std::cout << "\n--- TWOJE AKTYWNE REZERWACJE ---\n";
	bool found = false;  // Flaga czy znaleziono jakieś bilety
	// Przejdź przez wszystkie bilety
	for (const auto& t : tickets) {
		// Sprawdź czy bilet należy do tego pasażera
		if (t.getPassengerLogin() == get_Flogin()) {
			std::cout << t.toString() << "\n";  // Wyświetl informacje o bilecie
			found = true;
		}
	}
	// Jeśli nie znaleziono żadnych biletów, wyświetl komunikat
	if (!found) std::cout << "Nie posiadasz zadnych biletow.\n";
}