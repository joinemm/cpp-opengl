CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wno-float-conversion -Wno-unused-parameter -O2

LDFLAGS = -lGLEW -lGL -lGLU -lglfw

TARGET = bin/out
SOURCES = $(wildcard src/*.cpp)
OBJS = ${SOURCES:src/%.cpp=build/%.o}
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$(TARGET)

build:
	mkdir -p $@
	
bin:
	mkdir -p $@

$(TARGET): $(OBJS) | bin
	g++ $^ -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.cpp | build
	g++ $(CFLAGS) -MMD -MP -c $< -o $@

.PHONY: all clean run

-include $(DEPS)
