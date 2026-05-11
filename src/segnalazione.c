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

    printf(cyan  "\n====================================\n" reset);
    printf(cyan  "       SELEZIONA CATEGORIA\n" reset);
    printf(cyan  "====================================\n" reset);

    printf(yellow "[1] Illuminazione\n" reset);
    printf(red    "[2] Buche\n" reset);
    printf(green  "[3] Rifiuti\n" reset);
    printf(blue   "[4] Impianti\n" reset);

    printf(cyan "\nScelta -> " reset);
    sceltaCategoria = leggiIntero();

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

    printf(cyan "Descrizione -> " reset);
    leggiStringa(s->descrizione, sizeof(s->descrizione));

    printf(cyan "Urgenza (1-10) -> " reset);
    s->urgenza = leggiIntero();

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

        printf(cyan "Codice: " purple "%d\n" reset, head->codice);
        printf(cyan "Utente: " blue "%s\n" reset, head->utente);
        printf(cyan "Categoria: " yellow "%s\n" reset, head->categoria);
        printf(cyan "Descrizione: " reset "%s\n", head->descrizione);
        printf(cyan "Data: " reset "%s\n", head->data);
        printf(cyan "Urgenza: " red "%d\n" reset, head->urgenza);

        // COLORAZIONE STATO
        if (strcmp(head->stato, "aperta") == 0)
            printf(cyan "Stato: " green "%s\n" reset, head->stato);
        else if (strcmp(head->stato, "in lavorazione") == 0)
            printf(cyan "Stato: " yellow "%s\n" reset, head->stato);
        else
            printf(cyan "Stato: " red "%s\n" reset, head->stato);

        printf("---------------------\n");
        trovato = 1;
        head = head->next;
    }

    if (!trovato) msgError("Nessuna segnalazione");
}

// ===================== STAMPA SINGOLA =====================
void stampaSegnalazione(Segnalazione* head) {

    if (!head) return;

    printf(cyan "Codice: " purple "%d\n" reset, head->codice);
    printf(cyan "Utente: " green "%s\n" reset, head->utente);
    printf(cyan "Categoria: " yellow "%s\n" reset, head->categoria);
    printf(cyan "Descrizione: " reset "%s\n" reset, head->descrizione);
    printf(cyan "Data: " blue "%s\n" reset, head->data);
    printf(cyan "Urgenza: " red "%d\n" reset, head->urgenza);
    
    // COLORAZIONE STATO
    if (strcmp(head->stato, "aperta") == 0)
        printf(cyan "Stato: " green "%s\n" reset, head->stato);
    else if (strcmp(head->stato, "in lavorazione") == 0)
        printf(cyan "Stato: " yellow "%s\n" reset, head->stato);
    else
        printf(cyan "Stato: " red "%s\n" reset, head->stato);
}

// ===================== STATO UTENTE =====================
void statoSegnalazioneUtente(Segnalazione* head, char username[]) {

    int codice;
    printf("Codice -> ");
    codice = leggiIntero();

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
    int trovate = 0;

    printf(cyan "\n====================================\n" reset);
    printf(cyan "        SELEZIONA CATEGORIA\n" reset);
    printf(cyan "====================================\n" reset);

    printf(green  "[1] Illuminazione\n" reset);
    printf(yellow "[2] Buche\n" reset);
    printf(red    "[3] Rifiuti\n" reset);
    printf(blue   "[4] Impianti\n" reset);

    printf(cyan "\nScelta -> " reset);

    scelta = leggiIntero();
    screenClear();

    printf(cyan "====================================\n" reset);

    switch (scelta) {
        case 1:
            printf(cyan "        CATEGORIA ILLUMINAZIONE\n" reset);
            strcpy(cat, "illuminazione");
            break;

        case 2:
            printf(cyan "        CATEGORIA BUCHE\n" reset);
            strcpy(cat, "buche");
            break;

        case 3:
            printf(cyan "        CATEGORIA RIFIUTI\n" reset);
            strcpy(cat, "rifiuti");
            break;

        case 4:
            printf(cyan "        CATEGORIA IMPIANTI\n" reset);
            strcpy(cat, "impianti");
            break;

        default:
            msgError("Errore");
            return;
    }

    printf(cyan "====================================\n" reset);

    while (head) {

        if (strcmp(head->categoria, cat) == 0) {

            trovate = 1;

            printf(cyan "\nCodice: " purple "%d\n" reset, head->codice);
            printf(cyan "Utente: " blue "%s\n" reset, head->utente);
            printf(cyan "Descrizione: %s\n" reset, head->descrizione);

            if (strcmp(head->stato, "aperta") == 0)
                printf(cyan "Stato: " green "%s\n" reset, head->stato);
            else if (strcmp(head->stato, "in lavorazione") == 0)
                printf(cyan "Stato: " yellow "%s\n" reset, head->stato);
            else
                printf(cyan "Stato: " red "%s\n" reset, head->stato);

            printf(cyan "------------------------------------\n" reset);
        }

        head = head->next;
    }

    if (!trovate)
        msgInfo("Nessuna segnalazione in questa categoria");
}

