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
#include "interpret.h"
#include "lang.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int                              fd;      /* file descriptor */
    size_t                           length;  /* program length */
    char                            *program; /* program buffer */
    enum Token                      *tokens;  /* token buffer */
    struct InstructionizationResult  result;  /* instructionization result */

    if (argc != 2) {
        fprintf(stderr, "usage: %s <program>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    length = lseek(fd, 0, SEEK_END);
    if (length == (size_t) - 1) {
        perror("lseek");
        return 1;
    }

    program = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (program == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    close(fd);

    tokens = tokenize_program(program, &length);
    if (tokens == NULL) {
        printf("tokenize_program failed\n");
        return 1;
    }

    result = instructionize_program(tokens, length);
    if (result.program == NULL) {
        printf("instructionize_program failed\n");
        return 2;
    }

    interpret(result.program, result.length);

    return 0;
}
