# Makefile for Parallel Linear Regression

# Compiler and flags
MPICC = mpicc
CFLAGS = -O3 -Wall -std=c99
LDFLAGS = -lm

# Directories
SRCDIR = src
BUILDDIR = build

# Source files
SRC_FILES = $(SRCDIR)/data.c $(SRCDIR)/ols.c $(SRCDIR)/gd.c $(SRCDIR)/linear_solver.c $(SRCDIR)/utils.c
MAIN_SRC = main.c
OBJECTS = $(SRC_FILES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
MAIN_OBJ = $(BUILDDIR)/main.o

# Target executable
TARGET = parallel_lr

# Default target
all: $(BUILDDIR) $(TARGET)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Link
$(TARGET): $(MAIN_OBJ) $(OBJECTS)
	$(MPICC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile main
$(BUILDDIR)/main.o: $(MAIN_SRC)
	$(MPICC) $(CFLAGS) -c $< -o $@

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(MPICC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Clean all (including results)
cleanall: clean
	rm -f results/*.csv plots/*.png

# Run quick test
test: $(TARGET)
	mpirun -np 1 ./$(TARGET) -n 1000 -d 10
	mpirun -np 2 ./$(TARGET) -n 1000 -d 10
	mpirun -np 4 ./$(TARGET) -n 1000 -d 10

# Run full experiment
experiment: $(TARGET)
	@echo "Running Strong Scaling experiments..."
	@mkdir -p results
	mpirun -np 1 ./$(TARGET) > results/ols_p1.log
	mpirun -np 2 ./$(TARGET) > results/ols_p2.log
	mpirun -np 4 ./$(TARGET) > results/ols_p4.log
	mpirun -np 8 ./$(TARGET) > results/ols_p8.log

.PHONY: all clean cleanall test experiment
