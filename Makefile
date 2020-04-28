SRCS		= ./sources/main.c

INC_DIR		= ./includes/

OBJS		= ${SRCS:.c=.o}

CC			= clang
CFLAGS		= -Wall -Wextra -Werror

NAME		= minishell

LIBFT		= ./libft/libft.a
LIBFTINC	= ./libft

.c.o:		${SRCS} ${INCLUDES}
			${CC} ${CFLAGS} -I ${LIBFTINC} -I ${INC_DIR} -c $^ -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS}
			make -C ./libft
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBFT}

clean :
			rm -f ${OBJS}

fclean :	clean
			make clean -C ./libft
			rm -f ${NAME}

re :		fclean all
