#include <libds/heap_monitor.h>
#include "DatovaUroven.h"
#include "PrvaUroven.h"
#include "DruhaUroven.h"
#include "TretiaUroven.h"
#include <windows.h>

int main() {
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    DatovaUroven datovaUroven("kraje.csv", "okresy.csv", "obce.csv");
    PrvaUroven prvaUroven(datovaUroven);
    DruhaUroven druhaUroven(datovaUroven);
    TretiaUroven tretiaUroven(datovaUroven);
    int cisloUrovne = 42;

    while (cisloUrovne != 0) {
        std::cout << "Zadaj cislo urovne [1-3] alebo 0 pre koniec: ";
        std::string vstup;
        std::cin >> vstup;
        cisloUrovne = std::isdigit(vstup.at(0)) ? std::stoi(vstup.substr(0, 1)) : 42;
        std::cin.ignore(256, '\n');

        if (cisloUrovne == 1) {
            prvaUroven.spustiUroven();
        }
        else if (cisloUrovne == 2) {
            druhaUroven.RozhranieIteratora();
        }
        else if (cisloUrovne == 3) {
            tretiaUroven.spustiUroven();
        }
        else if (cisloUrovne == 0) {
            std::cout << "Koniec\n";
        }
        else {
            std::cout << "\x1B[31mNespravne cislo urovne\033[0m\n";
        }
    }

    return 0;
}
