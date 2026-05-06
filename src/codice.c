#include <string.h>
#include "codice.h"

int generaCodice(int sceltaCategoria, char categoria[]) {
    static int counter10 = 1000;
    static int counter20 = 2000;
    static int counter30 = 3000;
    static int counter40 = 4000;

    switch (sceltaCategoria) {

        case 1:
            strcpy(categoria, "illuminazione");
            return counter10++;

        case 2:
            strcpy(categoria, "buche");
            return counter20++;

        case 3:
            strcpy(categoria, "rifiuti");
            return counter30++;

        case 4:
            strcpy(categoria, "impianti");
            return counter40++;

        default:
            strcpy(categoria, "sconosciuta");
            return 99999;
    }
}