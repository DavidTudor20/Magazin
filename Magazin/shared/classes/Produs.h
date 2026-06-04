#ifndef PRODUS_H
#define PRODUS_H

#include <string>
#include <iostream>
#include <iomanip>

class Produs {
private:
    std::string cod_de_bare;
    std::string denumire;
    int cantitate;
    double pret;

public:
    // Constructor
    Produs(std::string cod, std::string nume, int cant, double p) 
        : cod_de_bare(cod), denumire(nume), cantitate(cant), pret(p) {}

    // Getters
    std::string getCod() const { return cod_de_bare; }
    std::string getDenumire() const { return denumire; }
    int getCantitate() const { return cantitate; }
    double getPret() const { return pret; }

    // Setters
    void setCantitate(int cant) { cantitate = cant; }
    void setPret(double p) { pret = p; }


    friend std::ostream& operator<<(std::ostream& os, const Produs& p) {
        os << std::left << std::setw(15) << p.cod_de_bare 
           << std::setw(20) << p.denumire 
           << std::setw(10) << p.cantitate 
           << p.pret << " RON";
        return os;
    }
};

#endif