/**
 * @file main.cpp
 * @brief Główny plik programu - System Rezerwacji Kolejowej
 * 
 * Aplikacja konsolowa do zarządzania rezerwacjami biletów kolejowych.
 * Obsługuje dwa typy użytkowników: Administratorów i Pasażerów.
 * Dane są przechowywane w plikach YAML dla trwałości między sesjami.
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

/**
 * @brief Rejestracja nowego użytkownika (pasażera) w systemie
 * 
 * Funkcja przeprowadza przez proces rejestracji:
 * - Sprawdza unikalność loginu
 * - Waliduje hasło (minimum 3 znaki)
 * - Generuje unikalny ID
 * - Zapisuje nowego użytkownika do pliku
 * 
 * @param users Wektor wszystkich użytkowników w systemie
 * @param dm Menedżer danych do zapisywania użytkowników
 */
void registerUser(std::vector<std::unique_ptr<User>>& users, DataManager& dm) {
    std::cout << "\n--- REJESTRACJA NOWEGO UZYTKOWNIKA ---\n";

    std::string newLogin, newPass;

    // Sprawdzenie unikalności loginu
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

    // Walidacja hasła
    while (true) {
        readLine("Podaj haslo: ", newPass);
        if (newPass.length() < 3) {
            std::cout << "Haslo musi miec co najmniej 3 znaki.\n";
        }
        else {
            break;
        }
    }

    // Generowanie unikalnego ID
    int maxId = 0;
    for (const auto& u : users) {
        if (u->get_Fid() > maxId) maxId = u->get_Fid();
    }
    int newId = maxId + 1;

    // Utworzenie nowego pasażera
    users.push_back(std::make_unique<Passenger>(newLogin, newPass, newId));

    // Zapisanie do pliku
    dm.saveUsers(users);

    std::cout << "SUKCES! Konto utworzone. Mozesz sie teraz zalogowac.\n";
}

/**
 * @brief Obsługa sesji pasażera
 * 
 * Pętla główna dla zalogowanego pasażera, obsługuje:
 * - Rezerwację biletów
 * - Przeglądanie rezerwacji
 * - Anulowanie rezerwacji
 * - Modyfikację rezerwacji
 * 
 * @param user Wskaźnik do zalogowanego pasażera
 * @param trains Wektor pociągów w systemie
 * @param tickets Wektor biletów w systemie
 * @param dm Menedżer danych do zapisywania zmian
 */
void passengerSession(Passenger* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, DataManager& dm) {
    bool sessionActive = true;
    while (sessionActive) {
        user->showMenu();

        int choice;
        readInt("Wybierz opcje: ", choice);

        switch (choice) {
        case 1:
            // Rezerwacja nowego biletu
            user->bookTicket(trains, tickets);
            dm.saveTrains(trains);
            dm.saveTickets(tickets);
            break;
        case 2:
            // Wyświetl moje bilety
            user->listMyTickets(tickets);
            break;
        case 3:
            // Anuluj rezerwację
            user->cancelBooking(trains, tickets);
            dm.saveTrains(trains); 
            dm.saveTickets(tickets); 
            break;
        case 4:
            // Modyfikuj rezerwację
            user->modifyBooking(trains, tickets);
            dm.saveTrains(trains);
            dm.saveTickets(tickets);
            break;
        case 5:
            // Wyloguj
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

/**
 * @brief Obsługa sesji administratora
 * 
 * Pętla główna dla zalogowanego administratora, obsługuje:
 * - Dodawanie nowych pociągów
 * - Usuwanie pociągów
 * - Generowanie raportów systemowych
 * 
 * @param user Wskaźnik do zalogowanego administratora
 * @param trains Wektor pociągów w systemie
 * @param tickets Wektor biletów w systemie
 * @param users Wektor użytkowników w systemie
 * @param dm Menedżer danych do zapisywania zmian
 */
void adminSession(Admin* user, std::vector<Train>& trains, std::vector<Ticket>& tickets, std::vector<std::unique_ptr<User>>& users, DataManager& dm) {
    bool sessionActive = true;
    while (sessionActive) {
        user->showMenu();

        int choice;
        readInt("Wybierz opcje: ", choice);

        switch (choice) {
        case 1:
            // Dodaj nowy pociąg
            user->addTrain(trains);
            dm.saveTrains(trains);
            break;
        case 2:
            // Usuń pociąg (wraz z powiązanymi biletami)
            user->removeTrain(trains, tickets);

            dm.saveTrains(trains);
            dm.saveTickets(tickets);
            break;
        case 3:
            // Generuj raport systemowy
            user->generateReport(trains, tickets, users);
            break;
        case 4:
            // Wyloguj
            sessionActive = false;
            std::cout << "Wylogowano.\n";
            break;
        default:
            std::cout << "Niepoprawna opcja.\n";
        }
    }
}

/**
 * @brief Główna funkcja programu
 * 
 * Inicjalizuje system, wczytuje dane z plików YAML i uruchamia główną pętlę aplikacji.
 * Obsługuje logowanie, rejestrację i zapisywanie danych przy wyjściu.
 * 
 * @return 0 przy poprawnym zakończeniu programu
 */
int main() {
    // Inicjalizacja menedżera danych
    DataManager dm;
    std::vector<std::unique_ptr<User>> users;
    std::vector<Train> trains;
    std::vector<Ticket> tickets;

    // Wczytanie danych z plików
    std::cout << "Inicjalizacja systemu...\n";
    dm.loadUsers(users);
    dm.loadTrains(trains);
    dm.loadTickets(tickets);

    // Utworzenie domyślnego konta administratora przy pierwszym uruchomieniu
    if (users.empty()) {
        std::cout << "Pierwsze uruchomienie. Tworzenie konta admin/admin.\n";
        users.push_back(std::make_unique<Admin>("admin", "admin", 1));
        dm.saveUsers(users);
    }

    // Główna pętla aplikacji
    bool appRunning = true;
    while (appRunning) {
        std::cout << "\n=== SYSTEM REZERWACJI KOLEJOWEJ ===\n";
        std::cout << "1. Zaloguj sie\n";
        std::cout << "2. Zarejestruj sie (Nowy Pasazer)\n";
        std::cout << "3. Wyjdz i Zapisz\n";

        int choice;
        readInt("Wybierz: ", choice);

        if (choice == 3) {
            // Wyjście z aplikacji
            appRunning = false;
        }
        else if (choice == 2) {
            // Rejestracja nowego użytkownika
            registerUser(users, dm);
        }
        else if (choice == 1) {
            // Logowanie
            std::string login, pass;
            readLine("Login: ", login);
            readLine("Haslo: ", pass);


            // Wyszukanie użytkownika z podanym loginem i hasłem
            auto it = std::find_if(users.begin(), users.end(),
                [&login, &pass](const std::unique_ptr<User>& u) {
                    return u->get_Flogin() == login && u->get_Fpassword() == pass;
                });

            if (it != users.end()) {
                User* loggedUser = it->get();

                // Rozpoznanie typu użytkownika i uruchomienie odpowiedniej sesji
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

    // Zapisanie wszystkich danych przed zakończeniem
    std::cout << "Zapisywanie danych...\n";
    dm.saveUsers(users);
    dm.saveTrains(trains);
    dm.saveTickets(tickets);

    return 0;
}