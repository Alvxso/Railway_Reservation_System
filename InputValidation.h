/**
 * @file InputValidation.h
 * @brief Deklaracje funkcji do wczytywania i walidacji danych wejściowych od użytkownika.
 */


#pragma once
#include <string>

 /**
  * @brief Wczytuje liczbę całkowitą od użytkownika po wyświetleniu podanego komunikatu.
  *
  * @param prompt Komunikat do wyświetlenia użytkownikowi.
  * @param value Zmienna, do której zostanie zapisana wczytana liczba całkowita.
  * @return true Jeśli wczytanie powiodło się.
  * @return false Jeśli wczytanie nie powiodło się (np. niepoprawny format).
  */
bool readInt(const std::string& prompt, int& value);

/**
 * @brief Wczytuje liczbę zmiennoprzecinkową (double) od użytkownika po wyświetleniu podanego komunikatu.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param value Zmienna, do której zostanie zapisana wczytana liczba zmiennoprzecinkowa.
 * @return true Jeśli wczytanie powiodło się.
 * @return false Jeśli wczytanie nie powiodło się.
 */
bool readDouble(const std::string& prompt, double& value);

/**
 * @brief Wczytuje linię tekstu od użytkownika po wyświetleniu podanego komunikatu.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param line Zmienna, do której zostanie zapisana wczytana linia tekstu.
 * @return true Jeśli wczytanie powiodło się.
 * @return false Jeśli wczytanie nie powiodło się.
 */
bool readLine(const std::string& prompt, std::string& line);

/**
 * @brief Sprawdza, czy podany tekst reprezentuje poprawny czas (format i zakres).
 *
 * @param input Tekst do walidacji.
 * @return true Jeśli tekst jest poprawnym formatem czasu.
 * @return false W przeciwnym razie.
 */
bool validateTime(const std::string& input);

/**
 * @brief Sprawdza, czy podany tekst jest poprawnym imieniem lub nazwiskiem (np. bez cyfr i znaków specjalnych).
 *
 * @param input Tekst do walidacji.
 * @return true Jeśli tekst jest poprawnym imieniem/nazwiskiem.
 * @return false W przeciwnym razie.
 */
bool validateName(const std::string& input);

/**
 * @brief Wczytuje linię tekstu od użytkownika po wyświetleniu podanego komunikatu i waliduje ją podaną funkcją walidującą.
 *
 * @param prompt Komunikat do wyświetlenia użytkownikowi.
 * @param line Zmienna, do której zostanie zapisana wczytana linia tekstu.
 * @param validator Wskaźnik na funkcję walidującą, która sprawdza poprawność wczytanego tekstu.
 * @return true Jeśli wczytanie i walidacja powiodły się.
 * @return false W przeciwnym razie.
 */
bool readValidatedLine(const std::string& prompt, std::string& line, bool (*validator)(const std::string&));
