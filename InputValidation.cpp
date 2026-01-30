/**
 * @file InputValidation.cpp
 * @brief Implementacja funkcji do bezpiecznego wczytywania i walidacji danych wejściowych.
 */

#include "InputValidation.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

 /**
  * @brief Wczytuje liczbę całkowitą od użytkownika z powtarzaniem próby aż do poprawnego wprowadzenia.
  *
  * @param prompt Komunikat do wyświetlenia użytkownikowi.
  * @param value Referencja do zmiennej, w której zostanie zapisana wczytana liczba.
  * @return true Po poprawnym wczytaniu liczby całkowitej.
  */
bool readInt(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nieprawidlowy format. Sprobuj ponownie.\n";
        }
    }
}

/**
 * @brief Wczytuje liczbę zmiennoprzecinkową od użytkownika z powtarzaniem próby aż do poprawnego wprowadzenia.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param value Referencja do zmiennej, w której zostanie zapisana wczytana liczba.
 * @return true Po poprawnym wczytaniu liczby zmiennoprzecinkowej.
 */
bool readDouble(const std::string& prompt, double& value) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nieprawidlowy format. Sprobuj ponownie.\n";
        }
    }
}

/**
 * @brief Wczytuje linię tekstu od użytkownika po wyświetleniu podanego komunikatu.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param line Referencja do zmiennej, w której zostanie zapisana wczytana linia.
 * @return true Zawsze zwraca true (brak walidacji na tym etapie).
 */
bool readLine(const std::string& prompt, std::string& line) {
    std::cout << prompt;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::getline(std::cin, line);
    return true;
}

/**
 * @brief Waliduje, czy podany tekst jest poprawnym imieniem lub nazwiskiem.
 * Dozwolone są tylko litery, spacje oraz znak '-'.
 *
 * @param input Tekst do walidacji.
 * @return true Jeśli tekst jest poprawny.
 * @return false W przeciwnym wypadku.
 */
bool validateName(const std::string& input) {
    if (input.empty()) return false;
    for (char c : input) {
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ' && c != '-') {
            return false;
        }
    }
    return true;
}

/**
 * @brief Waliduje, czy podany tekst jest poprawnym formatem czasu (cyfry i znak ':').
 *
 * @param input Tekst do walidacji.
 * @return true Jeśli tekst zawiera tylko cyfry i dwukropki.
 * @return false W przeciwnym wypadku.
 */
bool validateTime(const std::string& input) {
    if (input.empty()) return false;
    for (char c : input) {
        if (!std::isdigit(static_cast<unsigned char>(c)) && c != ':') {
            return false;
        }
    }
    return true;
}

/**
 * @brief Wczytuje linię tekstu i waliduje ją za pomocą podanej funkcji walidującej.
 * Powtarza wczytywanie aż do momentu, gdy walidacja przejdzie pomyślnie.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param line Referencja do zmiennej, w której zostanie zapisana wczytana linia.
 * @param validator Wskaźnik na funkcję walidującą tekst.
 * @return true Po pomyślnej walidacji.
 */
bool readValidatedLine(const std::string& prompt, std::string& line, bool (*validator)(const std::string&)) {
    do {
        readLine(prompt, line);
        if (!validator(line)) {
            std::cout << "Niepoprawny format. Sprobuj ponownie.\n";
        }
        else {
            return true;
        }
    } while (true);
}
