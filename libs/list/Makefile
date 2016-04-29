CC=gcc
FLAGS=-Wall -Wextra -Werror
NAME=liblist.a
SRC_DIR=srcs
INCLUDES=-I ./
BUILD_DIR= __build

SRC=list_add.c\
	list_del.c\
	list_move.c\
	list_splice.c\
	list_empty.c\
	list_len.c\
	list_slice.c\
	list_nth.c\
	list_insert.c

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(NAME)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< -o $@ $(INCLUDES)

$(NAME):$(BUILD_DIR) $(OBJ)
	@ar rc $@ $(OBJ)
	@ranlib $@
	@echo "$@ was created"

test:$(NAME)
	@$(CC) $(FLAGS) $^ test.c -o test
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f $(NAME) test

re: fclean all
