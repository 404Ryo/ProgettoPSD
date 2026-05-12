#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "segnalazione.h"
#include "colors.h"
#include "utils.h"
#include "codice.h"

/*
    Crea una nuova segnalazione associata a un utente.

    La funzione alloca dinamicamente una struttura Segnalazione e ne inizializza i campi.
    In caso di errore di allocazione, viene mostrato un messaggio e la funzione termina.

    Procedura:
    - associa la segnalazione all’utente loggato
    - permette la scelta della categoria tramite menu
    - assegna la categoria selezionata (o "sconosciuta" se input non valido)
    - genera un codice identificativo per la segnalazione
    - acquisisce la descrizione in modo sicuro
    - acquisisce e normalizza il livello di urgenza (compreso tra 1 e 10)
    - salva automaticamente data e ora correnti
    - inizializza lo stato a "aperta"
    - inizializza il puntatore next a NULL

    Infine:
    - la segnalazione viene salvata su file
    - viene mostrato un messaggio di conferma
    - viene restituito il puntatore alla nuova segnalazione
*/
Segnalazione* creaSegnalazione(char username[]) {

    Segnalazione* s = malloc(sizeof(Segnalazione));
    if (!s) {
        msgError("Errore allocazione memoria");
        return NULL;
    }

    strcpy(s->utente, username);

    int sceltaCategoria;

    printf(cyan  "====================================\n" reset);
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

/*
    Aggiunge una nuova segnalazione alla lista.

    La funzione crea una nuova segnalazione associata all’utente tramite creaSegnalazione().
    Se la creazione fallisce, la lista originale viene restituita invariata.

    Se la creazione va a buon fine:
    - la nuova segnalazione viene inserita in testa alla lista
    - il puntatore alla nuova testa della lista viene restituito
*/
Segnalazione* aggiungiSegnalazione(Segnalazione* head, char username[]) {

    Segnalazione* nuovo = creaSegnalazione(username);
    if (!nuovo) return head;

    nuovo->next = head;
    return nuovo;
}

/*
    Stampa tutte le segnalazioni presenti nella lista.

    La funzione scorre la lista collegata e mostra i dettagli di ogni segnalazione.

    Gestione dei permessi:
    - Se l’utente non è amministratore, vengono mostrate solo le segnalazioni
      appartenenti al proprio username.
    - Se l’utente è amministratore, vengono mostrate tutte le segnalazioni.

    Per ogni segnalazione vengono stampati:
    - codice
    - utente proprietario
    - categoria
    - descrizione
    - data di creazione
    - livello di urgenza
    - stato della segnalazione

    Se nessuna segnalazione viene trovata, viene mostrato un messaggio di errore.
*/
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

/*
    Stampa i dettagli di una singola segnalazione.

    La funzione verifica inizialmente se il puntatore è valido (non NULL);
    in caso contrario termina immediatamente.

    Vengono poi mostrati tutti i campi principali della segnalazione:
    - codice identificativo
    - utente proprietario
    - categoria
    - descrizione
    - data di creazione
    - livello di urgenza
    - stato della segnalazione
*/
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

/*
    Permette a un utente di visualizzare lo stato di una propria segnalazione.

    La funzione richiede l’inserimento del codice della segnalazione e poi
    scorre la lista per cercare una corrispondenza.

    Viene restituita la segnalazione solo se:
    - il codice corrisponde
    - e la segnalazione appartiene all’utente loggato

    Se la segnalazione viene trovata, viene stampato il suo stato.
    In caso contrario, viene mostrato un messaggio di errore.
*/
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

/*
    Cerca una segnalazione nella lista tramite codice identificativo.

    La funzione scorre la lista collegata e confronta il codice di ogni nodo
    con quello fornito in input.

    Se viene trovata una corrispondenza:
    - viene restituito il puntatore alla segnalazione

    Se nessuna segnalazione corrisponde:
    - viene restituito NULL
*/
Segnalazione* cercaPerCodice(Segnalazione* head, int codice) {

    while (head) {
        if (head->codice == codice) return head;
        head = head->next;
    }

    return NULL;
}

/*
    Permette di cercare e visualizzare le segnalazioni filtrandole per categoria.

    La funzione mostra un menu di categorie disponibili e permette all’utente di selezionarne una.
    In base alla scelta, viene assegnata la categoria corrispondente e la lista viene filtrata.

    Procedura:
    - acquisisce la scelta dell’utente
    - valida la categoria selezionata
    - scorre l’intera lista delle segnalazioni
    - stampa solo quelle appartenenti alla categoria scelta

    Per ogni segnalazione trovata vengono mostrati:
    - codice
    - utente
    - descrizione
    - stato

    Se non vengono trovate segnalazioni per la categoria selezionata,
    viene mostrato un messaggio informativo.
*/
void cercaPerCategoria(Segnalazione* head) {

    int scelta;
    char cat[50];
    int trovate = 0;

    printf(cyan "====================================\n" reset);
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

/*
    Permette all’amministratore di aggiornare lo stato di una segnalazione.

    La funzione verifica inizialmente i permessi: solo un admin può modificare lo stato.
    In caso contrario, viene mostrato un messaggio di errore e l’esecuzione termina.

    Procedura:
    - vengono mostrate le segnalazioni aperte
    - l’utente inserisce il codice della segnalazione da modificare
    - se il codice è 0, l’operazione viene annullata
    - viene cercata la segnalazione nella lista

    Gestione stati:
    - se lo stato è "aperta":
        -> può diventare "in lavorazione" oppure "chiuso"
        -> è disponibile anche l’opzione di annullamento

    - se lo stato è "in lavorazione":
        -> può essere chiusa oppure annullata

    - se lo stato è "chiuso":
        -> non può più essere modificato

    Dopo ogni modifica valida:
    - la lista viene salvata su file
    - viene mostrato un messaggio di conferma

    Nota: l’aggiornamento è persistente grazie alla funzione salvaTutto().
*/
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

        printf(cyan "====================================\n" reset);
        printf(cyan "         CAMBIA STATO\n" reset);
        printf(cyan "====================================\n" reset);

        printf(yellow "[1] In lavorazione\n" reset);
        printf(red    "[2] Chiuso\n" reset);
        printf(blue "[0] Annulla\n" reset);

        printf(cyan "\nScelta -> " reset);

        int scelta = leggiIntero();

        switch (scelta) {
            case 0:
                break;
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

        printf(cyan "====================================\n" reset);
        printf(cyan "     SEGNALAZIONE IN LAVORAZIONE\n" reset);
        printf(cyan "====================================\n" reset);

        printf(red "[1] Chiudi segnalazione\n" reset);
        printf(blue "[0] Annulla\n" reset);

        printf(cyan "\nScelta -> " reset);

        int scelta = leggiIntero();

        if (scelta == 1) {
            strcpy(s->stato, "chiuso");
        } else if (scelta == 0) {
            return;
            ungetc('\n', stdin);
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

/*
    Stampa tutte le segnalazioni rilevanti per l’amministratore.

    La funzione scorre l’intera lista e mostra solo le segnalazioni che si trovano
    negli stati "aperta" o "in lavorazione".

    Per ogni segnalazione vengono visualizzati:
    - codice identificativo
    - categoria
    - descrizione
    - stato

    Se non viene trovata nessuna segnalazione valida, viene mostrato un messaggio informativo.
*/
void stampaAperteAdmin(Segnalazione* head) {
    int trovate = 0;

    printf(blue "====================================\n" reset);
    printf(blue "        SEGNALAZIONI\n" reset);
    printf(blue "====================================\n" reset);

    while (head) {
        if (strcmp(head->stato, "aperta") == 0 || strcmp(head->stato, "in lavorazione") == 0) {

            printf(cyan "Codice: " purple "%d\n" reset, head->codice);
            printf(cyan "Tipologia: " yellow "%s\n" reset, head->categoria);
            printf(cyan "Descrizione: %s\n" reset, head->descrizione);
            
            // COLORAZIONE SCRITTA STATO
            if (strcmp(head->stato, "aperta") == 0)
                printf(cyan "Stato: " green "%s\n" reset, head->stato);
            else
                printf(cyan "Stato: " yellow "%s\n" reset, head->stato);
            printf(cyan "---------------------\n" reset);
            trovate = 1;
        }
        head = head->next;
    }

    if (!trovate)
        msgInfo("Nessuna segnalazione aperta");
}

/*
    Filtra e stampa le segnalazioni in base allo stato fornito.

    La funzione riceve una lista di segnalazioni e uno stato da utilizzare come filtro.
    Se lo stato è valido (diverso da "EXIT"), viene mostrata una sezione con intestazione
    e vengono stampate tutte le segnalazioni corrispondenti.

    Per ogni segnalazione filtrata vengono visualizzati:
    - codice
    - categoria
    - descrizione
    - livello di urgenza
    - data di creazione

    Se non vengono trovate segnalazioni con lo stato richiesto,
    viene mostrato un messaggio informativo.

    Nota: se lo stato è "EXIT", la funzione non esegue alcuna stampa utile
    e interrompe la gestione del filtro.
*/
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

/*
    Stampa le segnalazioni suddividendole in base al livello di urgenza.

    La funzione percorre la lista due volte:
    - prima per mostrare le segnalazioni urgenti (urgenza >= 8)
    - poi per mostrare quelle meno urgenti (urgenza < 8)

    In entrambi i casi vengono escluse le segnalazioni con stato "chiuso".

    Per ogni segnalazione vengono visualizzati:
    - livello di urgenza
    - codice identificativo
    - categoria
    - descrizione

    Se non vengono trovate segnalazioni in una delle due categorie,
    viene mostrato un messaggio informativo.

    La funzione serve a fornire una panoramica ordinata delle segnalazioni
    in base alla priorità di intervento.
*/
void stampaUrgenti(Segnalazione* head) {

    if (!head) {
        msgError("Nessuna segnalazione");
        return;
    }

    printf(red "====================================\n" reset);
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
    printf(blue "====================================\n" reset);
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

/*
    Permette all’amministratore di eliminare una segnalazione.

    Procedura:
    - vengono mostrate tutte le segnalazioni con stato "chiuso"
    - se non esistono segnalazioni eliminabili, la funzione termina
    - l’utente inserisce il codice della segnalazione da eliminare
    - viene verificata l’esistenza della segnalazione nella lista

    Prima dell’eliminazione:
    - viene richiesta conferma all’utente (Y/N)

    Eliminazione:
    - se il nodo è in testa viene aggiornato head
    - altrimenti viene ricollegata la lista bypassando il nodo
    - la memoria del nodo viene liberata con free()

    Infine:
    - la lista aggiornata viene salvata su file
    - viene mostrato un messaggio di conferma
*/
Segnalazione* eliminaSegnalazione(Segnalazione* head, int isAdmin) {

    if (!isAdmin) {
        msgError("Solo admin");
        return head;
    }

    screenClear();

    printf(cyan "====================================\n" reset);
    printf(cyan "        ELIMINA SEGNALAZIONE\n" reset);
    printf(cyan "====================================\n\n" reset);

    // mostra solo segnalazioni chiuse
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

/*
    Genera un report statistico completo sulle segnalazioni.

    La funzione analizza l’intera lista e calcola diverse metriche aggregate.

    Statistiche calcolate:
    - numero totale di segnalazioni
    - numero di segnalazioni per stato:
        * aperte
        * in lavorazione
        * chiuse
    - numero di segnalazioni per categoria:
        * illuminazione
        * buche
        * rifiuti
        * impianti

    Inoltre determina:
    - la categoria più frequente tra tutte le segnalazioni

    Il risultato viene stampato a schermo in un report formattato e suddiviso in sezioni:
    - riepilogo generale
    - distribuzione per stato
    - distribuzione per categoria
    - categoria più frequente

    La funzione non modifica la lista, ma esegue solo operazioni di analisi e output.
*/
void generaReport(Segnalazione* head) {

    int totale = 0;
    int aperte = 0;
    int chiuse = 0;
    int inlav = 0;

    int illuminazione = 0;
    int buche = 0;
    int rifiuti = 0;
    int impianti = 0;

    Segnalazione* tmp = head;

    while (tmp) {

        totale++;

        // Stato
        if (strcmp(tmp->stato, "aperta") == 0) {
            aperte++;
        }
        else if (strcmp(tmp->stato, "chiusa") == 0) {
            chiuse++;
        }
        else if (strcmp(tmp->stato, "in lavorazione") == 0) {
            inlav++;
        }


        // Categoria
        if (strcmp(tmp->categoria, "illuminazione") == 0)
            illuminazione++;

        else if (strcmp(tmp->categoria, "buche") == 0)
            buche++;

        else if (strcmp(tmp->categoria, "rifiuti") == 0)
            rifiuti++;

        else if (strcmp(tmp->categoria, "impianti") == 0)
            impianti++;

        tmp = tmp->next;
    }

    // Più Frequente
    int max = illuminazione;
    char frequente[50] = "illuminazione";

    if (buche > max) {
        max = buche;
        strcpy(frequente, "buche");
    }

    if (rifiuti > max) {
        max = rifiuti;
        strcpy(frequente, "rifiuti");
    }

    if (impianti > max) {
        max = impianti;
        strcpy(frequente, "impianti");
    }

    // Stampa
    printf(cyan "====================================\n" reset);
    printf(cyan "           REPORT FINALE\n" reset);
    printf(cyan "====================================\n\n" reset);

    printf(yellow "Totale segnalazioni: %d\n\n" reset, totale);

    printf(green "Segnalazioni aperte: %d\n" reset, aperte);
    printf(yellow   "Segnalazioni in corso: %d\n" reset, inlav);
    printf(red   "Segnalazioni chiuse: %d\n\n" reset, chiuse);
    

    printf(cyan "====================================\n" reset);
    printf(cyan "      SEGNALAZIONI PER CATEGORIA\n" reset);
    printf(cyan "====================================\n\n" reset);

    printf(yellow "Illuminazione: %d\n" reset, illuminazione);
    printf(yellow "Buche: %d\n" reset, buche);
    printf(yellow "Rifiuti: %d\n" reset, rifiuti);
    printf(yellow "Impianti: %d\n\n" reset, impianti);

    printf(cyan "====================================\n" reset);
    printf(cyan "       TIPOLOGIA PIU' FREQUENTE\n" reset);
    printf(cyan "====================================\n\n" reset);

    printf(green "%s (%d segnalazioni)\n" reset, frequente, max);
}

/*
    Salva una singola segnalazione su file in modalità append.

    La funzione apre il file "file/segnalazioni.txt" in modalità aggiunta ("a")
    e scrive i dati della segnalazione in formato testuale delimitato da '|'.

    Formato di salvataggio:
    codice | codiceCompleto | utente | categoria | descrizione | data | urgenza | stato

    Se il file non può essere aperto, la funzione termina senza eseguire operazioni.

    Dopo la scrittura, il file viene chiuso correttamente per garantire la persistenza dei dati.
*/
void salvaSegnalazione(Segnalazione* s) {

    FILE* f = fopen("file/segnalazioni.txt", "a");
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

/*
    Carica tutte le segnalazioni dal file e le ricostruisce in memoria come lista collegata.

    La funzione apre il file "file/segnalazioni.txt" in modalità lettura e legge riga per riga.
    Ogni riga rappresenta una segnalazione serializzata con campi separati dal carattere '|'.

    Procedura:
    - ogni riga viene suddivisa tramite strtok
    - i valori vengono convertiti e copiati nei campi della struttura Segnalazione
    - i valori numerici (codice e urgenza) vengono convertiti con atoi
    - le stringhe vengono copiate con strncpy per evitare overflow

    Gestione errori:
    - se l'allocazione dinamica fallisce, la riga viene ignorata
    - se un campo è mancante o non valido, la segnalazione viene scartata

    Costruzione lista:
    - la lista viene costruita in modo sequenziale
    - viene mantenuto un puntatore tail per inserimenti efficienti in coda

    Alla fine:
    - il file viene chiuso
    - viene restituito il puntatore alla testa della lista (o NULL se vuota)
*/
Segnalazione* caricaSegnalazioni() {

    FILE* f = fopen("file/segnalazioni.txt", "r");
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