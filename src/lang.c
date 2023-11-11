/* Copyright (c) 2023 Reperak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "lang.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Token tokenize(const char c)
{
    switch (c) {
        case '+': return TkIncrement;
        case '-': return TkDecrement;
        case '>': return TkMoveRight;
        case '<': return TkMoveLeft;
        case '.': return TkWrite;
        case ',': return TkRead;
        case '[': return TkLoopStart;
        case ']': return TkLoopEnd;
        default:  return TkNone;
    }
}

enum Token *tokenize_program(const char *const program,
                             size_t     *const length)
{
    enum Token  token;           /* current token */
    enum Token *tmp;             /* temporary pointer for realloc */
    enum Token *tokens;          /* token buffer */
    size_t      i;               /* loop counter */
    size_t      program_length;  /* program length */
    size_t      significant;     /* significant (i.e. non-TkNone) tokens */

    program_length = strlen(program);
    /* conservative allocation assuming all tokens are significant (will be trimmed later) */
    tokens         = malloc(program_length * sizeof(enum Token));
    significant    = 0;

    /* tokenize each character, skipping TkNone tokens */
    for (i = 0; i < program_length; i++) {
        token = tokenize(program[i]);
        if (token != TkNone) {
            tokens[significant] = token;
            significant += 1;
        }
    }

    /* reallocate buffer */
    tmp = realloc(tokens, significant * sizeof(enum Token));
    if (tmp == NULL) {
        return NULL;
    }
    tokens = tmp;

    *length = significant;
    return tokens;
}

void print_tokens(const enum Token *const tokens,
                  const size_t            length)
{
    size_t i;

    for (i = 0; i < length; i++) {
        switch (tokens[i]) {
            case TkIncrement:  printf("+"); break;
            case TkDecrement:  printf("-"); break;
            case TkMoveRight:  printf(">"); break;
            case TkMoveLeft:   printf("<"); break;
            case TkWrite:      printf("."); break;
            case TkRead:       printf(","); break;
            case TkLoopStart:  printf("["); break;
            case TkLoopEnd:    printf("]"); break;
            default:           printf("?"); break;
        }
    }
    printf("\n");
}
