ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC=clang
FLAGS=-Wall -Wextra -Werror

NAME=libft_malloc_$(HOSTTYPE).so
TEST=test

LIBS_DIR=./libs
DIR_LIBFT=$(LIBS_DIR)/libft
DIR_PRINTF=$(LIBS_DIR)/ft_printf
DIR_LIST=$(LIBS_DIR)/list

LIBS=-L $(DIR_PRINTF) -lprintf -L $(DIR_LIBFT) -lft -L $(DIR_LIST) -llist

SRC_DIR=srcs
INCLUDES=-I ./includes

BUILD_DIR= __build

SRC=malloc.c\
		free.c\
		realloc.c\
		calloc.c\
		show_mem.c\
		internal_map.c\
		map.c\
		block.c\
		context.c\


OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) $(NAME)

$(BUILD_DIR):
	@mkdir -p $@

exec:
	@make -C $(DIR_LIBFT)
	@make -C $(DIR_PRINTF)
	@make -C $(DIR_LIST)

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -fPIC -c $< -o $@ $(INCLUDES)

$(NAME):exec $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -shared -o $@
	@echo "$@ was created"
	@ln -f -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f libft_malloc.so

fclean: clean
	@rm -f $(NAME) $(TEST)
	@make $@ -C $(DIR_LIBFT)
	@make $@ -C $(DIR_PRINTF)
	@make $@ -C $(DIR_LIST)

re: fclean all

test: $(NAME)
	$(CC) test.c $(OBJ) $(LIBS) -o $(TEST)
