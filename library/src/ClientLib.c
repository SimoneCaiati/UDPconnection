/*
 * ClientLib.c
 *
 *  Created on: 18 dic 2024
 *      Author: simon
 */

#include "ClientLib.h"

void menu() {
    printf("Enter one of the following options:\n"
           "n) numeric\n"
           "a) alphanumeric\n"
           "m) mixed\n"
           "s) secure\n"
           "u) unambiguous\n"
           "h) help\n");
}

void helpMenu() {
    printf("Password Generator Help Menu\nCommands:\n"
           "h        : show this help menu\n"
           "n LENGTH : generate numeric password (digits only)\n"
           "a LENGTH : generate alphabetic password (lowercase letters)\n"
           "m LENGTH : generate mixed password (lowercase letters and numbers)\n"
           "s LENGTH : generate secure password (uppercase, lowercase, numbers, symbols)\n"
           "u LENGTH : generate unambiguous secure password (no similar-looking characters)\n"
           "q        : quit application\n\n"
           "LENGTH must be between 6 and 32 characters\n\n"
           "Ambiguous characters excluded in 'u' option:\n"
           "0 O o (zero and letters O)\n"
           "1 l I i (one and letters l, I)\n"
           "2 Z z (two and letter Z)\n"
           "5 S s (five and letter S)\n"
           "8 B (eight and letter B)\n");
}

int msgValidate(const char* messaggio, char* error) {
    char buffer[100];  // Container of the message that can be edited
    char del[] = " ";  // Set of delimiters
    strcpy(buffer, messaggio);
    char* tipo = strtok(buffer, del);
    char* len = strtok(NULL, del);
    int lenNum;

    if (*tipo == 'q') {
        strcpy(error, "Exit from the menu...");
        return 0;
    }
    if (*tipo == 'h') {
        strcpy(error, "Opening help menu...");
        helpMenu();
        return 0;
    }
    if (len == NULL) {
        strcpy(error, "String does not contain the space as required");
        return 0;
    }
    if (*tipo == 'm' || *tipo == 'n' || *tipo == 's' || *tipo == 'a' || *tipo == 'u') {
        if ((lenNum = atoi(len)) != 0) {
            if (lenNum >= 6 && lenNum <= 32) return 1;
            else {
                strcpy(error, "The length entered is outside the allowed range");
                return 0;
            }
        } else {
            strcpy(error, "You did not enter a number after the space");
            return 0;
        }
    }
    strcpy(error, "The type of password entered is not allowed");
    return 0;
}
