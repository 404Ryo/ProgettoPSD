#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "colors.h"
#include "segnalazione.h"

/*
    Pulisce lo schermo della console.

    La funzione utilizza un comando di sistema diverso in base al sistema operativo:
    - su Windows (_WIN32) esegue "cls"
    - su sistemi Unix/Linux/macOS esegue "clear"

    Questo permette di mantenere il programma portabile tra ambienti diversi
    senza modificare il codice principale.
*/
void screenClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
    Legge una stringa da input standard in modo sicuro.

    La funzione utilizza fgets per evitare overflow del buffer e gestisce:
    - input troppo lungo (svuota il buffer residuo e richiede reinserimento)
    - input vuoto (stringa vuota non valida)
    - rimozione automatica del carattere newline finale

    Il ciclo continua finché l’utente non inserisce una stringa valida.
*/
void leggiStringa(char* buffer, int size) {

    while (1) {

        if (!fgets(buffer, size, stdin))
            continue;

        // se NON c'è '\n', input troppo lungo → svuota buffer
        if (strchr(buffer, '\n') == NULL) {

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            msgError("Input troppo lungo");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            msgError("Input non valido");
            continue;
        }

        return;
    }
}

/*
    Legge un intero da input standard in modo sicuro.

    La funzione utilizza un buffer temporaneo e fgets per evitare problemi di overflow.
    L’input viene poi validato tramite sscanf per verificare che sia un numero intero valido.

    Se l’input non è valido, viene mostrato un messaggio di errore e richiesto nuovamente
    l’inserimento. Il ciclo continua finché non viene inserito un valore corretto.
*/
int leggiIntero() {
    char buffer[100];
    int valore;

    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin))
            continue;

        if (sscanf(buffer, "%d", &valore) == 1)
            return valore;

        msgError("Input non valido");
        printf(cyan "Inserisci un numero: " reset);
    }
}

