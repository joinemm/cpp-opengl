all: build run

build:
		g++ main.cpp -lGL -lglfw -lGLEW

run:
		./a.out
