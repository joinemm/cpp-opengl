NAME = cpp-opengl

CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wno-float-conversion -Wno-unused-parameter -O2
LDFLAGS = -lGLEW -lGL -lGLU -lglfw
PREFIX = /usr/local/$(NAME)
TARGET = game
SOURCES = $(wildcard src/*.cpp)
OBJS = ${SOURCES:src/%.cpp=build/%.o}
DEPS = $(OBJS:.o=.d)

define RUNNER :=
cd $(PREFIX)
./$(TARGET)
endef
export RUNNER

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

install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp $(TARGET) $(PREFIX)
	cp -r shaders $(PREFIX)
	cp -r assets $(PREFIX)
	@echo "$$RUNNER" > $(PREFIX)/bin/$(NAME)
	chmod +x $(PREFIX)/bin/$(NAME)

uninstall:
	rm -r $(PREFIX)

.PHONY: all clean run install uninstall

-include $(DEPS)
