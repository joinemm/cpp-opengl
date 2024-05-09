CFLAGS = -Wall
LDFLAGS = -lGL -lglfw -lGLEW

TARGET = bin/out
SOURCES = $(wildcard src/*.cpp)
OBJS = ${SOURCES:src/%.cpp=build/%.o}
DEPS = $(OBJS:.o=.d)

all: $(TARGET) run

clean:
		rm -rf build $(TARGET)

run: $(TARGET)
		./$(TARGET)

build:
		mkdir -p $@
	
bin:
		mkdir -p $@

$(TARGET): $(OBJS) | bin
		g++ $(CFLAGS) $(LDFLAGS) $^ -o $(TARGET)

build/%.o: src/%.cpp | build
		g++ $(CFLAGS) -MMD -MP -c $< -o $@

.PHONY: all clean run

-include $(DEPS)
