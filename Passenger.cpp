/**
 * @file Passenger.cpp
 * @brief Implementacja funkcjonalności pasażera w systemie rezerwacji kolejowej
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

/**
 * @brief Konstruktor klasy Passenger - inicjalizuje pasażera
 * @param login Login pasażera
 * @param password Hasło pasażera
 * @param id Unikalny identyfikator pasażera
 */
Passenger::Passenger(std::string login, std::string password, int id)
	: User(login, password, id) {
}

/**
 * @brief Zwraca typ użytkownika
 * @return Zawsze zwraca "PASSENGER"
 */
std::string Passenger::getType() const { return "PASSENGER"; }

/**
 * @brief Wyświetla menu pasażera z dostępnymi opcjami
 */
void Passenger::showMenu() {
	std::cout << "\n=== PANEL PASAZERA: " << get_Flogin() << " ===" << std::endl;
	std::cout << "1. Wyszukaj i zarezerwuj bilet" << std::endl;
	std::cout << "2. Moje rezerwacje" << std::endl;
	std::cout << "3. Anuluj rezerwacje" << std::endl;  
	std::cout << "4. Modyfikuj rezerwacje" << std::endl; 
	std::cout << "5. Wyloguj" << std::endl;
	std::cout << "Wybierz opcje: ";
}

/**
 * @brief Konwertuje tekst na małe litery
 * @param str Tekst do konwersji
 * @return Tekst zapisany małymi literami
 */
std::string toLowerCase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

/**
 * @brief Sprawdza czy tekst zawiera podany fragment (bez rozróżniania wielkości liter)
 * @param text Przeszukiwany tekst
 * @param term Szukany fragment
 * @return true jeśli fragment został znaleziony, false w przeciwnym razie
 */
bool containsString(const std::string& text, const std::string& term) {
	if (term.empty()) return true;
	return toLowerCase(text).find(toLowerCase(term)) != std::string::npos;
}

/**
 * @brief Wyszukuje i rezerwuje bilet na wybrany pociąg
 * 
 * Funkcja oferuje różne tryby wyszukiwania:
 * 1. Wyświetlenie wszystkich pociągów
 * 2. Filtrowanie po stacji początkowej
 * 3. Filtrowanie po stacji końcowej
 * 4. Zaawansowane wyszukiwanie z wieloma kryteriami
 * 
 * Po wyborze pociągu wyświetla mapę miejsc, pozwala wybrać klasę podróży,
 * pokazuje podsumowanie i przeprowadza proces płatności i rezerwacji.
 * 
 * @param trains Wektor pociągów dostępnych w systemie
 * @param tickets Wektor biletów - nowy bilet zostanie do niego dodany po pomyślnej rezerwacji
 */
