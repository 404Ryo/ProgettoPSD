#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "colors.h"
#include "segnalazione.h"

// ================= SCREEN =================
void screenClear() {
    printf("\033[2J\033[H");
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

        printf(red "Input non valido.\nInserisci un numero: " reset);
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

    printf(green  "1. Inserisci segnalazione\n" reset);
    printf(cyan   "2. Visualizza le mie segnalazioni\n" reset);
    printf(yellow "3. Cerca codice\n" reset);
    printf(blue   "4. Stato segnalazione\n" reset);
    printf(red    "0. Esci\n" reset);

    printf("\nScelta: ");
}

// ================= MENU ADMIN =================
void menuAdmin() {
    title();

    printf(green  "1. Inserisci segnalazione\n" reset);
    printf(cyan   "2. Visualizza tutte le segnalazioni\n" reset);
    printf(yellow "3. Cerca codice\n" reset);
    printf(purple "4. Categoria\n" reset);
    printf(blue   "5. Aggiorna stato\n" reset);
    printf(green  "6. Filtra stato\n" reset);
    printf(cyan   "7. Urgenti\n" reset);
    printf(red    "8. Elimina segnalazione\n" reset);
    printf(yellow "9. Report\n" reset);
    printf(white  "0. Esci\n" reset);

    printf("\nScelta: ");
}

// ================= MESSAGGI =================
void msgSuccess(const char* testo) {
    printf(green "[SUCCESSO] %s\n" reset, testo);
}

void msgError(const char* testo) {
    printf(red "[ERRORE] %s\n" reset, testo);
}

void msgInfo(const char* testo) {
    printf(cyan "[INFO] %s\n" reset, testo);
}

// ================= SALVA FILE =================
void salvaTutto(Segnalazione* head) {
    FILE* f = fopen("segnalazioni.txt", "w"); // <-- sovrascrive tutto
    if (!f) {
        msgError("Errore salvataggio file");
        return;
    }

    while (head) {
        fprintf(f, "%d|%s|%s|%s|%s|%d|%s\n",
            head->codice,
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