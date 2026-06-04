#ifndef COMANDA_H
#define COMANDA_H

#include "Produs.h"
#include "Data.h"
#include <vector>
#include <iostream>

class Comanda {
private:
    std::vector<Produs> produse; // Relație de Agregare/Compoziție
    Data data_comanda;           // Relație de Compoziție

public:
    Comanda(const std::vector<Produs>& p, const Data& d) 
        : produse(p), data_comanda(d) {}

    friend std::ostream& operator<<(std::ostream& os, const Comanda& c) {
        os << "Data comenzii: " << c.data_comanda << "\nProduse:\n";
        for (const auto& p : c.produse) {
            os << "  - " << p.getCod() << " (Cantitate: " << p.getCantitate() << ")\n";
        }
        return os;
    }
};

#endif