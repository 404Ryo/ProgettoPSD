#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "segnalazione.h"
#include "colors.h"
#include "utils.h"
#include "codice.h"

// ===================== CREA =====================
Segnalazione* creaSegnalazione(char username[]) {
    Segnalazione* s = malloc(sizeof(Segnalazione));
    if (!s) {
        msgError("Errore allocazione memoria");
        return NULL;
    }

    strcpy(s->utente, username);

    int sceltaCategoria;

    printf(cyan "Categoria:\n1) illuminazione\n2) buche\n3) rifiuti\n4) impianti\nScelta: " reset);
    scanf("%d", &sceltaCategoria);

    s->codice = generaCodice(sceltaCategoria, s->categoria);

    printf(cyan "Descrizione: " reset);
    scanf(" %[^\n]", s->descrizione);

    printf(cyan "Urgenza (1-10): " reset);
    scanf("%d", &s->urgenza);

    // timestamp automatico
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

// ===================== STAMPA =====================
void stampaSegnalazioni(Segnalazione* head, char username[], int isAdmin) {
    if (!head) {
        msgError("Nessuna segnalazione");
        return;
    }

    while (head) {

        if (!isAdmin && strcmp(head->utente, username) != 0) {
            head = head->next;
            continue;
        }

        printf(yellow "\n---------------------\n" reset);
        printf("Codice: %d\n", head->codice);
        printf("Utente: %s\n", head->utente);
        printf("Categoria: %s\n", head->categoria);
        printf("Descrizione: %s\n", head->descrizione);
        printf("Data: %s\n", head->data);
        printf("Urgenza: %d\n", head->urgenza);
        printf("Stato: %s\n", head->stato);

        head = head->next;
    }
}

// ===================== Stato Segnalazione Utente =====================
void statoSegnalazioneUtente(Segnalazione* head, char username[]) {
    int codice;

    printf("Codice: ");
    scanf("%d", &codice);

    while (head) {
        if (head->codice == codice &&
            strcmp(head->utente, username) == 0) {

            printf("Stato: %s\n", head->stato);
            return;
        }
        head = head->next;
    }

    msgError("Segnalazione non trovata o non tua");
}

// ===================== CERCA CODICE =====================
Segnalazione* cercaPerCodice(Segnalazione* head, int codice) {
    while (head) {
        if (head->codice == codice)
            return head;
        head = head->next;
    }
    return NULL;
}

// ===================== CERCA CATEGORIA =====================
void cercaPerCategoria(Segnalazione* head) {
    int scelta;
    char categoria[50];
    int trovato = 0;

    printf("\n=== CERCA PER CATEGORIA ===\n");
    printf("1. Illuminazione\n");
    printf("2. Buche\n");
    printf("3. Rifiuti\n");
    printf("4. Impianti\n");
    printf("Scelta: ");

    scelta = leggiIntero();

    switch (scelta) {
        case 1:
            strcpy(categoria, "illuminazione");
            break;
        case 2:
            strcpy(categoria, "buche");
            break;
        case 3:
            strcpy(categoria, "rifiuti");
            break;
        case 4:
            strcpy(categoria, "impianti");
            break;
        default:
            msgError("Scelta non valida");
            return;
    }

    screenClear();
    
    while (head) {
        if (strcmp(head->categoria, categoria) == 0) {
            printf(cyan "\n---------------------\n" reset);
            printf("Codice: %d\n", head->codice);
            printf("Descrizione: %s\n", head->descrizione);
            printf("Stato: %s\n", head->stato);
            trovato = 1;
        }
        head = head->next;
    }

    if (!trovato)
        msgError("Nessuna segnalazione trovata");
}

// ===================== UPDATE STATO (ADMIN) =====================
void aggiornaStato(Segnalazione* head, int isAdmin) {
    if (!isAdmin) {
        msgError("Solo admin può modificare lo stato");
        return;
    }

    screenClear();

    // ===== MOSTRA SEGNALAZIONI APERTE =====
    Segnalazione* tmp = head;
    int trovate = 0;

    printf("\n=== SEGNALAZIONI APERTE ===\n");

    while (tmp) {
        if (strcmp(tmp->stato, "aperta") == 0) {

            printf("\n---------------------\n");
            printf("Codice: %d\n", tmp->codice);
            printf("Tipologia: %s\n", tmp->categoria);
            printf("Descrizione: %s\n", tmp->descrizione);
            printf("Stato: %s\n", tmp->stato);

            trovate = 1;
        }
        tmp = tmp->next;
    }

    if (!trovate) {
        msgInfo("Nessuna segnalazione aperta");
        return;
    }

    // ===== INPUT CODICE =====
    int codice;
    printf("\nInserisci codice da aggiornare: ");
    codice = leggiIntero();   // usa la tua funzione sicura

    Segnalazione* s = cercaPerCodice(head, codice);

    if (!s) {
        msgError("Non trovata");
        return;
    }

    // ===== INPUT NUOVO STATO SICURO =====
    printf(cyan "Nuovo stato: " reset);
    getchar(); // pulizia buffer
    fgets(s->stato, sizeof(s->stato), stdin);
    s->stato[strcspn(s->stato, "\n")] = 0;

    msgSuccess("Stato aggiornato");
}

// ===================== VISUALIZZA SEGNALAZIONI APERTE (ADMIN) =====================
void stampaAperteAdmin(Segnalazione* head) {
    int trovate = 0;

    printf(cyan "\n=== SEGNALAZIONI APERTE ===\n" reset);

    while (head) {
        if (strcmp(head->stato, "aperta") == 0) {

            printf(cyan "\n---------------------\n" reset);
            printf(cyan "Codice: " purple "%d\n" reset, head->codice);
            printf(cyan "Tipologia: "purple "%s\n" reset, head->categoria);
            printf(cyan "Descrizione: "purple "%s\n" reset, head->descrizione);
            printf(cyan "Stato: " purple "%s\n" reset, head->stato);

            trovate = 1;
        }
        head = head->next;
    }

    if (!trovate)
        msgInfo("Nessuna segnalazione aperta");
}

// ===================== STATO =====================
void stampaPerStato(Segnalazione* head, char stato[]) {
    int ok = 0;

    while (head) {
        if (strcmp(head->stato, stato) == 0) {
            printf("%d - %s\n", head->codice, head->descrizione);
            ok = 1;
        }
        head = head->next;
    }

    if (!ok)
        msgError("Nessuna segnalazione con questo stato");
}

// ===================== URGENTI =====================
void stampaUrgenti(Segnalazione* head) {
    while (head) {
        if (head->urgenza >= 8)
            printf("URGENTE %d - %s\n", head->codice, head->descrizione);

        head = head->next;
    }
}

// ===================== DELETE =====================
Segnalazione* eliminaSegnalazione(Segnalazione* head, int codice, int isAdmin) {
    if (!isAdmin) {
        msgError("Solo admin può eliminare");
        return head;
    }

    Segnalazione *tmp = head, *prev = NULL;

    while (tmp && tmp->codice != codice) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (!tmp) {
        msgError("Non trovata");
        return head;
    }

    if (!prev)
        head = tmp->next;
    else
        prev->next = tmp->next;

    free(tmp);

    msgSuccess("Eliminata");
    return head;
}

// ===================== REPORT =====================
void generaReport(Segnalazione* head) {
    int t=0,a=0,c=0;

    while (head) {
        t++;
        if (strcmp(head->stato,"aperta")==0) a++;
        if (strcmp(head->stato,"chiusa")==0) c++;
        head = head->next;
    }

    printf("\nTotale:%d Aperte:%d Chiuse:%d\n", t,a,c);
}

// ===================== FILE SAVE =====================
void salvaSegnalazione(Segnalazione* s) {
    FILE* f = fopen("segnalazioni.txt", "a");
    if (!f) return;

    fprintf(f, "%d|%s|%s|%s|%s|%d|%s\n",
        s->codice,
        s->utente,
        s->categoria,
        s->descrizione,
        s->data,
        s->urgenza,
        s->stato
    );

    fclose(f);
}

// ===================== FILE LOAD =====================
Segnalazione* caricaSegnalazioni() {
    FILE* f = fopen("segnalazioni.txt", "r");
    if (!f) return NULL;

    Segnalazione* head = NULL;
    char line[512];

    while (fgets(line, sizeof(line), f)) {

        Segnalazione* s = malloc(sizeof(Segnalazione));
        if (!s) continue;

        char* token = strtok(line, "|");
        if (!token) { free(s); continue; }
        s->codice = atoi(token);

        token = strtok(NULL, "|");
        if (!token) { free(s); continue; }
        strncpy(s->utente, token, 49);

        token = strtok(NULL, "|");
        if (!token) { free(s); continue; }
        strncpy(s->categoria, token, 49);

        token = strtok(NULL, "|");
        if (!token) { free(s); continue; }
        strncpy(s->descrizione, token, 99);

        token = strtok(NULL, "|");
        if (!token) { free(s); continue; }
        strncpy(s->data, token, 29);

        token = strtok(NULL, "|");
        if (!token) { free(s); continue; }
        s->urgenza = atoi(token);

        token = strtok(NULL, "|\n");
        if (!token) { free(s); continue; }
        strncpy(s->stato, token, 19);

        s->utente[strcspn(s->utente, "\r\n")] = 0;
        s->categoria[strcspn(s->categoria, "\r\n")] = 0;
        s->descrizione[strcspn(s->descrizione, "\r\n")] = 0;
        s->data[strcspn(s->data, "\r\n")] = 0;
        s->stato[strcspn(s->stato, "\r\n")] = 0;

        s->next = head;
        head = s;
    }

    fclose(f);
    return head;
}