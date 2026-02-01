# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
DEBUGFLAGS := -g -O0
INCLUDES := -Iinclude

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin
DEPDIR := $(OBJDIR)/.deps

# Target executable
TARGET := $(BINDIR)/server

# Source files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPS := $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SOURCES))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS) | $(BINDIR)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@
	@echo "Build complete: $@"

# Compile with dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -MF $(DEPDIR)/$*.d -c $< -o $@

# Include dependency files
-include $(DEPS)

# Create directories
$(OBJDIR) $(BINDIR) $(DEPDIR):
	mkdir -p $@

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

# Clean
clean:
	@echo "Cleaning..."
	rm -rf $(OBJDIR) $(BINDIR)

# Rebuild
rebuild: clean all

# Run
run: all
	@echo "Running $(TARGET)..."
	@$(TARGET)

# Show files
show:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Dependencies: $(DEPS)"

.PHONY: all clean rebuild run debug show