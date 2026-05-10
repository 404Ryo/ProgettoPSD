#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "segnalazione.h"
#include "colors.h"
#include "utils.h"
#include "codice.h"

// ===================== CREA SEGNALAZIONE =====================
Segnalazione* creaSegnalazione(char username[]) {

    Segnalazione* s = malloc(sizeof(Segnalazione));
    if (!s) {
        msgError("Errore allocazione memoria");
        return NULL;
    }

    strcpy(s->utente, username);

    int sceltaCategoria;

    printf("Categoria:\n1) illuminazione\n2) buche\n3) rifiuti\n4) impianti\nScelta: ");
    scanf("%d", &sceltaCategoria);

    switch (sceltaCategoria) {
        case 1: strcpy(s->categoria, "illuminazione"); break;
        case 2: strcpy(s->categoria, "buche"); break;
        case 3: strcpy(s->categoria, "rifiuti"); break;
        case 4: strcpy(s->categoria, "impianti"); break;
        default: strcpy(s->categoria, "sconosciuta"); break;
    }

    // codice numerico SOLO per ricerca
    generaCodice(sceltaCategoria, s->categoria, s->codiceCompleto, &s->codice);

    screenClear();

    printf("Descrizione: ");
    scanf(" %[^\n]", s->descrizione);

    printf("Urgenza (1-10): ");
    scanf("%d", &s->urgenza);

    if (s->urgenza < 1) s->urgenza = 1;
    if (s->urgenza > 10) s->urgenza = 10;

    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(s->data, 30, "%d/%m/%Y %H:%M", tm_info);

    strcpy(s->stato, "aperta");
    s->next = NULL;

    salvaSegnalazione(s);

    msgSuccess("Segnalazione creata!");
    return s;
}

// ===================== AGGIUNGI =====================
Segnalazione* aggiungiSegnalazione(Segnalazione* head, char username[]) {

    Segnalazione* nuovo = creaSegnalazione(username);
    if (!nuovo) return head;

    nuovo->next = head;
    return nuovo;
}

// ===================== STAMPA SEGNALAZIONI =====================
void stampaSegnalazioni(Segnalazione* head, char username[], int isAdmin) {

    int trovato = 0;

    while (head) {

        if (!isAdmin && strcmp(head->utente, username) != 0) {
            head = head->next;
            continue;
        }

        printf("\n---------------------\n");
        printf("Codice: %d\n", head->codice);
        printf("Utente: %s\n", head->utente);
        printf("Categoria: %s\n", head->categoria);
        printf("Descrizione: %s\n", head->descrizione);
        printf("Data: %s\n", head->data);
        printf("Urgenza: %d\n", head->urgenza);
        printf("Stato: %s\n", head->stato);

        trovato = 1;
        head = head->next;
    }

    if (!trovato) msgError("Nessuna segnalazione");
}

// ===================== STAMPA SINGOLA =====================
void stampaSegnalazione(Segnalazione* s) {

    if (!s) return;

    printf("Codice: %d\n", s->codice);
    printf("Utente: %s\n", s->utente);
    printf("Categoria: %s\n", s->categoria);
    printf("Descrizione: %s\n", s->descrizione);
    printf("Data: %s\n", s->data);
    printf("Urgenza: %d\n", s->urgenza);
    printf("Stato: %s\n", s->stato);
}

// ===================== STATO UTENTE =====================
void statoSegnalazioneUtente(Segnalazione* head, char username[]) {

    int codice;
    printf("Codice: ");
    scanf("%d", &codice);

    while (head) {

        if (head->codice == codice && strcmp(head->utente, username) == 0) {
            printf("Stato: %s\n", head->stato);
            return;
        }

        head = head->next;
    }

    msgError("Non trovata");
}

// ===================== CERCA CODICE =====================
Segnalazione* cercaPerCodice(Segnalazione* head, int codice) {

    while (head) {
        if (head->codice == codice) return head;
        head = head->next;
    }

    return NULL;
}

