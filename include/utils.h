#ifndef UTILS_H
#define UTILS_H

typedef struct Segnalazione Segnalazione;

// INPUT SICURO
int leggiIntero();
void leggiStringa(char* buffer, int size);

// UI
void screenClear();
void pause();
void title();
void menuUser();
void menuAdmin();

// RICERCA
char* ricercaPerStato();
void ricercaPerCodice(Segnalazione* lista);

// MESSAGGI
void msgSuccess(const char* testo);
void msgError(const char* testo);
void msgInfo(const char* testo);

// SALVA FILE
void salvaTutto(Segnalazione* head);

#endif