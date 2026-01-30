# System Rezerwacji Kolejowej

System rezerwacji biletów kolejowych napisany w C++ z interfejsem konsolowym. Aplikacja umożliwia zarządzanie pociągami, użytkownikami oraz rezerwacjami biletów.

## 📋 Opis projektu

System Rezerwacji Kolejowej to aplikacja konsolowa zaprojektowana do zarządzania rezerwacjami miejsc w pociągach. Obsługuje dwa typy użytkowników:
- **Administratorzy** - zarządzają pociągami i generują raporty
- **Pasażerowie** - wyszukują połączenia, rezerwują, modyfikują i anulują bilety

Dane są przechowywane w plikach YAML, co zapewnia trwałość informacji między sesjami programu.

## ✨ Funkcjonalności

### Dla Administratora:
- ➕ Dodawanie nowych pociągów do systemu
- ❌ Usuwanie pociągów (z automatycznym anulowaniem powiązanych biletów)
- 📊 Generowanie raportów systemowych (statystyki użytkowników, pociągów, biletów i przychodów)

### Dla Pasażera:
- 🔍 Zaawansowane wyszukiwanie połączeń kolejowych (po stacji, dacie)
- 🎫 Rezerwacja biletów z wyborem miejsca i klasy podróży (I lub II klasa)
- 📋 Przeglądanie aktywnych rezerwacji
- ✏️ Modyfikacja rezerwacji (zmiana miejsca lub całkowite przebookowanie)
- 🗑️ Anulowanie rezerwacji

### Funkcje ogólne:
- 👤 Rejestracja nowych pasażerów
- 🔐 System logowania z walidacją danych
- 💾 Automatyczne zapisywanie danych do plików YAML
- ✅ Walidacja wprowadzanych danych

## 🏗️ Struktura projektu

Projekt składa się z następujących klas i plików:

### Klasy główne:

#### `User` (User.h, User.cpp)
- Abstrakcyjna klasa bazowa dla wszystkich użytkowników
- Przechowuje login, hasło i ID użytkownika
- Definiuje interfejs dla klas pochodnych

#### `Admin` (Admin.h, Admin.cpp)
- Dziedziczy po `User`
- Implementuje funkcjonalności administracyjne:
  - Dodawanie pociągów
  - Usuwanie pociągów
  - Generowanie raportów

#### `Passenger` (Passenger.h, Passenger.cpp)
- Dziedziczy po `User`
- Implementuje funkcjonalności pasażera:
  - Rezerwacja biletów z zaawansowanym wyszukiwaniem
  - Przeglądanie rezerwacji
  - Modyfikacja i anulowanie rezerwacji

#### `Train` (Train.h, Train.cpp)
- Reprezentuje pociąg w systemie
- Zarządza miejscami (rezerwacja, anulowanie)
- Przechowuje informacje o trasie i dacie
- Wyświetla wizualną mapę zajętości miejsc

#### `Ticket` (Ticket.h, Ticket.cpp)
- Reprezentuje bilet
- Przechowuje informacje o pasażerze, pociągu, miejscu i cenie
- Umożliwia modyfikację numeru miejsca

#### `DataManager` (DataManager.h, DataManager.cpp)
- Zarządza trwałością danych
- Zapisuje i wczytuje dane z plików YAML:
  - `users.yaml` - użytkownicy
  - `trains.yaml` - pociągi
  - `tickets.yaml` - bilety

#### `InputValidation` (InputValidation.h, InputValidation.cpp)
- Zapewnia bezpieczne wczytywanie danych od użytkownika
- Waliduje wprowadzane wartości (liczby, tekst)
- Obsługuje błędy wejścia

### Plik główny:

#### `main.cpp`
- Punkt wejścia aplikacji
- Implementuje główną pętlę programu
- Obsługuje logowanie i rejestrację
- Zarządza sesjami użytkowników

## 🚀 Kompilacja i uruchomienie

### Wymagania:
- Kompilator C++ obsługujący standard C++14 lub nowszy
- Visual Studio 2019/2022 (dla Windows) lub g++/clang++ (dla Linux/macOS)

### Kompilacja w Visual Studio:
1. Otwórz plik `Railway_Managment.sln`
2. Wybierz konfigurację (Debug/Release)
3. Naciśnij F7 lub wybierz Build → Build Solution
4. Uruchom aplikację (F5 lub Ctrl+F5)

### Kompilacja w wierszu poleceń (g++):
```bash
g++ -std=c++14 -o railway_system main.cpp Admin.cpp Passenger.cpp Train.cpp Ticket.cpp DataManager.cpp InputValidation.cpp
./railway_system
```

## 📖 Instrukcja użytkowania

### Pierwsze uruchomienie:
Przy pierwszym uruchomieniu system automatycznie utworzy konto administratora:
- **Login**: `admin`
- **Hasło**: `admin`

### Logowanie:
1. Wybierz opcję "Zaloguj się" z menu głównego
2. Wprowadź login i hasło
3. System rozpozna typ użytkownika i wyświetli odpowiednie menu

### Rejestracja (dla pasażerów):
1. Wybierz opcję "Zarejestruj się"
2. Wprowadź unikalny login
3. Wprowadź hasło (min. 3 znaki)
4. Konto zostanie utworzone i zapisane

### Rezerwacja biletu (pasażer):
1. Zaloguj się jako pasażer
2. Wybierz "Wyszukaj i zarezerwuj bilet"
3. Wybierz metodę wyszukiwania (po stacji, dacie lub wszystkie)
4. Wyświetli się lista dostępnych połączeń
5. Wybierz pociąg i numer miejsca
6. Wybierz klasę podróży (I lub II)
7. Potwierdź rezerwację

### Dodawanie pociągu (administrator):
1. Zaloguj się jako administrator
2. Wybierz "Dodaj nowy pociąg"
3. Wprowadź:
   - Unikalne ID pociągu
   - Stację początkową
   - Stację końcową
   - Datę (RRRR-MM-DD)
   - Liczbę miejsc (max 100)
4. Pociąg zostanie dodany do systemu

## 💾 Format danych YAML

### users.yaml
```yaml
---
type: ADMIN
id: 1
login: admin
password: admin
---
type: PASSENGER
id: 2
login: jan_kowalski
password: haslo123
```

### trains.yaml
```yaml
---
id: 1
origin: Warszawa
destination: Kraków
date: 2024-12-25
capacity: 50
occupied: 1,5,12,23
```

### tickets.yaml
```yaml
---
id: 1
trainId: 1
passenger: jan_kowalski
seat: 5
price: 89.50
```

## 🎯 Kluczowe algorytmy

### System cenowy:
- Cena bazowa: 40 PLN + (długość nazwy stacji docelowej × 5 PLN)
- II klasa: cena bazowa
- I klasa: cena bazowa × 1.5

### Zarządzanie miejscami:
- Miejsca numerowane od 1 do pojemności pociągu
- Wektor boolean przechowuje stan zajętości
- Wyświetlanie w układzie 4 miejsca na rząd

### Wyszukiwanie:
- Filtrowanie bez rozróżniania wielkości liter
- Możliwość kombinowania kryteriów (stacja + data)
- Wyświetlanie wyników w formacie tabeli

## 🔒 Bezpieczeństwo

- Walidacja wszystkich danych wejściowych
- Sprawdzanie unikalności loginów i ID
- Ochrona przed nieprawidłowymi wartościami
- Obsługa błędów wejścia/wyjścia plików

## 📝 Autor

Projekt stworzony jako system rezerwacji kolejowej z wykorzystaniem programowania obiektowego w C++.

## 📄 Licencja

Projekt edukacyjny - Railway Reservation System.