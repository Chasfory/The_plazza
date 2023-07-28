##
## EPITECH PROJECT, 2023
## Plazza
## File description:
## Makefile
##

NAME		=	plazza

SRC			=	src/main.cpp					\
				src/shell/Shell.cpp				\
				src/shell/ShellParsing.cpp		\
				src/kitchens/Kitchens.cpp		\
				src/kitchens/cooks/Cooks.cpp	\
				src/ACommunication.cpp			\
				src/reception/Reception.cpp		\

SRC_TEST	=

OBJ			=	$(SRC:.cpp=.o)

CPPFLAGS 	= 	-iquote include
CXXFLAGS	=	-W -Wall -Wextra -std=c++17
LDLIBS		=	-lpthread
all:	$(NAME)

$(NAME):	$(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDLIBS)

debug:	CXXFLAGS += -g3
debug:	re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

tests_run:
	g++ -o unit_tests $(SRC_TEST) $(SRC) $(CXXFLAGS) --coverage -lcriterion
	./unit_tests

.PHONY:	all debug clean fclean re tests_run
