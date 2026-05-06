#ifndef SEGNALAZIONE_H
#define SEGNALAZIONE_H

typedef struct Segnalazione {
    int codice;
    char utente[50];
    char categoria[50];
    char descrizione[150];
    char data[30];
    int urgenza;
    char stato[20];
    struct Segnalazione* next;
} Segnalazione;

// ================= CRUD =================
Segnalazione* creaSegnalazione(char username[]);
Segnalazione* aggiungiSegnalazione(Segnalazione* head, char username[]);

// ================= VISUALIZZAZIONE =================
void stampaSegnalazioni(Segnalazione* head, char username[], int isAdmin);
void stampaPerStato(Segnalazione* head, char stato[]);
void stampaUrgenti(Segnalazione* head);
void statoSegnalazioneUtente(Segnalazione* head, char username[]);

// ================= RICERCA =================
Segnalazione* cercaPerCodice(Segnalazione* head, int codice);
void cercaPerCategoria(Segnalazione* head);

// ================= ADMIN =================
void aggiornaStato(Segnalazione* head, int isAdmin);
void stampaAperteAdmin(Segnalazione* head);
Segnalazione* eliminaSegnalazione(Segnalazione* head, int codice, int isAdmin);

// ================= REPORT =================
void generaReport(Segnalazione* head);

// ================= FILE =================
void salvaSegnalazione(Segnalazione* s);
Segnalazione* caricaSegnalazioni();

// ================= BONUS (UTILITÀ FUTURA) =================
// utile per evitare crash da memory leak
void liberaLista(Segnalazione* head);

#endif