// ===================== CERCA CATEGORIA =====================
void cercaPerCategoria(Segnalazione* head) {

    int scelta;
    char cat[50];

    printf("1) illuminazione\n2) buche\n3) rifiuti\n4) impianti\nScelta: ");
    scanf("%d", &scelta);

    switch (scelta) {
        case 1: strcpy(cat, "illuminazione"); break;
        case 2: strcpy(cat, "buche"); break;
        case 3: strcpy(cat, "rifiuti"); break;
        case 4: strcpy(cat, "impianti"); break;
        default: msgError("Errore"); return;
    }

    while (head) {

        if (strcmp(head->categoria, cat) == 0) {
            printf("\nCodice: %d\n", head->codice);
            printf("Descrizione: %s\n", head->descrizione);
        }

        head = head->next;
    }
}

// ===================== UPDATE STATO =====================
void aggiornaStato(Segnalazione* head, int isAdmin) {

    if (!isAdmin) {
        msgError("Solo admin");
        return;
    }

    int codice;
    printf("Codice: ");
    scanf("%d", &codice);

    Segnalazione* s = cercaPerCodice(head, codice);

    if (!s) {
        msgError("Non trovata");
        return;
    }

    printf("1 aperta\n2 in lavorazione\n3 chiusa\nScelta: ");
    int c;
    scanf("%d", &c);

    if (c == 1) strcpy(s->stato, "aperta");
    else if (c == 2) strcpy(s->stato, "in lavorazione");
    else strcpy(s->stato, "chiusa");

    salvaTutto(head);
}

// ===================== STAMPA STATO =====================
void stampaPerStato(Segnalazione* head, char stato[]) {

    while (head) {

        if (strcmp(head->stato, stato) == 0) {
            printf("%d - %s\n", head->codice, head->descrizione);
        }

        head = head->next;
    }
}

// ===================== URGENTI =====================
void stampaUrgenti(Segnalazione* head) {

    while (head) {

        if (head->urgenza >= 8) {
            printf("\nURGENTE\n");
            printf("%d %s\n", head->codice, head->descrizione);
        }

        head = head->next;
    }
}

// ===================== DELETE =====================
Segnalazione* eliminaSegnalazione(Segnalazione* head, int isAdmin) {

    if (!isAdmin) {
        msgError("Solo admin");
        return head;
    }

    int codice;
    scanf("%d", &codice);

    Segnalazione *cur = head, *prev = NULL;

    while (cur && cur->codice != codice) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) return head;

    if (!prev) head = cur->next;
    else prev->next = cur->next;

    free(cur);

    salvaTutto(head);
    return head;
}

// ===================== REPORT =====================
void generaReport(Segnalazione* head) {

    int tot = 0, aperte = 0, chiuse = 0;

    while (head) {

        tot++;

        if (strcmp(head->stato, "aperta") == 0) aperte++;
        if (strcmp(head->stato, "chiusa") == 0) chiuse++;

        head = head->next;
    }

    printf("Tot:%d Aperte:%d Chiuse:%d\n", tot, aperte, chiuse);
}

// ===================== SALVA =====================
void salvaSegnalazione(Segnalazione* s) {

    FILE* f = fopen("segnalazioni.txt", "a");
    if (!f) return;

    fprintf(f, "%d|%s|%s|%s|%s|%s|%d|%s\n",
        s->codice,
        s->codiceCompleto,
        s->utente,
        s->categoria,
        s->descrizione,
        s->data,
        s->urgenza,
        s->stato
    );

    fclose(f);
}

// ===================== CARICA =====================
Segnalazione* caricaSegnalazioni() {

    FILE* f = fopen("segnalazioni.txt", "r");
    if (!f) return NULL;

    Segnalazione* head = NULL;
    char line[512];

    while (fgets(line, sizeof(line), f)) {

        Segnalazione* s = calloc(1, sizeof(Segnalazione));
        if (!s) continue;

        char* t = strtok(line, "|");
        if (!t) { free(s); continue; }
        s->codice = atoi(t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->codiceCompleto, t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->utente, t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->categoria, t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->descrizione, t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->data, t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        s->urgenza = atoi(t);

        t = strtok(NULL, "|\n");
        if (!t) { free(s); continue; }
        t[strcspn(t, "\r\n")] = 0;
        strcpy(s->stato, t);

        s->next = head;
        head = s;
    }

    fclose(f);
    return head;
}