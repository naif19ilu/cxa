# Cxa - Command line argument parser for C
# Jul 6, 2025
# Makefile :)

OBJS = tester.o cxa.o
FLAGS = -Wall -Wextra -Wpedantic
NAME = cxa

all: $(NAME)

$(NAME): $(OBJS)
	gcc	-o $(NAME) $(OBJS)
%.o: %.c
	gcc	$(FLAGS) -c $< -o $@
clean:
	rm	-rf $(OBJS) $(NAME)
