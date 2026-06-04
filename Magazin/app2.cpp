#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "shared/classes/Produs.h"
#include "shared/classes/TemplateUtils.h" 

const std::string FISIER_COS = "shared/files/cos_cumparaturi.txt";
const std::string FISIER_STOC = "shared/files/stoc.txt";
const std::string FISIER_COMENZI = "shared/files/comenzi.txt";

struct ElementCos {
    std::string cod_de_bare;
    int cantitate;
};

// --- FUNCTII PENTRU COS ---
std::vector<ElementCos> citireCos() {
    std::vector<ElementCos> cos;
    std::ifstream fin(FISIER_COS);
    std::string cod;
    int cant;
    while (fin >> cod >> cant) {
        cos.push_back({cod, cant});
    }
    return cos;
}

void salvareCos(const std::vector<ElementCos>& cos) {
    std::ofstream fout(FISIER_COS);
    for (const auto& item : cos) {
        fout << item.cod_de_bare << " " << item.cantitate << "\n";
    }
}

// --- FUNCTII PENTRU STOC ---
std::vector<Produs> citireStoc() {
    std::vector<Produs> stoc;
    std::ifstream fin(FISIER_STOC);
    if (!fin.is_open()) return stoc;

    int numar_produse;
    if (fin >> numar_produse) {
        std::string cod, nume;
        int cantitate;
        double pret;
        for (int i = 0; i < numar_produse; ++i) {
            fin >> cod >> nume >> cantitate >> pret;
            stoc.push_back(Produs(cod, nume, cantitate, pret));
        }
    }
    fin.close();
    return stoc;
}

void salvareStoc(const std::vector<Produs>& stoc) {
    std::ofstream fout(FISIER_STOC);
    fout << stoc.size() << "\n";
    for (const auto& p : stoc) {
        fout << p.getCod() << " " << p.getDenumire() << " " 
             << p.getCantitate() << " " << p.getPret() << "\n";
    }
    fout.close();
}

// --- PROGRAMUL PRINCIPAL ---
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Eroare: Nicio comanda introdusa.\n";
        return 1;
    }

    std::string comanda = argv[1];
    auto cos = citireCos();

    // 0. VIZUALIZARE STOC DIN APLICATIA CLIENTULUI
    if (comanda == "vizualizare_stoc") {
        auto stoc = citireStoc();
        afisareVector(stoc, "PRODUSE DISPONIBILE IN MAGAZIN");
    }
    // 1. VIZUALIZARE COS
    else if (comanda == "vizualizare_cos") {
        std::cout << "\n=== COSUL TAU ===\n";
        if(cos.empty()) std::cout << "Cosul este gol.\n";
        for (const auto& item : cos) {
            std::cout << "Cod: " << item.cod_de_bare << " | Cantitate: " << item.cantitate << "\n";
        }
    }
    // 2. ADAUGARE PRODUS IN COS
    else if (comanda == "adaugare_produs" && argc == 4) {
        std::string cod = argv[2];
        int cantitate = std::stoi(argv[3]);
        
        bool gasit = false;
        for (auto& item : cos) {
            if (item.cod_de_bare == cod) {
                item.cantitate += cantitate;
                gasit = true; break;
            }
        }
        if (!gasit) cos.push_back({cod, cantitate});
        
        salvareCos(cos);
        std::cout << "Produs adaugat temporar in cos.\n";
    }
    // 3. MODIFICARE CANTITATE IN COS
    else if (comanda == "modificare_produs" && argc == 4) {
        std::string cod = argv[2];
        int cantitate_noua = std::stoi(argv[3]);
        
        bool gasit = false;
        for (auto& item : cos) {
            if (item.cod_de_bare == cod) {
                item.cantitate = cantitate_noua;
                gasit = true; break;
            }
        }
        if (gasit) {
            salvareCos(cos);
            std::cout << "Cantitatea produsului din cos a fost modificata.\n";
        } else {
            std::cout << "Produsul cu codul " << cod << " nu a fost gasit in cos.\n";
        }
    }
    // 4. STERGERE PRODUS DIN COS
    else if (comanda == "stergere_produs" && argc == 3) {
        std::string cod = argv[2];
        
        auto it = std::remove_if(cos.begin(), cos.end(),
            [&cod](const ElementCos& item) { return item.cod_de_bare == cod; });
            
        if (it != cos.end()) {
            cos.erase(it, cos.end());
            salvareCos(cos);
            std::cout << "Produs eliminat cu succes din cos.\n";
        } else {
            std::cout << "Produsul nu a fost gasit in cos.\n";
        }
    }
    // 5. FINALIZARE COMANDA (CUMPARARE + UPDATE STOC)
    else if (comanda == "cumparare") {
        if (cos.empty()) {
            std::cout << "Eroare: Nu poti plasa o comanda cu un cos gol.\n";
            return 1;
        }

        auto stoc = citireStoc();
        bool eroare_stoc = false;


        for (const auto& item_cos : cos) {
            bool produs_gasit_in_stoc = false;
            for (const auto& produs_stoc : stoc) {
                if (produs_stoc.getCod() == item_cos.cod_de_bare) {
                    produs_gasit_in_stoc = true;
                    if (item_cos.cantitate > produs_stoc.getCantitate()) {
                        std::cout << "EROARE STOC: Produsul " << item_cos.cod_de_bare 
                                  << " (In stoc: " << produs_stoc.getCantitate() 
                                  << ", Dorit: " << item_cos.cantitate << ")\n";
                        eroare_stoc = true;
                    }
                    break;
                }
            }
            if (!produs_gasit_in_stoc) {
                std::cout << "EROARE: Produsul " << item_cos.cod_de_bare << " nu mai exista in magazin.\n";
                eroare_stoc = true;
            }
        }

  
        if (eroare_stoc) {
            std::cout << "\nComanda anulata! Te rugam sa modifici/stergi produsele lipsa din cos.\n";
            return 1;
        }

   
        for (const auto& item_cos : cos) {
            for (auto& produs_stoc : stoc) {
                if (produs_stoc.getCod() == item_cos.cod_de_bare) {
                    produs_stoc.setCantitate(produs_stoc.getCantitate() - item_cos.cantitate);
                    break;
                }
            }
        }
        salvareStoc(stoc); 


        std::ofstream fComenzi(FISIER_COMENZI, std::ios::app); 
        fComenzi << "12/06/2026\nProduse cumparate: ";
        for (const auto& item : cos) {
            fComenzi << item.cod_de_bare << " (x" << item.cantitate << ") ";
        }
        fComenzi << "\n--------------------------\n";
        fComenzi.close();

        std::ofstream curataCos(FISIER_COS);
        curataCos.close();

        std::cout << "Comanda a fost plasata cu succes! Stocul magazinului a fost actualizat automat.\n";
    }
    else {
        std::cerr << "Comanda invalida. Sintaxe valide pentru client:\n";
        std::cerr << "  ./app2.exe vizualizare_stoc\n";
        std::cerr << "  ./app2.exe vizualizare_cos\n";
        std::cerr << "  ./app2.exe adaugare_produs <cod> <cant>\n";
        std::cerr << "  ./app2.exe modificare_produs <cod> <cant_noua>\n";
        std::cerr << "  ./app2.exe stergere_produs <cod>\n";
        std::cerr << "  ./app2.exe cumparare\n";
    }

    return 0;
}