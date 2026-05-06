#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segnalazione.h"
#include "account.h"
#include "utils.h"

// input string sicuro
void leggiStringa(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

int main() {

    int scelta;
    int isAdmin = 0;
    char username[50];

    // SAFE LOGIN
    do {
        screenClear();

        printf("1. Login\n");
        printf("2. Registrati\n");

        printf("Scelta: ");
        scelta = leggiIntero();

        if (scelta == 2) {
            signin();
            pause();
        }

    } while (scelta != 1 || !login(username, &isAdmin));

    Segnalazione* lista = caricaSegnalazioni();

    // LISTA VUOTA
    if (!lista) msgInfo("Lista vuota");

    int codice;
    char stato[50];

    getchar();
    // LOOP
    do {
        screenClear();

        if (isAdmin) menuAdmin();
        else menuUser();

        scelta = leggiIntero();

        switch (scelta) {

            case 1:
                lista = aggiungiSegnalazione(lista, username);
                break;

            case 2:
                stampaSegnalazioni(lista, username, isAdmin);
                break;

            case 3:
                printf("Codice: ");
                codice = leggiIntero();

                if (cercaPerCodice(lista, codice))
                    msgSuccess("Trovata");
                else
                    msgError("Non trovata");
                break;

            case 4:
                if (!isAdmin)
                    statoSegnalazioneUtente(lista, username);
                else {
                    screenClear();
                    cercaPerCategoria(lista);
                }
                break;

            case 5:
                if (isAdmin) {
                    aggiornaStato(lista, isAdmin);
                }
                break;

            case 6:
                if (isAdmin) {
                    printf("Stato: ");
                    leggiStringa(stato, 50);
                    stampaPerStato(lista, stato);
                }
                break;

            case 7:
                stampaUrgenti(lista);
                break;

            case 8:
                if (isAdmin) {
                    lista = eliminaSegnalazione(lista, isAdmin);
                }
                break;

            case 9:
                if (isAdmin)
                    generaReport(lista);
                break;
        }

        pause();

    } while (scelta != 0);

    return 0;
}