void Passenger::bookTicket(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	if (trains.empty()) {
		std::cout << "\n[INFO] Aktualnie brak pociagow w bazie.\n";
		return;
	}

	std::cout << "\n--- WYSZUKIWARKA POLACZEN ---\n";
	std::cout << "1. Pokaz wszystkie pociagi\n";
	std::cout << "2. Szukaj po stacji poczatkowej (Skad)\n";
	std::cout << "3. Szukaj po stacji koncowej (Dokad)\n";
	std::cout << "4. Wyszukiwanie zaawansowane (Filtruj dowolne pola)\n";

	int searchOption;
	readInt("Wybierz opcje filtrowania: ", searchOption);

	std::vector<Train> displayList;

	// Wybór trybu wyszukiwania
	if (searchOption == 1) {
		displayList = trains;
	}
	else if (searchOption == 2 || searchOption == 3) {
		std::string searchTerm;
		readLine("Wpisz nazwe stacji: ", searchTerm);

		if (searchOption == 2) {
			// Filtrowanie po stacji początkowej
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getOrigin(), searchTerm); });
		}
		else {
			// Filtrowanie po stacji końcowej
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getDestination(), searchTerm); });
		}
	}
	else if (searchOption == 4) {
		// Wyszukiwanie zaawansowane - wiele kryteriów
		std::string from, to, date;

		std::cout << "\n--- Planowanie Podrozy ---\n";
		std::cout << "(Wcisnij ENTER aby pominac dany filtr)\n";

		std::cout << "Skad: ";
		std::getline(std::cin, from);

		std::cout << "Dokad: ";
		std::getline(std::cin, to);

		std::cout << "Data (RRRR-MM-DD): ";
		std::getline(std::cin, date);

		// Filtrowanie z wieloma kryteriami
		std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
			[&from, &to, &date](const Train& t) {
				bool matchOrigin = containsString(t.getOrigin(), from);
				bool matchDest = containsString(t.getDestination(), to);
				bool matchDate = containsString(t.getDate(), date);
				return matchOrigin && matchDest && matchDate;
			});
	}
	else {
		std::cout << "Niepoprawna opcja wyszukiwania.\n";
		return;
	}

	// Sprawdzenie czy znaleziono jakiekolwiek połączenia
	if (displayList.empty()) {
		std::cout << "\n[INFO] Nie znaleziono polaczen spelniajacych kryteria.\n";
		return;
	}

	// Wyświetlenie wyników wyszukiwania w formacie tabeli
	std::cout << "\nZnaleziono " << displayList.size() << " pasujacych polaczen:\n";
	std::cout << "-----------------------------------------------------------------------\n";
	std::cout << std::left << std::setw(6) << "ID"
		<< std::setw(20) << "Odjazd"
		<< std::setw(20) << "Przyjazd"
		<< std::setw(12) << "Data"
		<< "Wolne" << "\n";
	std::cout << "-----------------------------------------------------------------------\n";

	for (const auto& t : displayList) {
		std::cout << std::left << std::setw(6) << t.getID()
			<< std::setw(20) << t.getOrigin()
			<< std::setw(20) << t.getDestination()
			<< std::setw(12) << t.getDate()
			<< (t.getCapacity() - t.getOccupiedSeatsCount())
			<< "\n";
	}
	std::cout << "-----------------------------------------------------------------------\n";

	// Wybór pociągu do rezerwacji
	int tId;
	readInt("\nPodaj ID pociagu, aby zarezerwowac (0 aby anulowac): ", tId);

	if (tId == 0) return;

	// Wyszukanie wybranego pociągu
	auto it = std::find_if(trains.begin(), trains.end(), [tId](const Train& t) {
		return t.getID() == tId;
		});

	if (it != trains.end()) {
		// Wyświetlenie mapy miejsc
		it->displaySeats();

		// Wybór numeru miejsca
		int seatNum;
		readInt("Wybierz numer miejsca: ", seatNum);

		if (it->isSeatFree(seatNum)) {

			// Obliczenie ceny bazowej
			double basePrice = 40.0 + (it->getDestination().length() * 5.0);

			// Wybór klasy podróży
			std::cout << "\n--- WYBOR KLASY PODROZY ---\n";
			std::cout << "1. Druga Klasa (Standard) - Cena: " << std::fixed << std::setprecision(2) << basePrice << " PLN\n";
			std::cout << "2. Pierwsza Klasa (Komfort) - Cena: " << (basePrice * 1.5) << " PLN (+50%)\n";

			int classChoice;
			readInt("Wybierz klase (1/2): ", classChoice);

			// Obliczenie ceny końcowej w zależności od klasy
			double finalPrice = basePrice;
			if (classChoice == 2) {
				finalPrice = basePrice * 1.5;
			}

			// Wyświetlenie podsumowania rezerwacji
			std::cout << "\n--- PODSUMOWANIE REZERWACJI ---\n";
			std::cout << "Pociag:  " << it->getOrigin() << " -> " << it->getDestination() << "\n";
			std::cout << "Data:    " << it->getDate() << "\n";
			std::cout << "Miejsce: " << seatNum << "\n";
			std::cout << "Klasa:   " << (classChoice == 2 ? "Pierwsza" : "Druga") << "\n";
			std::cout << "DO ZAPLATY: " << finalPrice << " PLN\n";
			std::cout << "-------------------------------\n";

			// Potwierdzenie zakupu
			std::string confirm;
			readLine("Czy potwierdzasz zakup? (t/n): ", confirm);

			if (confirm == "t" || confirm == "T") {
				// Rezerwacja miejsca w pociągu
				if (it->reserveSeat(seatNum)) { 
					// Generowanie nowego ID biletu
					int newTicketId = (tickets.empty() ? 1 : tickets.back().getTicketId() + 1);

					// Utworzenie nowego biletu
					tickets.emplace_back(newTicketId, tId, get_Flogin(), seatNum, finalPrice);

					std::cout << "\n[SUKCES] Platnosc przyjeta. Bilet zostal wygenerowany!\n";
				}
				else {
					std::cout << "\n[BLAD] Ktos wlasnie zajal to miejsce. Sprobuj ponownie.\n";
				}
			}
			else {
				std::cout << "\nAnulowano rezerwacje.\n";
			}
		}
		else {
			std::cout << "\n[BLAD] Miejsce nr " << seatNum << " jest juz zajete.\n";
		}
	}
	else {
		std::cout << "\n[BLAD] Pociag o ID " << tId << " nie istnieje.\n";
	}
}

