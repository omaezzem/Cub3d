# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 09:42:39 by omaezzem          #+#    #+#              #
#    Updated: 2025/11/11 13:23:09 by omaezzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME  	= cub3D

NAME_BONUS = 

CC 		= gcc

CFLAGS 	= -Wall -Wextra -Werror 

SRCS 	= cub.c  init_mlx.c utils/ft_split.c utils/ft_strjoin.c utils/utils.c parsing/pars.c get_next_line/get_next_line_utils.c get_next_line/get_next_line.c\
				errors/failed_msg.c ray_casting/ray.c
all: $(NAME)

OBJS	= $(SRCS:.c=.o)
BOBJS   = $(BSRC:.c=.o)

$(NAME) : $(OBJS)
	@$(CC) -lmlx -framework OpenGL -framework AppKit  $^ -o $@

%.o: %.c get_next_line/get_next_line.h cub.h
	$(CC) $(CFLAGS) -c $< -o $@ 

bonus: $(NAME_BONUS)

$(NAME_BONUS) : $(BOBJS)
	@$(CC) -lmlx -framework OpenGL -framework AppKit  $^ -o $@

clean:
	@rm -f $(OBJS) $(BOBJS)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)

re: fclean all