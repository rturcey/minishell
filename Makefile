SRCS		= ./sources/main.c ./sources/general_parser.c ./sources/env_utils.c \
				./sources/export.c ./sources/env_utils2.c ./sources/cmd_utils.c \
				./sources/cmd_utils2.c ./sources/obj_utils.c ./sources/obj_utils2.c \
				./sources/string_utils.c

INC_DIR		= ./includes/

OBJS		= ${SRCS:.c=.o}

CC			= clang
CFLAGS		= -Wall -Wextra -Werror -g

NAME		= minishell

LIBFT		= ./libft/libft.a
LIBFTINC	= ./libft

.c.o:		${SRCS} ${INCLUDES}
			${CC} ${CFLAGS} -I ${LIBFTINC} -I ${INC_DIR} -c $^ -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS}
			make -C ./libft
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBFT}

leaks :		${NAME}
			valgrind --tool=memcheck --leak-check=summary --leak-resolution=high --show-reachable=yes ./${NAME}

clean :
			rm -f ${OBJS}

fclean :	clean
			make fclean -C ./libft
			rm -f ${NAME}

re :		fclean all
