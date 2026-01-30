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

// Obsługuje sesję zalogowanego pasażera
void passengerSession(Passenger* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, DataManager& dm) {
    bool sessionActive = true;  // Flaga kontrolująca pętlę sesji
    while (sessionActive) {
        user->showMenu();  // Wyświetl menu pasażera

        int choice;
        readInt("Wybierz opcje: ", choice);

        // Obsługa wybranej opcji
        switch (choice) {
        case 1:  // Rezerwacja biletu
            user->bookTicket(trains, tickets);
            dm.saveTrains(trains);    // Zapisz zmiany w pociągach
            dm.saveTickets(tickets);  // Zapisz nowe bilety
            break;
        case 2:  // Wyświetl moje bilety
            user->listMyTickets(tickets);
            break;
        case 3:  // Wyloguj
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:  // Niepoprawny wybór
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

// Obsługuje sesję zalogowanego administratora
void adminSession(Admin* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, std::vector<std::unique_ptr<User>>& users, DataManager& dm) {
    bool sessionActive = true;  // Flaga kontrolująca pętlę sesji
    while (sessionActive) {
        user->showMenu();  // Wyświetl menu administratora

        int choice;
        readInt("Wybierz opcje: ", choice);

        // Obsługa wybranej opcji
        switch (choice) {
        case 1:  // Dodaj pociąg
            user->addTrain(trains);
            dm.saveTrains(trains);  // Zapisz zmiany
            break;
        case 2:  // Usuń pociąg
            user->removeTrain(trains, tickets);

            dm.saveTrains(trains);   // Zapisz zaktualizowaną listę pociągów
            dm.saveTickets(tickets); // Zapisz zaktualizowaną listę biletów
            break;
        case 3:  // Generuj raport
            user->generateReport(trains, tickets, users);
            break;
        case 4:  // Wyloguj
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:  // Niepoprawny wybór
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

// Główna funkcja programu
int main() {
    DataManager dm;  // Menadżer zarządzający danymi
    std::vector<std::unique_ptr<User>> users;  // Lista wszystkich użytkowników
    std::vector<Train> trains;   // Lista wszystkich pociągów
    std::vector<Ticket> tickets; // Lista wszystkich biletów

    std::cout << "Inicjalizacja systemu...\n";
    // Wczytaj dane z plików
    dm.loadUsers(users);
    dm.loadTrains(trains);
    dm.loadTickets(tickets);

    // Jeśli nie ma żadnych użytkowników, utwórz konto administratora
    if (users.empty()) {
        std::cout << "Pierwsze uruchomienie. Tworzenie konta admin/admin.\n";
        users.push_back(std::make_unique<Admin>("admin", "admin", 1));
        dm.saveUsers(users);
    }

    bool appRunning = true;  // Flaga kontrolująca pętlę główną
    while (appRunning) {
        // Wyświetl menu główne
        std::cout << "\n=== SYSTEM REZERWACJI KOLEJOWEJ ===\n";
        std::cout << "1. Zaloguj sie\n";
        std::cout << "2. Zarejestruj sie (Nowy Pasazer)\n";
        std::cout << "3. Wyjdz i Zapisz\n";

        int choice;
        readInt("Wybierz: ", choice);

        if (choice == 3) {  // Wyjście z aplikacji
            appRunning = false;
        }
        else if (choice == 2) {  // Rejestracja nowego użytkownika
            registerUser(users, dm);
        }
        else if (choice == 1) {  // Logowanie
            std::string login, pass;
            readLine("Login: ", login);
            readLine("Haslo: ", pass);


            // Znajdź użytkownika o podanych danych logowania
            auto it = std::find_if(users.begin(), users.end(),
                [&login, &pass](const std::unique_ptr<User>& u) {
                    return u->get_Flogin() == login && u->get_Fpassword() == pass;
                });

            if (it != users.end()) {  // Użytkownik znaleziony - poprawne dane
                User* loggedUser = it->get();

                // Sprawdź typ użytkownika i uruchom odpowiednią sesję
                if (auto admin = dynamic_cast<Admin*>(loggedUser)) {
                    adminSession(admin, trains, tickets, users, dm);  // Sesja administratora
                }
                else if (auto passenger = dynamic_cast<Passenger*>(loggedUser)) {
                    passengerSession(passenger, trains, tickets, dm);  // Sesja pasażera
                }
            }
            else {  // Niepoprawne dane logowania
                std::cout << "Bledny login lub haslo!\n";
            }
        }
        else {  // Niepoprawny wybór
            std::cout << "Niepoprawna opcja.\n";
        }
    }

    // Przed zakończeniem programu zapisz wszystkie dane
    std::cout << "Zapisywanie danych...\n";
    dm.saveUsers(users);
    dm.saveTrains(trains);
    dm.saveTickets(tickets);

    return 0;  // Zakończ program
}