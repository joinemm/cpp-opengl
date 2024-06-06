CFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
		 -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
		 -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept \
		 -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
		 -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
		 -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused 

LDFLAGS = -lGL -lglfw -lGLEW

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
	g++ $(CFLAGS) $(LDFLAGS) $^ -o $(TARGET)

build/%.o: src/%.cpp | build
	g++ $(CFLAGS) -MMD -MP -c $< -o $@

.PHONY: all clean run

-include $(DEPS)
