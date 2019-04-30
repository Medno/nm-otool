# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/08 14:57:58 by pchadeni          #+#    #+#              #
#    Updated: 2019/04/30 15:05:06 by pchadeni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------Name of the project------#

NM = ft_nm
OTOOL = ft_otool
NAME = $(NM) $(OTOOL)

#------All sources------#

NM_MAIN = nm.c

OTOOL_MAIN = otool.c

MAIN_SRCS = type.c					\
			merge_sort.c			\
			merge_sort_uint.c		\
			error.c					\
			print.c					\
			print_nm.c				\
			tools.c					\
			tools_2.c				\
			symbols_struct.c		\
			handle_fat.c			\
			handle_archive.c		\
			handle_macho_struct.c	\
			list_symbols.c			\
			add_sect.c				\
			cpu_name.c				\
			handle_file.c			\
			options.c

OBJ += $(addprefix ./$(OBJ_PATH)/, $(MAIN_SRCS:.c=.o))
OBJ_NM += $(addprefix ./$(OBJ_PATH)/, $(NM_MAIN:.c=.o))
OBJ_OTOOL += $(addprefix ./$(OBJ_PATH)/, $(OTOOL_MAIN:.c=.o))


#------Includes------#

INCLUDES = ./inc/
INC += -I $(INCLUDES)
INC += -I $(LIB_PATH)/$(INCLUDES)
LIB = $(LIB_PATH)/libft.a

HEAD = $(INCLUDES)/lib_nm_otool.h

#------Path------#

SRCS_PATH = srcs
OBJ_PATH  = obj
LIB_PATH  = libft

#------Main rules------#

all: $(OBJ_PATH) makelib $(NAME)

$(NAME): $(LIB) $(OBJ) $(OBJ_NM) $(OBJ_OTOOL)
	@printf "\33[2KObjects created $(BOLD_GREEN)✓$(EOC)\n"
	@printf "Start making $(NAME)...\n"
	@$(CC) $(C_FLAGS) $(FLAGS) $(INC) -o $(NM) $(OBJ) $(OBJ_NM) $(LIB)
	@$(CC) $(C_FLAGS) $(FLAGS) $(INC) -o $(OTOOL) $(OBJ) $(OBJ_OTOOL) $(LIB)
	@printf "$(BOLD_GREEN)⤖     $(NAME)$(EOC) created $(BOLD_GREEN)✓$(EOC)\n"

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@printf "$(OBJ_PATH) created\n"

$(OBJ_PATH)/%.o: $(SRCS_PATH)/%.c $(HEAD)
	@$(CC) $(FLAGS) $(C_FLAGS) $(INC) -o $@ -c $<
	@printf " $(COL_GREEN)[OK]$(EOC) $(COL_YELLOW)Compiling:$(EOC) $<\r"

makelib:
	@make -C $(LIB_PATH) NOERR=$(NOERR) DEV=$(DEV) SAN=$(SAN)

clean: cleanlib
	@rm -Rf $(OBJ_PATH)
	@rm -Rf $(TEST_OBJ)
	@echo "$(BOLD_GREEN)$(NAME)$(EOC) clean $(BOLD_GREEN)✓$(EOC)"

cleanlib:
	@make clean -C $(LIB_PATH)

fclean: fcleanlib clean
	@rm -Rf $(NAME) $(LINK)
	@echo "$(BOLD_GREEN)$(NAME) $(LINK)$(EOC) fclean $(BOLD_GREEN)✓$(EOC)"

fcleanlib:
	@make fclean -C $(LIB_PATH)

re: fclean all

#------Compilation's flags------#

CC = gcc

# flags
C_FLAGS = -Wall -Wextra

ifneq ($(NOERR),yes)
C_FLAGS += -Werror
endif

ifeq ($(DEV),yes)
FLAGS += -g
endif

ifeq ($(SAN),yes)
FLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

#------Define colors------#

BOLD_GREEN = \033[01;32m
COL_GREEN = \033[0;32m
COL_YELLOW = \033[0;33m
EOC = \033[0m

.PHONY: all re fclean clean
