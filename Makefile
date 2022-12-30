DEBUG = 0
# Specify the target executable and the source files it depends on
EXE = advent_of_code_day
SOURCES = $(wildcard src/*.cc)
# OBJECTS = $(SOURCES:%.cc=obj/%.o)
OBJECTS = $(addprefix obj/,$(notdir $(SOURCES:.cc=.o)))

# Specify the C++ compiler and flags
CC = g++
CFLAGS = -Iinclude -std=c++11 -Wall -Wextra -Werror
LDFLAGS =

ifeq ($(DEBUG), 1)
    CFLAGS += -g
    LDFLAGS += -g
endif

# Default target
all: $(EXE)

# Link the executable from the object files
$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXE)

# Compile the source files into object files
obj/%.o: src/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build artifacts
.PHONY: clean
clean:
	rm -f $(EXE) $(OBJECTS)

# Run the executable
.PHONY: run
run: $(EXE)
	./$(EXE)


