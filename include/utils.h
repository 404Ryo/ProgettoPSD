#ifndef UTILS_H
#define UTILS_H


typedef struct Segnalazione Segnalazione;

// input sicuro
int leggiIntero();

// UI
void screenClear();
void pause();
void title();
void menuUser();
void menuAdmin();

// messaggi
void msgSuccess(const char* testo);
void msgError(const char* testo);
void msgInfo(const char* testo);

// File
void salvaTutto(Segnalazione* head);

#endif