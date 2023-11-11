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

#include "bytecode.h"

#include "lang.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct InstructionizationResult instructionize_program(const enum Token *const tokens,
                                                       size_t                  length)
{
    struct Instruction              *instructions; /* instruction buffer */
    struct Instruction              *tmp;          /* temporary for realloc */
    struct InstructionizationResult  result;       /* result */
    size_t                           i;            /* loop counter */
    uint32_t                         st;           /* stack temporary */
    uint32_t                         stack[256];   /* stack */
    size_t                           sp;           /* stack pointer */

    sp = 0;

    /* allocate instruction buffer */
    instructions = malloc((length + 1) * sizeof(struct Instruction));
    if (instructions == NULL) {
        result.program = NULL;
        return result;
    }

    /* instructionize each token */
    for (i = 0; i < length; i++) {
        switch (tokens[i]) {
            case TkIncrement: {
                instructions[i].opcode  = OpAddData;
                instructions[i].operand = 1;
            } break;

            case TkDecrement: {
                instructions[i].opcode  = OpSubData;
                instructions[i].operand = 1;
            } break;

            case TkMoveRight: {
                instructions[i].opcode  = OpAddPtr;
                instructions[i].operand = 1;
            } break;

            case TkMoveLeft: {
                instructions[i].opcode  = OpSubPtr;
                instructions[i].operand = 1;
            } break;

            case TkWrite: {
                instructions[i].opcode = OpWrite;
            } break;

            case TkRead: {
                instructions[i].opcode = OpRead;
            } break;

            case TkLoopStart: {
                /* surely there will be no more than 256 nested loops */
                if (sp >= 256) {
                    /* damn */
                    printf("instructionization stack overflow, more than 256 nested loops\n");
                    abort();
                }
                instructions[i].opcode = OpBiz;
                stack[sp++] = i;
            } break;

            case TkLoopEnd: {
                /* a stack underflow here indicates a bug, not a limitation */
                if (sp == 0) {
                    printf("instructionization stack underflow\n");
                    abort();
                }
                instructions[i].opcode = OpBnz;
                st = stack[--sp];
                instructions[st].operand = i;
                instructions[i].operand = st;
            } break;

            case TkNone: {
                /* unreachable */
                abort();
            }
        }
    }

    /* append termination opcode */
    instructions[i].opcode = OpTerminate;

    /* reallocate instruction buffer */
    tmp = realloc(instructions, i * sizeof(struct Instruction));
    if (tmp != NULL) {
        instructions = tmp;
    }

    result.program = instructions;
    result.length  = i;

    return result;
}

void print_instructions(const struct Instruction *const instructions,
                        size_t                          length)
{
    size_t i;

    for (i = 0; i < length; i++) {
        switch (instructions[i].opcode) {
            case OpAddData: {
                printf("OpAddData(%u)\n", instructions[i].operand);
            } break;

            case OpSubData: {
                printf("OpSubData(%u)\n", instructions[i].operand);
            } break;

            case OpAddPtr: {
                printf("OpAddPtr(%u)\n", instructions[i].operand);
            } break;

            case OpSubPtr: {
                printf("OpSubPtr(%u)\n", instructions[i].operand);
            } break;

            case OpWrite: {
                printf("OpWrite\n");
            } break;

            case OpRead: {
                printf("OpRead\n");
            } break;

            case OpBiz: {
                printf("OpBiz(%u)\n", instructions[i].operand);
            } break;

            case OpBnz: {
                printf("OpBnz(%u)\n", instructions[i].operand);
            } break;

            default: {
                printf("Unknown opcode: %d\n", instructions[i].opcode);
            } break;
        }
    }
}
