CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -pthread

SRCDIR = src
BUILDDIR = build
BINDIR = bin

TARGET = $(BINDIR)/server

# List all the source files for each service
SERVICE1_SOURCES := $(SRCDIR)/service1/main.cpp $(SRCDIR)/service1/service1.cpp
SERVICE2_SOURCES := $(SRCDIR)/service2/main.cpp $(SRCDIR)/service2/service2.cpp
SERVICE3_SOURCES := $(SRCDIR)/service3/main.cpp $(SRCDIR)/service3/service3.cpp

# Define object files for each service source
SERVICE1_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/service1/%.o,$(SERVICE1_SOURCES))
SERVICE2_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/service2/%.o,$(SERVICE2_SOURCES))
SERVICE3_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/service3/%.o,$(SERVICE3_SOURCES))

# Define all object files
OBJECTS := $(SERVICE1_OBJECTS) $(SERVICE2_OBJECTS) $(SERVICE3_OBJECTS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
    mkdir -p $(BINDIR)
    $(CC) $(LDFLAGS) $^ -o $@

# Define rules to build object files for each service
$(BUILDDIR)/service1/%.o: $(SRCDIR)/service1/%.cpp
    mkdir -p $(dir $@)
    $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/service2/%.o: $(SRCDIR)/service2/%.cpp
    mkdir -p $(dir $@)
    $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/service3/%.o: $(SRCDIR)/service3/%.cpp
    mkdir -p $(dir $@)
    $(CC) $(CFLAGS) -c $< -o $@

# Define clean rule to remove build artifacts
clean:
    rm -rf $(TARGET) $(BUILDDIR)/*