/**
 * @brief Wyświetla wszystkie aktywne rezerwacje pasażera
 * 
 * Przeszukuje wszystkie bilety w systemie i wyświetla tylko te,
 * które należą do zalogowanego pasażera
 * 
 * @param tickets Wektor wszystkich biletów w systemie
 */
void Passenger::listMyTickets(const std::vector<Ticket>& tickets) const {
	std::cout << "\n--- TWOJE AKTYWNE REZERWACJE ---\n";
	bool found = false;
	for (const auto& t : tickets) {
		if (t.getPassengerLogin() == get_Flogin()) {
			std::cout << t.toString() << "\n";
			found = true;
		}
	}
	if (!found) std::cout << "Nie posiadasz zadnych biletow.\n";
}

/**
 * @brief Anuluje wybraną rezerwację pasażera
 * 
 * Funkcja:
 * - Wyświetla listę biletów pasażera
 * - Pozwala wybrać bilet do anulowania
 * - Usuwa bilet z systemu
 * - Zwalnia miejsce w pociągu
 * 
 * @param trains Wektor pociągów - miejsce zostanie zwolnione
 * @param tickets Wektor biletów - bilet zostanie usunięty
 */
void Passenger::cancelBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- ANULOWANIE REZERWACJI ---\n";

	// Wyświetlenie aktywnych rezerwacji
	listMyTickets(tickets);

	// Wybór biletu do anulowania
	int ticketId;
	readInt("\nPodaj ID biletu do anulowania (0 aby wrocic): ", ticketId);
	if (ticketId == 0) return;

	// Wyszukanie biletu należącego do pasażera
	auto itTicket = std::find_if(tickets.begin(), tickets.end(),
		[ticketId, this](const Ticket& t) {
			return t.getTicketId() == ticketId && t.getPassengerLogin() == this->get_Flogin();
		});

	if (itTicket != tickets.end()) {
		// Pobierz dane o pociągu i miejscu
		int trainId = itTicket->getTrainId();
		int seatNum = itTicket->getSeatNumber();

		// Znajdź pociąg i zwolnij miejsce
		auto itTrain = std::find_if(trains.begin(), trains.end(),
			[trainId](const Train& t) { return t.getID() == trainId; });

		if (itTrain != trains.end()) {
			itTrain->cancelSeat(seatNum);
			std::cout << "[INFO] Zwolniono miejsce nr " << seatNum << " w pociagu ID " << trainId << ".\n";
		}

		// Usuń bilet z systemu
		tickets.erase(itTicket);
		std::cout << "[SUKCES] Rezerwacja zostala anulowana.\n";
	}
	else {
		std::cout << "[BLAD] Nie znaleziono biletu o takim ID przypisanego do Ciebie.\n";
	}
}

