#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "shared/classes/Produs.h"

const std::string FISIER_COS = "shared/files/cos_cumparaturi.txt";
const std::string FISIER_STOC = "shared/files/stoc.txt";
const std::string FISIER_COMENZI = "shared/files/comenzi.txt";


struct ElementCos {
    std::string cod_de_bare;
    int cantitate;
};

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Eroare: Nicio comanda introdusa.\n";
        return 1;
    }

    std::string comanda = argv[1];
    auto cos = citireCos();

    if (comanda == "vizualizare_cos") {
        std::cout << "\n=== COSUL TAU ===\n";
        if(cos.empty()) std::cout << "Cosul este gol.\n";
        for (const auto& item : cos) {
            std::cout << "Cod: " << item.cod_de_bare << " | Cantitate: " << item.cantitate << "\n";
        }
    }
    else if (comanda == "adaugare_produs" && argc == 4) {
        std::string cod = argv[2];
        int cantitate = std::stoi(argv[3]);
        
        // Verificam daca e deja in cos
        bool gasit = false;
        for (auto& item : cos) {
            if (item.cod_de_bare == cod) {
                item.cantitate += cantitate;
                gasit = true; break;
            }
        }
        if (!gasit) cos.push_back({cod, cantitate});
        
        salvareCos(cos);
        std::cout << "Produs adaugat in cos.\n";
    }
    else if (comanda == "cumparare") {
        if (cos.empty()) {
            std::cout << "Nu poti plasa o comanda cu un cos gol.\n";
            return 1;
        }

        
        std::ofstream fComenzi(FISIER_COMENZI, std::ios::app); 
        fComenzi << "12 06 2026\n"; 
        for (const auto& item : cos) {
            fComenzi << item.cod_de_bare << " ";
        }
        fComenzi << "\n";
        fComenzi.close();

        
        std::ofstream curataCos(FISIER_COS);
        curataCos.close();

        std::cout << "Comanda a fost plasata cu succes! Cosul a fost golit.\n";
    }
    else {
        std::cerr << "Comanda invalida pentru client.\n";
    }

    return 0;
}