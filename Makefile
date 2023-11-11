BIN		:= wight

CC		:= clang
LD		:= clang

CFLAGS	:= -g -fwrapv -O3 -std=c89 -Wall -Wextra -Wpedantic -Wno-gnu-binary-literal
LDFLAGS	:= -g

BIN_DIR	:= bin
SRC_DIR	:= src
OBJ_DIR	:= obj

SRC		:= $(wildcard $(SRC_DIR)/*.c)
OBJ		:= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP		:= $(OBJ:%.o=%.d)

.PHONY: all clean

all: $(BIN_DIR)/$(BIN)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR)/$(BIN): $(OBJ)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)
