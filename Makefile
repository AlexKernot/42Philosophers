NAME=philo

FILES=main.c mutex.c ft_atoi.c philos.c

OUTPUTDIR = bin
BIN=$(FILES:%.c=$(OUTPUTDIR)/%.o)

CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -I./includes/ -Werror

VPATH= src

$(OUTPUTDIR)/%.o: %.c
	mkdir -p $(OUTPUTDIR)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(BIN)
	$(CC) $(BIN) -o $(NAME)

clean:
	rm -f $(BIN)
	rm -d $(OUTPUTDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re