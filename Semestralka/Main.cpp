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

    DatovaUroven datovaUroven(true);
    //PrvaUroven prvaUroven;
    DruhaUroven druhaUroven(datovaUroven);
    TretiaUroven tretiaUroven(datovaUroven);

    return 0;
}
