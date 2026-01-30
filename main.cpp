/**
 * System Rezerwacji Kolejowej - Główny plik programu
 * 
 * Program umożliwia:
 * - Rejestrację i logowanie użytkowników (pasażerowie i administratorzy)
 * - Rezerwację biletów przez pasażerów
 * - Zarządzanie pociągami przez administratorów
 * - Zapisywanie i wczytywanie danych z plików YAML
 */

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "User.h"
#include "Admin.h"
#include "Passenger.h"
#include "Train.h"
#include "Ticket.h"
#include "DataManager.h"
#include "InputValidation.h"


void registerUser(std::vector<std::unique_ptr<User>>& users, DataManager& dm) {
    std::cout << "\n--- REJESTRACJA NOWEGO UZYTKOWNIKA ---\n";

    std::string newLogin, newPass;

    while (true) {
        readLine("Podaj login: ", newLogin);

        auto it = std::find_if(users.begin(), users.end(),
            [&newLogin](const std::unique_ptr<User>& u) {
                return u->get_Flogin() == newLogin;
            });

        if (it != users.end()) {
            std::cout << "Blad: Taki login jest juz zajety. Wybierz inny.\n";
        }
        else if (newLogin.empty()) {
            std::cout << "Blad: Login nie moze byc pusty.\n";
        }
        else {
            break;
        }
    }

    while (true) {
        readLine("Podaj haslo: ", newPass);
        if (newPass.length() < 3) {
            std::cout << "Haslo musi miec co najmniej 3 znaki.\n";
        }
        else {
            break;
        }
    }

    int maxId = 0;
    for (const auto& u : users) {
        if (u->get_Fid() > maxId) maxId = u->get_Fid();
    }
    int newId = maxId + 1;

    users.push_back(std::make_unique<Passenger>(newLogin, newPass, newId));

    dm.saveUsers(users);

    std::cout << "SUKCES! Konto utworzone. Mozesz sie teraz zalogowac.\n";
}

void passengerSession(Passenger* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, DataManager& dm) {
    bool sessionActive = true;
    while (sessionActive) {
        user->showMenu();

        int choice;
        readInt("Wybierz opcje: ", choice);

        switch (choice) {
        case 1:
            user->bookTicket(trains, tickets);
            dm.saveTrains(trains);
            dm.saveTickets(tickets);
            break;
        case 2:
            user->listMyTickets(tickets);
            break;
        case 3:
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

void adminSession(Admin* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, std::vector<std::unique_ptr<User>>& users, DataManager& dm) {
    bool sessionActive = true;
    while (sessionActive) {
        user->showMenu();

        int choice;
        readInt("Wybierz opcje: ", choice);

        switch (choice) {
        case 1:
            user->addTrain(trains);
            dm.saveTrains(trains);
            break;
        case 2:
            user->removeTrain(trains, tickets);

            dm.saveTrains(trains);
            dm.saveTickets(tickets);
            break;
        case 3:
            user->generateReport(trains, tickets, users);
            break;
        case 4:
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

int main() {
    DataManager dm;
    std::vector<std::unique_ptr<User>> users;
    std::vector<Train> trains;
    std::vector<Ticket> tickets;

    std::cout << "Inicjalizacja systemu...\n";
    dm.loadUsers(users);
    dm.loadTrains(trains);
    dm.loadTickets(tickets);

    if (users.empty()) {
        std::cout << "Pierwsze uruchomienie. Tworzenie konta admin/admin.\n";
        users.push_back(std::make_unique<Admin>("admin", "admin", 1));
        dm.saveUsers(users);
    }

    bool appRunning = true;
    while (appRunning) {
        std::cout << "\n=== SYSTEM REZERWACJI KOLEJOWEJ ===\n";
        std::cout << "1. Zaloguj sie\n";
        std::cout << "2. Zarejestruj sie (Nowy Pasazer)\n";
        std::cout << "3. Wyjdz i Zapisz\n";

        int choice;
        readInt("Wybierz: ", choice);

        if (choice == 3) {
            appRunning = false;
        }
        else if (choice == 2) {
            registerUser(users, dm);
        }
        else if (choice == 1) {
            std::string login, pass;
            readLine("Login: ", login);
            readLine("Haslo: ", pass);


            auto it = std::find_if(users.begin(), users.end(),
                [&login, &pass](const std::unique_ptr<User>& u) {
                    return u->get_Flogin() == login && u->get_Fpassword() == pass;
                });

            if (it != users.end()) {
                User* loggedUser = it->get();

                if (auto admin = dynamic_cast<Admin*>(loggedUser)) {
                    adminSession(admin, trains, tickets, users, dm);
                }
                else if (auto passenger = dynamic_cast<Passenger*>(loggedUser)) {
                    passengerSession(passenger, trains, tickets, dm);
                }
            }
            else {
                std::cout << "Bledny login lub haslo!\n";
            }
        }
        else {
            std::cout << "Niepoprawna opcja.\n";
        }
    }

    std::cout << "Zapisywanie danych...\n";
    dm.saveUsers(users);
    dm.saveTrains(trains);
    dm.saveTickets(tickets);

    return 0;
}