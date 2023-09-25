NAME	=	philo
SRCS	=	philosophers.c \
			ft_atoi.c \
			start.c \
			utils.c

OBJS	=	${SRCS:.c=.o}
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f

all: 		${NAME}

${NAME}:	${OBJS}
			$(CC) ${CFLAGS} ${OBJS} -o ${NAME}
clean:
			${RM} ${OBJS}
			@echo "$(NAME) objects cleaned ✅"

fclean: 	clean
			$(RM) $(NAME)
			@echo "$(NAME) cleaned ✅"

re:			fclean all

.PHONY: all clean fclean re