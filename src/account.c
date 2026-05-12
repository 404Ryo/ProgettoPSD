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

// ===================== CHECK USER =====================
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

// ===================== REGISTER =====================
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

// ===================== LOGIN =====================
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