/*
    Mette in pausa l’esecuzione del programma fino alla pressione di INVIO.

    La funzione:
    - mostra un messaggio all’utente
    - attende l’input da tastiera
    - gestisce eventuali caratteri residui nel buffer di input
    - pulisce lo schermo prima di riprendere l’esecuzione

    Serve per permettere all’utente di leggere l’output prima del refresh della schermata.
*/
void pause() {
    printf("\nPremi INVIO per continuare...");
    fflush(stdout);

    int c;
    c = getchar();
    if (c != '\n' && c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    screenClear();
}

// ================= TITLE =================
void title() {
    printf(blue);
    printf("=====================================\n");
    printf("   SISTEMA SEGNALAZIONI COMUNE\n");
    printf("=====================================\n");
    printf(reset);
}

// ================= MENU USER =================
void menuUser() {
    title();

    printf(green  "[1] Inserisci segnalazione\n" reset);
    printf(cyan   "[2] Visualizza le mie segnalazioni\n" reset);
    printf(yellow "[3] Cerca codice\n" reset);
    printf(blue   "[4] Stato segnalazione\n" reset);
    printf(red    "[0] Esci\n" reset);

    printf("\nScelta -> ");
}

// ================= MENU ADMIN =================
void menuAdmin() {
    title();

    printf(green  "[1] Inserisci segnalazione\n" reset);
    printf(cyan   "[2] Visualizza tutte le segnalazioni\n" reset);
    printf(yellow "[3] Cerca codice\n" reset);
    printf(purple "[4] Aggiorna stato\n" reset);
    printf(blue   "[5] Filtra Categoria\n" reset);
    printf(green  "[6] Filtra stato\n" reset);
    printf(cyan   "[7] Urgenti\n" reset);
    printf(red    "[8] Elimina segnalazione\n" reset);
    printf(yellow "[9] Report\n" reset);
    printf(white  "[0] Esci\n" reset);

    printf("\nScelta -> ");
}

/*
    Permette di selezionare uno stato di una segnalazione tramite menu.

    La funzione mostra un elenco di stati disponibili e richiede all’utente
    di inserire una scelta valida.

    Comportamento:
    - Se l’utente inserisce 0, la funzione ritorna "EXIT" per annullare l’operazione.
    - Se la scelta è valida (1–3), viene restituita una stringa corrispondente allo stato:
        1 -> "aperta"
        2 -> "in lavorazione"
        3 -> "chiuso"
    - In caso di input non valido, viene mostrato un messaggio di errore e la richiesta 
    viene ripetuta.
*/
char* ricercaPerStato() {
    int scelta;

    printf(cyan "====================================\n" reset);
    printf(cyan "           FILTRO STATO\n" reset);
    printf(cyan "====================================\n" reset);

    printf(green  "[1] Aperta\n" reset);
    printf(yellow "[2] In lavorazione\n" reset);
    printf(red    "[3] chiuso\n" reset);
    printf(cyan   "[0] Esci\n" reset);
    printf(cyan "====================================\n" reset);

    while (1) {
        printf(cyan "Scelta -> " reset);

        scelta = leggiIntero();

        if (scelta == 0)
            return "EXIT";

        if (scelta >= 1 && scelta <= 3)
            break;

        msgError("Scelta non valida");
        
    }

    screenClear();
    
    switch (scelta) {
        case 1: return "aperta";
        case 2: return "in lavorazione";
        case 3: return "chiuso";
    }

    return NULL;
}

/*
    Ricerca una segnalazione tramite codice identificativo.

    La funzione richiede all’utente l’inserimento di un codice numerico e ne valida l’input.
    Se il codice non è valido (<= 0), viene mostrato un messaggio di errore e la funzione termina.

    Se il codice è valido:
    - viene cercata la segnalazione nella lista
    - se non trovata, viene mostrato un messaggio di errore
    - se trovata, viene mostrato un messaggio di successo e stampati i dettagli
*/
void ricercaPerCodice(Segnalazione* lista) {

    printf(cyan "\n====================================\n" reset);
    printf(cyan "        RICERCA SEGNALAZIONE\n" reset);
    printf(cyan "====================================\n" reset);

    printf(yellow "Inserisci codice -> " reset);

    int codice = leggiIntero();

    if (codice <= 0) {
        msgError("Codice non valido");
        return;
    }

    screenClear();

    Segnalazione* head = cercaPerCodice(lista, codice);

    if (!head) {
        msgError("Segnalazione non trovata");
        return;
    }

    msgSuccess("Segnalazione trovata");

    printf(cyan "\n====================================\n" reset);
    printf(cyan "          DETTAGLIO\n" reset);
    printf(cyan "====================================\n" reset);

    stampaSegnalazione(head);
}

// ================= MESSAGGI =================
void msgSuccess(const char* testo) {
    printf(green "[SUCCESSO] %s\n" reset, testo);
}

void msgError(const char* testo) {
    printf(red "[ERRORE] %s\n" reset, testo);
}

void msgInfo(const char* testo) {
    printf(blue "[INFO] %s\n" reset, testo);
}

// ================= SALVA FILE =================
void salvaTutto(Segnalazione* head) {
    FILE* f = fopen("file/segnalazioni.txt", "w");
    if (!f) {
        msgError("Errore salvataggio file");
        return;
    }

    while (head) {
        fprintf(f, "%d|%s|%s|%s|%s|%s|%d|%s\n",
            head->codice,
            head->codiceCompleto,
            head->utente,
            head->categoria,
            head->descrizione,
            head->data,
            head->urgenza,
            head->stato
        );

        head = head->next;
    }

    fclose(f);
}

// ================= CONTROLLO CODICE =================
int codiceEsiste(Segnalazione* head, int codice) {
    while (head) {
        if (head->codice == codice)
            return 1;
        head = head->next;
    }
    return 0;
}