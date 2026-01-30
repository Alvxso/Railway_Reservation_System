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
	std::cout << "1. Wyszukaj i zarezerwuj bilet" << std::endl;
	std::cout << "2. Moje rezerwacje" << std::endl;
	std::cout << "3. Anuluj rezerwacje" << std::endl;  
	std::cout << "4. Modyfikuj rezerwacje" << std::endl; 
	std::cout << "5. Wyloguj" << std::endl;
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

		if (it->isSeatFree(seatNum)) {

			double basePrice = 40.0 + (it->getDestination().length() * 5.0);

			std::cout << "\n--- WYBOR KLASY PODROZY ---\n";
			std::cout << "1. Druga Klasa (Standard) - Cena: " << std::fixed << std::setprecision(2) << basePrice << " PLN\n";
			std::cout << "2. Pierwsza Klasa (Komfort) - Cena: " << (basePrice * 1.5) << " PLN (+50%)\n";

			int classChoice;
			readInt("Wybierz klase (1/2): ", classChoice);

			double finalPrice = basePrice;
			if (classChoice == 2) {
				finalPrice = basePrice * 1.5;
			}

			std::cout << "\n--- PODSUMOWANIE REZERWACJI ---\n";
			std::cout << "Pociag:  " << it->getOrigin() << " -> " << it->getDestination() << "\n";
			std::cout << "Data:    " << it->getDate() << "\n";
			std::cout << "Miejsce: " << seatNum << "\n";
			std::cout << "Klasa:   " << (classChoice == 2 ? "Pierwsza" : "Druga") << "\n";
			std::cout << "DO ZAPLATY: " << finalPrice << " PLN\n";
			std::cout << "-------------------------------\n";

			std::string confirm;
			readLine("Czy potwierdzasz zakup? (t/n): ", confirm);

			if (confirm == "t" || confirm == "T") {
				if (it->reserveSeat(seatNum)) { 
					int newTicketId = (tickets.empty() ? 1 : tickets.back().getTicketId() + 1);

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

void Passenger::cancelBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- ANULOWANIE REZERWACJI ---\n";

	listMyTickets(tickets);

	int ticketId;
	readInt("\nPodaj ID biletu do anulowania (0 aby wrocic): ", ticketId);
	if (ticketId == 0) return;

	auto itTicket = std::find_if(tickets.begin(), tickets.end(),
		[ticketId, this](const Ticket& t) {
			return t.getTicketId() == ticketId && t.getPassengerLogin() == this->get_Flogin();
		});

	if (itTicket != tickets.end()) {
		int trainId = itTicket->getTrainId();
		int seatNum = itTicket->getSeatNumber();

		auto itTrain = std::find_if(trains.begin(), trains.end(),
			[trainId](const Train& t) { return t.getID() == trainId; });

		if (itTrain != trains.end()) {
			itTrain->cancelSeat(seatNum);
			std::cout << "[INFO] Zwolniono miejsce nr " << seatNum << " w pociagu ID " << trainId << ".\n";
		}

		tickets.erase(itTicket);
		std::cout << "[SUKCES] Rezerwacja zostala anulowana.\n";
	}
	else {
		std::cout << "[BLAD] Nie znaleziono biletu o takim ID przypisanego do Ciebie.\n";
	}
}

void Passenger::modifyBooking(std::vector<Train>& trains, std::vector<Ticket>& tickets) {
	std::cout << "\n--- MODYFIKACJA REZERWACJI ---\n";
	listMyTickets(tickets);

	int ticketId;
	readInt("\nPodaj ID biletu do zmiany (0 aby wrocic): ", ticketId);
	if (ticketId == 0) return;

	auto itTicket = std::find_if(tickets.begin(), tickets.end(),
		[ticketId, this](const Ticket& t) {
			return t.getTicketId() == ticketId && t.getPassengerLogin() == this->get_Flogin();
		});

	if (itTicket == tickets.end()) {
		std::cout << "[BLAD] Nieprawidlowe ID biletu.\n";
		return;
	}

	auto itTrain = std::find_if(trains.begin(), trains.end(),
		[id = itTicket->getTrainId()](const Train& t) { return t.getID() == id; });

	if (itTrain == trains.end()) {
		std::cout << "[BLAD] Pociag z tej rezerwacji juz nie istnieje.\n";
		return;
	}

	std::cout << "\nCo chcesz zmienic?\n";
	std::cout << "1. Zmien tylko miejsce (ten sam pociag)\n";
	std::cout << "2. Zmien date/pociag (wymaga przebookowania)\n";
	int choice;
	readInt("Wybierz: ", choice);

	if (choice == 1) {
		std::cout << "Obecne miejsce: " << itTicket->getSeatNumber() << "\n";
		itTrain->displaySeats(); 

		int newSeat;
		readInt("Wybierz nowe miejsce: ", newSeat);

		if (newSeat == itTicket->getSeatNumber()) {
			std::cout << "To to samo miejsce. Anulowano zmiane.\n";
			return;
		}

		if (itTrain->isSeatFree(newSeat)) {
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
		std::cout << "Aby zmienic pociag, musimy anulowac obecny bilet i utworzyc nowy.\n";
		std::string confirm;
		readLine("Czy kontynuowac? (t/n): ", confirm);

		if (confirm == "t" || confirm == "T") {
			itTrain->cancelSeat(itTicket->getSeatNumber());
			tickets.erase(itTicket);

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