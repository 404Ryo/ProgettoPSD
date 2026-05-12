#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "colors.h"
#include "segnalazione.h"

// ================= SCREEN =================
void screenClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ================= INPUT CHAR SICURO =================
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

// ================= INPUT INTERO SICURO =================
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

// ================= PAUSA SICURA =================
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

// ================= RICERCA PER STATO =================
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

// ================= RICERCA PER CODICE =================
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