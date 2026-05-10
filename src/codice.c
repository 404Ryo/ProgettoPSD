#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "codice.h"

// ===================== ASSEGNA CATEGORIA =====================
static void assegnaCategoria(int scelta, char categoria[], char prefix[]) {

    switch (scelta) {

        case 1:
            strcpy(categoria, "illuminazione");
            strcpy(prefix, "LMN");
            break;

        case 2:
            strcpy(categoria, "buche");
            strcpy(prefix, "BCH");
            break;

        case 3:
            strcpy(categoria, "rifiuti");
            strcpy(prefix, "RFT");
            break;

        case 4:
            strcpy(categoria, "impianti");
            strcpy(prefix, "MPN");
            break;

        default:
            strcpy(categoria, "sconosciuta");
            strcpy(prefix, "UNK");
            break;
    }
}

// ===================== GENERA CODICE =====================
void generaCodice(int sceltaCategoria,
                  char categoria[],
                  char outputCodice[],
                  int* idNumerico) {

    static int seedInizializzato = 0;
    if (!seedInizializzato) {
        srand((unsigned)time(NULL));
        seedInizializzato = 1;
    }

    char prefix[10];

    assegnaCategoria(sceltaCategoria, categoria, prefix);

    // data
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    char data[20];
    strftime(data, sizeof(data), "%Y%m%d", tm_info);

    // random più stabile
    int random = rand() % 10000 + 1;

    *idNumerico = random;

    // codice finale
    sprintf(outputCodice, "%s-%s-%04d", prefix, data, random);
}