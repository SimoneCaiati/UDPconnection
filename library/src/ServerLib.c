/*
 * ServerLib.c
 *
 *  Created on: 18 dic 2024
 *      Author: simon
 */

#include "ServerLib.h"

char random_char(const char *charset) {
    size_t len = strlen(charset);
    return charset[rand() % len];
}

void generate_numeric(char *output, size_t length) {
    const char *numbers = "0123456789";
    for (size_t i = 0; i < length; i++) {
        output[i] = random_char(numbers);
    }
    output[length] = '\0'; // end string
}

void generate_alpha(char *output, size_t length) {
    const char *lowercase = "abcdefghijklmnopqrstuvwxyz";
    for (size_t i = 0; i < length; i++) {
        output[i] = random_char(lowercase);
    }
    output[length] = '\0'; // end string
}

void generate_mixed(char *output, size_t length) {
    const char *lowercase_numbers = "abcdefghijklmnopqrstuvwxyz0123456789";
    for (size_t i = 0; i < length; i++) {
        output[i] = random_char(lowercase_numbers);
    }
    output[length] = '\0'; // end string
}

void generate_secure(char *output, size_t length) {
    const char *full_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_-+=<>?";
    for (size_t i = 0; i < length; i++) {
        output[i] = random_char(full_charset);
    }
    output[length] = '\0'; // end string
}

void generate_unambiguous(char *output, size_t length){
	const char *full_charset = "ACDEFGHJKLMNPQRTUVWXYabcdefghjkmnpqrtuvwxy34679!@#$%^&*()_-+=<>?";
	    for (size_t i = 0; i < length; i++) {
	        output[i] = random_char(full_charset);
	    }
	    output[length] = '\0'; // end string
}
