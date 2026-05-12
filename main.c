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

/*
    Loop principale del sistema di autenticazione.

    Mostra un menu iniziale che permette all’utente di:
    - effettuare il login
    - registrarsi a un nuovo account

    Il ciclo continua finché:
    - l’utente non seleziona il login (scelta == 1)
    - e le credenziali inserite risultano valide (login() restituisce successo)

    Se l’utente sceglie la registrazione (opzione 2), viene eseguita la funzione
    signin() e successivamente il sistema torna al menu dopo una pausa.

    Questo approccio garantisce che l’accesso al sistema principale sia consentito
    solo dopo autenticazione corretta.
*/
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

    if (!lista) msgInfo("Lista vuota");
    
/*
    Ciclo principale del sistema di gestione segnalazioni.

    Il programma resta in esecuzione in un loop infinito e mostra un menu diverso
    in base al ruolo dell’utente:
    - menuAdmin() se l’utente è amministratore
    - menuUser() se l’utente è un utente standard

    L’utente inserisce una scelta che viene gestita tramite switch-case.

    Funzionalità principali:
    - Utente:
        - (1) Inserimento nuova segnalazione;
        - (2) Visualizzare TUTTE le proprie segnalazioni;
        - (3) Ricerca segnalazione per codice;
        - (4) Visualizzare stato segnalazione personale;
    - Amministratore:
        - (1) Inserimento nuova segnalazione
        - (2) Visualizzare TUTTE le segnalazioni (utente/admin)
        - (3) Ricerca segnalazione per codice
        - (4) Aggiornare stato di una segnalazione
        - (5) Filtrare le segnalazioni per categoria
        - (6) Filtrare le segnalazioni per stato
        - (7) Visualizzare segnalazioni urgenti
        - (8) Eliminare una segnalazione
        - (9) Generare il report completo

    Il programma termina all'inserimento di "0"

    Dopo ogni operazione viene effettuata una pausa per permettere la lettura
    dell’output prima del refresh del menu.
*/
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