// ===================== UPDATE STATO =====================
void aggiornaStato(Segnalazione* head, int isAdmin) {

    if (!isAdmin) {
        msgError("Solo admin può modificare lo stato");
        return;
    }

    screenClear();

    stampaAperteAdmin(head);

    printf(cyan "\nInserisci codice (0 per uscire) -> " reset);

    int codice = leggiIntero();

    if (codice == 0)
        return;

    Segnalazione* s = cercaPerCodice(head, codice);

    if (!s) {
        msgError("Segnalazione non trovata");
        return;
    }

    screenClear();

    // ================= APERTA =================
    if (strcmp(s->stato, "aperta") == 0) {

        printf(cyan "\n====================================\n" reset);
        printf(cyan "         CAMBIA STATO\n" reset);
        printf(cyan "====================================\n" reset);

        printf(yellow "[1] In lavorazione\n" reset);
        printf(red    "[2] Chiuso\n" reset);

        printf(cyan "\nScelta -> " reset);

        int scelta = leggiIntero();

        switch (scelta) {

            case 1:
                strcpy(s->stato, "in lavorazione");
                break;

            case 2:
                strcpy(s->stato, "chiuso");
                break;

            default:
                msgError("Scelta non valida");
                return;
        }
    }

    // ================= IN LAVORAZIONE =================
    else if (strcmp(s->stato, "in lavorazione") == 0) {

        printf(cyan "\n====================================\n" reset);
        printf(cyan "     SEGNALAZIONE IN LAVORAZIONE\n" reset);
        printf(cyan "====================================\n" reset);

        printf(red "[1] Chiudi segnalazione\n" reset);

        printf(cyan "\nScelta -> " reset);

        int scelta = leggiIntero();

        if (scelta == 1) {
            strcpy(s->stato, "chiuso");
        } else {
            msgError("Scelta non valida");
            return;
        }
    }

    // ================= CHIUSO =================
    else if (strcmp(s->stato, "chiuso") == 0) {

        msgInfo("La segnalazione è già chiuso");
        return;
    }

    salvaTutto(head);

    msgSuccess("Stato aggiornato correttamente");
}

// ================= Stampa ================= 
void stampaAperteAdmin(Segnalazione* head) {
    int trovate = 0;

    printf(cyan "\n====================================\n" reset);
    printf(cyan "        SEGNALAZIONI\n" reset);
    printf(cyan "====================================\n" reset);

    while (head) {
        if (strcmp(head->stato, "aperta") == 0 || strcmp(head->stato, "in lavorazione") == 0) {

            printf(cyan "\n---------------------\n" reset);
            printf(cyan "Codice: " purple "%d\n" reset, head->codice);
            printf(cyan "Tipologia: " yellow "%s\n" reset, head->categoria);
            printf(cyan "Descrizione: %s\n" reset, head->descrizione);
            
            // COLORAZIONE SCRITTA STATO
            if (strcmp(head->stato, "aperta") == 0)
                printf(cyan "\tStato: " green "%s\n" reset, head->stato);
            else
                printf(cyan "\tStato: " yellow "%s\n" reset, head->stato);

            trovate = 1;
        }
        head = head->next;
    }

    if (!trovate)
        msgInfo("Nessuna segnalazione aperta");
}

// ===================== STAMPA STATO =====================
void stampaPerStato(Segnalazione* head, char stato[]) {

    int trovate = 0;
    if (strcmp(stato, "EXIT") == 1){
        printf(cyan "====================================\n" reset);
        printf(cyan "      SEGNALAZIONI FILTRATE\n" reset);
        if (strcmp(stato, "aperta") == 0)
            printf(cyan "\tStato: " green "%s\n" reset, stato);
        else if (strcmp(stato, "in lavorazione") == 0)
            printf(cyan "\tStato: " yellow "%s\n" reset, stato);
        else
            printf(cyan "\tStato: " red "%s\n" reset, stato);
        printf(cyan "====================================\n" reset);

        while (head) {

            if (strcmp(head->stato, stato) == 0) {

                printf(cyan "\n-------------------------------\n" reset);
                printf(cyan "Codice: " purple "%d\n" reset, head->codice);
                printf(cyan "Categoria: " yellow "%s\n" reset, head->categoria);
                printf(cyan "Descrizione: %s\n" reset, head->descrizione);
                printf(cyan "Urgenza: %d\n" reset, head->urgenza);
                printf(cyan "Data: %s\n" reset, head->data);

                trovate = 1;
            }

            head = head->next;
        }

        if (!trovate) {
            msgInfo("Nessuna segnalazione con questo stato");
        }
    }
    else{
        ungetc('\n', stdin);
    }
}

