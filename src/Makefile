# Makefile for Parallel Linear Regression

# Compiler and flags
MPICC = mpicc
CFLAGS = -O3 -Wall -std=c99
LDFLAGS = -lm

# Source files
SOURCES = main.c data.c ols.c gd.c linear_solver.c utils.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = data.h ols.h gd.h linear_solver.h utils.h

# Target executable
TARGET = parallel_lr

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(MPICC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile
%.o: %.c $(HEADERS)
	$(MPICC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Clean all (including results)
cleanall: clean
	rm -f ../results/*.csv ../plots/*.png

# Run tests locally (requires OpenMPI)
test: $(TARGET)
	mpirun -np 1 ./$(TARGET)
	mpirun -np 2 ./$(TARGET)
	mpirun -np 4 ./$(TARGET)

.PHONY: all clean cleanall test
