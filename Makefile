CFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
		 -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
		 -Wmissing-include-dirs -Wnoexcept \
		 -Woverloaded-virtual -Wredundant-decls -Wshadow \
		 -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
		 -Wswitch-default -Wundef -Werror -Wno-unused 

# these are annoying
# -Wmissing-declarations -Wold-style-cast

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
