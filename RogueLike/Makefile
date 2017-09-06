CC      = g++
CFLAGS  = -Wall -g -std=c++14
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

NAME = RogueLike

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)


all: $(OBJ)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.cpp
		$(CC) $(CFLAGS) -c $<
	


