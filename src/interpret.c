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

#include "interpret.h"

#include "bytecode.h"

#include <stdio.h>
#include <stdlib.h>

int interpret(const struct Instruction *instructions,
              const size_t              length)
{
    uint8_t *data; /* data buffer */
    size_t   dp;   /* data pointer */
    size_t   i;    /* loop counter */
    size_t   pc;   /* program counter */

    pc = 0;
    dp = 0;

    /* allocate data buffer */
    data = malloc(30000);
    if (data == NULL) {
        return 1;
    }

    /* zero-initialize data buffer */
    for (i = 0; i < 30000; i++) {
        data[i] = 0;
    }

    while (pc < length) {
        switch (instructions[pc].opcode) {
            case OpAddData: {
                data[dp] += instructions[pc].operand;
            } break;

            case OpSubData: {
                data[dp] -= instructions[pc].operand;
            } break;

            case OpAddPtr: {
                dp += instructions[pc].operand;
            } break;

            case OpSubPtr: {
                dp -= instructions[pc].operand;
            } break;

            case OpWrite: {
                putchar(data[dp]);
            } break;

            case OpRead: {
                data[dp] = getchar();
            } break;

            case OpBiz: {
                if (data[dp] == 0) {
                    pc = instructions[pc].operand;
                }
            } break;

            case OpBnz: {
                if (data[dp] != 0) {
                    pc = instructions[pc].operand;
                }
            } break;
        }
        pc++;
    }

    free(data);
    return 0;
}
