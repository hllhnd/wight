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

#pragma once

#include "lang.h"

#include <stdint.h>
#include <stddef.h>

#define OpTerminate 0
#define OpAddData   1
#define OpSubData   2
#define OpAddPtr    3
#define OpSubPtr    4
#define OpWrite     5
#define OpRead      6
#define OpBiz       7
#define OpBnz       8

struct Instruction
{
    uint8_t  opcode  : 4;
    uint32_t operand : 28;
};

struct InstructionizationResult
{
    size_t              length;
    struct Instruction *program;
};

struct InstructionizationResult instructionize_program(const enum Token *tokens, size_t length);
void print_instructions(const struct Instruction *instructions, size_t length);
