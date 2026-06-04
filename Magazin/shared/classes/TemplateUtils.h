#ifndef TEMPLATE_UTILS_H
#define TEMPLATE_UTILS_H

#include <iostream>
#include <vector>

// Functie template pentru afisarea oricarui vector de elemente
template <typename T>
void afisareVector(const std::vector<T>& colectie, const std::string& titlu) {
    std::cout << "\n=== " << titlu << " ===\n";
    if (colectie.empty()) {
        std::cout << "Nu exista inregistrari.\n";
        return;
    }
    for (const auto& element : colectie) {
        std::cout << element << "\n";
    }
    std::cout << "===================\n";
}

#endif