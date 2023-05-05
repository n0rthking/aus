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
    int cisloUrovne = 0;

    while (cisloUrovne != 42) {
        std::cout << "Zadaj cislo urovne: ";
        std::cin >> cisloUrovne;
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
        else if (cisloUrovne == 4) {
            return 2;
        }
        else if (cisloUrovne == 42) {
            std::cout << "\nthe meaning of life\n";
            return 42;
        }
        else {
            std::cout << "Nespravne cislo urovne\n";
        }
    }

    return 0;
}
