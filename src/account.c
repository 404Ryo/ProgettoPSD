#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "account.h"
#include "colors.h" 
#include "utils.h"

#define FILE_UTENTI "file/accounts.txt"


// ===================== TO LOWER =====================
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/*
    Verifica se un username è già presente nel file degli utenti.

    La funzione apre il file contenente gli account registrati e scorre
    tutti gli utenti salvati confrontando lo username.

    Procedura:
    - apre il file in modalità lettura
    - legge ogni record (username, password, isAdmin)
    - confronta lo username corrente con quello passato in input
    - se trova una corrispondenza, chiude il file e restituisce 1

    Se il file non esiste o non vengono trovate corrispondenze:
    - la funzione restituisce 0

    Nota: viene utilizzato fscanf per la lettura strutturata dei dati.
*/
int userExists(char username[]) {
    FILE *fp = fopen(FILE_UTENTI, "r");
    if (!fp) return 0;

    Account temp;

    while (fscanf(fp, "%49s %49s %d",
        temp.username,
        temp.password,
        &temp.isAdmin) == 3) {

        if (strcmp(temp.username, username) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*
    Gestisce la registrazione di un nuovo utente.

    La funzione apre il file degli utenti in modalità append e consente
    la creazione di un nuovo account.

    Procedura:
    - apre il file degli utenti
    - richiede all’utente di inserire uno username unico
    - verifica la presenza dello username tramite userExists()
    - normalizza lo username (conversione in minuscolo)
    - richiede la password
    - imposta automaticamente il ruolo come utente standard (isAdmin = 0)
    - salva i dati nel file in formato testo

    Formato di salvataggio:
    username password isAdmin

    Se lo username è già presente, viene mostrato un errore e richiesto nuovamente.

    Alla fine della registrazione viene mostrato un messaggio di conferma.
*/
void signin() {
    Account a;
    FILE *fp = fopen(FILE_UTENTI, "a");

    if (!fp) {
        msgError("Errore file utenti");
        return;
    }

    printf(cyan "====================================\n" reset);
    printf(cyan "        REGISTRAZIONE\n" reset);
    printf(cyan "====================================\n" reset);

    // username unico
    do {
        printf("Username ->" reset);
        leggiStringa(a.username, sizeof(a.username));

        toLowerCase(a.username);

        if (userExists(a.username))
            msgError("Username gia' esistente");

    } while (userExists(a.username));

    printf("Password -> ");
    leggiStringa(a.password, sizeof(a.password));

    a.isAdmin = 0;

    fprintf(fp, "%s %s %d\n",
        a.username,
        a.password,
        a.isAdmin);

    fclose(fp);

    msgSuccess(cyan "Registrazione completata" reset);
}

/*
    Gestisce il processo di autenticazione dell’utente.

    La funzione apre il file contenente gli account registrati e confronta
    le credenziali inserite dall’utente con quelle salvate.

    Procedura:
    - apre il file degli utenti in modalità lettura
    - richiede username e password all’utente
    - normalizza l’username (conversione in minuscolo)
    - scorre il file confrontando username e password
    - se trova corrispondenza:
        * copia l’username nella variabile di output
        * imposta il flag isAdmin
        * restituisce successo (1)

    In caso di login riuscito:
    - mostra un messaggio differente per admin o utente

    In caso di errore:
    - mostra messaggio di credenziali errate
    - restituisce fallimento (0)

    Nota: la funzione utilizza pause() per permettere all’utente di leggere i messaggi.
*/
int login(char username[], int *isAdmin) {
    FILE *fp = fopen(FILE_UTENTI, "r");

    if (!fp) {
        msgError("Nessun utente registrato");
        return 0;
    }

    char user[50], pass[50];
    Account temp;

    printf(cyan "====================================\n" reset);
    printf(cyan "        LOGIN\n" reset);
    printf(cyan "====================================\n" reset);

    printf(cyan "Username -> " reset);
    leggiStringa(user, sizeof(user));

    toLowerCase(user);

    printf(cyan "Password -> " reset);
    leggiStringa(pass, sizeof(pass));

    while (fscanf(fp, "%49s %49s %d",
        temp.username,
        temp.password,
        &temp.isAdmin) == 3) {

        if (strcmp(temp.username, user) == 0 &&
            strcmp(temp.password, pass) == 0) {

            strcpy(username, temp.username);
            *isAdmin = temp.isAdmin;

            fclose(fp);

            if (*isAdmin)
                msgSuccess("Login ADMIN riuscito");
            else
                msgSuccess("Login UTENTE riuscito");

            pause();
            return 1;
        }
    }

    fclose(fp);
    msgError("Username o Password errati");
    pause();
    return 0;
}