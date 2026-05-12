#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "codice.h"

/*
    Assegna la categoria e il prefisso associato in base alla scelta dell’utente.

    La funzione riceve:
    - un valore numerico (scelta)
    - una stringa categoria da riempire
    - una stringa prefix da riempire

    In base alla scelta, vengono impostati:
    - il nome della categoria
    - un prefisso alfanumerico utilizzato per la generazione del codice

    Mappatura:
    - 1 → illuminazione / LMN
    - 2 → buche / BCH
    - 3 → rifiuti / RFT
    - 4 → impianti / MPN
    - default → sconosciuta / UNK

    La funzione è dichiarata static per limitarne la visibilità al solo file
    in cui è definita.
*/
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

/*
    Genera un codice identificativo per una segnalazione.

    La funzione costruisce un codice univoco combinando:
    - prefisso della categoria
    - data corrente (formato YYYYMMDD)
    - numero casuale

    Procedura:
    - inizializza il generatore di numeri casuali una sola volta
    - ottiene categoria e prefisso tramite assegnaCategoria()
    - recupera la data corrente del sistema
    - genera un numero casuale per rendere il codice univoco
    - salva il valore numerico generato in idNumerico

    Formato finale del codice:
    PREFIX-YYYYMMDD-RANDOM

    Esempio:
    LMN-20260512-0423
*/
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