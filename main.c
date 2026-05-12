#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segnalazione.h"
#include "account.h"
#include "utils.h"
#include "colors.h"

int main() {

    int scelta;
    int isAdmin = 0;
    char username[50];

    screenClear();

    // SAFE LOGIN
    do {
        screenClear();

        printf(cyan  "========================================\n" reset);
        printf(cyan  "         SISTEMA DI ACCESSO\n" reset);
        printf(cyan  "========================================\n" reset);

        printf(green  "[1] Login\n" reset);
        printf(blue   "[2] Registrati\n" reset);

        printf(cyan "\nScelta -> " reset);

        scelta = leggiIntero();

        screenClear();

        if (scelta == 2) {
            signin();
            pause();
        }
    } while (scelta != 1 || !login(username, &isAdmin));


    Segnalazione* lista = caricaSegnalazioni();

    // LISTA VUOTA
    if (!lista) msgInfo("Lista vuota");

    // LOOP
    while(1) {

        if (isAdmin) menuAdmin();
        else menuUser();

        scelta = leggiIntero();

        if (scelta == 0) {
            return 0;
        }
        else{
            switch (scelta) {

                case 1:
                    screenClear();
                    lista = aggiungiSegnalazione(lista, username);
                    break;

                case 2:
                    screenClear();
                    stampaSegnalazioni(lista, username, isAdmin);
                    break;

                case 3:
                    screenClear();
                    ricercaPerCodice(lista);
                    break;

                case 4:
                    if (isAdmin) {
                        aggiornaStato(lista, isAdmin);
                    }
                    else {
                        statoSegnalazioneUtente(lista, username);
                    }
                    break;

                case 5:
                    if (isAdmin) {
                        screenClear();
                        cercaPerCategoria(lista);
                    }
                    break;

                case 6:
                    if (isAdmin) {

                        screenClear();

                        char* stato = ricercaPerStato();

                        stampaPerStato(lista, stato);
                    }
                    break;

                case 7:
                    if(isAdmin){
                        screenClear();
                        stampaUrgenti(lista);
                    }
                    break;

                case 8:
                    if (isAdmin) {
                        lista = eliminaSegnalazione(lista, isAdmin);
                    }
                    break;

                case 9:
                    if (isAdmin) {
                        screenClear();
                        generaReport(lista);
                    }
                    break;
                default:
                    msgError("Scelta non valida. Riprova");
            }

            pause();
        }
    }

    return 0;
}