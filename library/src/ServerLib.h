/*
 * ServerLib.h
 *
 *  Created on: 18 dic 2024
 *      Author: simon
 */

#ifndef SERVERLIB_H_
#define SERVERLIB_H_

#include "header.h"

char random_char(const char *charset);

void generate_numeric(char *output, size_t length);

void generate_alpha(char *output, size_t length);

void generate_mixed(char *output, size_t length);

void generate_secure(char *output, size_t length);

void generate_unambiguous(char *output, size_t length);

#endif /* SERVERLIB_H_ */
