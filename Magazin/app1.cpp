#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "shared/classes/Produs.h"
#include "shared/classes/TemplateUtils.h"

// Calea catre fisierul de stoc
const std::string FISIER_STOC = "shared/files/stoc.txt";
const std::string FISIER_COMENZI = "shared/files/comenzi.txt";

// Functie pentru citirea stocului din fisier

std::unique_ptr<std::vector<Produs>> citireStoc() {
    auto stoc = std::make_unique<std::vector<Produs>>();
    std::ifstream fin(FISIER_STOC);
    if (!fin.is_open()) return stoc;

    int numar_produse;
    if (fin >> numar_produse) {
        std::string cod, nume;
        int cantitate;
        double pret;
        for (int i = 0; i < numar_produse; ++i) {
            fin >> cod >> nume >> cantitate >> pret;
            stoc->push_back(Produs(cod, nume, cantitate, pret));
        }
    }
    fin.close();
    return stoc;
}

// Functie pentru salvarea stocului in fisier
void salvareStoc(const std::vector<Produs>& stoc) {
    std::ofstream fout(FISIER_STOC);
    fout << stoc.size() << "\n";
    for (const auto& p : stoc) {
        fout << p.getCod() << " " << p.getDenumire() << " " 
             << p.getCantitate() << " " << p.getPret() << "\n";
    }
    fout.close();
}

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Eroare: Nicio comanda introdusa.\n";
        return 1;
    }

    std::string comanda = argv[1];
    auto stoc_ptr = citireStoc();

    // 1. VIZUALIZARE PRODUSE
    if (comanda == "vizualizare_produse_de_pe_stoc") {
        afisareVector(*stoc_ptr, "STOC MAGAZIN");
    } 
    // 2. ADAUGARE PRODUS
    else if (comanda == "adaugare_produs" && argc == 6) {
        std::string cod = argv[2];
        std::string nume = argv[3];
        int cantitate = std::stoi(argv[4]);
        double pret = std::stod(argv[5]);

        bool gasit = false;
        for (auto& p : *stoc_ptr) {
            if (p.getCod() == cod) {
                p.setCantitate(p.getCantitate() + cantitate);
                gasit = true; break;
            }
        }
        if (!gasit) {
            stoc_ptr->push_back(Produs(cod, nume, cantitate, pret));
        }
        salvareStoc(*stoc_ptr);
        std::cout << "Produs adaugat/actualizat cu succes.\n";
    }
    // 3. STERGERE PRODUS
    else if (comanda == "stergere_produs" && argc == 3) {
        std::string cod = argv[2];
        
        
        auto it = std::remove_if(stoc_ptr->begin(), stoc_ptr->end(), 
            [&cod](const Produs& p) { return p.getCod() == cod; });
        
        if (it != stoc_ptr->end()) {
            stoc_ptr->erase(it, stoc_ptr->end());
            salvareStoc(*stoc_ptr);
            std::cout << "Produs sters cu succes.\n";
        } else {
            std::cout << "Produsul nu a fost gasit.\n";
        }
    }
    // 4. MODIFICARE PRODUS (Cantitate sau Pret)
    else if (comanda == "modificare_produs" && argc == 5) {
        std::string tip_modificare = argv[2]; 
        std::string cod = argv[3];
        std::string valoare_noua = argv[4];

        bool gasit = false;
        for (auto& p : *stoc_ptr) {
            if (p.getCod() == cod) {
                gasit = true;
                if (tip_modificare == "cantitate") {
                    p.setCantitate(std::stoi(valoare_noua));
                    std::cout << "Cantitatea produsului " << cod << " a fost modificata cu succes.\n";
                } 
                else if (tip_modificare == "pret") {
                    p.setPret(std::stod(valoare_noua)); 
                    std::cout << "Pretul produsului " << cod << " a fost modificat cu succes.\n";
                } 
                else {
                    std::cerr << "Eroare: Tip de modificare invalid. Folositi 'pret' sau 'cantitate'.\n";
                    return 1;
                }
                break; 
            }
        }

        if (gasit) {
            salvareStoc(*stoc_ptr);
        } else {
            std::cerr << "Eroare: Produsul cu codul " << cod << " nu exista in stoc.\n";
        }
    }
    // 5. VIZUALIZARE COMENZI
    else if (comanda == "vizualizare_comenzi") {
        std::ifstream fin(FISIER_COMENZI);
        if (!fin.is_open()) {
            std::cout << "Nu exista nicio comanda inregistrata in sistem.\n";
        } else {
            std::cout << "\n=== ISTORIC COMENZI PLASATE ===\n";
            std::string linie;
            while (std::getline(fin, linie)) {
                std::cout << linie << "\n";
            }
            fin.close();
            std::cout << "===============================\n";
        }
    }
    // COMANDA INVALIDA SAU LIPSESC ARGUMENTE
    else {
        std::cerr << "Eroare: Comanda invalida sau argumente insuficiente.\n";
        std::cerr << "Sintaxe valide:\n";
        std::cerr << " ./app1.exe vizualizare_produse_de_pe_stoc\n";
        std::cerr << " ./app1.exe adaugare_produs <cod> <nume> <cantitate> <pret>\n";
        std::cerr << " ./app1.exe stergere_produs <cod>\n";
        std::cerr << " ./app1.exe modificare_produs <pret|cantitate> <cod> <valoare>\n";
        std::cerr << " ./app1.exe vizualizare_comenzi\n";
    }

    return 0;
}