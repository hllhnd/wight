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

int interpret(const struct Instruction *instructions)
{
    uint8_t *data; /* data buffer */
    size_t   dp;   /* data pointer */
    size_t   i;    /* loop counter */
    size_t   pc;   /* program counter */

    static void* dispatch_table[] = {
        &&end,
        &&add_data,
        &&sub_data,
        &&add_ptr,
        &&sub_ptr,
        &&write,
        &&read,
        &&biz,
        &&bnz
    };

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

#define DISPATCH() goto *dispatch_table[instructions[pc].opcode]

    add_data: {
        data[dp] += instructions[pc].operand;
        pc += 1;
    } DISPATCH();

    sub_data: {
        data[dp] -= instructions[pc].operand;
        pc += 1;
    } DISPATCH();

    add_ptr: {
        dp += instructions[pc].operand;
        pc += 1;
    } DISPATCH();

    sub_ptr: {
        dp -= instructions[pc].operand;
        pc += 1;
    } DISPATCH();

    write: {
        putchar(data[dp]);
        pc += 1;
    } DISPATCH();

    read: {
        data[dp] = getchar();
        pc += 1;
    } DISPATCH();

    biz: {
        if (data[dp] == 0) {
            pc = instructions[pc].operand;
        } else {
            pc += 1;
        }
    } DISPATCH();

    bnz: {
        if (data[dp] != 0) {
            pc = instructions[pc].operand;
        } else {
            pc += 1;
        }
    } DISPATCH();

#undef DISPATCH

end:
    free(data);
    return 0;
}