/**
 * @brief Modyfikuje wybraną rezerwację pasażera
 * 
 * Funkcja oferuje dwie opcje modyfikacji:
 * 1. Zmiana numeru miejsca w tym samym pociągu
 * 2. Całkowite przebookowanie na inny pociąg (wymaga anulowania starej rezerwacji)
 * 
 * @param trains Wektor pociągów w systemie
 * @param tickets Wektor biletów w systemie
 */
void Passenger::modifyBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- MODYFIKACJA REZERWACJI ---\n";
	listMyTickets(tickets);

	// Wybór biletu do modyfikacji
	int ticketId;
	readInt("\nPodaj ID biletu do zmiany (0 aby wrocic): ", ticketId);
	if (ticketId == 0) return;

	// Wyszukanie biletu należącego do pasażera
	auto itTicket = std::find_if(tickets.begin(), tickets.end(),
		[ticketId, this](const Ticket& t) {
			return t.getTicketId() == ticketId && t.getPassengerLogin() == this->get_Flogin();
		});

	if (itTicket == tickets.end()) {
		std::cout << "[BLAD] Nieprawidlowe ID biletu.\n";
		return;
	}

	// Znajdź pociąg powiązany z biletem
	auto itTrain = std::find_if(trains.begin(), trains.end(),
		[id = itTicket->getTrainId()](const Train& t) { return t.getID() == id; });

	if (itTrain == trains.end()) {
		std::cout << "[BLAD] Pociag z tej rezerwacji juz nie istnieje.\n";
		return;
	}

	// Wybór typu modyfikacji
	std::cout << "\nCo chcesz zmienic?\n";
	std::cout << "1. Zmien tylko miejsce (ten sam pociag)\n";
	std::cout << "2. Zmien date/pociag (wymaga przebookowania)\n";
	int choice;
	readInt("Wybierz: ", choice);

	if (choice == 1) {
		// Opcja 1: Zmiana miejsca w tym samym pociągu
		std::cout << "Obecne miejsce: " << itTicket->getSeatNumber() << "\n";
		itTrain->displaySeats(); 

		int newSeat;
		readInt("Wybierz nowe miejsce: ", newSeat);

		if (newSeat == itTicket->getSeatNumber()) {
			std::cout << "To to samo miejsce. Anulowano zmiane.\n";
			return;
		}

		if (itTrain->isSeatFree(newSeat)) {
			// Zwolnij stare miejsce i zarezerwuj nowe
			itTrain->cancelSeat(itTicket->getSeatNumber());
			itTrain->reserveSeat(newSeat);
			itTicket->setSeatNumber(newSeat);
			std::cout << "[SUKCES] Zmieniono miejsce na " << newSeat << ".\n";
		}
		else {
			std::cout << "[BLAD] Wybrane miejsce jest zajete.\n";
		}
	}
	else if (choice == 2) {
		// Opcja 2: Przebookowanie na inny pociąg
		std::cout << "Aby zmienic pociag, musimy anulowac obecny bilet i utworzyc nowy.\n";
		std::string confirm;
		readLine("Czy kontynuowac? (t/n): ", confirm);

		if (confirm == "t" || confirm == "T") {
			// Anuluj starą rezerwację
			itTrain->cancelSeat(itTicket->getSeatNumber());
			tickets.erase(itTicket);

			// Przejdź do wyszukiwarki połączeń
			std::cout << "Stara rezerwacja usunieta. Przechodzimy do wyszukiwarki...\n";
			bookTicket(trains, tickets);
		}
		else {
			std::cout << "Anulowano operacje.\n";
		}
	}
	else {
		std::cout << "Niepoprawna opcja.\n";
	}
}