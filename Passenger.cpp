#include "Passenger.h"
#include "InputValidation.h"
#include <iostream>
#include <algorithm> 
#include <iterator>  
#include <iomanip>   
#include <string>
#include <limits>    
#include <cctype>    

Passenger::Passenger(std::string login, std::string password, int id)
	: User(login, password, id) {
}

std::string Passenger::getType() const { return "PASSENGER"; }

void Passenger::showMenu() {
	std::cout << "\n=== PANEL PASAZERA: " << get_Flogin() << " ===" << std::endl;
	std::cout << "1. Wyszukiwarka pociagow i rezerwacja" << std::endl;
	std::cout << "2. Moje rezerwacje" << std::endl;
	std::cout << "3. Wyloguj" << std::endl;
	std::cout << "Wybierz opcje: ";
}


std::string toLowerCase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

bool containsString(const std::string& text, const std::string& term) {
	if (term.empty()) return true;
	return toLowerCase(text).find(toLowerCase(term)) != std::string::npos;
}


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

	if (searchOption == 1) {
		displayList = trains;
	}
	else if (searchOption == 2 || searchOption == 3) {
		std::string searchTerm;
		readLine("Wpisz nazwe stacji: ", searchTerm);

		if (searchOption == 2) {
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getOrigin(), searchTerm); });
		}
		else {
			std::copy_if(trains.begin(), trains.end(), std::back_inserter(displayList),
				[&searchTerm](const Train& t) { return containsString(t.getDestination(), searchTerm); });
		}
	}
	else if (searchOption == 4) {
		std::string from, to, date;

		std::cout << "\n--- Planowanie Podrozy ---\n";
		std::cout << "(Wcisnij ENTER aby pominac dany filtr)\n";

		std::cout << "Skad: ";
		std::getline(std::cin, from);

		std::cout << "Dokad: ";
		std::getline(std::cin, to);

		std::cout << "Data (RRRR-MM-DD): ";
		std::getline(std::cin, date);

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

	if (displayList.empty()) {
		std::cout << "\n[INFO] Nie znaleziono polaczen spelniajacych kryteria.\n";
		return;
	}

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

	int tId;
	readInt("\nPodaj ID pociagu, aby zarezerwowac (0 aby anulowac): ", tId);

	if (tId == 0) return;

	auto it = std::find_if(trains.begin(), trains.end(), [tId](const Train& t) {
		return t.getID() == tId;
		});

	if (it != trains.end()) {
		it->displaySeats();

		int seatNum;
		readInt("Wybierz numer miejsca: ", seatNum);

		if (it->reserveSeat(seatNum)) {
			int newTicketId = (tickets.empty() ? 1 : tickets.back().getTicketId() + 1);
			double price = 50.0;

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