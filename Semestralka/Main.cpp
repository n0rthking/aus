#include "libds/heap_monitor.h"
//#include "PrvaUroven.h"
#include "DruhaUroven.h"
#include <windows.h>

int main() {
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    //PrvaUroven prvaUroven;
    DruhaUroven druhaUroven;

    return 0;
}
