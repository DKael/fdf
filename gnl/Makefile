# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:28:31 by hyungdki          #+#    #+#              #
#    Updated: 2023/06/17 20:29:32 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	get_next_line.c	\
		get_next_line_utils.c	\

OBJS = 	${SRCS:.c=.o}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = libgnl.a

TOTAL_OBJS = $(OBJS)

${NAME} : ${TOTAL_OBJS}
		ar -rsc ${NAME} ${TOTAL_OBJS}

%.o :%.c
	@${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: 
	@$(MAKE) clean
	rm -f  ${NAME}

re: 
	@$(MAKE) fclean
	@$(MAKE) all

bonus:
	@make all

.PHONY: all clean fclean re bonus