// ===================== URGENTI =====================
void stampaUrgenti(Segnalazione* head) {

    if (!head) {
        msgError("Nessuna segnalazione");
        return;
    }

    printf(red "\n====================================\n" reset);
    printf(red "        SEGNALAZIONI URGENTI\n" reset);
    printf(red "====================================\n" reset);

    Segnalazione* tmp = head;
    int trovate = 0;

    // ================= URGENTI =================
    while (tmp) {

        if (strcmp(tmp->stato, "chiuso") != 0 && tmp->urgenza >= 8) {

            printf(cyan "Urgenza: " green "%d\n" reset, tmp->urgenza);
            printf(cyan "Codice: " purple "%d\n" reset, tmp->codice);
            printf(cyan "Categoria: " yellow "%s\n" reset, tmp->categoria);
            printf(cyan "Descrizione: %s\n" reset, tmp->descrizione);
            printf(cyan "---------------------\n" reset);

            trovate = 1;
        }

        tmp = tmp->next;
    }

    if (!trovate) {
        msgInfo("Nessuna segnalazione urgente (>= 8)");
    }

    // ================= MENO URGENTI =================
    printf(blue "\n====================================\n" reset);
    printf(blue "        MENO URGENTI\n" reset);
    printf(blue "====================================\n" reset);

    tmp = head;
    trovate = 0;

    while (tmp) {

        if (strcmp(tmp->stato, "chiuso") != 0 && tmp->urgenza < 8) {

            printf(cyan "Urgenza: " green "%d\n" reset, tmp->urgenza);
            printf(cyan "Codice: " purple "%d\n" reset, tmp->codice);
            printf(cyan "Categoria: " yellow "%s\n" reset, tmp->categoria);
            printf(cyan "Descrizione: %s\n" reset, tmp->descrizione);
            printf(cyan "---------------------\n" reset);

            trovate = 1;
        }

        tmp = tmp->next;
    }

    if (!trovate) {
        msgInfo("Nessuna segnalazione meno urgente");
    }
}

// ===================== DELETE =====================
Segnalazione* eliminaSegnalazione(Segnalazione* head, int isAdmin) {

    if (!isAdmin) {
        msgError("Solo admin");
        return head;
    }

    screenClear();

    printf(cyan "====================================\n" reset);
    printf(cyan "        ELIMINA SEGNALAZIONE\n" reset);
    printf(cyan "====================================\n\n" reset);

    // 🔥 mostra solo segnalazioni chiuse
    Segnalazione* tmp = head;
    int trovate = 0;

    while (tmp) {

        if (strcmp(tmp->stato, "chiuso") == 0) {

            printf(cyan "Codice: " purple "%d\n" reset, tmp->codice);
            printf(cyan "Utente: " blue "%s\n" reset, tmp->utente);
            printf(cyan "Categoria: %s\n" reset, tmp->categoria);
            printf(cyan "------------------------------------\n" reset);

            trovate = 1;
        }

        tmp = tmp->next;
    }

    if (!trovate) {
        msgInfo("Nessuna segnalazione chiusa");
        return head;
    }

    printf(yellow "\n➤ Inserisci codice -> " reset);

    int codice = leggiIntero();

    if (codice <= 0) {
        msgError("Codice non valido");
        return head;
    }

    Segnalazione *cur = head, *prev = NULL;

    while (cur && cur->codice != codice) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        msgError("Segnalazione non trovata");
        return head;
    }

    char conferma[10];

    printf(red "\n⚠ Sei sicuro di voler eliminare? (Y/N): " reset);

    leggiStringa(conferma, sizeof(conferma));

    if (conferma[0] != 'Y' && conferma[0] != 'y') {
        msgInfo("Operazione annullata");
        return head;
    }

    // eliminazione nodo
    if (!prev)
        head = cur->next;
    else
        prev->next = cur->next;

    free(cur);

    salvaTutto(head);

    msgSuccess("Segnalazione eliminata");

    return head;
}

// ===================== REPORT =====================
void generaReport(Segnalazione* head) {

    int tot = 0, aperte = 0, chiuse = 0;

    while (head) {

        tot++;

        if (strcmp(head->stato, "aperta") == 0) aperte++;
        if (strcmp(head->stato, "chiuso") == 0) chiuse++;

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
    Segnalazione* tail = NULL;

    char line[512];

    while (fgets(line, sizeof(line), f)) {

        Segnalazione* s = calloc(1, sizeof(Segnalazione));
        if (!s) continue;

        char* t = strtok(line, "|");
        if (!t) { free(s); continue; }
        s->codice = atoi(t);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        strncpy(s->codiceCompleto, t, sizeof(s->codiceCompleto)-1);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        strncpy(s->utente, t, sizeof(s->utente)-1);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        strncpy(s->categoria, t, sizeof(s->categoria)-1);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        strncpy(s->descrizione, t, sizeof(s->descrizione)-1);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        strncpy(s->data, t, sizeof(s->data)-1);

        t = strtok(NULL, "|");
        if (!t) { free(s); continue; }
        s->urgenza = atoi(t);

        t = strtok(NULL, "|\n");
        if (!t) { free(s); continue; }
        strncpy(s->stato, t, sizeof(s->stato)-1);

        s->next = NULL;

        // append corretto
        if (!head) {
            head = s;
            tail = s;
        } else {
            tail->next = s;
            tail = s;
        }
    }

    fclose(f);
    return head;
}