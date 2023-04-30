#include "libds/heap_monitor.h"
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
    //PrvaUroven prvaUroven(datovaUroven);
    //DruhaUroven druhaUroven(datovaUroven);
    TretiaUroven tretiaUroven(datovaUroven);

    return 0;
}
