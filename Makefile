LDFLAGS=-lGL -lglfw -lGLEW

NAME = out
SRC = main.cpp texture.cpp shader.cpp
OBJ = ${SRC:.cpp=.o}

all: build run

build: main.o texture.o shader.o link

link: $(OBJ)
		g++ $(LDFLAGS) -o $(NAME) $(OBJ)

main.o: main.cpp
		g++ $(LDFLAGS) -c main.cpp 

texture.o: texture.cpp texture.h
		g++ $(LDFLAGS) -c texture.cpp

shader.o: shader.cpp shader.h
		g++ $(LDFLAGS) -c shader.cpp

clean:
		rm -f $(OBJ) $(NAME)

run:
		./$(NAME)
